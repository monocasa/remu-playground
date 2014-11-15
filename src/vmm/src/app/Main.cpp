#include "os/Log.h"
#include "os/MemoryManager.h"

uint64_t *emulation_pml3 = nullptr;
uint64_t *emulation_pml2 = nullptr;

void appMain()
{
	emulation_pml2 = (uint64_t*)os::mm::allocate_page();
	emulation_pml3 = (uint64_t*)os::mm::allocate_page();

	for( unsigned int ii = 0; ii < (4096 / sizeof(uint64_t)); ii++ ) {
		emulation_pml2[ii] = 0;
		emulation_pml3[ii] = 0;
	}

	emulation_pml2[0] = 0x100000000UL | 0x83;

	const uint64_t pml2_phys_addr = ((uint64_t)emulation_pml2) - 0xFFFFFFFF80000000UL;

	emulation_pml3[0] = pml2_phys_addr | 1;

	os::mm::set_lower_pml3(emulation_pml3, 0);

	uint32_t *first_instr = reinterpret_cast<uint32_t*>(0x00010000);

	os::log("instr_ptr(%p) = %08x", first_instr, *first_instr);
}

