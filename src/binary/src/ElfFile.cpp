#include "binary/ElfFile.h"

#include "oshal/Exception.h"

#include <sstream>

namespace remu { namespace binary {

ElfFile::ElfFile( oshal::File& file, const BinaryOptions& options )
  : _file( file )
  , _options( options )
{
	file.seek( 0 );
	file.readBuffer( _ident, sizeof(_ident) );

	if( !validateIdent(_ident) ) {
		throw oshal::Exception( "ELF Ident failed on %s", _file.name().c_str() );
	}

	file.setEndian( isLittleEndian() ? oshal::File::LITTLE : oshal::File::BIG );

	if( is32Bit() ) {
		throw oshal::Exception("32Bit read not implemented for ELF Header");
	}
	else {
		_type      = file.read<uint16_t>();
		_machine   = file.read<uint16_t>();
		_version   = file.read<uint32_t>();
		_entry     = file.read<uint64_t>();
		_phoff     = file.read<uint64_t>();
		_shoff     = file.read<uint64_t>();
		_flags     = file.read<uint32_t>();
		_ehsize    = file.read<uint16_t>();
		_phentsize = file.read<uint16_t>();
		_phnum     = file.read<uint16_t>();
		_shentsize = file.read<uint16_t>();
		_shnum     = file.read<uint16_t>();
		_shstrndx  = file.read<uint16_t>();
	}

	for( uint16_t ii = 0; ii < _shnum; ii++ ) {
		file.seek( _shoff + (ii * _shentsize) );

		ElfShdr shdr;

		if( is32Bit() ) {
			throw oshal::Exception( "32 bit read not implemented for ELF Shdr" );
		}
		else {
			shdr.name_offset = file.read<uint32_t>();
			shdr.type        = file.read<uint32_t>();
			shdr.flags       = file.read<uint64_t>();
			shdr.addr        = file.read<uint64_t>();
			shdr.offset      = file.read<uint64_t>();
			shdr.size        = file.read<uint64_t>();
			shdr.link        = file.read<uint32_t>();
			shdr.info        = file.read<uint32_t>();
			shdr.addralign   = file.read<uint64_t>();
			shdr.entsize     = file.read<uint64_t>();
		}
		_shdrs.push_back( shdr );
	}

	if( _shdrs.size() <= _shstrndx ) {
		return;
	}

	for( size_t ii = 0; ii < _shdrs.size(); ii++ ) {
		Segment seg;
		seg.num = ii;
		seg.name = readString( _shstrndx, _shdrs[ii].name_offset );
		seg.base = _shdrs[ii].addr;
		seg.size = _shdrs[ii].size;
		seg.executable = _shdrs[ii].flags & SHF_EXECINSTR;
		_sectionSegments.push_back( seg );
	}

	for( size_t ii = 0; ii < _shdrs.size(); ii++ ) {
		if( _shdrs[ii].type == SHT_SYMTAB ) {
			const int num_entities = _shdrs[ii].size / _shdrs[ii].entsize;

			for( int jj = 0; jj < num_entities; jj++ ) {
				file.seek( _shdrs[ii].offset + (jj * _shdrs[ii].entsize) );

				ElfSym sym;

				if( is32Bit() ) {
					throw oshal::Exception( "32 bit read not implements for ELF Sym" );
				}
				else {
					sym.name  = file.read<uint32_t>();
					sym.info  = file.read<uint8_t>();
					sym.other = file.read<uint8_t>();
					sym.shndx = file.read<uint16_t>();
					sym.value = file.read<uint64_t>();
					sym.size  = file.read<uint64_t>();
				}

				if( (sym.info == STT_FUNC || sym.info == STT_OBJECT) ) {
					Symbol symbol;
					symbol.name = readString( _shstrndx, sym.name );
					symbol.base = sym.value;

					_symbolDB.addSymbol( symbol );
				}
			}
		}
	}
}

bool ElfFile::validateIdent( oshal::File& file )
{
	uint8_t ident[16];

	file.seek( 0 );

	file.readBuffer( ident, sizeof(ident) );

	return validateIdent( ident );
}

MachineType ElfFile::machineType() const
{
	switch( _machine ) {
		case EM_PPC:   return MachineType::PPC;
		case EM_PPC64: return MachineType::PPC;
		case EM_ARM:   return MachineType::ARM;
		default:       return MachineType::UNKNOWN;
	}
}

std::string ElfFile::formatName() const
{
	std::stringstream ss;
	if( is32Bit() ) {
		ss << "elf32-";
	}
	else {
		ss << "elf64-";
	}

	switch( _machine ) {
	case EM_PPC:   ss << "powerpc"; break;
	case EM_PPC64: ss << "powerpc"; break;
	case EM_ARM:   ss << "arm";     break;
	default:       ss << "unknown"; break;
	}

	return ss.str();
}

const std::vector<Segment>& ElfFile::getSectionSegments()
{
	return _sectionSegments;
}

const std::vector<Segment>& ElfFile::getLoadSegments()
{
	return _loadSegments;
}

const SymbolDB& ElfFile::getSymbolDB()
{
	return _symbolDB;
}

std::vector<uint8_t> ElfFile::readSegment( const Segment& segment )
{
	if( segment.num == 0 || segment.num >= _shnum ) {
		throw oshal::Exception( "Attempt to read non existant section" );
	}

	auto& shdr = _shdrs[ segment.num ];

	std::vector<uint8_t> data( shdr.size );

	_file.seek( shdr.offset );
	_file.readBuffer( data.data(), data.size() );

	return data;
}

bool ElfFile::validateIdent( uint8_t *ident )
{
	if( ident[EI_MAG0] != ELFMAG0 || ident[EI_MAG1] != ELFMAG1 || 
	    ident[EI_MAG2] != ELFMAG2 || ident[EI_MAG3] != ELFMAG3 ) {
		return false;
	}

	if( (ident[EI_CLASS] != ELFCLASS32) && (ident[EI_CLASS] != ELFCLASS64) ) {
		return false;
	}

    if( (ident[EI_DATA] != ELFDATA2LSB) && (ident[EI_DATA] != ELFDATA2MSB) ){
		return false;
	}

    if( ident[EI_VERSION] != EV_CURRENT ) {
		return false;
	}

	return true;
}

std::string ElfFile::readString( uint16_t section, uint32_t name_offset )
{
	std::stringstream ss;

	if( section > _shnum ) {
		throw oshal::Exception( "Attempt to read string from non-existant section:  %x", section );
	}

	if( _str_sections.find(section) == _str_sections.end() ) {
		auto& shdr = _shdrs[section];
		_str_sections.emplace( std::pair<uint16_t, std::vector<char>>(section, std::vector<char>(shdr.size)) );
		_file.seek( shdr.offset );
		_file.readBuffer( (uint8_t*)_str_sections[section].data(), _str_sections[section].size() );
	}

	uint32_t cur_offset = name_offset;

	auto& section_vec = _str_sections[section];

	while( (cur_offset < section_vec.size()) && (section_vec[cur_offset] != '\0') )
	{
		ss << section_vec[ cur_offset++ ];
	}

	return ss.str();
}

}} /*namespace remu::binary*/

