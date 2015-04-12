#ifndef REMU_JITPP_ACINSTR_H
#define REMU_JITPP_ACINSTR_H

#include "jitpp/ACState.h"

namespace remu { namespace jitpp {

enum class ACOpType {
	B,
	S,
	L,
	Q,
	TB,
	TS,
	TL,
	TQ,
	P,
	IMMS8,
	IMMS16,
	IMMS32,
	IMMS64,
	IMMU8,
	IMMU16,
	IMMU32,
	IMMU64,
	ADDR32,
	ADDR64,
	FAR_ADDR,
};

struct ACOperand
{
	ACOpType type;
	union {
		int      reg;
		int8_t   s8;
		int16_t  s16;
		int32_t  s32;
		int64_t  s64;
		uint8_t  u8;
		uint16_t u16;
		uint32_t u32;
		uint64_t u64;
		uint32_t addr32;
		uint64_t addr64;
		ACFarPointer far_addr;
	};

	static ACOperand emitReg(ACOpType type, int reg) {
		ACOperand op;
		op.type = type;
		op.reg = reg;
		return op;
	}

	static ACOperand emitB(int reg) {
		return emitReg(ACOpType::B, reg);
	}

	static ACOperand emitS32(int32_t value) {
		ACOperand op;
		op.type = ACOpType::IMMS32;
		op.s32 = value;
		return op;
	}

	static ACOperand emitU32(uint32_t value) {
		ACOperand op;
		op.type = ACOpType::IMMU32;
		op.u32 = value;
		return op;
	}

	static ACOperand emitAddr32(uint32_t addr32) {
		ACOperand op;
		op.type = ACOpType::ADDR32;
		op.addr32 = addr32;
		return op;
	}
};

enum class ACInstrType
{
	UNKNOWN,
	UNIMPLEMENTED,

	LDR_ADDR,
	LDR_IMM,
	STR_ADDR,
	STR_ADDR_S32,

	B,
	BL,
	BX,
};

enum class ACPredType
{
	ALWAYS,
	PRED_TRUE,
	PRED_FALSE,
};

struct ACInstr
{
	ACInstrType type;
	ACPredType pred_type;
	int pred_num;
	ACOperand op[2];

	ACInstr()
	  : type( ACInstrType::UNKNOWN )
	  , pred_type( ACPredType::ALWAYS )
	  , pred_num( 0 )
	{ }

	ACInstr( ACInstrType type )
	  : type( type )
	  , pred_type( ACPredType::ALWAYS )
	  , pred_num( 0 )
	{ }

	static ACInstr emitUnimplemented() {
		ACInstr instr( ACInstrType::UNIMPLEMENTED );
		return instr;
	}

	static ACInstr emitLdrImmU32(ACOpType regType, int reg, uint32_t value) {
		ACInstr instr( ACInstrType::LDR_IMM );
		instr.op[0] = ACOperand::emitReg( regType, reg );
		instr.op[1] = ACOperand::emitU32( value );
		return instr;
	}

	static ACInstr emitStrAddr(ACOpType srcType, int srcReg, ACOpType addrType, int addrReg ) {
		ACInstr instr( ACInstrType::STR_ADDR );
		instr.op[0] = ACOperand::emitReg(srcType, srcReg);
		instr.op[1] = ACOperand::emitReg(addrType, addrReg);
		return instr;
	}

	static ACInstr emitStrAddrS32(ACOpType srcType, int srcReg, ACOpType addrType, int addrReg, int32_t off ) {
		ACInstr instr( ACInstrType::STR_ADDR );
		instr.op[0] = ACOperand::emitReg(srcType, srcReg);
		instr.op[1] = ACOperand::emitReg(addrType, addrReg);
		instr.op[2] = ACOperand::emitS32(off);
		return instr;
	}

	static ACInstr emitB32(uint32_t target) {
		ACInstr instr( ACInstrType::B );
		instr.op[0] = ACOperand::emitAddr32( target );
		return instr;
	}

	static ACInstr emitBL32(ACOpType regType, int reg, uint32_t target) {
		ACInstr instr( ACInstrType::BL );
		instr.op[0] = ACOperand::emitReg( regType, reg );
		instr.op[1] = ACOperand::emitAddr32( target );
		return instr;
	}

	static ACInstr emitBX(ACOpType regType, int reg) {
		ACInstr instr( ACInstrType::BX );
		instr.op[0] = ACOperand::emitReg( regType, reg );
		return instr;
	}
};

}} /*namespace remu::jitpp*/

#endif /*REMU_JITPP_ACINSTR_H*/

