#ifndef REMU_NES_H
#define REMU_NES_H

#define NES_GPIO_PORT_LATCH 11
#define NES_GPIO_PORT_CLOCK 10
#define NES_GPIO_PORT_DATA 4

typedef enum
{
  NES_A = 0,
  NES_B,
  NES_SELECT,
  NES_START,
  NES_UP,
  NES_DOWN,
  NES_LEFT,
  NES_RIGHT,
  NES_BUTTON_COUNT
} nes_button_t;

class Nes
{
public:
  Emulator* emu;

  uint32_t last_latch;
  uint32_t last_clock;
  uint32_t counter;

  /* Button States */
  uint32_t state[NES_BUTTON_COUNT];
  SDLKey binding[NES_BUTTON_COUNT];
};

void nes_init(Nes*, Emulator*);
void nes_gpio_write(Nes*, uint32_t, uint32_t);
void nes_on_key_down(Nes*, SDLKey);
void nes_on_key_up(Nes*, SDLKey);

#endif /* REMU_NES_H */

