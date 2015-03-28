#ifndef REMU_JITPP_ARM_ARMTRANSLATORFACTORY_H
#define REMU_JIPTT_ARM_ARMTRANSLATORFACTORY_H

#include "jitpp/arm/ArmTranslator.h"

#include <cstdint>

namespace remu { namespace jitpp { namespace arm {

class ArmTranslatorFactory
{
public:
	ArmTranslator& translator_for_code_segment( uint64_t code_segment ) {
		(void)code_segment;
		return a32_translator;
	}

private:
	ArmTranslator a32_translator;
};

}}} /*namespace remu::jitpp::arm*/

#endif /*REMU_JITPP_ARM_ARMTRANSLATORFACTORY_H*/

