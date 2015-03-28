#ifndef REMU_JITPP_ARM_ARMTRANSLATOR_H
#define REMU_JITPP_ARM_ARMTRANSLATOR_H

#include "jitpp/BasicBlock.h"

#include <cstdio>

namespace remu { namespace jitpp { namespace arm {

class ArmTranslator
{
public:
	BasicBlock* generate_basic_block( uint64_t pc, const void *host_base ) {
		int page_offset = pc % 4096;
		const uint64_t page_base = pc & ~4095;
		const uint32_t *page_instrs = reinterpret_cast<const uint32_t*>( host_base );
		bool continue_block = true;
		BasicBlock *basic_block = new BasicBlock;

		while( continue_block ) {
			uint32_t instr = page_instrs[page_offset];
			printf("%lx: %08x\n", page_base + page_offset, instr );
			if( page_offset != 0 ) {
				continue_block = false;
			}
			page_offset += 4;
		}

		return basic_block;
	}
};

}}} /*namespace remu::jitpp::arm*/

#endif /*REMU_JITPP_ARM_ARMTRANSLATOR_H*/

