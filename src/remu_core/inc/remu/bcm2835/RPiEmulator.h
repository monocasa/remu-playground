#ifndef REMU_BMC2835_RPIEMULATOR_H
#define REMU_BCM2835_RPIEMULATOR_H

#include "remu/arm/Cpu.h"
#include "remu/bcm2835/Framebuffer.h"
#include "remu/bcm2835/Gpio.h"
#include "remu/bcm2835/Peripheral.h"
#include "remu/bcm2835/Timer.h"
#include "remu/bitbang/Button.h"
#include "remu/bitbang/Nes.h"
#include "remu/Emulator.h"
#include "remu/Memory.h"

namespace remu { namespace bcm2835 {

class RPiEmulator : public Emulator
{
public:
  RPiEmulator(const EmulatorOptions &opt)
    : Emulator(opt)
    , opt(opt)
    , memory(this, opt.mem_size)
    , cpu(this, &memory, opt.start_addr)
    , pr(*this, memory)
    , gpio(*this, memory)
    , timer(*this, memory)
    , mbox(*this, memory)
    , fb(opt.mem_size, *this, memory, mbox)
    , buttons { bitbang::Button(gpio, fb, opt.gpio_test_offset + 0, SDLK_KP0),
                bitbang::Button(gpio, fb, opt.gpio_test_offset + 1, SDLK_KP1),
                bitbang::Button(gpio, fb, opt.gpio_test_offset + 2, SDLK_KP2),
                bitbang::Button(gpio, fb, opt.gpio_test_offset + 3, SDLK_KP3),
                bitbang::Button(gpio, fb, opt.gpio_test_offset + 4, SDLK_KP4),
                bitbang::Button(gpio, fb, opt.gpio_test_offset + 5, SDLK_KP5),
                bitbang::Button(gpio, fb, opt.gpio_test_offset + 6, SDLK_KP6),
                bitbang::Button(gpio, fb, opt.gpio_test_offset + 7, SDLK_KP7),
                bitbang::Button(gpio, fb, opt.gpio_test_offset + 8, SDLK_KP8),
                bitbang::Button(gpio, fb, opt.gpio_test_offset + 9, SDLK_KP9) }
    , nes(nullptr)
    , dma(0x20007000, 0x1000, memory)
    , clock(0x20101000, 0x1000, memory)
    , pwm(0x2020C000, 0x1000, memory)
    , last_refresh(0)
  {
    if(opt.nes_enabled) {
      nes = new bitbang::Nes(*this, gpio, fb);
    }
  }

  virtual ~RPiEmulator()
  {
    delete nes;
  }

  void load();

  void execute() override final;
  void dump() override final;

private:
  void tick();

  static const int NUM_BUTTONS = 10;

  static const uint64_t DRAM_BASE = 0x00000000;

  class RPiStubRegion : private StubRegion
  {
  public:
    RPiStubRegion(uint32_t base, uint32_t length, Memory &mem);
    virtual ~RPiStubRegion();

  private:
    Memory &mem;
  };

  const EmulatorOptions &opt;

  /* Modules */
  Memory          memory;
  arm::Cpu        cpu;
  Peripheral      pr;
  Gpio            gpio;
  Timer           timer;
  Mbox            mbox;
  Framebuffer     fb;
  bitbang::Button buttons[NUM_BUTTONS];
  bitbang::Nes   *nes;
  RPiStubRegion   dma;
  RPiStubRegion   clock;
  RPiStubRegion   pwm;

  /* Refresh */
  uint64_t      last_refresh;
};

}} /*namespace remu::bcm2835*/

#endif /*REMU_BCM2835_RPIEMULATOR_H*/

