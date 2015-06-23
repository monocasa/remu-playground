#include "jitpp/BasicBlockInterpreter.h"

#include <cstdio>

namespace remu { namespace jitpp {

bool BasicBlockInterpreter::execute_block( const BasicBlock &block, ACState &cpu_state )
{
	if( block.instrs.size() == 0 ) {
		printf("instrs.siz() == 0\n");
		return false;
	}

	for( auto& instr : block.instrs ) {
		switch( instr.type ) {
			////////////////// M I S C   I N S T R U C T I O N S /////////////////////////////
			case ACInstrType::UNKNOWN: {
				printf("Unknown\n");
				return false;
			}
			break;

			case ACInstrType::UNIMPLEMENTED: {
				printf("Unimplemented\n");
				return false;
			}
			break;

			////////////////// L O A D / S T O R E   I N S T R U C T I O N S /////////////////
			case ACInstrType::LDR_ADDR: {
				if( instr.op[0].type == ACOpType::L && 
				    instr.op[1].type == ACOpType::ADDR32 &&
				    instr.op[2].type == ACOpType::IMMS32 ) {
					const auto ptr = reinterpret_cast<uint32_t*>( instr.op[1].addr32 + instr.op[2].s32 );
					cpu_state.l[ instr.op[0].reg ] = *ptr;
				}
				else if( instr.op[0].type == ACOpType::L &&
				         instr.op[1].type == ACOpType::L &&
				         instr.op[2].type == ACOpType::IMMS32 ) {
					const auto ptr = reinterpret_cast<uint32_t*>( cpu_state.l[instr.op[1].reg] + instr.op[2].s32 );
					cpu_state.l[ instr.op[0].reg ] = *ptr;
				}
				else {
					printf("LDR_ADDR {{type=%d},{type=%d},{type=%d}}\n", instr.op[0].type, instr.op[1].type, instr.op[2].type );
					return false;
				}
			}
			break;

			case ACInstrType::LDR_IMM: {
				if( instr.op[0].type == ACOpType::L && 
				    instr.op[1].type == ACOpType::IMMU32 ) {
					cpu_state.l[ instr.op[0].reg ] = instr.op[1].u32;
				}
				else {
					printf("LDR_IMM {{type=%d},{type=%d}}\n", instr.op[0].type, instr.op[1].type );
					return false;
				}
			}
			break;

			case ACInstrType::LDR_REG: {
				if( instr.op[0].type == ACOpType::L && 
				    instr.op[1].type == ACOpType::L ) {
					cpu_state.l[ instr.op[0].reg ] = cpu_state.l[ instr.op[1].reg ];
				}
				else {
					printf("LDR_REG {{type=%d},{type=%d}}\n", instr.op[0].type, instr.op[1].type );
					return false;
				}
			}
			break;

			case ACInstrType::STR_ADDR: {
				if( instr.op[0].type == ACOpType::L && 
				    instr.op[1].type == ACOpType::L && 
				    instr.op[2].type == ACOpType::IMMS32 ) {
					const uint32_t addr = cpu_state.l[ instr.op[1].reg ] + instr.op[2].s32;
					*reinterpret_cast<uint32_t*>( addr ) = cpu_state.l[ instr.op[0].reg ];
				}
				else {
					printf("STR_ADDR {{type=%d},{type=%d},{type=%d}}\n", instr.op[0].type, instr.op[1].type, instr.op[2].type );
					return false;
				}
			}
			break;

			case ACInstrType::STR_ADDR_S32: {
				printf("STR_ADDR_S32\n");
				return false;
			}
			break;

			////////////////// B R A N C H   I N S T R U C T I O N S /////////////////////////
			case ACInstrType::B: {
				if( instr.op[0].type == ACOpType::ADDR32 ) {
					cpu_state.ip.program_counter = instr.op[0].addr32;
				}
				else {
					printf("B {type=%d}\n", instr.op[1].type);
					return false;
				}
			}
			break;

			case ACInstrType::BL: {
				printf("BL\n");
				return false;
			}
			break;

			case ACInstrType::BX: {
				if( instr.op[0].type == ACOpType::L ) {
					cpu_state.ip.program_counter = cpu_state.l[ instr.op[0].reg ];
				}
				else {
					printf("BX {type=%d}\n", instr.op[1].type);
					return false;
				}
			}
			break;

			////////////////// A R I T H M E T I C   I N S T R U C T I O N S  ////////////////
			case ACInstrType::ADD: {
				if( instr.op[0].type == ACOpType::L && 
				    instr.op[1].type == ACOpType::L && 
				    instr.op[2].type == ACOpType::IMMU32 ) {
					const uint32_t result = cpu_state.l[ instr.op[1].reg ] + instr.op[2].s32;
					cpu_state.l[ instr.op[0].reg ] = result;
				}
				else {
					printf("ADD {{type=%d},{type=%d},{type=%d}}\n", instr.op[0].type, instr.op[1].type, instr.op[2].type );
					return false;
				}
			}
			break;


			case ACInstrType::SUB: {
				if( instr.op[0].type == ACOpType::L && 
				    instr.op[1].type == ACOpType::L && 
				    instr.op[2].type == ACOpType::IMMS32 ) {
					const uint32_t result = cpu_state.l[ instr.op[1].reg ] - instr.op[2].s32;
					cpu_state.l[ instr.op[0].reg ] = result;
				}
				else {
					printf("SUB {{type=%d},{type=%d},{type=%d}}\n", instr.op[0].type, instr.op[1].type, instr.op[2].type );
					return false;
				}
			}
			break;

			////////////////// L O G I C A L   I N S T R U C T I O N S  //////////////////////
			case ACInstrType::AND: {
				if( instr.op[0].type == ACOpType::L && 
				    instr.op[1].type == ACOpType::L && 
				    instr.op[2].type == ACOpType::IMMU32 ) {
					const uint32_t result = cpu_state.l[ instr.op[1].reg ] & instr.op[2].u32;
					cpu_state.l[ instr.op[0].reg ] = result;
				}
				else {
					printf("AND {{type=%d},{type=%d},{type=%d}}\n", instr.op[0].type, instr.op[1].type, instr.op[2].type );
					return false;
				}
			}
			break;

			case ACInstrType::OR: {
				if( instr.op[0].type == ACOpType::L && 
				    instr.op[1].type == ACOpType::L && 
				    instr.op[2].type == ACOpType::IMMU32 ) {
					const uint32_t result = cpu_state.l[ instr.op[1].reg ] | instr.op[2].u32;
					cpu_state.l[ instr.op[0].reg ] = result;
				}
				else {
					printf("OR {{type=%d},{type=%d},{type=%d}}\n", instr.op[0].type, instr.op[1].type, instr.op[2].type );
					return false;
				}
			}
			break;
		}
	}

	return true;
}

}} /*namespace remu::jitpp*/

