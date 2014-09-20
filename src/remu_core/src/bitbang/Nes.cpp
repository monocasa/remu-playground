#include "remu/bitbang/Nes.h"
#include "remu/GpioBlock.h"
#include "remu/KeyDispatcher.h"

namespace remu { namespace bitbang {

Nes::Nes(Ui &ui, GpioBlock &gpio, KeyDispatcher &kd)
  : ui(ui)
  , gpio(gpio)
  , last_latch(0)
  , last_clock(0)
  , counter(0)
{
  memset(&state, 0, sizeof(state));
  memset(&binding, 0, sizeof(binding));

  /* Set up key bindings */
  binding[NES_A]      = SDLK_SPACE;
  binding[NES_B]      = SDLK_TAB;
  binding[NES_START]  = SDLK_RETURN;
  binding[NES_SELECT] = SDLK_p;
  binding[NES_LEFT]   = SDLK_a;
  binding[NES_RIGHT]  = SDLK_d;
  binding[NES_UP]     = SDLK_w;
  binding[NES_DOWN]   = SDLK_s;

  gpio.addGpioListener(this);
  kd.addKeyListener(this);
}

void Nes::onKeyDown(SDLKey key)
{
  /* Search for the button bound to this SDLKey */
  for (int i = 0; i < NES_BUTTON_COUNT; ++i)
  {
    if (binding[i] == key)
    {
      state[i] = 1;
    }
  }
}

void Nes::onKeyUp(SDLKey key)
{
  /* Search for the button bound to this SDLKey */
  for (int i = 0; i < NES_BUTTON_COUNT; ++i)
  {
    if (binding[i] == key)
    {
      state[i] = 0;
    }
  }
}

void Nes::onGpioWrite(uint32_t port, uint32_t value)
{
  if (port == NES_GPIO_PORT_LATCH)
  {
    /* On the rising edge of the latch, reset counter */
    if (last_latch == 0 && value == 1)
    {
      counter = 0;
      writeButton(counter++);
    }

    last_latch = value;
  }
  else if (port == NES_GPIO_PORT_CLOCK)
  {
    /* On the rising edge of the clock, write the next button */
    if (value == 1 && last_clock == 0)
    {
      if (counter < NES_BUTTON_COUNT)
      {
        writeButton(counter);
      }
      else
      {
        gpio.setPortState(NES_GPIO_PORT_DATA, 1);
      }
      counter++;
    }

    last_clock = value;
  }
}

void Nes::writeButton(uint32_t button)
{
  gpio.setPortState(NES_GPIO_PORT_DATA, state[button] ? 0 : 1);
}

}} /*namespace remu::bitbang*/

