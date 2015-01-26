#include "jitpp/arm/Disassembler.h"

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

} /*anonymous namespace*/

void appMain()
{
	mapArmPhysicalMem();

	remu::jitpp::arm::Disassembler dis;

	uint32_t *first_instr = reinterpret_cast<uint32_t*>(0x00008000);

	char buffer[64];

	for( int ii = 0; ii < 75; ii++ ) {
		const uint64_t cur_addr = 0x8000 + (sizeof(uint32_t) * ii);
		const uint32_t instr = first_instr[ii];

		dis.disassemble(instr, cur_addr, buffer, 64);

		printf("%08lx : %08x : %s\n", cur_addr, instr, buffer);
	}
}

