#include "jitpp/arm/Disassembler.h"
#include "jitpp/arm/ArmTranslatorFactory.h"
#include "jitpp/arm/ArmTranslator.h"
#include "jitpp/ACState.h"
#include "jitpp/CodeCache.h"

#include <os/Board.h>
#include <os/MemoryManager.h>

#include <cstdio>

namespace {

uint64_t *emulation_pml2[4] = 
{
	nullptr, nullptr, nullptr, nullptr,
};

uint64_t *emulation_pml3 = nullptr;

void mapArmPhysicalMem()
{
	emulation_pml2[0] = (uint64_t*)os::mm::allocate_page();
	emulation_pml2[1] = (uint64_t*)os::mm::allocate_page();
	emulation_pml2[2] = (uint64_t*)os::mm::allocate_page();
	emulation_pml2[3] = (uint64_t*)os::mm::allocate_page();

	emulation_pml3 = (uint64_t*)os::mm::allocate_page();

	const int NUM_PAGE_TABLE_ENTRIES_PER_PAGE = (4096 / sizeof(uint64_t));

	for( int ii = 0; ii < NUM_PAGE_TABLE_ENTRIES_PER_PAGE; ii++ ) {
		emulation_pml3[ii] = 0;
	}

	for( int ii = 0; ii < NUM_PAGE_TABLE_ENTRIES_PER_PAGE; ii++ ) {
		for( int jj = 0; jj < 4; jj++ ) {
			const uint64_t PHYS_PAGE = ((2 * 1024 * 1024) * ii) + 
			                           ((1 * 1024 * 1024 *1024) * jj) + 
			                           0x100000000UL;
			emulation_pml2[jj][ii] = PHYS_PAGE | 0x83;
		}
	}

	for( int ii = 0; ii < 4; ii++ ) {
		const uint64_t pml2_phys_addr = ((uint64_t)emulation_pml2[ii]) - 0xFFFFFFFF80000000UL;

		emulation_pml3[ii] = pml2_phys_addr | 1;
	}

	os::mm::set_lower_pml3(emulation_pml3, 0);
}

class VMMCodeCacheRandomStrategy
{
protected:
	static int get_rand_way( int ways ) {
		return os::board::high_performance_timer() % ways;
	}
};

class DeletionEvictionPolicy
{
protected:
	static void on_item_evicted( remu::jitpp::CodePage<4096, remu::jitpp::arm::ArmTranslator> **evicted ) {
		delete *evicted;
	}
};

class NoMmuHostPageSelectionStrategy
{
protected:
	static void* get_host_page( const remu::jitpp::ACFarPointer& ip ) {
		return reinterpret_cast<void*>( ip.program_counter & ~4095 );
	}
};

using Arm11CpuCodeCache = remu::jitpp::CodeCache<4,1024,4096,
                                                 VMMCodeCacheRandomStrategy,
                                                 DeletionEvictionPolicy,
                                                 remu::jitpp::arm::ArmTranslatorFactory,
                                                 remu::jitpp::arm::ArmTranslator,
                                                 NoMmuHostPageSelectionStrategy>;

} /*anonymous namespace*/

void appMain()
{
	mapArmPhysicalMem();

	remu::jitpp::arm::Disassembler dis;
	remu::jitpp::ACState cpu_state;
	Arm11CpuCodeCache *code_cache = new Arm11CpuCodeCache();

	cpu_state.clear();
	cpu_state.ip.program_counter = 0x00008000;

	bool running = true;

	while( running ) {
		auto code_page = code_cache->getPageForFarPointer( cpu_state.ip );
		printf("code_page:  %p (host_page=%p guest_page=%0lx:%lx)\n", code_page, code_page->getHostBase(),
		       cpu_state.ip.code_segment, cpu_state.ip.program_counter );
		running = code_page->execute( cpu_state );
		running = false;
	}

	uint32_t *first_instr = reinterpret_cast<uint32_t*>(cpu_state.ip.program_counter);

	char buffer[64];

	for( int ii = 0; ii < 75; ii++ ) {
		const uint64_t cur_addr = 0x8000 + (sizeof(uint32_t) * ii);
		const uint32_t instr = first_instr[ii];

		dis.disassemble(instr, cur_addr, buffer, 64);

		printf("%08lx : %08x : %s\n", cur_addr, instr, buffer);
	}

	char* ptr1 = new char[1 * 1024 * 1024];
	printf("ptr1 = %p\n", ptr1);
	char* ptr2 = new char[1 * 1024 * 1024];
	printf("ptr2 = %p\n", ptr2);
	delete ptr1;
	printf("free(ptr1)\n");
	ptr1 = new char[10];
	printf("ptr1 = %p\n", ptr1);
}

