#include "os/Log.h"
#include "os/MemoryManager.h"

uint64_t *emulation_pml2 = nullptr;
uint64_t *emulation_pml3 = nullptr;

void appMain()
{
	emulation_pml2 = (uint64_t*)os::mm::allocate_page();
	emulation_pml3 = (uint64_t*)os::mm::allocate_page();

	const int NUM_PAGE_TABLE_ENTRIES_PER_PAGE = (4096 / sizeof(uint64_t));

	for( int ii = 0; ii < NUM_PAGE_TABLE_ENTRIES_PER_PAGE; ii++ ) {
		emulation_pml3[ii] = 0;
	}

	for( int ii = 0; ii < NUM_PAGE_TABLE_ENTRIES_PER_PAGE; ii++ ) {
		const uint64_t PHYS_PAGE = ((2 * 1024 * 1024) * ii) + 0x100000000UL;
		emulation_pml2[ii] = PHYS_PAGE | 0x83;
	}

	const uint64_t pml2_phys_addr = ((uint64_t)emulation_pml2) - 0xFFFFFFFF80000000UL;

	emulation_pml3[0] = pml2_phys_addr | 1;

	os::mm::set_lower_pml3(emulation_pml3, 0);

	uint32_t *first_instr = reinterpret_cast<uint32_t*>(0x00010000);

	os::log("instr_ptr(%p) = %08x\n", first_instr, *first_instr);

	uint32_t *timer_low = reinterpret_cast<uint32_t*>(0x20003004);

	os::log("bcm2835 TIMER_LOW(%p) = %08x\n", timer_low, *timer_low);
}

