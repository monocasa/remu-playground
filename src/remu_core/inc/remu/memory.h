#ifndef REMU_MEMORY_H
#define REMU_MEMORY_H

#include "remu/ioregion.h"

#include <cstdlib>

namespace remu {

class Emulator;
class RamRegion;

/**
 * Memory system
 */
class Memory
{
public:
  Memory(Emulator *emu, size_t mem_size);
  virtual ~Memory();

  void dump();

  uint8_t readByte(uint32_t address);
  uint16_t readWordLe(uint32_t address);
  uint32_t readDwordLe(uint32_t address);

  void writeByte(uint32_t address, uint8_t val);
  void writeWordLe(uint32_t address, uint16_t val);
  void writeDwordLe(uint32_t address, uint32_t val);

  void addRegion(IoRegion *region) {
    iomap.addRegion(region);
  }

  void removeRegion(IoRegion *region) {
    iomap.removeRegion(region);
  }

  const std::list<RamRegion*>& getRamRegions() const {
    return ramList;
  }

  uint8_t* getDramArrayBase() {
    return data;
  }

private:
  Emulator             *emu;
  const size_t          mem_size;
  uint8_t              *data;
  IoMap                 iomap;
  RamRegion            *ramRegion;
  std::list<RamRegion*> ramList;
};

} /*namesapce remu */

#endif /* REMU_MEMORY_H */

