#ifndef REMU_JITPP_BASICBLOCKINTERPRETER_H
#define REMU_JITPP_BASICBLOCKINTERPRETER_H

#include "jitpp/ACState.h"
#include "jitpp/BasicBlock.h"

namespace remu { namespace jitpp {

class BasicBlockInterpreter
{
public:
	bool execute_block( const BasicBlock &block, ACState &cpu_state );
};

}} /*namespace remu::jitpp*/

#endif /*REMU_JITPP_BASICABLOCKINTERPRETER_H*/

