#ifndef REMU_BCM2835_TIMER_H
#define REMU_BCM2835_TIMER_H

#include "remu/ioregion.h"

namespace remu {
class Emulator;
class Memory;
} /*namespace remu*/

namespace remu { namespace bcm2835 {

class Timer : private IoRegion
{
public:
  Timer(remu::Emulator &emu, remu::Memory &mem);
  virtual ~Timer();

private:
  static const uint32_t TIMER_BASE = 0x20003000;
  static const int TIMER_BANK_SIZE = 0xC;

  enum {
    TIMER_LOW  = 4,
    TIMER_HIGH = 8,
  };

  uint64_t readIo(uint64_t addr, unsigned int size) override final;
  void writeIo(uint64_t addr, uint64_t val, unsigned int size) override final;

  remu::Emulator &emu;
  remu::Memory   &mem;
};

}} /*namespace remu::bcm2835*/

#endif /*REMU_BCM2825_TIMER_H*/

