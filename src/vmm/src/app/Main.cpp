#include "jitpp/arm/ArmStatePrinter.h"
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
	}

	remu::jitpp::arm::ArmStatePrinter::print( cpu_state );
}

