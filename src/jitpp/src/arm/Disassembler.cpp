#include "jitpp/arm/Disassembler.h"

#include <cstdio>

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

void Disassembler::onPld(int rn, uint32_t imm)
{
	::snprintf(_buffer, _buffer_size, "%-8s [r%d, #0x%x]", "pld", rn, imm);
}

}}} /*namespace remu::jitpp::arm*/

