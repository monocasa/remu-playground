#ifndef REMU_NES_H
#define REMU_NES_H

#include "keylistener.h"
#include "gpiolistener.h"

class Framebuffer;
class Gpio;
class Ui;

class Nes : public KeyListener,
            public GpioListener
{
public:
  Nes(Ui& ui, Gpio &gpio, Framebuffer *fb);
  virtual ~Nes() = default;

  void onKeyUp(SDLKey key) override final;
  void onKeyDown(SDLKey key) override final;

  void onGpioWrite(uint32_t port, uint32_t value) override final;

private:
  static const int NES_GPIO_PORT_DATA  = 4;
  static const int NES_GPIO_PORT_CLOCK = 10;
  static const int NES_GPIO_PORT_LATCH = 11;
  enum {
    NES_A = 0,
    NES_B,
    NES_SELECT,
    NES_START,
    NES_UP,
    NES_DOWN,
    NES_LEFT,
    NES_RIGHT,
    NES_BUTTON_COUNT
  };

  void writeButton(uint32_t button);

  Ui   &ui;
  Gpio &gpio;

  uint32_t last_latch;
  uint32_t last_clock;
  uint32_t counter;

  /* Button States */
  uint32_t state[NES_BUTTON_COUNT];
  SDLKey binding[NES_BUTTON_COUNT];
};

#endif /* REMU_NES_H */

