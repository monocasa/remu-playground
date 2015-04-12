#ifndef REMU_JITPP_BASICBLOCK_H
#define REMU_JITPP_BASICBLOCK_H

#include "jitpp/ACInstr.h"

#include <vector>

namespace remu { namespace jitpp {

struct BasicBlock
{
	uint64_t base;

	std::vector<ACInstr> instrs;
};

}} /*namespace remu::jitpp*/

#endif /*REMU_JITPP_BASICBLOCK_H*/

