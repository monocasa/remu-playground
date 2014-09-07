#ifndef REMU_MEMORY_H
#define REMU_MEMORY_H

#include "ioregion.h"

class Gpio;

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
  Gpio        *gpio;

  void dump();

  uint8_t readByte(uint32_t address);
  uint16_t readWordLe(uint32_t address);
  uint32_t readDwordLe(uint32_t address);

  void writeByte(uint32_t address, uint8_t val);
  void writeWordLe(uint32_t address, uint16_t val);
  void writeDwordLe(uint32_t address, uint32_t val);

  /* R E F A C T O R I N G   S T U B */
  void setGpio(Gpio *gpio) {
    this->gpio = gpio;
  }

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

