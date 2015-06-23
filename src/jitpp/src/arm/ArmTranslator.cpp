#include "jitpp/arm/ArmTranslator.h"

namespace remu { namespace jitpp { namespace arm {

void ArmTranslator::onUnknownInstr(uint32_t instr)
{
	printf("UnknownInstr:  %08x\n", instr );
	continue_block = false;
}

void ArmTranslator::onNop()
{
	printf("Nop\n" );
}

void ArmTranslator::onAddImm(Dissector::CC cc, bool s, int rd, int rn, uint32_t imm)
{
	if( cc != Dissector::CC_AL || s || rd == 15 || rn == 15 ) {
		printf("OrrImm(cc=%x, s=%d, rd=%d, rn=%d, imm=0x%x)\n", cc, s, rd, rn, imm);
		cur_basic_block->instrs.push_back( ACInstr::emitUnimplemented() );
	}
	else {
		cur_basic_block->instrs.push_back( ACInstr::emitAddU32(ACOpType::L, rd, ACOpType::L, rn, imm) );
	}
}

void ArmTranslator::onAndImm(Dissector::CC cc, bool s, int rd, int rn, uint32_t imm)
{
	printf("AndImm(cc=%x, s=%d, rd=%d, rn=%d, imm=0x%x)\n", cc, s, rd, rn, imm);
	cur_basic_block->instrs.push_back( ACInstr::emitUnimplemented() );
}

void ArmTranslator::onB(Dissector::CC cc, uint64_t target)
{
	printf("B(cc=%x, target=%lx)\n", cc, target);
	continue_block = false;
}

void ArmTranslator::onBicImm(Dissector::CC cc, int rd, int rn, uint32_t imm)
{
	if( cc != Dissector::CC_AL || rd == 15 || rn == 15 ) {
		printf("BicImm(cc=%x, rd=%d, rn=%d, imm=0x%x)\n", cc, rd, rn, imm);
		cur_basic_block->instrs.push_back( ACInstr::emitUnimplemented() );
	}
	else {
		cur_basic_block->instrs.push_back( ACInstr::emitAndU32(ACOpType::L, rd, ACOpType::L, rn, ~imm) );
	}
}

void ArmTranslator::onBl(Dissector::CC cc, uint64_t target)
{
	if( cc != Dissector::CC_AL ) {
		cur_basic_block->instrs.push_back( ACInstr::emitUnimplemented() );
	}
	else {
		cur_basic_block->instrs.push_back( ACInstr::emitLdrImmU32(ACOpType::L, 14, cur_pc + 4) );
		cur_basic_block->instrs.push_back( ACInstr::emitB32(target) );
	}
	continue_block = false;
}

void ArmTranslator::onBx(Dissector::CC cc, int rm)
{
	if( cc != Dissector::CC_AL || rm == 15 ) {
		printf("Bx(cc=%x, rm=%d)\n", cc, rm);
	}
	else {
		cur_basic_block->instrs.push_back( ACInstr::emitBX(ACOpType::L, rm) );
	}
	continue_block = false;
}

void ArmTranslator::onCmpImm(Dissector::CC cc, int rn, uint32_t imm)
{
	printf("CmpImm(cc=%x, rn=%d, imm=0x%x)\n", cc, rn, imm);
	cur_basic_block->instrs.push_back( ACInstr::emitUnimplemented() );
}

void ArmTranslator::onLdrImm(Dissector::CC cc, int rt, int rn, int32_t off)
{
	if( cc != Dissector::CC_AL ) {
		cur_basic_block->instrs.push_back( ACInstr::emitUnimplemented() );
	}
	else if( 15 == rn ) {
		cur_basic_block->instrs.push_back( ACInstr::emitLdrAddr32(ACOpType::L, rt, cur_pc + 8 + off) );
	}
	else {
		cur_basic_block->instrs.push_back( ACInstr::emitLdrAddrS32(ACOpType::L, rt, ACOpType::L, rn, off) );
	}
}

void ArmTranslator::onMovImm(Dissector::CC cc, bool s, int rd, uint32_t imm)
{
	if( cc != Dissector::CC_AL || s ) {
		cur_basic_block->instrs.push_back( ACInstr::emitUnimplemented() );
	}
	else {
		cur_basic_block->instrs.push_back( ACInstr::emitLdrImmU32(ACOpType::L, rd, imm) );
	}
}

void ArmTranslator::onMovReg(Dissector::CC cc, bool s, int rd, int rm)
{
	if( cc != Dissector::CC_AL || s || rd == 15 || rm == 15 ) {
		printf("MovReg(cc=%x, s=%d, rd=%d, rm=%d)\n", cc, s, rd, rm);
		cur_basic_block->instrs.push_back( ACInstr::emitUnimplemented() );
	}
	else {
		cur_basic_block->instrs.push_back( ACInstr::emitLdrReg(ACOpType::L, rd, ACOpType::L, rm) );
	}
}

void ArmTranslator::onMla(Dissector::CC cc, bool s, int rd, int rn, int rm, int ra)
{
	printf("Mla(cc=%x, s=%d, rd=%d, rn=%d, rm=%d, ra=%d)\n", cc, s, rd, rn, rm, ra);
	cur_basic_block->instrs.push_back( ACInstr::emitUnimplemented() );
}

void ArmTranslator::onMul(Dissector::CC cc, bool s, int rd, int rn, int rm)
{
	printf("Mul(cc=%x, s=%d, rd=%d, rn=%d, rm=%d\n", cc, s, rd, rn, rm);
	cur_basic_block->instrs.push_back( ACInstr::emitUnimplemented() );
}

void ArmTranslator::onOrrImm(Dissector::CC cc, int rd, int rn, uint32_t imm)
{
	if( cc != Dissector::CC_AL || rd == 15 || rn == 15 ) {
		printf("OrrImm(cc=%x, rd=%d, rn=%d, imm=0x%x)\n", cc, rd, rn, imm);
		cur_basic_block->instrs.push_back( ACInstr::emitUnimplemented() );
	}
	else {
		cur_basic_block->instrs.push_back( ACInstr::emitOrU32(ACOpType::L, rd, ACOpType::L, rn, imm) );
	}
}

void ArmTranslator::onPld(int rn, uint32_t imm)
{
	printf("Pld(rn=%d, imm=0x%x)\n", rn, imm);
	cur_basic_block->instrs.push_back( ACInstr::emitUnimplemented() );
}

void ArmTranslator::onStmfd(Dissector::CC cc, int rn, bool w, uint16_t reg_list)
{
	int32_t offset = -4;
	int num_regs = 0;

	if( cc != Dissector::CC_AL ) {
		cur_basic_block->instrs.push_back( ACInstr::emitUnimplemented() );
		continue_block = false;
		return;
	}

	for( int reg = 0; reg < 15; reg++ ) {
		if( (reg_list >> reg) & 1 ) {
			cur_basic_block->instrs.push_back( ACInstr::emitStrAddrS32(ACOpType::L, reg, ACOpType::L, rn, offset) );
			offset -= 4;
			num_regs++;
		}
	}

	if( reg_list >> 15 ) {
		cur_basic_block->instrs.push_back( ACInstr::emitUnimplemented() );
		continue_block = false;
		return;
	}

	if( w ) {
		cur_basic_block->instrs.push_back( ACInstr::emitSubS32(ACOpType::L, rn, ACOpType::L, rn, num_regs * sizeof(uint32_t) ) );
	}
}

void ArmTranslator::onStrImm(Dissector::CC cc, int rt, int rn, int32_t off)
{
	if( cc != Dissector::CC_AL || rt == 15 || rn == 15 ) {
		printf("StrImm(cc=%x, rt=%d, rn=%d, off=%d)\n", cc, rt, rn, off);
		cur_basic_block->instrs.push_back( ACInstr::emitUnimplemented() );
		continue_block = false;
		return;
	}

	cur_basic_block->instrs.push_back( ACInstr::emitStrAddrS32(ACOpType::L, rt, ACOpType::L, rn, off) );
}

void ArmTranslator::onSvc(Dissector::CC cc, uint32_t imm)
{
	printf("Svc(cc=%x, imm=0x%x)\n", cc, imm);
	continue_block = false;
}

void ArmTranslator::onTstImm(Dissector::CC cc, int rn, uint32_t imm)
{
	printf("TstImm(cc=%x, rn=%d, imm=0x%x)\n", cc, rn, imm);
	cur_basic_block->instrs.push_back( ACInstr::emitUnimplemented() );
}

void ArmTranslator::onUmull(Dissector::CC cc, bool s, int rdlo, int rdhi, int rn, int rm)
{
	printf("Umull(cc=%x, s=%d, rdlo=%d, rdhi=%d, rn=%d, rm=%d)\n", cc, s, rdlo, rdhi, rn, rm);
	cur_basic_block->instrs.push_back( ACInstr::emitUnimplemented() );
}

}}} /*namespace remu::jitpp::arm*/

