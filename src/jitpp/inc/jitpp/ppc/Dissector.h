#ifndef REMU_JITPP_PPC_DISSECTOR_H
#define REMU_JITPP_PPC_DISSECTOR_H

#include "util/bitops.h"

#include <cstdint>

namespace remu { namespace jitpp { namespace ppc {

class Dissector
{
private:
	struct ppc_op {
		uint32_t op;

		int opcd() { return op >> 26; }
		bool rc() { return op & 1; }

		//A Form
		int a_fra() { return (op >> 16) & 0x1F; }
		int a_frb() { return (op >> 11) & 0x1F; }
		int a_frc() { return (op >>  6) & 0x1F; }
		int a_frt() { return (op >> 21) & 0x1F; }
		int a_xo()  { return (op >>  1) & 0x1F; }

		//B Form
		bool    b_aa() { return op & 2; }
		int16_t b_bd() { return util::bitops::signExtend<int16_t, 16>(op & 0xFFFC); }
		int     b_bi() { return (op >> 21) & 0x1F; }
		int     b_bo() { return (op >> 16) & 0x1F; }
		bool    b_lk() { return op & 1; }

		//D Form
		int      d_bf()  { return (op >> 23) & 0x7; }
		int16_t  d_d()   { return d_si(); }
		int      d_frs() { return d_rs(); }
		int      d_frt() { return d_rt(); }
		bool     d_l()   { return (op >> 21) & 1; }
		int      d_ra()  { return (op >> 16) & 0x1F; }
		int      d_rs()  { return (op >> 21) & 0x1F; }
		int      d_rt()  { return (op >> 21) & 0x1F; }
		int16_t  d_si()  { return op & 0xFFFF; }
		uint16_t d_ui()  { return op & 0xFFFF; }
		uint8_t  d_to()  { return 0; }

		//DS Form
		int16_t ds_ds() { return util::bitops::signExtend<int16_t, 16>(op & 0xFFFC); }
		int     ds_ra() { return d_ra(); }
		int     ds_rs() { return d_rs(); }
		int     ds_rt() { return d_rt(); }

		//DQ Form
		int16_t dq_dq() { return 0; }
		int     dq_ra() { return 0; }
		int     dq_rt() { return 0; }

		//I Form
		bool    i_aa() { return op & 2; }
		int32_t i_li() { return util::bitops::signExtend<int32_t, 26>(op & 0x3FFFFFC); }
		bool    i_lk() { return op & 1; }

		//M Form
		int m_mb() { return (op >>  6) & 0x1F; }
		int m_me() { return (op >>  1) & 0x1F; }
		int m_ra() { return (op >> 16) & 0x1F; }
		int m_rb() { return (op >> 11) & 0x1F; }
		int m_rs() { return (op >> 21) & 0x1F; }
		int m_sh() { return (op >> 11) & 0x1F; }

		//MD_FORM
		int md_mb() { return ((op >> 6) & 0x1F) | (op & 0x20); }
		int md_me() { return ((op >> 6) & 0x1F) | (op & 0x20); }
		int md_ra() { return d_ra(); }
		int md_rs() { return d_rs(); }
		int md_sh() { return ((op << 4) & 0x20) | ((op >> 11) & 0x1f); }

		//SC Form
		int sc_lev() { return 0; }

		//X Form
		int  x_bf()       { return (op >> 23) & 0x7; }
		int  x_ih()       { return (op >> 21) & 0x7; }
		bool x_l_cmp()    { return (op >> 21) & 0x1; }
		bool x_l_mtmsr()  { return (op >> 16) & 0x1; }
		bool x_l_mtmsrd() { return (op >> 16) & 0x1; }
		int  x_l_sync()   { return (op >> 21) & 0x3; }
		int  x_l_tlbiel() { return (op >> 21) & 0x1F; }
		int  x_ra()       { return (op >> 16) & 0x1F; }
		int  x_rb()       { return (op >> 11) & 0x1F; }
		int  x_rs()       { return (op >> 21) & 0x1F; }
		int  x_rt()       { return (op >> 21) & 0x1F; }
		int  x_sh()       { return (op >> 11) & 0x1F; }
		int  x_xo()       { return (op >> 1) & 0x3FF; }

		//XFX Form
		int xfx_fxm() { return (op >> 12) & 0xFF; }
		int xfx_rs()  { return (op >> 21) & 0x1F; }
		int xfx_rt()  { return (op >> 21) & 0x1F; }
		int xfx_spr() { return ((op >> 6) & 0x3E0) | ((op >> 16) & 0x1F); }
		int xfx_tbr() { return xfx_spr(); }

		//XL Form
		bool xl_lk() { return op & 1; }

