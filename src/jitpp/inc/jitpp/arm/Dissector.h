#ifndef REMU_JITPP_ARM_DISSECTOR_H
#define REMU_JITPP_ARM_DISSECTOR_H

#include <cstdint>

namespace remu { namespace jitpp { namespace arm {

class Dissector
{
public:
	enum CC
	{
		CC_EQ   = 0x00,
		CC_NE   = 0x01,
		CC_CS   = 0x02,
		CC_CC   = 0x03,
		CC_MI   = 0x04,
		CC_PL   = 0x05,
		CC_VS   = 0x06,
		CC_VC   = 0x07,
		CC_HI   = 0x08,
		CC_LS   = 0x09,
		CC_GE   = 0x0A,
		CC_LT   = 0x0B,
		CC_GT   = 0x0C,
		CC_LE   = 0x0D,
		CC_AL   = 0x0E,
		CC_RSVD = 0x0F,
	};

protected:
	void dissect(uint32_t instr, uint64_t addr);

	virtual void onUnknownInstr(uint32_t instr) = 0;

	virtual void onNop() = 0;

	virtual void onBx(CC cc, int rm) = 0;

	virtual void onPld(int rn, uint32_t imm) = 0;

private:
	void onRsvdCondInstr(uint32_t instr, uint64_t addr);
};

}}} /*namespace remu::jitpp::arm*/

#endif /*REMU_JITPP_ARM_DISSECTOR_H*/

