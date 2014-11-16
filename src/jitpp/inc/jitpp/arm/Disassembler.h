#ifndef REMU_JITPP_ARM_DISASSEMBLER_H
#define REMU_JITPP_ARM_DISASSEMBLER_H

#include "jitpp/arm/Dissector.h"

#include <cstdint>
#include <cstdlib>

namespace remu { namespace jitpp { namespace arm {

class Disassembler : private Dissector
{
public:
	void disassemble(uint32_t instr, uint64_t addr, char *buffer, size_t buffer_size);

private:
	char  *_buffer;
	size_t _buffer_size;

	void onUnknownInstr(uint32_t instr) override final;

	void onPld(int rn, uint32_t imm) override final;
};

}}} /*namespace remu::jitpp::arm*/

#endif /*REMU_JITPP_ARM_DISASSEMBLER_H*/

