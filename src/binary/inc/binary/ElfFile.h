#ifndef REMU_BINARY_ELFFILE_H
#define REMU_BINARY_ELFFILE_H

#include "binary/Binary.h"

#include "oshal/File.h"

namespace remu { namespace binary {

class ElfFile : public Binary
{
public:
	ElfFile( oshal::File& file, const BinaryOptions& options );

	static bool validateIdent( oshal::File &file );

	MachineType machineType() const override;

	std::string formatName() const override;

	const std::vector<Segment>& getSectionSegments() override;

	const std::vector<Segment>& getLoadSegments() override;

	const SymbolDB& getSymbolDB() override final;

	std::vector<uint8_t> readSegment( const Segment &segment ) override final;

private:
	static const int EI_MAG0 = 0;
	static const int EI_MAG1 = 1;
	static const int EI_MAG2 = 2;
	static const int EI_MAG3 = 3;
	static const int EI_CLASS   = 4;
	static const int EI_DATA    = 5;
	static const int EI_VERSION = 6;
	static const int EI_NIDENT  = 16;

	static const uint8_t ELFMAG0 = 0x7F;
	static const uint8_t ELFMAG1 = 'E';
	static const uint8_t ELFMAG2 = 'L';
	static const uint8_t ELFMAG3 = 'F';

	static const uint8_t ELFCLASS32 = 1;
	static const uint8_t ELFCLASS64 = 2;

	static const uint8_t ELFDATA2LSB = 1;
	static const uint8_t ELFDATA2MSB = 2;

	static const uint16_t EM_PPC   = 20;
	static const uint16_t EM_PPC64 = 21;
	static const uint16_t EM_ARM   = 40;

	static const uint8_t EV_CURRENT = 1;

	static const uint16_t ET_EXEC = 2;

	static const uint32_t PT_LOAD = 1;

	static const uint32_t SHT_SYMTAB = 2;
	static const uint32_t SHT_STRTAB = 3;

	static const uint64_t SHF_EXECINSTR = (1ULL << 2);

	static const uint8_t STT_OBJECT = 1;
	static const uint8_t STT_FUNC   = 2;

	struct ElfShdr
	{
		uint32_t name_offset;
		uint32_t type;
		uint64_t flags;
		uint64_t addr;
		uint64_t offset;
		uint64_t size;
		uint32_t link;
		uint32_t info;
		uint64_t addralign;
		uint64_t entsize;
	};

	struct ElfSym
	{
		uint32_t name;
		uint8_t  info;
		uint8_t  other;
		uint16_t shndx;
		uint64_t value;
		uint64_t size;
	};

	static bool validateIdent( uint8_t *ident );

	std::string readString( uint16_t section, uint32_t name_offset );

	bool isLittleEndian() const {
		return _ident[EI_DATA] == ELFDATA2LSB;
	}

	bool is32Bit() const {
		return _ident[EI_CLASS] == ELFCLASS32;
	}

	oshal::File& _file;
	const BinaryOptions _options;
	uint8_t _ident[ EI_NIDENT ];
	uint16_t _type;
	uint16_t _machine;
	uint32_t _version;
	uint64_t _entry;
	uint64_t _phoff;
	uint64_t _shoff;
	uint32_t _flags;
	uint16_t _ehsize;
	uint16_t _phentsize;
	uint16_t _phnum;
	uint16_t _shentsize;
	uint16_t _shnum;
	uint16_t _shstrndx;

	std::vector<ElfShdr> _shdrs;
	std::map<uint16_t, std::vector<char>> _str_sections;

	std::vector<Segment> _sectionSegments;
	std::vector<Segment> _loadSegments;

	SymbolDB _symbolDB;
};

}} /*namespace remu::binary*/

#endif /*REMU_BINARY_ELFFILE_H*/

