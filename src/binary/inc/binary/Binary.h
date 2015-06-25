#ifndef REMU_BINARY_BINARY_H
#define REMU_BINARY_BINARY_H

#include <cstdint>
#include <string>

namespace remu { namespace binary {

class Binary
{
public:
	struct Symbol {
		std::string name;
		uint64_t addr;
	};

};

Binary* binaryForFile( const std::string& filename );

}} /*namespace remu::binary*/

#endif /*REMU_BINARY_BINARY_H*/

