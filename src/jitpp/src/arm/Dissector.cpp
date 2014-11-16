#include "jitpp/arm/Dissector.h"

#include <cstdio>

using remu::jitpp::arm::Dissector;

namespace {

Dissector::CC getCc(uint32_t instr)
{
	return (Dissector::CC)((instr >> 28) & 0xF);
}

int getRm(uint32_t instr)
{
	return instr & 0xF;
}

} /*anonymous namespace*/

namespace remu { namespace jitpp { namespace arm {

void Dissector::dissect(uint32_t instr, uint64_t addr)
{
	if( instr == 0x00000000 ) {
		onNop();
		return;
	}

	if(CC_RSVD == getCc(instr)) {
		onRsvdCondInstr(instr, addr);
		return;
	}

	switch( (instr >> 24) & 0xF )
	{

	case 0: case 1: case 2: case 3: {
		if( (instr & 0x0FFFFFF0) == 0x012FFF10 ) {
			onBx(getCc(instr), getRm(instr));
		}
		else {
			onUnknownInstr(instr);
		}
	}
	return;

	default: {
		onUnknownInstr(instr);
	}
	return;

	}
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

