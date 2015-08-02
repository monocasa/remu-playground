#ifndef REMU_JITPP_PPC_DISASSEMBLER_H
#define REMU_JITPP_PPC_DISASSEMBLER_H

#include "jitpp/ppc/Dissector.h"
#include "jitpp/Disassembler.h"

namespace remu { namespace jitpp { namespace ppc {

class Disassembler : public ::remu::jitpp::Disassembler,
                     private Dissector
{
public:
	virtual ~Disassembler() = default;

	size_t disassemble( const util::memslice &slice,
	                    uint64_t addr,
	                    char *buffer,
	                    size_t buffer_size ) override final;

private:
	uint64_t branch_target(int32_t li, bool aa) const;

	void print_mem_args(const char *op, int r, int16_t d, int ra);
	void print_mem_args_float(const char* op, int fr, int16_t d, int ra);
	void print_ir(const char *op, int imm, int r);
	void print_fff(const char *op, int fr0, int fr1, int fr2, bool rc);
	void print_ffff(const char *op, int fr0, int fr1, int fr2, int fr3, bool rc);
	void print_r(const char *op, int r);
	void print_ri(const char *op, int r0, int imm, bool rc);
	void print_rr(const char *op, int r0, int r1, bool rc);
	void print_rri(const char *op, int r0, int r1, int imm, bool rc);
	void print_rrii(const char *op, int r0, int r1, int imm0, int imm1, bool rc);
	void print_rriii(const char *op, int r0, int r1, int imm0, int imm1, int imm2, bool rc);
	void print_rrr(const char *op, int r0, int r1, int r2, bool rc);
	void print_rrrii(const char *op, int r0, int r1, int r2, int imm0, int imm1, bool rc);
	void print_b(const char *op, int32_t li, bool aa);

	void onUnknownInstr(uint32_t instr) override final;

	void onIllegalInstr(uint32_t instr) override final;
	void onRsvdInstr(uint32_t instr) override final;
	
	void onAdd(int rt, int ra, int rb, bool oe, bool rc) override final;
	void onAddi(int rt, int ra, int16_t si) override final;
	void onAddis(int rt, int ra, int16_t si) override final;
	void onAddic(int rt, int ra, int16_t si, bool rc) override final;
	void onAnd(int rs, int ra, int rb, bool rc) override final;
	void onAndc(int rs, int ra, int rb, bool rc) override final;
	void onAndi(int ra, int rs, uint16_t ui) override final;
	void onAndis(int ra, int rs, uint16_t ui) override final;
	void onB(int32_t li, bool aa, bool lk) override final;
	void onBc(int bo, int bi, int16_t bd, bool aa, bool lk) override final;
	void onBctr(bool lk) override final;
	void onBlr() override final;
	void onCmp(int bf, bool l, int ra, int rb) override final;
	void onCmpi(int bf, bool l, int ra, int16_t si) override final;
	void onCmpl(int bf, bool l, int ra, int rb) override final;
	void onCmpli(int bf, bool l, int ra, uint16_t ui) override final;
	void onDcbf(int ra, int rb) override final;
	void onDcbst(int ra, int rb) override final;
	void onDivdu(int rt, int ra, int rb, bool oe, bool rc) override final;
	void onExtsh(int ra, int rs, bool rc) override final;
	void onExtsw(int ra, int rs, bool rc) override final;
	void onFmul(int frt, int fra, int frc, bool rc) override final;
	void onFnmadd(int frt, int fra, int frc, int frb, bool rc) override final;
	void onIcbi(int ra, int rb) override final;
	void onIsync() override final;
	void onLbz(int rt, int16_t d, int ra) override final;
	void onLbzu(int rt, int16_t d, int ra) override final;
	void onLbzx(int rt, int ra, int rb) override final;
	void onLd(int rt, int16_t ds, int ra) override final;
	void onLdu(int rt, int16_t ds, int ra) override final;
	void onLfd(int frt, int16_t d, int ra) override final;
	void onLfdu(int frt, int16_t d, int ra) override final;
	void onLfs(int frt, int16_t d, int ra) override final;
	void onLfsu(int frt, int16_t d, int ra) override final;
	void onLq(int rt, int16_t dq, int ra) override final;
	void onLha(int rt, int16_t d, int ra) override final;
	void onLhau(int rt, int16_t d, int ra) override final;
	void onLhax(int rt, int ra, int rb) override final;
	void onLhz(int rt, int16_t d, int ra) override final;
	void onLhzu(int rt, int16_t d, int ra) override final;
	void onLhzx(int rt, int ra, int rb) override final;
	void onLmw(int rt, int16_t d, int ra) override final;
	void onLwa(int rt, int16_t ds, int ra) override final;
	void onLwz(int rt, int16_t d, int ra) override final;
	void onLwzu(int rt, int16_t d, int ra) override final;
	void onLwzx(int rt, int ra, int rb) override final;
	void onMfcr(int rt) override final;
	void onMfmsr(int rt) override final;
	void onMfspr(int rt, int spr) override final;
	void onMftb(int rt, int tbr) override final;
	void onMtcrf(int fxm, int rs) override final;
	void onMtmsr(int rs, bool l) override final;
	void onMtmsrd(int rs, bool l) override final;
	void onMtocrf(int fxm, int rs) override final;
	void onMtspr(int spr, int rs) override final;
	void onMulld(int rt, int ra, int rb, bool oe, bool rc) override final;
	void onMulli(int rt, int ra, int16_t si) override final;
	void onNeg(int rt, int ra, bool oe, bool rc) override final;
	void onNor(int ra, int rs, int rb, bool rc) override final;
	void onOr(int rs, int ra, int rb, bool rc) override final;
	void onOri(int ra, int rs, uint16_t ui) override final;
	void onOris(int ra, int rs, uint16_t ui) override final;
	void onRfid() override final;
	void onRldicl(int ra, int rs, int sh, int mb, bool rc) override final;
	void onRldicr(int ra, int rs, int sh, int me, bool rc) override final;
	void onRldimi(int ra, int rs, int sh, int mb, bool rc) override final;
	void onRlwimi(int ra, int rs, int sh, int mb, int me, bool rc) override final;
	void onRlwinm(int ra, int rs, int sh, int mb, int me, bool rc) override final;
	void onRlwnm(int ra, int rs, int rb, int mb, int mr, bool rc) override final;
	void onSc(int lev) override final;
	void onSld(int ra, int rs, int rb, bool rc) override final;
	void onSlw(int ra, int rs, int rb, bool rc) override final;
	void onSrad(int ra, int rs, int rb, bool rc) override final;
	void onSradi(int ra, int rs, int sh, bool rc) override final;
	void onSrawi(int ra, int rs, int sh, bool rc) override final;
	void onStb(int rs, int16_t d, int ra) override final;
	void onStbu(int rs, int16_t d, int ra) override final;
	void onStbx(int rs, int ra, int rb) override final;
	void onStd(int rs, int16_t ds, int ra) override final;
	void onStdu(int rs, int16_t ds, int ra) override final;
	void onStfd(int frs, int16_t d, int ra) override final;
	void onStfdu(int frs, int16_t d, int ra) override final;
	void onStfs(int frs, int16_t d, int ra) override final;
	void onStfsu(int frs, int16_t d, int ra) override final;
	void onSth(int rs, int16_t d, int ra) override final;
	void onSthu(int rs, int16_t d, int ra) override final;
	void onSthx(int rs, int ra, int rb) override final;
	void onStmw(int rs, int16_t d, int ra) override final;
	void onStq(int rs, int16_t ds, int ra) override final;
	void onStw(int rs, int16_t d, int ra) override final;
	void onStwu(int rs, int16_t d, int ra) override final;
	void onStwx(int rs, int ra, int rb) override final;
	void onSubf(int rt, int ra, int rb, bool oe, bool rc) override final;
	void onSubfe(int rt, int ra, int rb, bool oe, bool rc) override final;
	void onSubfic(int rt, int ra, int16_t si) override final;
	void onSubfze(int rt, int ra, bool oe, bool rc) override final;
	void onSync(int l) override final;
	void onTdi(uint8_t to, int ra, int16_t si) override final;
	void onTlbiel(int rb, int l) override final;
	void onTwi(uint8_t to, int ra, int16_t si) override final;
	void onXori(int ra, int rs, uint16_t ui) override final;
	void onXoris(int ra, int rs, uint16_t ui) override final;

	char instr_name[64];
	char instr_args[128];

	uint64_t cur_addr;
};

}}} /*namespace remu::jitpp::ppc*/

#endif /*REMU_JITPP_PPC_DISASSEMBLER_H*/

