#include "jitpp/arm/Dissector.h"

#include <cstdio>

namespace {

const int COND_RSVD = 0xF;

int getCond(uint32_t instr)
{
	return (instr >> 28) & 0xF;
}

} /*anonymous namespace*/

namespace remu { namespace jitpp { namespace arm {

void Dissector::dissect(uint32_t instr, uint64_t addr)
{
	if( instr == 0x00000000 ) {
		onNop();
		return;
	}

	if(COND_RSVD == getCond(instr)) {
		onRsvdCondInstr(instr, addr);
		return;
	}

	onUnknownInstr(instr);
}

void Dissector::onRsvdCondInstr(uint32_t instr, uint64_t addr)
{
	(void)addr;

	switch( instr & 0x0FF0F000 )
	{

	case 0x05D0F000: {
		onPld( ((instr >> 16) & 0xF), instr & 0x00000FFF );
	}
	return;

	default: {
		printf("instr & 0xFFF0F000 = %08x\n", instr & 0xFFF0F000);

		onUnknownInstr(instr);
	}
	return;

	}
}

}}} /*namespace remu::jitpp:arm*/

