#ifndef REMU_UTIL_MEMSLICE_H
#define REMU_UTIL_MEMSLICE_H

#include <cstdint>
#include <cstdlib>

namespace remu { namespace util {

struct memslice
{
	uint8_t* data;
	size_t size;
};

}} /*namespace remu::util*/

#endif /*REMU_UTIL_MEMSLICE_H*/

