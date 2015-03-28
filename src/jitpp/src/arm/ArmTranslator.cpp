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
	printf("AddImm(cc=%x, s=%d, rd=%d, rn=%d, imm=0x%x)\n", cc, s, rd, rn, imm);
}

void ArmTranslator::onAndImm(Dissector::CC cc, bool s, int rd, int rn, uint32_t imm)
{
	printf("AndImm(cc=%x, s=%d, rd=%d, rn=%d, imm=0x%x)\n", cc, s, rd, rn, imm);
}

void ArmTranslator::onB(Dissector::CC cc, uint64_t target)
{
	printf("B(cc=%x, target=%lx)\n", cc, target);
	continue_block = false;
}

void ArmTranslator::onBicImm(Dissector::CC cc, int rd, int rn, uint32_t imm)
{
	printf("BicImm(cc=%x, rd=%d, rn=%d, imm=0x%x)\n", cc, rd, rn, imm);
}

void ArmTranslator::onBl(Dissector::CC cc, uint64_t target)
{
	printf("Bl(cc=%x, target=%lx)\n", cc, target);
	continue_block = false;
} 

void ArmTranslator::onBx(Dissector::CC cc, int rm)
{
	printf("Bx(cc=%x, rm=%d)\n", cc, rm);
	continue_block = false;
}

void ArmTranslator::onCmpImm(Dissector::CC cc, int rn, uint32_t imm)
{
	printf("CmpImm(cc=%x, rn=%d, imm=0x%x)\n", cc, rn, imm);
}

void ArmTranslator::onLdrImm(Dissector::CC cc, int rt, int rn, int32_t off)
{
	printf("LdrImm(cc=%x, rt=%d, rn=%d, off=%d\n", cc, rt, rn, off);
}

void ArmTranslator::onMovImm(Dissector::CC cc, bool s, int rd, uint32_t imm)
{
	printf("MovImm(cc=%x, s=%d, rd=%d, imm=0x%x)\n", cc, s, rd, imm);
}

void ArmTranslator::onMovReg(Dissector::CC cc, bool s, int rd, int rm)
{
	printf("MovReg(cc=%x, s=%d, rd=%d, rm=%d)\n", cc, s, rd, rm);
}

void ArmTranslator::onMla(Dissector::CC cc, bool s, int rd, int rn, int rm, int ra)
{
	printf("Mla(cc=%x, s=%d, rd=%d, rn=%d, rm=%d, ra=%d)\n", cc, s, rd, rn, rm, ra);
}

void ArmTranslator::onMul(Dissector::CC cc, bool s, int rd, int rn, int rm)
{
	printf("Mul(cc=%x, s=%d, rd=%d, rn=%d, rm=%d\n", cc, s, rd, rn, rm);
}

void ArmTranslator::onOrrImm(Dissector::CC cc, int rd, int rn, uint32_t imm)
{
	printf("OrrImm(cc=%x, rd=%d, rn=%d, imm=0x%x)\n", cc, rd, rn, imm);
}

void ArmTranslator::onPld(int rn, uint32_t imm)
{
	printf("Pld(rn=%d, imm=0x%x)\n", rn, imm);
}

void ArmTranslator::onStmfd(Dissector::CC cc, int rn, bool w, uint16_t reg_list)
{
	printf("Stmfd(cc=%x, rn=%d, w=%d, reg_list=%04x", cc, rn, w, reg_list);
}

void ArmTranslator::onStrImm(Dissector::CC cc, int rt, int rn, int32_t off)
{
	printf("StrImm(cc=%x, rt=%d, rn=%d, off=%d)\n", cc, rt, rn, off);
}

void ArmTranslator::onSvc(Dissector::CC cc, uint32_t imm)
{
	printf("Svc(cc=%x, imm=0x%x)\n", cc, imm);
	continue_block = false;
}

void ArmTranslator::onTstImm(Dissector::CC cc, int rn, uint32_t imm)
{
	printf("TstImm(cc=%x, rn=%d, imm=0x%x)\n", cc, rn, imm);
}

void ArmTranslator::onUmull(Dissector::CC cc, bool s, int rdlo, int rdhi, int rn, int rm)
{
	printf("Umull(cc=%x, s=%d, rdlo=%d, rdhi=%d, rn=%d, rm=%d)\n", cc, s, rdlo, rdhi, rn, rm);
}

}}} /*namespace remu::jitpp::arm*/

