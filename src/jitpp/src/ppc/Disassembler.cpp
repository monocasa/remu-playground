#include "jitpp/ppc/Disassembler.h"

#include <cstdio>
#include <cstring>

namespace remu { namespace jitpp { namespace ppc {

size_t Disassembler::disassemble( const util::memslice &slice, 
                                  uint64_t addr, 
                                  char *buffer, 
                                  size_t buffer_size )
{
	(void)buffer_size;

	if( slice.size == 0 ) {
		sprintf( buffer, "Wat.  slice.size == 0" );
		return 0;
	}
	else if( (slice.size < 4) || ((addr & 3) != 0) ) {
		sprintf( buffer, ".byte 0x%x", slice.data[0] );
		return 1;
	}

	strcpy( instr_name, "" );
	strcpy( instr_args, "" );

	cur_addr = addr;

	uint32_t instr = (slice.data[0] << 24) | (slice.data[1] << 16) | (slice.data[2] << 8) | (slice.data[3] << 0);

	dissect( instr );

	if( strlen(instr_name) == 0 ) {
		sprintf( buffer, "TODO" );
	}
	else if( strlen(instr_args) == 0 ) {
		sprintf( buffer, "%s", instr_name );
	}
	else {
		sprintf( buffer, "%-8s%s", instr_name, instr_args );
	}

	return 4;
}

uint64_t Disassembler::branch_target(int32_t li, bool aa) const
{
	return aa ? (li & 0xFFFFFFFFULL) : (li + cur_addr);
}

void Disassembler::print_mem_args(const char* op, int r, int16_t d, int ra)
{
	sprintf( instr_name, "%s", op );
	sprintf( instr_args, "r%d,%d(r%d)", r, d, ra );
}

void Disassembler::print_mem_args_float(const char* op, int fr, int16_t d, int ra)
{
	sprintf( instr_name, "%s", op );
	sprintf( instr_args, "f%d,%d(r%d)", fr, d, ra );
}

void Disassembler::print_ir(const char *op, int imm, int r)
{
	sprintf( instr_name, "%s", op );
	sprintf( instr_args, "%d,r%d", imm, r );
}

void Disassembler::print_fff(const char *op, int fr0, int fr1, int fr2, bool rc)
{
	sprintf( instr_name, "%s%s", op, rc ? "." : "" );
	sprintf( instr_args, "f%d,f%d,f%d", fr0, fr1, fr2 );
}

void Disassembler::print_ffff(const char *op, int fr0, int fr1, int fr2, int fr3, bool rc)
{
	sprintf( instr_name, "%s%s", op, rc ? "." : "" );
	sprintf( instr_args, "f%d,f%d,f%d,f%d", fr0, fr1, fr2, fr3 );
}

void Disassembler::print_r(const char *op, int r)
{
	sprintf( instr_name, "%s", op );
	sprintf( instr_args, "r%d", r );
}

void Disassembler::print_ri(const char *op, int r0, int imm, bool rc)
{
	sprintf( instr_name, "%s%s", op, rc ? "." : "" );
	sprintf( instr_args, "r%d,%d", r0, imm );
}

void Disassembler::print_rr(const char *op, int r0, int r1, bool rc)
{
	sprintf( instr_name, "%s%s", op, rc ? "." : "" );
	sprintf( instr_args, "r%d,r%d", r0, r1 );
}

void Disassembler::print_rri(const char *op, int r0, int r1, int imm, bool rc)
{
	sprintf( instr_name, "%s%s", op, rc ? "." : "" );
	sprintf( instr_args, "r%d,r%d,%d", r0, r1, imm );
}

void Disassembler::print_rrii(const char *op, int r0, int r1, int imm0, int imm1, bool rc)
{
	sprintf( instr_name, "%s%s", op, rc ? "." : "" );
	sprintf( instr_args, "r%d,r%d,%d,%d", r0, r1, imm0, imm1 );
}

void Disassembler::print_rriii(const char *op, int r0, int r1, int imm0, int imm1, int imm2, bool rc)
{
	sprintf( instr_name, "%s%s", op, rc ? "." : "" );
	sprintf( instr_args, "r%d,r%d,%d,%d,%d", r0, r1, imm0, imm1, imm2 );
}

void Disassembler::print_rrr(const char *op, int r0, int r1, int r2, bool rc)
{
	sprintf( instr_name, "%s%s", op, rc ? "." : "" );
	sprintf( instr_args, "r%d,r%d,r%d", r0, r1, r2 );
}

void Disassembler::print_rrrii(const char *op, int r0, int r1, int r2, int imm0, int imm1, bool rc)
{
	sprintf( instr_name, "%s%s", op, rc ? "." : "" );
	sprintf( instr_args, "r%d,r%d,r%d,%d,%d", r0, r1, r2, imm0, imm1 );
}

void Disassembler::print_b(const char *op, int32_t li, bool aa)
{
	sprintf( instr_name, "%s", op );
	sprintf( instr_args, "0x%lx", branch_target(li, aa) );
}

void Disassembler::onUnknownInstr(uint32_t instr)
{
	sprintf( instr_name, ".long 0x%x", instr );
}

void Disassembler::onIllegalInstr(uint32_t instr)
{
	sprintf( instr_name, ".long 0x%x", instr );
}

void Disassembler::onRsvdInstr(uint32_t instr)
{
	sprintf( instr_name, ".long 0x%x", instr );
}

void Disassembler::onAdd(int rt, int ra, int rb, bool oe, bool rc)
{
	if( oe ) {
		print_rrr( "addo", rt, ra, rb, rc );
	}
	else {
		print_rrr( "add", rt, ra, rb, rc );
	}
}

void Disassembler::onAddi(int rt, int ra, int16_t si)
{
	if( ra == 0 ) {
		print_ri( "li", rt, si, false );
	}
	else {
		print_rri( "addi", rt, ra, si, false );
	}
}

void Disassembler::onAddis(int rt, int ra, int16_t si)
{
	if( ra == 0 ) {
		print_ri( "lis", rt, si, false );
	}
	else {
		print_rri( "addis", rt, ra, si, false );
	}
}

void Disassembler::onAddic(int rt, int ra, int16_t si, bool rc)
{
	print_rri( "addic", rt, ra, si, rc );
}

void Disassembler::onAnd(int rs, int ra, int rb, bool rc)
{
	print_rrr( "and", rs, ra, rb, rc );
}

void Disassembler::onAndc(int rs, int ra, int rb, bool rc)
{
	print_rrr( "andc", rs, ra, rb, rc );
}

void Disassembler::onAndi(int ra, int rs, uint16_t ui)
{
	print_rri( "andi", ra, rs, ui, true );
}

void Disassembler::onAndis(int ra, int rs, uint16_t ui)
{
	print_rri( "andis", ra, rs, ui, true );
}

void Disassembler::onB(int32_t li, bool aa, bool lk)
{
	print_b( lk ? "bl" : "b", li, aa );
}

void Disassembler::onBc(int bo, int bi, int16_t bd, bool aa, bool lk)
{
	if( bo == 0 && bi == 4 ) {
		print_b( "bge", bd, aa );
	}
	else if( bo == 0 && bi == 16 ) {
		print_b( "bdnz", bd, aa );
	}
	else if( bo == 2 && bi == 4 ) {
		print_b( "bne", bd, aa );
	}
	else if( bo == 2 && bi == 12 ) {
		print_b( "beq", bd, aa );
	}
	else if( bo == 18 && bi == 12 ) {
		sprintf( instr_name, "beq" );
		sprintf( instr_args, "cr4,0x%lx", branch_target(bd, aa) );
	}
	else if( bo == 28 && bi == 4 ) {
		sprintf( instr_name, "bge" );
		sprintf( instr_args, "cr7,0x%lx", branch_target(bd, aa) );
	}
	else if( bo == 28 && bi == 7 ) {
		sprintf( instr_name, "bge+" );
		sprintf( instr_args, "cr7,0x%lx", branch_target(bd, aa) );
	}
	else if( bo == 28 && bi == 12 ) {
		sprintf( instr_name, "blt" );
		sprintf( instr_args, "cr7,0x%lx", branch_target(bd, aa) );
	}
	else if( bo == 28 && bi == 14 ) {
		sprintf( instr_name, "blt-" );
		sprintf( instr_args, "cr7,0x%lx", branch_target(bd, aa) );
	}
	else if( bo == 29 && bi == 4 ) {
		sprintf( instr_name, "ble" );
		sprintf( instr_args, "cr7,0x%lx", branch_target(bd, aa) );
	}
	else if( bo == 29 && bi == 7 ) {
		sprintf( instr_name, "ble+" );
		sprintf( instr_args, "cr7,0x%lx", branch_target(bd, aa) );
	}
	else if( bo == 29 && bi == 12 ) {
		sprintf( instr_name, "bgt" );
		sprintf( instr_args, "cr7,0x%lx", branch_target(bd, aa) );
	}
	else if( bo == 29 && bi == 14 ) {
		sprintf( instr_name, "bgt-" );
		sprintf( instr_args, "cr7,0x%lx", branch_target(bd, aa) );
	}
	else if( bo == 30 && bi == 4 ) {
		sprintf( instr_name, "bne" );
		sprintf( instr_args, "cr7,0x%lx", branch_target(bd, aa) );
	}
	else if( bo == 30 && bi == 6 ) {
		sprintf( instr_name, "bne-" );
		sprintf( instr_args, "cr7,0x%lx", branch_target(bd, aa) );
	}
	else if( bo == 30 && bi == 7 ) {
		sprintf( instr_name, "bne+" );
		sprintf( instr_args, "cr7,0x%lx", branch_target(bd, aa) );
	}
	else if( bo == 30 && bi == 12 ) {
		sprintf( instr_name, "beq" );
		sprintf( instr_args, "cr7,0x%lx", branch_target(bd, aa) );
	}
	else {
		sprintf( instr_name, "bc%s%s", lk ? "l" : "", aa ? "a" : "" );
		sprintf( instr_args, "%d,%d,0x%lx", bo, bi, branch_target(bd, aa) );
	}
}

void Disassembler::onBctr(bool lk)
{
	sprintf( instr_name, "bctr%s", lk ? "l" : "" );
}

void Disassembler::onBlr()
{
	sprintf( instr_name, "blr" );
}

void Disassembler::onCmp(int bf, bool l, int ra, int rb)
{
	sprintf( instr_name, "cmp%s", l ? "d" : "w" );

	if( bf == 0 ) {
		sprintf( instr_args, "r%d,r%d", ra, rb );
	}
	else {
		sprintf( instr_args, "cr%d,r%d,r%d", bf, ra, rb );
	}
}

void Disassembler::onCmpi(int bf, bool l, int ra, int16_t si)
{
	sprintf( instr_name, "cmp%si", l ? "d" : "w" );

	if( bf == 0 ) {
		sprintf( instr_args, "r%d,%d", ra, si );
	}
	else {
		sprintf( instr_args, "cr%d,r%d,%d", bf, ra, si );
	}
}

void Disassembler::onCmpl(int bf, bool l, int ra, int rb)
{
	sprintf( instr_name, "cmpl%s", l ? "d" : "w" );

	if( bf == 0 ) {
		sprintf( instr_args, "r%d,r%d", ra, rb );
	}
	else {
		sprintf( instr_args, "cr%d,r%d,r%d", bf, ra, rb );
	}
}

void Disassembler::onCmpli(int bf, bool l, int ra, uint16_t ui)
{
	sprintf( instr_name, "cmpl%si", l ? "d" : "w" );

	if( bf == 0 ) {
		sprintf( instr_args, "r%d,%d", ra, ui );
	}
	else {
		sprintf( instr_args, "cr%d,r%d,%d", bf, ra, ui );
	}
}

void Disassembler::onDcbf(int ra, int rb)
{
	if( ra == 0 ) {
		print_ir( "dcbf", ra, rb );
	}
	else {
		print_rr( "dcbf", ra, rb, false );
	}
}

void Disassembler::onDcbst(int ra, int rb)
{
	if( ra == 0 ) {
		print_ir( "dcbst", ra, rb );
	}
	else {
		print_rr( "dcbst", ra, rb, false );
	}
}

void Disassembler::onDivdu(int rt, int ra, int rb, bool oe, bool rc)
{
	if( oe ) {
		print_rrr( "divduo", rt, ra, rb, rc );
	}
	else {
		print_rrr( "divdu", rt, ra, rb, rc );
	}
}

void Disassembler::onIcbi(int ra, int rb)
{
	if( ra == 0 ) {
		print_ir( "icbi", ra, rb );
	}
	else {
		print_rr( "icbi", ra, rb, false );
	}
}

void Disassembler::onExtsh(int ra, int rs, bool rc)
{
	print_rr( "extsh", ra, rs, rc );
}

void Disassembler::onExtsw(int ra, int rs, bool rc)
{
	print_rr( "extsw", ra, rs, rc );
}

void Disassembler::onFmul(int frt, int fra, int frc, bool rc)
{
	print_fff( "fmul", frt, fra, frc, rc );
}

void Disassembler::onFnmadd(int frt, int fra, int frc, int frb, bool rc)
{
	print_ffff( "fnmadd", frt, fra, frc, frb, rc );
}

void Disassembler::onIsync()
{
	sprintf( instr_name, "isync" );
}

void Disassembler::onLbz(int rt, int16_t d, int ra)
{
	print_mem_args( "lbz", rt, d, ra );
}

void Disassembler::onLbzu(int rt, int16_t d, int ra)
{
	print_mem_args( "lbzu", rt, d, ra );
}

void Disassembler::onLbzx(int rt, int ra, int rb)
{
	print_rrr( "lbzx", rt, ra, rb, false );
}

void Disassembler::onLd(int rt, int16_t ds, int ra)
{
	print_mem_args( "ld", rt, ds, ra );
}

void Disassembler::onLdu(int rt, int16_t ds, int ra)
{
	print_mem_args( "ldu", rt, ds, ra );
}

void Disassembler::onLfd(int frt, int16_t d, int ra)
{
	print_mem_args_float( "lfd", frt, d, ra );
}

void Disassembler::onLfdu(int frt, int16_t d, int ra)
{
	print_mem_args_float( "lfdu", frt, d, ra );
}

void Disassembler::onLfs(int frt, int16_t d, int ra)
{
	print_mem_args_float( "lfs", frt, d, ra );
}

void Disassembler::onLfsu(int frt, int16_t d, int ra)
{
	print_mem_args_float( "lfsu", frt, d, ra );
}

void Disassembler::onLq(int rt, int16_t dq, int ra)
{
	print_mem_args( "lq", rt, dq, ra );
}

void Disassembler::onLha(int rt, int16_t d, int ra)
{
	print_mem_args( "lha", rt, d, ra );
}

void Disassembler::onLhau(int rt, int16_t d, int ra)
{
	print_mem_args( "lhau", rt, d, ra );
}

void Disassembler::onLhax(int rt, int ra, int rb)
{
	print_rrr( "lhax", rt, ra, rb, false );
}

void Disassembler::onLhz(int rt, int16_t d, int ra)
{
	print_mem_args( "lhz", rt, d, ra );
}

void Disassembler::onLhzu(int rt, int16_t d, int ra)
{
	print_mem_args( "lhzu", rt, d, ra );
}

void Disassembler::onLhzx(int rt, int ra, int rb)
{
	print_rrr( "lhzx", rt, ra, rb, false );
}

void Disassembler::onLmw(int rt, int16_t d, int ra)
{
	print_mem_args( "lmw", rt, d, ra );
}

void Disassembler::onLwa(int rt, int16_t ds, int ra)
{
	print_mem_args( "lwa", rt, ds, ra );
}

void Disassembler::onLwz(int rt, int16_t d, int ra)
{
	if( ra != 0 ) {
		print_mem_args( "lwz", rt, d, ra );
	}
	else {
		sprintf( instr_name, "lwz" );
		sprintf( instr_args, "r%d,%d(%d)", rt, d, ra );
	}
}

void Disassembler::onLwzu(int rt, int16_t d, int ra)
{
	print_mem_args( "lwzu", rt, d, ra );
}

void Disassembler::onLwzx(int rt, int ra, int rb)
{
	print_rrr( "lwzx", rt, ra, rb, false );
}

void Disassembler::onMfcr(int rt)
{
	sprintf( instr_name, "mfcr" );
	sprintf( instr_args, "r%d", rt );
}

void Disassembler::onMfmsr(int rt)
{
	sprintf( instr_name, "mfmsr" );
	sprintf( instr_args, "r%d", rt );
}

void Disassembler::onMfspr(int rt, int spr)
{
	switch( spr ) {
		case 1:   print_r("mfxer", rt);   break;

		case 8:   print_r("mflr", rt);    break;
		case 9:   print_r("mfctr", rt);   break;

		case 18:  print_r("mfdsisr", rt); break;
		case 19:  print_r("mfdar", rt);   break;

		case 26:  print_r("mfsrr0", rt);  break;
		case 27:  print_r("mfsrr1", rt);  break;

		case 272: print_ri("mfsprg", rt, 0, false); break;
		case 273: print_ri("mfsprg", rt, 1, false); break;
		case 274: print_ri("mfsprg", rt, 2, false); break;
		case 275: print_ri("mfsprg", rt, 3, false); break;

		case 287: print_r("mfpvr", rt); break;

		default:  print_ri("mfspr", rt, spr, false); break;
	}
}

void Disassembler::onMftb(int rt, int tbr)
{
	switch( tbr ) {
	case 268: print_r( "mftbl", rt ); break;
	case 269: print_r( "mftbu", rt ); break;

	default: {
		print_ri( "mftb", rt, tbr, false );
	}
	break;
	}
}

void Disassembler::onMtcrf(int fxm, int rs)
{
	if( fxm == 0xFF ) {
		print_r( "mtcr", rs );
	}
	else {
		print_ir( "mtcrf", fxm, rs );
	}
}

void Disassembler::onMtmsr(int rs, bool l)
{
	if( l ) {
		print_ri( "mtmsr", rs, l, false );
	}
	else {
		print_r("mtmsr", rs);
	}
}

void Disassembler::onMtmsrd(int rs, bool l)
{
	print_ri( "mtmsrd", rs, l, false );
}

void Disassembler::onMtocrf(int fxm, int rs)
{
	print_ir( "mtocrf", fxm, rs );
}

void Disassembler::onMtspr(int spr, int rs)
{
	switch( spr ) {
		case 1:    print_r("mtxer", rs);  break;

		case 8:    print_r("mtlr", rs);   break;
		case 9:    print_r("mtctr", rs);  break;

		case 26:   print_r("mtsrr0", rs); break;
		case 27:   print_r("mtsrr1", rs); break;

		case 152:  print_r("mtcmpe", rs); break;

		case 272:  print_ir("mtsprg", 0, rs); break;
		case 273:  print_ir("mtsprg", 1, rs); break;
		case 274:  print_ir("mtsprg", 2, rs); break;
		case 275:  print_ir("mtsprg", 3, rs); break;

		case 1017: print_r("mtl2cr", rs); break;

		default:   print_ir("mtspr", spr, rs); break;
	}
}

void Disassembler::onMulld(int rt, int ra, int rb, bool oe, bool rc)
{
	if( oe ) {
		print_rrr( "mulldo", rt, ra, rb, rc );
	}
	else {
		print_rrr( "mulld", rt, ra, rb, rc );
	}
}

void Disassembler::onMulli(int rt, int ra, int16_t si)
{
	sprintf( instr_name, "mulli" );
	sprintf( instr_args, "r%d,r%d,%d", rt, ra, si );
}

void Disassembler::onNeg(int rt, int ra, bool oe, bool rc)
{
	if( oe ) {
		print_rr( "nego", rt, ra, rc );
	}
	else {
		print_rr( "neg", rt, ra, rc );
	}
}

void Disassembler::onNor(int ra, int rs, int rb, bool rc)
{
	if( rs == rb ) {
		print_rr( "not", ra, rs, rc );
	}
	else {
		print_rrr( "nor", ra, rs, rb, rc );
	}
}

void Disassembler::onOr(int ra, int rs, int rb, bool rc)
{
	if( rs == rb ) {
		print_rr( "mr", ra, rs, rc );
	}
	else {
		print_rrr( "or", ra, rs, rb, rc );
	}
}

void Disassembler::onOri(int ra, int rs, uint16_t ui)
{
	if( ra == 0 && rs == 0 && ui == 0 ) {
		sprintf( instr_name, "nop" );
	}
	else {
		print_rri( "ori", ra, rs, ui, false );
	}
}

void Disassembler::onOris(int ra, int rs, uint16_t ui)
{
	print_rri( "oris", ra, rs, ui, false );
}

void Disassembler::onRfid()
{
	sprintf( instr_name, "rfid" );
}

void Disassembler::onRldicl(int ra, int rs, int sh, int mb, bool rc)
{
	if( mb == 0 ) {
		print_rri( "rotldi", ra, rs, sh, rc );
	}
	else if( sh == 0 ) {
		print_rri( "clrldi", ra, rs, mb, rc );
	}
	else {
		print_rrii( "rldicl", ra, rs, sh, mb, rc );
	}
}

void Disassembler::onRldicr(int ra, int rs, int sh, int me, bool rc)
{
	print_rrii( "rldicr", ra, rs, sh, me, rc );
}

void Disassembler::onRldimi(int ra, int rs, int sh, int mb, bool rc)
{
	print_rrii( "rldimi", ra, rs, sh, mb, rc );
}

void Disassembler::onRlwimi(int ra, int rs, int sh, int mb, int me, bool rc)
{
	print_rriii( "rlwimi", ra, rs, sh, mb, me, rc );
}

void Disassembler::onRlwinm(int ra, int rs, int sh, int mb, int me, bool rc)
{
	if( mb == 0 && me == 31 ) {
		sprintf( instr_name, "rotlwi%s", rc ? "." : "" );
		sprintf( instr_args, "r%d,r%d,%d", ra, rs, sh );
	}
	else if( sh == 0 && me == 31 ) {
		sprintf( instr_name, "clrlwi%s", rc ? "." : "" );
		sprintf( instr_args, "r%d,r%d,%d", ra, rs, mb );
	}
	else {
		sprintf( instr_name, "rlwinm%s", rc ? "." : "" );
		sprintf( instr_args, "r%d,r%d,%d,%d,%d", ra, rs, sh, mb, me );
	}
}

void Disassembler::onRlwnm(int ra, int rs, int rb, int mb, int mr, bool rc)
{
	print_rrrii("rlwnm", ra, rs, rb, mb, mr, rc);
}

void Disassembler::onSc(int lev)
{
	(void)lev;
}

void Disassembler::onSld(int ra, int rs, int rb, bool rc)
{
	print_rrr( "sld", ra, rs, rb, rc );
}

void Disassembler::onSlw(int ra, int rs, int rb, bool rc)
{
	print_rrr( "slw", ra, rs, rb, rc );
}

void Disassembler::onSrad(int ra, int rs, int rb, bool rc)
{
	print_rrr( "srad", ra, rs, rb, rc );
}

void Disassembler::onSradi(int ra, int rs, int sh, bool rc)
{
	print_rri( "sradi", ra, rs, sh, rc );
}

void Disassembler::onSrawi(int ra, int rs, int sh, bool rc)
{
	print_rri( "srawi", ra, rs, sh, rc );
}

void Disassembler::onStb(int rs, int16_t d, int ra)
{
	print_mem_args( "stb", rs, d, ra );
}

void Disassembler::onStbu(int rs, int16_t d, int ra)
{
	print_mem_args( "stbu", rs, d, ra );
}

void Disassembler::onStbx(int rs, int ra, int rb)
{
	print_rrr( "stbx", rs, ra, rb, false );
}

void Disassembler::onStd(int rs, int16_t ds, int ra)
{
	print_mem_args( "std", rs, ds, ra );
}

void Disassembler::onStdu(int rs, int16_t ds, int ra)
{
	print_mem_args( "stdu", rs, ds, ra );
}

void Disassembler::onStfd(int frs, int16_t d, int ra)
{
	print_mem_args_float( "stfd", frs, d, ra );
}

void Disassembler::onStfdu(int frs, int16_t d, int ra)
{
	print_mem_args_float( "stfdu", frs, d, ra );
}

void Disassembler::onStfs(int frs, int16_t d, int ra)
{
	print_mem_args_float( "stfs", frs, d, ra );
}

void Disassembler::onStfsu(int frs, int16_t d, int ra)
{
	print_mem_args_float( "stfsu", frs, d, ra );
}

void Disassembler::onSth(int rs, int16_t d, int ra)
{
	print_mem_args( "sth", rs, d, ra );
}

void Disassembler::onSthu(int rs, int16_t d, int ra)
{
	print_mem_args( "sthu", rs, d, ra );
}

void Disassembler::onSthx(int rs, int ra, int rb)
{
	print_rrr( "sthx", rs, ra, rb, false );
}

void Disassembler::onStmw(int rs, int16_t d, int ra)
{
	print_mem_args( "stmw", rs, d, ra );
}

void Disassembler::onStq(int rs, int16_t ds, int ra)
{
	print_mem_args( "stq", rs, ds, ra );
}

void Disassembler::onStw(int rs, int16_t d, int ra)
{
	print_mem_args( "stw", rs, d, ra );
}

void Disassembler::onStwu(int rs, int16_t d, int ra)
{
	print_mem_args( "stwu", rs, d, ra );
}

void Disassembler::onStwx(int rs, int ra, int rb)
{
	print_rrr( "stwx", rs, ra, rb, false );
}

void Disassembler::onSubf(int rt, int ra, int rb, bool oe, bool rc)
{
	if( oe ) {
		print_rrr( "subfo", rt, ra, rb, rc );
	}
	else {
		print_rrr( "subf", rt, ra, rb, rc );
	}
}

void Disassembler::onSubfe(int rt, int ra, int rb, bool oe, bool rc)
{
	if( oe ) {
		print_rrr( "subfeo", rt, ra, rb, rc );
	}
	else {
		print_rrr( "subfe", rt, ra, rb, rc );
	}
}

void Disassembler::onSubfic(int rt, int ra, int16_t si)
{
	print_rri( "subfic", rt, ra, si, false );
}

void Disassembler::onSubfze(int rt, int ra, bool oe, bool rc)
{
	if( oe ) {
		print_rr( "subfzeo", rt, ra, rc );
	}
	else {
		print_rr( "subfze", rt, ra, rc );
	}
}

void Disassembler::onSync(int l)
{
	(void)l;
	sprintf( instr_name, "sync    " );
}

void Disassembler::onTdi(uint8_t to, int ra, int16_t si)
{
	(void)to;
	(void)ra;
	(void)si;
}

void Disassembler::onTlbiel(int rb, int l)
{
	print_ri( "tlbiel", rb, l, false );
}

void Disassembler::onTwi(uint8_t to, int ra, int16_t si)
{
	(void)to;
	(void)ra;
	(void)si;
}

void Disassembler::onXori(int ra, int rs, uint16_t ui)
{
	print_rri( "xori", ra, rs, ui, false );
}

void Disassembler::onXoris(int ra, int rs, uint16_t ui)
{
	print_rri( "xoris", ra, rs, ui, false );
}

}}} /*namespace remu::jitpp::ppc*/

