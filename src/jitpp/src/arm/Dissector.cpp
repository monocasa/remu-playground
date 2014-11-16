#include "jitpp/arm/Dissector.h"

#include <cstdio>

using remu::jitpp::arm::Dissector;

namespace {

template<typename T, unsigned B>
static inline T signExtend(const T x) {
  struct {
    T x:B;
  } s;

  return s.x = x;
}

Dissector::CC getCc(uint32_t instr)
{
	return (Dissector::CC)((instr >> 28) & 0xF);
}

int getReg0(uint32_t instr)
{
	return instr & 0xF;
}

int getReg2(uint32_t instr)
{
	return (instr >> 8) & 0xF;
}

int getReg3(uint32_t instr)
{
	return (instr >> 12) & 0xF;
}

int getReg4(uint32_t instr)
{
	return (instr >> 16) & 0xF;
}

uint64_t getBranchOffset(uint32_t instr, uint64_t addr)
{
	const int32_t offset = (signExtend<int32_t,24>(instr) * 4) + 8;
	return addr + offset;
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
			onBx(getCc(instr), getReg0(instr));
		}
		else if( (instr & 0x0FF0F0F0) == 0x00000090 ) {
			onMul(getCc(instr), false, getReg4(instr), getReg0(instr), getReg2(instr));
		}
		else if( (instr & 0x0FF0F0F0) == 0x00100090 ) {
			onMul(getCc(instr), true, getReg4(instr), getReg0(instr), getReg2(instr));
		}
		else if( (instr & 0x0FF000F0) == 0x00200090 ) {
			onMla(getCc(instr), false, getReg4(instr), getReg0(instr), getReg2(instr), getReg3(instr));
		}
		else if( (instr & 0x0FF000F0) == 0x00300090 ) {
			onMla(getCc(instr), true, getReg4(instr), getReg0(instr), getReg2(instr), getReg3(instr));
		}
		else if( (instr & 0x0FF000F0) == 0x00800090 ) {
			onUmull(getCc(instr), false, getReg3(instr), getReg4(instr), getReg0(instr), getReg2(instr));
		}
		else if( (instr & 0x0FF000F0) == 0x00900090 ) {
			onUmull(getCc(instr), true, getReg3(instr), getReg4(instr), getReg0(instr), getReg2(instr));
		}
		else {
			onUnknownInstr(instr);
		}
	}
	return;

	case 0xA: {
		onB(getCc(instr), getBranchOffset(instr, addr));
	}
	return;

	case 0xB: {
		onBl(getCc(instr), getBranchOffset(instr, addr));
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

