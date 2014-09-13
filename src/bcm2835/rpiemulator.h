#ifndef REMU_BMC2835_RPIEMULATOR_H
#define REMU_BCM2835_RPIEMULATOR_H

#include "common.h"

#include "bitbang/button.h"
#include "bitbang/nes.h"
#include "emulator.h"

namespace remu {

class RPiEmulator : public Emulator
{
public:
  RPiEmulator(const EmulatorOptions &opt)
    : Emulator(opt)
    , buttons { bitbang::Button(*gpio, fb, opt.gpio_test_offset + 0, SDLK_KP0),
                bitbang::Button(*gpio, fb, opt.gpio_test_offset + 1, SDLK_KP1),
                bitbang::Button(*gpio, fb, opt.gpio_test_offset + 2, SDLK_KP2),
                bitbang::Button(*gpio, fb, opt.gpio_test_offset + 3, SDLK_KP3),
                bitbang::Button(*gpio, fb, opt.gpio_test_offset + 4, SDLK_KP4),
                bitbang::Button(*gpio, fb, opt.gpio_test_offset + 5, SDLK_KP5),
                bitbang::Button(*gpio, fb, opt.gpio_test_offset + 6, SDLK_KP6),
                bitbang::Button(*gpio, fb, opt.gpio_test_offset + 7, SDLK_KP7),
                bitbang::Button(*gpio, fb, opt.gpio_test_offset + 8, SDLK_KP8),
                bitbang::Button(*gpio, fb, opt.gpio_test_offset + 9, SDLK_KP9) }
    , nes(nullptr)
    , dma(0x20007000, 0x1000, memory)
    , clock(0x20101000, 0x1000, memory)
    , pwm(0x2020C000, 0x1000, memory)
  {
    if(opt.nes_enabled) {
      nes = new bitbang::Nes(*this, *gpio, fb);
    }
  }

  virtual ~RPiEmulator()
  {
    delete nes;
  }

private:
  static const int NUM_BUTTONS = 10;

  class RPiStubRegion : private StubRegion
  {
  public:
    RPiStubRegion(uint32_t base, uint32_t length, Memory &mem);
    virtual ~RPiStubRegion();

  private:
    Memory &mem;
  };

  bitbang::Button buttons[NUM_BUTTONS];
  bitbang::Nes   *nes;
  RPiStubRegion   dma;
  RPiStubRegion   clock;
  RPiStubRegion   pwm;
};

} /*namespace remu*/

#endif /*REMU_BCM2835_RPIEMULATOR_H*/

