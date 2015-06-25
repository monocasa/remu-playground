#include "jitpp/ppc/Dissector.h"

namespace remu { namespace jitpp { namespace ppc {

void Dissector::dissectVectorOp4(ppc_op op)
{
	onUnknownInstr(op.op);
}

void Dissector::dissectCr(ppc_op op)
{
	onUnknownInstr(op.op);
}

void Dissector::dissectTable2(ppc_op op)
{
	onUnknownInstr(op.op);
}

void Dissector::dissectSpecial(ppc_op op)
{
	onUnknownInstr(op.op);
}

void Dissector::dissectTable3(ppc_op op)
{
	onUnknownInstr(op.op);
}

void Dissector::dissectTable13(ppc_op op)
{
	onUnknownInstr(op.op);
}

void Dissector::dissectTable4(ppc_op op)
{
	onUnknownInstr(op.op);
}

void Dissector::dissectTable14(ppc_op op)
{
	onUnknownInstr(op.op);
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

