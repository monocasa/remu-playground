#include "jitpp/arm/Disassembler.h"

#include <cstdio>

using remu::jitpp::arm::Dissector;

namespace {

const char *reg_names[16]
{
	"r0",  "r1",  "r2",  "r3",
	"r4",  "r5",  "r6",  "r7",
	"r8",  "r9",  "r10", "r11",
	"r12", "r13", "r14", "r15",
};

const char *getRegName(int reg)
{
	if( reg < 16 ) {
		return reg_names[reg];
	}
	else {
		return "UNK";
	}
}

const char *cc_names[16]
{
	"eq", "ne", "cs", "cc",
	"mi", "pl", "vs", "vc",
	"hi", "ls", "ge", "lt",
	"gt", "le", "",   "RSV"
};

const char *getCcName(Dissector::CC cc)
{
	if( cc < 16 ) {
		return cc_names[cc];
	}
	else {
		return "UNK";
	}
}

} /*anonymous namespace*/

namespace remu { namespace jitpp { namespace arm {

void Disassembler::disassemble(uint32_t instr, uint64_t addr, char *buffer, size_t buffer_size)
{
	_buffer = buffer;
	_buffer_size = buffer_size;

	dissect(instr, addr);
}

void Disassembler::onUnknownInstr(uint32_t instr)
{
	::snprintf(_buffer, _buffer_size, "UNKNOWN_INSTR<0x%08x>", instr);
}

void Disassembler::onNop()
{
	::snprintf(_buffer, _buffer_size, "nop");
}

void Disassembler::onBx(CC cc, int rm)
{
	char name[8];

	::sprintf(name, "bx%s", getCcName(cc));

	::snprintf(_buffer, _buffer_size, "%-8s r%d", name, rm);
}

void Disassembler::onPld(int rn, uint32_t imm)
{
	::snprintf(_buffer, _buffer_size, "%-8s [%s, #0x%x]", "pld", getRegName(rn), imm);
}

}}} /*namespace remu::jitpp::arm*/

