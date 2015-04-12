#include "jitpp/BasicBlockInterpreter.h"

#include <cstdio>

namespace remu { namespace jitpp {

bool BasicBlockInterpreter::execute_block( const BasicBlock &block, ACState &cpu_state )
{
	(void)block;
	(void)cpu_state;

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
				printf("LDR_ADDR\n");
				return false;
			}
			break;

			case ACInstrType::LDR_IMM: {
				if( instr.op[0].type != ACOpType::L || instr.op[1].type != ACOpType::IMMU32 ) {
					printf("LDR_IMM {{type=%d},{type=%d}}\n", instr.op[0].type, instr.op[1].type );
					return false;
				}
				else {
					cpu_state.l[ instr.op[0].reg ] = instr.op[1].u32;
				}
			}
			break;

			case ACInstrType::STR_ADDR: {
				printf("STR_ADDR\n");
				return false;
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

			default: {
				printf( "Instr.type = %d\n", instr.type );
				return false;
			}
			break;
		}
	}

	return true;
}

}} /*namespace remu::jitpp*/

