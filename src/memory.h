#ifndef REMU_MEMORY_H
#define REMU_MEMORY_H

#include "ioregion.h"

/**
 * Memory system
 */
class Memory
{
public:
  Memory(Emulator *emu, size_t mem_size);
  virtual ~Memory();

  Emulator    *emu;
  const size_t mem_size;
  uint8_t     *data;

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

private:
  IoMap        iomap;
};

#endif /* REMU_MEMORY_H */

