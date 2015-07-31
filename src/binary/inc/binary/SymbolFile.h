#ifndef REMU_BINARY_SYMBOLFILE_H
#define REMU_BINARY_SYMBOLFILE_H

#include <cstdint>
#include <map>
#include <string>
#include <vector>

namespace remu { namespace binary {

struct Symbol
{
	std::string name;
	uint64_t base;
};

class SymbolDB
{
public:
	const Symbol* getSymbolClosest( uint64_t addr );
	const Symbol* getSymbolExact( uint64_t addr );

	void addSymbol( const Symbol& symbol );

private:
	std::map<uint64_t, std::vector<Symbol>> _symbols;
};

class SymbolFile
{
public:
	virtual const SymbolDB& getSymbolDB() = 0;
};

}} /*namespace remu::binary*/

#endif /*REMU_BINARY_SYMBOLFILE_H*/

