#include "binary/Binary.h"
#include "binary/ElfFile.h"

#include "oshal/File.h"

namespace {

const remu::binary::SymbolDB emptySymbolDB;

} /*anonymous namespace*/

namespace remu { namespace binary {

const SymbolDB& Binary::getSymbolDB()
{
	return emptySymbolDB;
}

std::unique_ptr<Binary> binaryForFile( oshal::File& file, const BinaryOptions& options )
{
	if( ElfFile::validateIdent(file) ) {
		return std::unique_ptr<Binary>( new ElfFile(file, options) );
	}

	return std::unique_ptr<Binary>();
}

}} /*namespace remu::binary*/

