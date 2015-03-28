#ifndef REMU_JITPP_CODEPAGE_H
#define REMU_JITPP_CODEPAGE_H

#include "jitpp/ACState.h"

namespace remu { namespace jitpp {

struct BasicBlock;

template<int SIZE, class Translator>
class CodePage
{
public:
	CodePage( const ACFarPointer &initial_pc, Translator &translator, void *host_base )
	  : base( { ((initial_pc.program_counter / SIZE) * SIZE), initial_pc.code_segment } )
	  , translator( translator )
	  , host_base( host_base )
	{
		memset( basic_blocks, 0, sizeof(basic_blocks) );
	}

	~CodePage() {
		for( int ii = 0; ii < SIZE; ii++ ) {
			if( basic_blocks[ii] ) {
				delete basic_blocks[ii];
			}
		}
	}

	const ACFarPointer& getBase() const {
		return base;
	}

	const void* getHostBase() const {
		return host_base;
	}

	bool execute( ACState &cpu_state ) {
		const int pc_in_page = cpu_state.ip.program_counter % SIZE;
		auto block = basic_blocks[pc_in_page];
		if( !block ) {
			block = translator.generate_basic_block(cpu_state.ip.program_counter, host_base);
			if( !block ) {
				return false;
			}
		}
		
		return true;
	}

private:
	ACFarPointer base;
	Translator &translator;
	const void *host_base;

	BasicBlock *basic_blocks[SIZE];
};

}} /*namespace remu::jitpp*/

#endif /*REMU_JITPP_CODEPAGE_H*/

