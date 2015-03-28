#ifndef REMU_JITPP_CODEPAGE_H
#define REMU_JITPP_CODEPAGE_H

#include "jitpp/ACState.h"

namespace remu { namespace jitpp {

template<int SIZE>
class CodePage
{
public:
	CodePage( const ACFarPointer &initial_pc )
	  : base( { ((initial_pc.program_counter / SIZE) * SIZE), initial_pc.code_segment } )
	{ }

	const ACFarPointer& getBase() const {
		return base;
	}

	bool execute( ACState &cpu_state ) {
		(void)cpu_state;
		return false;
	}

private:
	ACFarPointer base;

};

}} /*namespace remu::jitpp*/

#endif /*REMU_JITPP_CODEPAGE_H*/