		//XO Form
		int xo_ra() { return x_ra(); }
		int xo_rb() { return x_rb(); }
		int xo_rt() { return x_rt(); }

		//XS Form
		int xs_ra() { return (op >> 16) & 0x1F; }
		int xs_rs() { return (op >> 21) & 0x1F; }
		int xs_sh() { return ((op >> 11) & 0x1F) | ((op << 4) & 0x20); }
	};

	void dissectVectorOp4(ppc_op op);
	void dissectCr(ppc_op op);
	void dissectTable2(ppc_op op);
	void dissectSpecial(ppc_op op);
	void dissectTable3(ppc_op op);
	void dissectTable13(ppc_op op);
	void dissectTable4(ppc_op op);
	void dissectTable14(ppc_op op);

protected:
	virtual void onUnknownInstr(uint32_t instr) = 0;

	virtual void onIllegalInstr(uint32_t instr) = 0;
	virtual void onRsvdInstr(uint32_t instr) = 0;

	virtual void onAdd(int rt, int ra, int rb, bool oe, bool rc) = 0;
	virtual void onAddi(int rt, int ra, int16_t si) = 0;
	virtual void onAddis(int rt, int ra, int16_t si) = 0;
	virtual void onAddic(int rt, int ra, int16_t si, bool rc) = 0;
	virtual void onAnd(int rs, int ra, int rb, bool rc) = 0;
	virtual void onAndc(int rs, int ra, int rb, bool rc) = 0;
	virtual void onAndi(int ra, int rs, uint16_t ui) = 0;
	virtual void onAndis(int ra, int rs, uint16_t ui) = 0;
	virtual void onB(int32_t li, bool aa, bool lk) = 0;
	virtual void onBc(int bo, int bi, int16_t bd, bool aa, bool lk) = 0;
	virtual void onBctr(bool lk) = 0;
	virtual void onBlr() = 0;
	virtual void onCmp(int bf, bool l, int ra, int rb) = 0;
	virtual void onCmpi(int bf, bool l, int ra, int16_t si) = 0;
	virtual void onCmpl(int bf, bool l, int ra, int rb) = 0;
	virtual void onCmpli(int bf, bool l, int ra, uint16_t ui) = 0;
	virtual void onDcbf(int ra, int rb) = 0;
	virtual void onDcbst(int ra, int rb) = 0;
	virtual void onDivdu(int rt, int ra, int rb, bool oe, bool rc) = 0;
	virtual void onDivwu(int rt, int ra, int rb, bool oe, bool rc) = 0;
	virtual void onEieio() = 0;
	virtual void onExtsh(int ra, int rs, bool rc) = 0;
	virtual void onExtsw(int ra, int rs, bool rc) = 0;
	virtual void onFmul(int frt, int fra, int frc, bool rc) = 0;
	virtual void onFmadd(int frt, int fra, int frc, int frb, bool rc) = 0;
	virtual void onFmadds(int frt, int fra, int frc, int frb, bool rc) = 0;
	virtual void onFnmadd(int frt, int fra, int frc, int frb, bool rc) = 0;
	virtual void onIcbi(int ra, int rb) = 0;
	virtual void onIsync() = 0;
	virtual void onLbz(int rt, int16_t d, int ra) = 0;
	virtual void onLbzu(int rt, int16_t d, int ra) = 0;
	virtual void onLbzx(int rt, int ra, int rb) = 0;
	virtual void onLd(int rt, int16_t ds, int ra) = 0;
	virtual void onLdu(int rt, int16_t ds, int ra) = 0;
	virtual void onLfd(int frt, int16_t d, int ra) = 0;
	virtual void onLfdu(int frt, int16_t d, int ra) = 0;
	virtual void onLfs(int frt, int16_t d, int ra) = 0;
	virtual void onLfsu(int frt, int16_t d, int ra) = 0;
	virtual void onLq(int rt, int16_t dq, int ra) = 0;
	virtual void onLha(int rt, int16_t d, int ra) = 0;
	virtual void onLhau(int rt, int16_t d, int ra) = 0;
	virtual void onLhax(int rt, int ra, int rb) = 0;
	virtual void onLhz(int rt, int16_t d, int ra) = 0;
	virtual void onLhzu(int rt, int16_t d, int ra) = 0;
	virtual void onLhzx(int rt, int ra, int rb) = 0;
	virtual void onLmw(int rt, int16_t d, int ra) = 0;
	virtual void onLwa(int rt, int16_t ds, int ra) = 0;
	virtual void onLwz(int rt, int16_t d, int ra) = 0;
	virtual void onLwzu(int rt, int16_t d, int ra) = 0;
	virtual void onLwzx(int rt, int ra, int rb) = 0;
	virtual void onMfcr(int rt) = 0;
	virtual void onMfmsr(int rt) = 0;
	virtual void onMfspr(int rt, int spr) = 0;
	virtual void onMftb(int rt, int tbr) = 0;
	virtual void onMtcrf(int fxm, int rs) = 0;
	virtual void onMtmsr(int rs, bool l) = 0;
	virtual void onMtmsrd(int rs, bool l) = 0;
	virtual void onMtocrf(int fxm, int rs) = 0;
	virtual void onMtspr(int spr, int rs) = 0;
	virtual void onMulld(int rt, int ra, int rb, bool oe, bool rc) = 0;
	virtual void onMulli(int rt, int ra, int16_t si) = 0;
	virtual void onMullw(int rt, int ra, int rb, bool oe, bool rc) = 0;
	virtual void onNeg(int rt, int ra, bool oe, bool rc) = 0;
	virtual void onNor(int ra, int rs, int rb, bool rc) = 0;
	virtual void onOr(int rs, int ra, int rb, bool rc) = 0;
	virtual void onOri(int ra, int rs, uint16_t ui) = 0;
	virtual void onOris(int ra, int rs, uint16_t ui) = 0;
	virtual void onRfid() = 0;
	virtual void onRldicl(int ra, int rs, int sh, int mb, bool rc) = 0;
	virtual void onRldicr(int ra, int rs, int sh, int me, bool rc) = 0;
	virtual void onRldimi(int ra, int rs, int sh, int mb, bool rc) = 0;
	virtual void onRlwimi(int ra, int rs, int sh, int mb, int me, bool rc) = 0;
	virtual void onRlwinm(int ra, int rs, int sh, int mb, int me, bool rc) = 0;
	virtual void onRlwnm(int ra, int rs, int rb, int mb, int mr, bool rc) = 0;
	virtual void onSc(int lev) = 0;
	virtual void onSlbia(int ih) = 0;
	virtual void onSlbmte(int rs, int rb) = 0;
	virtual void onSld(int ra, int rs, int rb, bool rc) = 0;
	virtual void onSlw(int ra, int rs, int rb, bool rc) = 0;
	virtual void onSrad(int ra, int rs, int rb, bool rc) = 0;
	virtual void onSradi(int ra, int rs, int sh, bool rc) = 0;
	virtual void onSrawi(int ra, int rs, int sh, bool rc) = 0;
	virtual void onSrw(int ra, int rs, int rb, bool rc) = 0;
	virtual void onStb(int rs, int16_t d, int ra) = 0;
	virtual void onStbu(int rs, int16_t d, int ra) = 0;
	virtual void onStbx(int rs, int ra, int rb) = 0;
	virtual void onStd(int rs, int16_t ds, int ra) = 0;
	virtual void onStdu(int rs, int16_t ds, int ra) = 0;
	virtual void onStfd(int frs, int16_t d, int ra) = 0;
	virtual void onStfdu(int frs, int16_t d, int ra) = 0;
	virtual void onStfs(int frs, int16_t d, int ra) = 0;
	virtual void onStfsu(int frs, int16_t d, int ra) = 0;
	virtual void onSth(int rs, int16_t d, int ra) = 0;
	virtual void onSthu(int rs, int16_t d, int ra) = 0;
	virtual void onSthx(int rs, int ra, int rb) = 0;
	virtual void onStmw(int rs, int16_t d, int ra) = 0;
	virtual void onStq(int rs, int16_t ds, int ra) = 0;
	virtual void onStw(int rs, int16_t d, int ra) = 0;
	virtual void onStwu(int rs, int16_t d, int ra) = 0;
	virtual void onStwx(int rs, int ra, int rb) = 0;
	virtual void onSubf(int rt, int ra, int rb, bool oe, bool rc) = 0;
	virtual void onSubfc(int rt, int ra, int rb, bool oe, bool rc) = 0;
	virtual void onSubfe(int rt, int ra, int rb, bool oe, bool rc) = 0;
	virtual void onSubfic(int rt, int ra, int16_t si) = 0;
	virtual void onSubfze(int rt, int ra, bool oe, bool rc) = 0;
	virtual void onSync(int l) = 0;
	virtual void onTdi(uint8_t to, int ra, int16_t si) = 0;
	virtual void onTlbiel(int rb, int l) = 0;
	virtual void onTwi(uint8_t to, int ra, int16_t si) = 0;
	virtual void onXori(int ra, int rs, uint16_t ui) = 0;
	virtual void onXoris(int ra, int rs, uint16_t ui) = 0;

	void dissect(uint32_t instr);

};

}}} /*namespace remu::jitpp::ppc*/

#endif /*REMU_JITPP_PPC_DISSECTOR_H*/

