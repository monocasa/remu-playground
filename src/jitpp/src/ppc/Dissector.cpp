#include "jitpp/ppc/Dissector.h"

#include <cstdio>

namespace remu { namespace jitpp { namespace ppc {

void Dissector::dissectVectorOp4(ppc_op op)
{
	onUnknownInstr(op.op);
}

void Dissector::dissectCr(ppc_op op)
{
	switch( op.x_xo() ) {
	case 16:  onBlr();               break;

	case 18:  onRfid();              break;

	case 150: onIsync();             break;

	case 528: onBctr(op.xl_lk());    break;

	default:  onUnknownInstr(op.op); break;
	}
}

void Dissector::dissectTable2(ppc_op op)
{
	switch( (op.op >> 1) & 7 ) {
	case 0:
	case 1: onRldicl(op.md_ra(), op.md_rs(), op.md_sh(), op.md_mb(), op.rc()); break;

	case 2:
	case 3: onRldicr(op.md_ra(), op.md_rs(), op.md_sh(), op.md_me(), op.rc()); break;

	case 6:
	case 7: onRldimi(op.md_ra(), op.md_rs(), op.md_sh(), op.md_mb(), op.rc()); break;

	default: onUnknownInstr(op.op); break;
	}
}

void Dissector::dissectSpecial(ppc_op op)
{
	switch( op.x_xo() ) {

	case 0:   onCmp(op.x_bf(), op.x_l_cmp(), op.x_ra(), op.x_rb());        break;

	case 8:   onSubfc(op.x_rt(), op.x_ra(), op.x_rb(), false, op.rc());    break;

	case 19: {
		if( (op.op >> 20 & 1) == 0 ) {
			onMfcr(op.xfx_rt());
		}
		else {
			onUnknownInstr(op.op);
		}
	}
	break;

	case 23:  onLwzx(op.x_rt(), op.x_ra(), op.x_rb());                     break;
	case 24:  onSlw(op.x_ra(), op.x_rs(), op.x_rb(), op.rc());             break;

	case 27:  onSld(op.x_ra(), op.x_rs(), op.x_rb(), op.rc());             break;
	case 28:  onAnd(op.x_ra(), op.x_rs(), op.x_rb(), op.rc());             break;

	case 32:  onCmpl(op.x_bf(), op.x_l_cmp(), op.x_ra(), op.x_rb());       break;

	case 40:  onSubf(op.xo_rt(), op.xo_ra(), op.xo_rb(), false, op.rc());  break;

	case 54:  onDcbst(op.x_ra(), op.x_rb());                               break;

	case 60:  onAndc(op.x_ra(), op.x_rs(), op.x_rb(), op.rc());            break;

	case 83:  onMfmsr(op.x_rt());                                          break;

	case 86:  onDcbf(op.x_ra(), op.x_rb());                                break;
	case 87:  onLbzx(op.x_rt(), op.x_ra(), op.x_rb());                     break;

	case 104: onNeg(op.xo_rt(), op.xo_ra(), false, op.rc());               break;

	case 124: onNor(op.x_ra(), op.x_rs(), op.x_rb(), op.rc());             break;

	case 136: onSubfe(op.xo_rt(), op.xo_ra(), op.xo_rb(), false, op.rc()); break;

	case 138: onAdde(op.xo_rt(), op.xo_ra(), op.xo_rb(), false, op.rc());  break;

	case 144: {
		if( (op.op >> 20 & 1) == 1 ) {
			onMtocrf(op.xfx_fxm(), op.xfx_rs());
		}
		else {
			onMtcrf(op.xfx_fxm(), op.xfx_rs());
		}
	}
	break;

	case 146: onMtmsr(op.x_rs(), op.x_l_mtmsr());                          break;

	case 151: onStwx(op.x_rs(), op.x_ra(), op.x_rb());                     break;

	case 178: onMtmsrd(op.x_rs(), op.x_l_mtmsrd());                        break;

	case 200: onSubfze(op.xo_rt(), op.xo_ra(), false, op.rc());            break;

	case 215: onStbx(op.x_rs(), op.x_ra(), op.x_rb());                     break;

	case 233: onMulld(op.x_rt(), op.x_ra(), op.x_rb(), false, op.rc());    break;

	case 235: onMullw(op.x_rt(), op.x_ra(), op.x_rb(), false, op.rc());    break;

	case 266: onAdd(op.xo_rt(), op.xo_ra(), op.xo_rb(), false, op.rc());   break;

	case 274: onTlbiel(op.x_rb(), op.x_l_tlbiel());                        break;

	case 279: onLhzx(op.x_rt(), op.x_ra(), op.x_rb());                     break;

	case 316: onXor(op.x_ra(), op.x_rs(), op.x_rb(), op.rc());             break;

	case 339: onMfspr(op.xfx_rt(), op.xfx_spr());                          break;

	case 343: onLhax(op.x_rt(), op.x_ra(), op.x_rb());                     break;

	case 371: onMftb(op.xfx_rt(), op.xfx_tbr());                           break;

	case 402: onSlbmte(op.x_rs(), op.x_rb());                              break;

	case 407: onSthx(op.x_rs(), op.x_ra(), op.x_rb());                     break;

	case 444: onOr(op.x_ra(), op.x_rs(), op.x_rb(), op.rc());              break;

	case 457: onDivdu(op.xo_rt(), op.xo_ra(), op.xo_rb(), false, op.rc()); break;

	case 459: onDivwu(op.xo_rt(), op.xo_ra(), op.xo_rb(), false, op.rc()); break;

	case 467: onMtspr(op.xfx_spr(), op.xfx_rs());                          break;

	case 476: onNand(op.x_ra(), op.x_rs(), op.x_rb(), op.rc());            break;

	case 498: onSlbia(op.x_ih());                                          break;

	case 520: onSubfc(op.x_rt(), op.x_ra(), op.x_rb(), true, op.rc());     break;

	case 536: onSrw(op.x_ra(), op.x_rs(), op.x_rb(), op.rc());             break;

	case 552: onSubf(op.xo_rt(), op.xo_ra(), op.xo_rb(), true, op.rc());   break;

	case 598: onSync(op.x_l_sync());                                       break;

	case 616: onNeg(op.xo_rt(), op.xo_ra(), true, op.rc());                break;

	case 648: onSubfe(op.xo_rt(), op.xo_ra(), op.xo_rb(), true, op.rc());  break;

	case 650: onAdde(op.xo_rt(), op.xo_ra(), op.xo_rb(), true, op.rc());   break;

	case 712: onSubfze(op.xo_rt(), op.xo_ra(), true, op.rc());             break;

	case 745: onMulld(op.x_rt(), op.x_ra(), op.x_rb(), true, op.rc());     break;

	case 747: onMullw(op.x_rt(), op.x_ra(), op.x_rb(), true, op.rc());     break;

	case 778: onAdd(op.xo_rt(), op.xo_ra(), op.xo_rb(), true, op.rc());    break;

	case 794: onSrad(op.x_ra(), op.x_rs(), op.x_rb(), op.rc());            break;

	case 824: onSrawi(op.x_ra(), op.x_rs(), op.x_sh(), op.rc());           break;

	//Doubled up because the 6th shift bit is shard with x_xo
	case 826: onSradi(op.xs_ra(), op.xs_rs(), op.xs_sh(), op.rc());        break;
	case 827: onSradi(op.xs_ra(), op.xs_rs(), op.xs_sh(), op.rc());        break;

	case 854: onEieio();                                                   break;

	case 922: onExtsh(op.x_ra(), op.x_rs(), op.rc());                      break;

	case 969: onDivdu(op.xo_rt(), op.xo_ra(), op.xo_rb(), true, op.rc());  break;

	case 971: onDivwu(op.xo_rt(), op.xo_ra(), op.xo_rb(), true, op.rc());  break;

	case 982: onIcbi(op.x_ra(), op.x_rb());                                break;

	case 986: onExtsw(op.x_ra(), op.x_rs(), op.rc());                      break;

	default:  onUnknownInstr(op.op);                                       break;
	}
}

void Dissector::dissectTable3(ppc_op op)
{
	switch( op.op & 3 ) {
	case 0: onLd(op.ds_rt(), op.ds_ds(), op.ds_ra());  break;
	case 1: onLdu(op.ds_rt(), op.ds_ds(), op.ds_ra()); break;
	case 2: onLwa(op.ds_rt(), op.ds_ds(), op.ds_ra()); break;
	default: onIllegalInstr(op.op);                    break;
	}
}

void Dissector::dissectTable13(ppc_op op)
{
	switch( op.a_xo() ) {

	case 29: onFmadds(op.a_frt(), op.a_fra(), op.a_frc(), op.a_frb(), op.rc()); break;

	default: onIllegalInstr(op.op);

	}
}

void Dissector::dissectTable4(ppc_op op)
{
	switch( op.op & 3 ) {
	case 0: onStd(op.ds_rs(), op.ds_ds(), op.ds_ra()); break;
	case 1: onStdu(op.ds_rs(), op.ds_ds(), op.ds_ra()); break;
	case 2: {
		if( op.ds_rs() % 2 ) {
			onIllegalInstr(op.op);
		}
		else {
			onStq(op.ds_rs(), op.ds_ds(), op.ds_ra());
		}
	}
	break;

	default: onIllegalInstr(op.op);
	}
}

void Dissector::dissectTable14(ppc_op op)
{
	switch( op.a_xo() ) {

	case 8:  dissectTable14_x_form(op);                                         break;

	case 25: onFmul(op.a_frt(), op.a_fra(), op.a_frc(), op.rc());               break;

	case 29: onFmadd(op.a_frt(), op.a_fra(), op.a_frc(), op.a_frb(), op.rc());  break;

	case 31: onFnmadd(op.a_frt(), op.a_fra(), op.a_frc(), op.a_frb(), op.rc()); break;

	default: onIllegalInstr(op.op);

	}
}

void Dissector::dissectTable14_x_form(ppc_op op)
{
	switch( op.x_xo() ) {

	case 264: onFabs(op.x_frt(), op.x_frb(), op.rc()); break;

	default: onIllegalInstr(op.op);

	}
}

void Dissector::dissect(uint32_t instr)
{
	ppc_op op;
	op.op = instr;

	if( instr == 0 ) {
		onRsvdInstr(instr);
		return;
	}

	switch(op.opcd()) {
	case 0:  onIllegalInstr(op.op);                                                    return; //00000000 handled above
	case 1:  onIllegalInstr(op.op);                                                    return;
	case 2:  onTdi(op.d_to(), op.d_ra(), op.d_si());                                   return;
	case 3:  onTwi(op.d_to(), op.d_ra(), op.d_si());                                   return;
	case 4:  dissectVectorOp4(op);                                                     return;
	case 5:  onIllegalInstr(op.op);                                                    return;
	case 6:  onIllegalInstr(op.op);                                                    return;
	case 7:  onMulli(op.d_rt(), op.d_ra(), op.d_si());                                 return;
	case 8:  onSubfic(op.d_rt(), op.d_ra(), op.d_si());                                return;
	case 9:  onUnknownInstr(op.op);                                                    return; //Not listed as illegal in App. D
	case 10: onCmpli(op.d_bf(), op.d_l(), op.d_ra(), op.d_ui());                       return;
	case 11: onCmpi(op.d_bf(), op.d_l(), op.d_ra(), op.d_si());                        return;
	case 12: onAddic(op.d_rt(), op.d_ra(), op.d_si(), false);                          return;
	case 13: onAddic(op.d_rt(), op.d_ra(), op.d_si(), true);                           return;
	case 14: onAddi(op.d_rt(), op.d_ra(), op.d_si());                                  return;
	case 15: onAddis(op.d_rt(), op.d_ra(), op.d_si());                                 return;
	case 16: onBc(op.b_bo(), op.b_bi(), op.b_bd(), op.b_aa(), op.b_lk());              return;
	case 17: onSc(op.sc_lev());                                                        return;
	case 18: onB(op.i_li(), op.i_aa(), op.i_lk());                                     return;
	case 19: dissectCr(op);                                                            return;
	case 20: onRlwimi(op.m_ra(), op.m_rs(), op.m_sh(), op.m_mb(), op.m_me(), op.rc()); return;
	case 21: onRlwinm(op.m_ra(), op.m_rs(), op.m_sh(), op.m_mb(), op.m_me(), op.rc()); return;
	case 22: onUnknownInstr(op.op);                                                    return; //Not listed as illegal in App. D
	case 23: onRlwnm(op.m_ra(), op.m_rs(), op.m_rb(), op.m_mb(), op.m_me(), op.rc());  return;
	case 24: onOri(op.d_ra(), op.d_rs(), op.d_ui());                                   return;
	case 25: onOris(op.d_ra(), op.d_rs(), op.d_ui());                                  return;
	case 26: onXori(op.d_ra(), op.d_rs(), op.d_ui());                                  return;
	case 27: onXoris(op.d_ra(), op.d_rs(), op.d_ui());                                 return;
	case 28: onAndi(op.d_ra(), op.d_rs(), op.d_ui());                                  return;
	case 29: onAndis(op.d_ra(), op.d_rs(), op.d_ui());                                 return;
	case 30: dissectTable2(op);                                                        return;
	case 31: dissectSpecial(op);                                                       return;
	case 32: onLwz(op.d_rt(), op.d_d(), op.d_ra());                                    return;
	case 33: onLwzu(op.d_rt(), op.d_d(), op.d_ra());                                   return;
	case 34: onLbz(op.d_rt(), op.d_d(), op.d_ra());                                    return;
	case 35: onLbzu(op.d_rt(), op.d_d(), op.d_ra());                                   return;
	case 36: onStw(op.d_rs(), op.d_d(), op.d_ra());                                    return;
	case 37: onStwu(op.d_rs(), op.d_d(), op.d_ra());                                   return;
	case 38: onStb(op.d_rs(), op.d_d(), op.d_ra());                                    return;
	case 39: onStbu(op.d_rs(), op.d_d(), op.d_ra());                                   return;
	case 40: onLhz(op.d_rt(), op.d_d(), op.d_ra());                                    return;
	case 41: onLhzu(op.d_rt(), op.d_d(), op.d_ra());                                   return;
	case 42: onLha(op.d_rt(), op.d_d(), op.d_ra());                                    return;
	case 43: onLhau(op.d_rt(), op.d_d(), op.d_ra());                                   return;
	case 44: onSth(op.d_rs(), op.d_d(), op.d_ra());                                    return;
	case 45: onSthu(op.d_rs(), op.d_d(), op.d_ra());                                   return;
	case 46: onLmw(op.d_rt(), op.d_d(), op.d_ra());                                    return;
	case 47: onStmw(op.d_rs(), op.d_d(), op.d_ra());                                   return;
	case 48: onLfs(op.d_frt(), op.d_d(), op.d_ra());                                   return;
	case 49: onLfsu(op.d_frt(), op.d_d(), op.d_ra());                                  return;
	case 50: onLfd(op.d_frt(), op.d_d(), op.d_ra());                                   return;
	case 51: onLfdu(op.d_frt(), op.d_d(), op.d_ra());                                  return;
	case 52: onStfs(op.d_frs(), op.d_d(), op.d_ra());                                  return;
	case 53: onStfsu(op.d_frs(), op.d_d(), op.d_ra());                                 return;
	case 54: onStfd(op.d_frs(), op.d_d(), op.d_ra());                                  return;
	case 55: onStfdu(op.d_frs(), op.d_d(), op.d_ra());                                 return;
	case 56: onLq(op.dq_rt(), op.dq_dq(), op.dq_ra());                                 return;
	case 57: onIllegalInstr(op.op);                                                    return;
	case 58: dissectTable3(op);                                                        return;
	case 59: dissectTable13(op);                                                       return;
	case 60: onIllegalInstr(op.op);                                                    return;
	case 61: onIllegalInstr(op.op);                                                    return;
	case 62: dissectTable4(op);                                                        return;
	case 63: dissectTable14(op);                                                       return;

	default: onUnknownInstr(op.op);                                                    return; //Should never happen.  Parse error
	}
}

}}} /*namespace remu::jitpp::ppc*/

