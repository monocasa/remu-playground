#include "bcm2835/timer.h"
#include "emulationexception.h"
#include "emulator.h"
#include "memory.h"

namespace remu {

Timer::Timer(Emulator &emu, Memory &mem)
  : IoRegion(0x20003000, 0xC)
  , emu(emu)
  , mem(mem)
{
  mem.addRegion(this);
}

Timer::~Timer()
{
  mem.removeRegion(this);
}

uint64_t Timer::readIo(uint64_t addr, unsigned int size)
{
  if (size != sizeof(uint32_t))
  {
    throw EmulationException("Only 32 bit Timer reads are supported:  addr=%08lx size=%d", addr, size);
  }

  switch (addr)
  {
    case 4:
    {
      uint64_t timer_value = emu.getSystemTimer();
      return (uint32_t)(timer_value & 0xffffffff);
    }

    case 8:
    {
      uint64_t timer_value = emu.getSystemTimer();
      return (uint32_t)((timer_value >> 32) & 0xffffffff);
    }

    default:
    {
      emu.error("Read from unknown addr %08lx:%dB", addr, size);
      return 0xFFFFFFFFULL;
    }
  }
}

void Timer::writeIo(uint64_t addr, uint64_t val, unsigned int size)
{
  throw EmulationException("Timer writes not implemented:  addr=%08lx val=%lx, size=%d", addr, val, size);
}

} /*namespace remu*/

