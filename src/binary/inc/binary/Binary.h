#ifndef REMU_BINARY_BINARY_H
#define REMU_BINARY_BINARY_H

#include "binary/SymbolFile.h"

#include "oshal/File.h"

#include <cstdint>
#include <memory>
#include <string>

namespace remu { namespace binary {

enum class MachineType
{
	UNKNOWN,
	PPC,
	ARM,
};

struct BinaryOptions
{
	uint64_t load_addr;
	uint64_t entry_point;
	MachineType machine;
};

struct Segment
{
	int num;
	std::string name;
	uint64_t base;
	uint64_t size;
	bool executable;
};

static const uint64_t UNKNOWN_LOAD_ADDR   = 0xFFFFFFFFFFFFFFFFUL;
static const uint64_t UNKNOWN_ENTRY_POINT = 0xFFFFFFFFFFFFFFFFUL;

class Binary : public SymbolFile
{
public:
	virtual MachineType machineType() const = 0;

	virtual std::string formatName() const = 0;

	virtual const std::vector<Segment>& getSectionSegments() = 0;

	virtual const std::vector<Segment>& getLoadSegments() = 0;

	const SymbolDB& getSymbolDB() override;

	virtual std::vector<uint8_t> readSegment( const Segment &segment ) = 0;
};

std::unique_ptr<Binary> binaryForFile( oshal::File& file, const BinaryOptions& binary );

}} /*namespace remu::binary*/

#endif /*REMU_BINARY_BINARY_H*/

