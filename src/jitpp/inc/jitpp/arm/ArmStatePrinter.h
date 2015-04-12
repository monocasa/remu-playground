#ifndef REMU_JITPP_ARM_ARMSTATEPRINTER_H
#define REMU_JITPP_ARM_ARMSTATEPRINTER_H

#include "jitpp/ACState.h"

#include <cstdio>

namespace remu { namespace jitpp { namespace arm {

class ArmStatePrinter
{
public:
	static void print( const ACState &cpu_state ) {
		const uint32_t* r = cpu_state.l;
		printf(" r0:  %8x |  r1: %8x |  r2: %8x |  r3: %8x\n",  r[ 0], r[ 1], r[ 2], r[ 3] );
		printf(" r4:  %8x |  r5: %8x |  r6: %8x |  r7: %8x\n",  r[ 4], r[ 5], r[ 6], r[ 7] );
		printf(" r8:  %8x |  r9: %8x | r10: %8x | r11: %8x\n",  r[ 8], r[ 9], r[10], r[11] );
		printf(" fp:  %8x |  sp: %8x |  lr: %8x |  pc: %8lx\n", r[12], r[13], r[14], cpu_state.ip.program_counter );
	}
};

}}} /*namespace remu::jitpp::arm*/

#endif /*REMU_JITPP_ARM_ARMSTATEPRINTER_H*/

