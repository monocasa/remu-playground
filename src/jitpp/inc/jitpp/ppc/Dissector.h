#ifndef REMU_JITPP_PPC_DISSECTOR_H
#define REMU_JITPP_PPC_DISSECTOR_H

#include <cstdint>

namespace remu { namespace jitpp { namespace ppc {

class Dissector
{
private:
	struct ppc_op {
		uint32_t op;

		int opcd() { return op >> 26; }
		bool rc() { return op & 1; }

		//B Form
		bool    b_aa() { return 0; }
		int32_t b_bd() { return 0; }
		int     b_bi() { return 0; }
		int     b_bo() { return 0; }
		bool    b_lk() { return 0; }

		//D Form
		int      d_bf()  { return 0; }
		int16_t  d_d()   { return 0; }
		int      d_frs() { return 0; }
		int      d_frt() { return 0; }
		bool     d_l()   { return 0; }
		int      d_ra()  { return 0; }
		int      d_rs()  { return 0; }
		int      d_rt()  { return 0; }
		int16_t  d_si()  { return op & 0xFFFF; }
		uint16_t d_ui()  { return op & 0xFFFF; }
		uint8_t  d_to()  { return 0; }

		//DQ Form
		int16_t dq_dq() { return 0; }
		int     dq_ra() { return 0; }
		int     dq_rt() { return 0; }

		//I Form
		bool    i_aa() { return 0; }
		int32_t i_li() { return 0; }
		bool    i_lk() { return 0; }

		//M Form
		int m_mb() { return 0; }
		int m_me() { return 0; }
		int m_ra() { return 0; }
		int m_rb() { return 0; }
		int m_rs() { return 0; }
		int m_sh() { return 0; }

		//SC Form
		int sc_lev() { return 0; }
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

	virtual void onAddi(int rt, int ra, int16_t si) = 0;
	virtual void onAddis(int rt, int ra, int16_t si) = 0;
	virtual void onAddic(int rt, int ra, int16_t si, bool rc) = 0;
	virtual void onAndi(int ra, int rs, uint16_t ui) = 0;
	virtual void onAndis(int ra, int rs, uint16_t ui) = 0;
	virtual void onB(int32_t li, bool aa, bool lk) = 0;
	virtual void onBc(int bo, int bi, int32_t bd, bool aa, bool lk) = 0;
	virtual void onCmpi(int bf, bool l, int ra, int16_t si) = 0;
	virtual void onCmpli(int bf, bool l, int ra, uint16_t ui) = 0;
	virtual void onLbz(int rt, int16_t d, int ra) = 0;
	virtual void onLbzu(int rt, int16_t d, int ra) = 0;
	virtual void onLfd(int frt, int16_t d, int ra) = 0;
	virtual void onLfdu(int frt, int16_t d, int ra) = 0;
	virtual void onLfs(int frt, int16_t d, int ra) = 0;
	virtual void onLfsu(int frt, int16_t d, int ra) = 0;
	virtual void onLq(int rt, int16_t dq, int ra) = 0;
	virtual void onLha(int rt, int16_t d, int ra) = 0;
	virtual void onLhau(int rt, int16_t d, int ra) = 0;
	virtual void onLhz(int rt, int16_t d, int ra) = 0;
	virtual void onLhzu(int rt, int16_t d, int ra) = 0;
	virtual void onLmw(int rt, int16_t d, int ra) = 0;
	virtual void onLwz(int rt, int16_t d, int ra) = 0;
	virtual void onLwzu(int rt, int16_t d, int ra) = 0;
	virtual void onMulli(int rt, int ra, int16_t si) = 0;
	virtual void onOri(int ra, int rs, uint16_t ui) = 0;
	virtual void onOris(int ra, int rs, uint16_t ui) = 0;
	virtual void onRlwimi(int ra, int rs, int sh, int mb, int me, bool rc) = 0;
	virtual void onRlwinm(int ra, int rs, int sh, int mb, int me, bool rc) = 0;
	virtual void onRlwnm(int ra, int rs, int rb, int mb, int mr, bool rc) = 0;
	virtual void onSc(int lev) = 0;
	virtual void onStb(int rs, int16_t d, int ra) = 0;
	virtual void onStbu(int rs, int16_t d, int ra) = 0;
	virtual void onStfd(int frs, int16_t d, int ra) = 0;
	virtual void onStfdu(int frs, int16_t d, int ra) = 0;
	virtual void onStfs(int frs, int16_t d, int ra) = 0;
	virtual void onStfsu(int frs, int16_t d, int ra) = 0;
	virtual void onSth(int rs, int16_t d, int ra) = 0;
	virtual void onSthu(int rs, int16_t d, int ra) = 0;
	virtual void onStmw(int rs, int16_t d, int ra) = 0;
	virtual void onStw(int rs, int16_t d, int ra) = 0;
	virtual void onStwu(int rs, int16_t d, int ra) = 0;
	virtual void onSubfic(int rt, int ra, int16_t si) = 0;
	virtual void onTdi(uint8_t to, int ra, int16_t si) = 0;
	virtual void onTwi(uint8_t to, int ra, int16_t si) = 0;
	virtual void onXori(int ra, int rs, uint16_t ui) = 0;
	virtual void onXoris(int ra, int rs, uint16_t ui) = 0;

	void dissect(uint32_t instr);

};

}}} /*namespace remu::jitpp::ppc*/

#endif /*REMU_JITPP_PPC_DISSECTOR_H*/

