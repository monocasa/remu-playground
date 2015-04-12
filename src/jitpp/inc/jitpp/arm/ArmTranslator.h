#ifndef REMU_JITPP_ARM_ARMTRANSLATOR_H
#define REMU_JITPP_ARM_ARMTRANSLATOR_H

#include "jitpp/arm/Dissector.h"
#include "jitpp/BasicBlock.h"

#include <cstdio>

namespace remu { namespace jitpp { namespace arm {

class ArmTranslator : private Dissector
{
public:
	BasicBlock* generate_basic_block( uint64_t pc, const void *host_base ) {
		int page_offset = pc % PAGE_SIZE;
		const uint64_t page_base = pc & ~(PAGE_SIZE - 1);
		const uint32_t *page_instrs = reinterpret_cast<const uint32_t*>( host_base );

		continue_block = true;

		cur_basic_block = new BasicBlock;

		while( continue_block ) {
			uint32_t instr = page_instrs[page_offset / sizeof(uint32_t)];

			cur_pc = page_offset + page_base;
			dissect(instr, page_base + page_offset);

			page_offset += 4;

			if( page_offset >= PAGE_SIZE ) {
				continue_block = false;
			}
		}

		return cur_basic_block;
	}

private:
	static const int PAGE_SIZE = 4096;

	bool continue_block;
	BasicBlock *cur_basic_block;
	uint64_t cur_pc;

	void onUnknownInstr(uint32_t instr) override final;

	void onNop() override final;

	void onAddImm(CC cc, bool s, int rd, int rn, uint32_t imm) override final;
	void onAndImm(CC cc, bool s, int rd, int rn, uint32_t imm) override final;
	void onB(CC cc, uint64_t target) override final;
	void onBicImm(CC cc, int rd, int rn, uint32_t imm) override final;
	void onBl(CC cc, uint64_t target) override final; 
	void onBx(CC cc, int rm) override final;
	void onCmpImm(CC cc, int rn, uint32_t imm) override final;
	void onLdrImm(CC cc, int rt, int rn, int32_t off) override final;
	void onMovImm(CC cc, bool s, int rd, uint32_t imm) override final;
	void onMovReg(CC cc, bool s, int rd, int rm) override final;
	void onMla(CC cc, bool s, int rd, int rn, int rm, int ra) override final;
	void onMul(CC cc, bool s, int rd, int rn, int rm) override final;
	void onOrrImm(CC cc, int rd, int rn, uint32_t imm) override final;
	void onPld(int rn, uint32_t imm) override final;
	void onStmfd(CC cc, int rn, bool w, uint16_t reg_list) override final;
	void onStrImm(CC cc, int rt, int rn, int32_t off) override final;
	void onSvc(CC cc, uint32_t imm) override final;
	void onTstImm(CC cc, int rn, uint32_t imm) override final;
	void onUmull(CC cc, bool s, int rdlo, int rdhi, int rn, int rm) override final;
};

}}} /*namespace remu::jitpp::arm*/

#endif /*REMU_JITPP_ARM_ARMTRANSLATOR_H*/

