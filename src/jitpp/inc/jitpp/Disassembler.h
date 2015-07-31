#ifndef REMU_JITPP_DISASSEMBLER_H
#define REMU_JITPP_DISASSEMBLER_H

#include "util/memslice.h"

namespace remu { namespace jitpp {

class Disassembler
{
public:
	virtual ~Disassembler() = default;

	virtual size_t disassemble( const util::memslice &slice, 
	                            uint64_t addr, 
	                            char *buffer, 
	                            size_t buffer_size ) = 0;
};

}} /*namespace remu::jitpp*/

#endif /*REMU_JITPP_DISASSEMBLER_H*/

