#ifndef REMU_BMC2835_RPIEMULATOR_H
#define REMU_BCM2835_RPIEMULATOR_H

#include "bitbang/button.h"
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
  { }

private:
  static const int NUM_BUTTONS = 10;

  bitbang::Button buttons[NUM_BUTTONS];
};

} /*namespace remu*/

#endif /*REMU_BCM2835_RPIEMULATOR_H*/

