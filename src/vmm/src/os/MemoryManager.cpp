#include <os/Board.h>
#include <os/InterruptManager.h>
#include <os/MemoryManager.h>
#include <os/Types.h>

#include <cstdio>

//Provided by linker
extern uint64_t __boot_pml4[];
extern uint64_t __boot_pml3[];
extern uint64_t __boot_pml2[];
extern uint8_t  __bss_end[];

namespace {

struct Page {
	uint8_t data[4096];
};

struct HeapPageInfo {
	uint8_t allocated : 1;
	uint8_t RSVD      : 7;
};

static const int KIB = 1024;
static const int MIB = 1024 * KIB;
static const int GIB = 1024 * MIB;

static const uint64_t VMM_PAGE_SIZE =   2 * MIB;
static const uint64_t VMM_WRAM_SIZE = 512 * MIB;
static const int NUM_WRAM_PAGES = VMM_WRAM_SIZE / VMM_PAGE_SIZE;

static const uint64_t VMM_WRAM_VIRT_BASE = 0xFFFFFFFF80000000UL;

//Top level page table
//  Full 256TiB view of 512 512GiB PML3s
uint64_t *pml4;

//Middle page table for top of memory (shared between cores, should be static)
//  512GiB view of 512 1GiB PML2s
uint64_t *higher_pml3;

//Bottom level page table for VMM Work RAM (shared between cores, should be static)
//   1GiB of 512 2MiB PML1s
uint64_t *vmm_pml2;

Page *contig_heap;
uint64_t contig_heap_size;
uint64_t contig_num_pages;

HeapPageInfo *contig_heap_info;
uint64_t contig_heap_num_info_pages;

void invalidate_page( uint64_t addr )
{
	asm volatile( "invlpg %0"
	                :
	                : "m"(addr)
	                : "memory" );
}

void invalidate_all_pages()
{
	asm volatile( "movq %cr3, %rax" );
	asm volatile( "movq %rax, %cr3" );
}

void* vmm_virt_to_phys( void *virt )
{
	uint64_t addr = reinterpret_cast<uint64_t>( virt );
	if( addr >= 0xFFFFFFFFA0000000UL ) {
		return nullptr;
	}
	else if( addr >= 0xFFFFFFFF80000000UL ) {
		return reinterpret_cast<void*>( addr & 0x7FFFFFFFUL );
	}

	return nullptr;
}

void* phys_to_vmm_virt( void *phys )
{
	uint64_t addr = reinterpret_cast<uint64_t>( phys );
	if( addr <= 0x7FFFFFFFUL ) {
		return reinterpret_cast<void*>( addr | VMM_WRAM_VIRT_BASE );
	}

	return nullptr;
}

void init_upper_wram()
{
	for( int i = 0; i < NUM_WRAM_PAGES; i++ ) {
		uint64_t physaddr = i * VMM_PAGE_SIZE;
		uint64_t virtaddr = physaddr | VMM_WRAM_VIRT_BASE;
		uint64_t pte = physaddr | 0x83;  // is a 2MB page, w/r, preset
		vmm_pml2[ i ] = pte;
		invalidate_page( virtaddr );
	}
}

void unmap_boot_lower()
{
	higher_pml3[0] = 0;

	pml4[0] = 0;

	invalidate_all_pages();
}

void initialize_contiguous_heap()
{
	uint64_t heap_end_addr = reinterpret_cast<uint64_t>( phys_to_vmm_virt( &__bss_end[0] ) );

	// Round to the next page
	heap_end_addr += 4095;
	heap_end_addr &= 0xFFFFFFFFFFFFF000UL;

	contig_heap = reinterpret_cast<Page*>( heap_end_addr );
	contig_heap_size = (VMM_WRAM_SIZE + VMM_WRAM_VIRT_BASE) - heap_end_addr;
	contig_num_pages = contig_heap_size / 4096;

	contig_heap_info = reinterpret_cast<HeapPageInfo*>( &contig_heap[0] );
	contig_heap_num_info_pages = contig_num_pages * sizeof(HeapPageInfo);
	contig_heap_num_info_pages += 4095;
	contig_heap_num_info_pages &= 0xFFFFFFFFFFFFF000UL;
	contig_heap_num_info_pages /= 4096;

	for( uint64_t page = 0; page < contig_num_pages; page++ ) {
		if( page < contig_heap_num_info_pages ) {
			contig_heap_info[ page ].allocated = true;
		}
		else {
			contig_heap_info[ page ].allocated = false;
		}
	}

	printf("Heap size = 0x%08lx bytes\n", contig_heap_size);
}

} /*anonymous namespace*/

namespace os { namespace mm {

void init()
{
	::pml4        = reinterpret_cast<uint64_t*>(::phys_to_vmm_virt(__boot_pml4));
	::higher_pml3 = reinterpret_cast<uint64_t*>(::phys_to_vmm_virt(__boot_pml3));
	::vmm_pml2    = reinterpret_cast<uint64_t*>(::phys_to_vmm_virt(__boot_pml2));

	::init_upper_wram();

	::unmap_boot_lower();

	::initialize_contiguous_heap();
}

void* allocate_page()
{
	for( int i = (::contig_num_pages - 1); i > 0; i-- ) {
		if( !::contig_heap_info[i].allocated ) {
			::contig_heap_info[i].allocated = true;
			auto page = &::contig_heap[i];
			return page;
		}
	}

	return nullptr;
}

void set_lower_pml3( void *pml3, uint64_t virt_base )
{
	uint64_t phys_pml3_addr = reinterpret_cast<uint64_t>(::vmm_virt_to_phys(pml3));
	uint64_t offset = virt_base / 0x0000008000000000UL;

	pml4[ offset ] = phys_pml3_addr | 1;

	::invalidate_all_pages();
}

void on_page_fault( os::intm::Context *ctx )
{
	printf( "Page fault!\n" );

	ctx->print();

	os::board::shutdown();
}

}} /*namespace os::mm*/

