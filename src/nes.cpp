#include "common.h"

static inline void
nes_write_button(Nes* nes, uint32_t button)
{
  nes->gpio->setPortState(NES_GPIO_PORT_DATA, nes->state[button] ? 0 : 1);
}

void Nes::onGpioWrite(uint32_t port, uint32_t value)
{
  if (port == NES_GPIO_PORT_LATCH)
  {
    /* On the rising edge of the latch, reset counter */
    if (last_latch == 0 && value == 1)
    {
      counter = 0;
      nes_write_button(this, counter++);
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
        nes_write_button(this, counter);
      }
      else
      {
        gpio->setPortState(NES_GPIO_PORT_DATA, 1);
      }
      counter++;
    }

    last_clock = value;
  }
}

void
nes_init(Nes* nes, Emulator* emu, Gpio *gpio)
{
  nes->emu = emu;
  nes->gpio = gpio;
  nes->last_latch = 0;
  nes->last_clock = 0;
  nes->counter = 0;
  memset(&nes->state, 0, sizeof(nes->state));
  memset(&nes->binding, 0, sizeof(nes->binding));

  /* Set up key bindings */
  nes->binding[NES_A]      = SDLK_SPACE;
  nes->binding[NES_B]      = SDLK_TAB;
  nes->binding[NES_START]  = SDLK_RETURN;
  nes->binding[NES_SELECT] = SDLK_p;
  nes->binding[NES_LEFT]   = SDLK_a;
  nes->binding[NES_RIGHT]  = SDLK_d;
  nes->binding[NES_UP]     = SDLK_w;
  nes->binding[NES_DOWN]   = SDLK_s;
}

void
nes_gpio_write(Nes* nes, uint32_t port, uint32_t value)
{
  nes->onGpioWrite(port, value);
}

void
nes_on_key_down(Nes* nes, SDLKey key)
{
  /* Search for the button bound to this SDLKey */
  for (int i = 0; i < NES_BUTTON_COUNT; ++i)
  {
    if (nes->binding[i] == key)
    {
      nes->state[i] = 1;
    }
  }
}

void
nes_on_key_up(Nes* nes, SDLKey key)
{
  /* Search for the button bound to this SDLKey */
  for (int i = 0; i < NES_BUTTON_COUNT; ++i)
  {
    if (nes->binding[i] == key)
    {
      nes->state[i] = 0;
    }
  }
}

