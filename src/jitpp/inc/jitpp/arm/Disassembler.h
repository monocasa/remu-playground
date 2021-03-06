#ifndef REMU_JITPP_ARM_DISASSEMBLER_H
#define REMU_JITPP_ARM_DISASSEMBLER_H

#include "jitpp/arm/Dissector.h"
#include "jitpp/Disassembler.h"

#include <cstdint>
#include <cstdlib>

namespace remu { namespace jitpp { namespace arm {

class Disassembler : public ::remu::jitpp::Disassembler
                   , private Dissector
{
public:
	void disassemble(uint32_t instr, uint64_t addr, char *buffer, size_t buffer_size);

	size_t disassemble(const util::memslice &slice, 
	                   uint64_t addr, 
	                   char *buffer, 
	                   size_t buffer_size) override final;

private:
	static const int ARGS_SIZE = 64;

	char  *_buffer;
	size_t _buffer_size;
	char   _args[ARGS_SIZE];

	void printInstr(const char *instr, bool s);
	void printInstr(const char *instr, bool s, Dissector::CC cc);

	void noArgs();

	void singleGprArgs(int reg);
	void dualGprArgs(int reg0, int reg1);
	void tripleGprArgs(int reg0, int reg1, int reg2);
	void quadGprArgs(int reg0, int reg1, int reg2, int reg3);

	void logicalImmArgs(int rd, int rn, uint32_t imm);

	void branchTargetArg(uint64_t target);

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

#endif /*REMU_JITPP_ARM_DISASSEMBLER_H*/

