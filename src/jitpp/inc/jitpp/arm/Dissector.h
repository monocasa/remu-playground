#ifndef REMU_JITPP_ARM_DISSECTOR_H
#define REMU_JITPP_ARM_DISSECTOR_H

#include <cstdint>

namespace remu { namespace jitpp { namespace arm {

class Dissector
{
protected:
	void dissect(uint32_t instr, uint64_t addr);

	virtual void onUnknownInstr(uint32_t instr) = 0;

	virtual void onNop() = 0;

	virtual void onPld(int rn, uint32_t imm) = 0;

private:
	void onRsvdCondInstr(uint32_t instr, uint64_t addr);
};

}}} /*namespace remu::jitpp::arm*/

#endif /*REMU_JITPP_ARM_DISSECTOR_H*/

