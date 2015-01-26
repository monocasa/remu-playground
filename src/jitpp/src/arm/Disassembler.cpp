#include "jitpp/arm/Disassembler.h"

#include <cstdio>
#include <cstring>

using remu::jitpp::arm::Dissector;

namespace {

const char *reg_names[16]
{
	"r0",  "r1",  "r2",  "r3",
	"r4",  "r5",  "r6",  "r7",
	"r8",  "r9",  "r10", "r11",
	"r12", "sp",  "lr",  "pc",
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

void printRegList(char *buffer, uint16_t reg_list)
{
	bool first_written = false;

	if( reg_list == 0 ) {
		buffer[0] = '\0';
		return;
	}

	for( int i = 0; i < 16; i++ ) {
		if( reg_list & 1 ) {
			if( !first_written ) {
				buffer += sprintf(buffer, "%s", getRegName(i));
				first_written = true;
			}
			else {
				buffer += sprintf(buffer, ", %s", getRegName(i));
			}
		}
		reg_list >>= 1;
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

void Disassembler::printInstr(const char *instr, bool s)
{
	printInstr(instr, s, CC_AL);
}

void Disassembler::printInstr(const char *instr, bool s, CC cc)
{
	char instr_name[9];
	::snprintf(instr_name, 9, "%s%s%s", 
	           instr,
	           s ? "s" : "",
	           getCcName(cc) );

	::snprintf(_buffer, _buffer_size, "%-8s %s", instr_name, _args);
}

void Disassembler::noArgs()
{
	::strcpy(_args, "");
}

void Disassembler::singleGprArgs(int reg)
{
	::snprintf(_args, ARGS_SIZE, "%s", getRegName(reg));
}

void Disassembler::tripleGprArgs(int reg0, int reg1, int reg2)
{
	::snprintf(_args, ARGS_SIZE, "%s, %s, %s", 
	           getRegName(reg0), 
	           getRegName(reg1), 
	           getRegName(reg2));
}

void Disassembler::quadGprArgs(int reg0, int reg1, int reg2, int reg3)
{
	::snprintf(_args, ARGS_SIZE, "%s, %s, %s, %s", 
	           getRegName(reg0), 
	           getRegName(reg1), 
	           getRegName(reg2), 
	           getRegName(reg3));
}

void Disassembler::branchTargetArg(uint64_t target)
{
	::snprintf(_args, ARGS_SIZE, "loc_%lx", target);
}

void Disassembler::onUnknownInstr(uint32_t instr)
{
	::snprintf(_buffer, _buffer_size, "UNKNOWN_INSTR<0x%08x>", instr);
}

void Disassembler::onNop()
{
	::snprintf(_buffer, _buffer_size, "nop");
}

void Disassembler::onB(CC cc, uint64_t target)
{
	branchTargetArg(target);

	printInstr("b", false, cc);
}

void Disassembler::onBl(CC cc, uint64_t target)
{
	branchTargetArg(target);

	printInstr("bl", false, cc);
}

void Disassembler::onBx(CC cc, int rm)
{
	singleGprArgs(rm);

	printInstr("bx", false, cc);
}

void Disassembler::onLdrImm(CC cc, int rt, int rn, int32_t off)
{
	if( off == 0 ) {
		::sprintf(_args, "%s, [%s]", getRegName(rt), getRegName(rn));
	}
	else {
		::sprintf(_args, "%s, [%s, #%d]", getRegName(rt), getRegName(rn), off);
	}

	printInstr("ldr", false, cc);
}

void Disassembler::onMla(CC cc, bool s, int rd, int rn, int rm, int ra)
{
	quadGprArgs(rd, rn, rm, ra);

	printInstr("mla", s, cc);
}

void Disassembler::onMovImm(CC cc, bool s, int rd, uint32_t imm)
{
	::sprintf(_args, "%s, #0x%x", getRegName(rd), imm);

	printInstr("mov", s, cc);
}

void Disassembler::onMul(CC cc, bool s, int rd, int rn, int rm)
{
	tripleGprArgs(rd, rn, rm);

	printInstr("mul", s, cc);
}

void Disassembler::onPld(int rn, uint32_t imm)
{
	::sprintf(_args,"[%s, #0x%x]", getRegName(rn), imm);

	printInstr("pld", false);
}

void Disassembler::onStmfd(CC cc, int rn, bool w, uint16_t reg_list)
{
	char reg_list_buffer[64];

	printRegList(reg_list_buffer, reg_list);

	if( w && rn == 13 ) {
		::sprintf(_args, "{%s}", reg_list_buffer);

		printInstr("push", false, cc);
	}
	else {
		::sprintf(_args, "%s%s, {%s}", getRegName(rn), w ? "!" : "", reg_list_buffer);

		printInstr("stmfd", false, cc);
	}
}

void Disassembler::onStrImm(CC cc, int rt, int rn, int32_t off)
{
	if( off == 0 ) {
		::sprintf(_args, "%s, [%s]", getRegName(rt), getRegName(rn));
	}
	else {
		::sprintf(_args, "%s, [%s, #%d]", getRegName(rt), getRegName(rn), off);
	}

	printInstr("str", false, cc);
}

void Disassembler::onSvc(CC cc, uint32_t imm)
{
	::sprintf(_args, "#0x%x", imm);

	printInstr("svc", false, cc);
}

void Disassembler::onUmull(CC cc, bool s, int rdlo, int rdhi, int rn, int rm)
{
	quadGprArgs(rdlo, rdhi, rn, rm);

	printInstr("umull", s, cc);
}

}}} /*namespace remu::jitpp::arm*/

