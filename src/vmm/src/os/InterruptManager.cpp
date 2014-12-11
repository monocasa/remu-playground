#include <os/Board.h>
#include <os/InterruptManager.h>
#include <os/MemoryManager.h>

#include <cstdint>
#include <cstdio>

extern "C" void pagefault_prologue();
extern "C" void breakpoint_prologue();

namespace {

static const int NUM_IDT_ENTRIES = 256;

static const int BREAKPOINT_ISR_NUM = 3;
static const int PAGEFAULT_ISR_NUM = 14;

struct Idtr
{
	uint16_t limit;
	uint64_t base;
} __attribute__((__packed__));

struct IdtEntry
{
	static const uint16_t PRESENT_FLAG        = 0x8000;
	static const uint16_t INTERRUPT_GATE_FLAG = 0x0E00;

	uint16_t offset_15_0;
	uint16_t selector;
	uint16_t flags;
	uint16_t offset_31_16;
	uint32_t offset_63_32;
	uint32_t rsvd;

	void clear() {
		offset_15_0 = 0;
		selector = 0;
		flags = 0;
		offset_31_16 = 0;
		offset_63_32 = 0;
		rsvd = 0;
	}

	void set( void(*isr)() ) {
		uint64_t isr_ptr = reinterpret_cast<uint64_t>( isr );

		offset_15_0  = isr_ptr;
		offset_31_16 = isr_ptr >> 16;
		offset_63_32 = isr_ptr >> 32;

		selector = 0x08; // CS_SELECTOR

		flags = PRESENT_FLAG | INTERRUPT_GATE_FLAG;

		rsvd = 0;
	}

} __attribute__((__packed__));

IdtEntry *idt_entries = nullptr;

Idtr idtr;

void populate_idt_entries()
{
	for( int ii = 0; ii < NUM_IDT_ENTRIES; ii++ ) {
		idt_entries[ii].clear();
	}

	idt_entries[ BREAKPOINT_ISR_NUM ].set( &breakpoint_prologue );
	idt_entries[ PAGEFAULT_ISR_NUM ].set( &pagefault_prologue );
}

void install_idt()
{
	idtr.limit = (NUM_IDT_ENTRIES * sizeof(IdtEntry)) - 1;
	idtr.base = reinterpret_cast<uint64_t>( idt_entries );

	printf( "idt(%p){ limit=%d, base=%lx }\n", &idtr, idtr.limit, idtr.base );

	asm volatile( "lidt %0" : : "m"(idtr) );
}

} /*anonymous namespace*/

namespace os { namespace intm {

void fault_handler( os::intm::Context *ctx )
{
	switch( ctx->intNum ) {
		case PAGEFAULT_ISR_NUM: {
			os::mm::on_page_fault( ctx );
		}
		break;

		default: {
			printf( "Unknown Interrupt Number %d\n", ctx->intNum );

			ctx->print();

			os::board::shutdown();
		}
		break;
	}
}

void Context::print()
{
	printf( "rax: %16lx | rbx: %16lx | rcx: %16lx | rdx: %16lx\n", rax, rbx, rcx, rdx );
	printf( "rdi: %16lx | rsi: %16lx | rbp: %16lx | rsp: %16lx\n", rdi, rsi, rbp, rsp );
	printf( " r8: %16lx |  r9: %16lx | r10: %16lx | r11: %16lx\n", r8, r9, r10, r11 );
	printf( "r12: %16lx | r13: %16lx | r14: %16lx | r15: %16lx\n", r12, r13, r14, r15 );
	printf( "rip: %16lx\n", rip );
}

void disable_interrupts()
{
	asm volatile( "cli" );
}

void enable_interrupts()
{
	asm volatile( "sti" );
}

void init()
{
	::idt_entries = reinterpret_cast<::IdtEntry*>( os::mm::allocate_page() );

	printf( "idt_entries @ %p\n", ::idt_entries );

	::populate_idt_entries();
	::install_idt();

	enable_interrupts();
}

}} /*namespace os::intm*/

