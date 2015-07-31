#include "binary/SymbolFile.h"

namespace remu { namespace binary {

const Symbol* SymbolDB::getSymbolExact( uint64_t addr )
{
	if( _symbols.find(addr) == _symbols.end() ) {
		return nullptr;
	}
	else {
		return _symbols[addr].data();
	} 
}

void SymbolDB::addSymbol( const Symbol& symbol )
{
	if( _symbols.find(symbol.base) == _symbols.end() ) {
		_symbols.emplace( std::pair<uint64_t, std::vector<Symbol>>(symbol.base, std::vector<Symbol>()) );
	}

	_symbols[ symbol.base ].push_back( symbol );
}

}} /*namespace remu::binary*/

