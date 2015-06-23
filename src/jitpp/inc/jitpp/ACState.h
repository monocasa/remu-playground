#ifndef REMU_JITPP_ACSTATE_H
#define REMU_JITPP_ACSTATE_H

#include <cstdint>
#include <cstring>

namespace remu { namespace jitpp {

struct ACFarPointer
{
	uint64_t program_counter;
	uint64_t code_segment;

	inline bool operator==( const ACFarPointer& other ) const {
		return (this->program_counter == other.program_counter) && (this->code_segment == other.code_segment);
	}

	inline bool operator!=( const ACFarPointer& other ) const {
		return !(*this == other);
	}
};

struct ACState
{
	union {
		uint8_t  b[4096];
		uint16_t s[2048];
		uint32_t l[1024];
		uint64_t q[ 512];
	};
	ACFarPointer ip;

	void clear() {
		memset( this, 0, sizeof(*this) );
	}
};

}} /*namespace remu::jitpp*/

#endif /*REMU_JITPP_ACSTATE_H*/

