#ifndef REMU_JITPP_ARM_ARMTRANSLATOR_H
#define REMU_JITPP_ARM_ARMTRANSLATOR_H

#include "jitpp/BasicBlock.h"

namespace remu { namespace jitpp { namespace arm {

class ArmTranslator
{
public:
	BasicBlock* generate_basic_block( uint64_t pc, void *host_base ) {
		(void)pc;
		(void)host_base;
		return nullptr;
	}
};

}}} /*namespace remu::jitpp::arm*/

#endif /*REMU_JITPP_ARM_ARMTRANSLATOR_H*/

