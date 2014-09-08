#ifndef REMU_BCM2835_FRAMEBUFFER_H
#define REMU_BCM2835_FRAMEBUFFER_H

#include "keydispatcher.h"

#include <vector>

namespace remu {

class Gpio;
class KeyListener;

/**
 * Framebuffer request structure
 */
typedef union
{
  uint32_t data[10];
  struct
  {
    uint32_t phys_width;
    uint32_t phys_height;
    uint32_t virt_width;
    uint32_t virt_height;
    uint32_t pitch;
    uint32_t depth;
    uint32_t off_x;
    uint32_t off_y;
    uint32_t addr;
    uint32_t size;
  } fb;
} framebuffer_req_t;

/**
 * Framebuffer data
 */
class Framebuffer : public KeyDispatcher
{
public:
  Framebuffer(size_t mem_size);
  virtual ~Framebuffer() = default;

  void tick();

  /* Emulator reference */
  Emulator     *emu;
  Memory       *mem;
  const size_t  mem_size;

  /* Framebuffer */
  uint8_t*      framebuffer;
  size_t        fb_bpp;
  size_t        fb_pitch;
  size_t        fb_size;
  uint32_t      fb_address;
  uint16_t      fb_palette[256];

  /* Flag if set if query is malformed */
  int           error;

  /* Window */
  SDL_Surface*  surface;
  uint32_t      width;
  uint32_t      height;
  uint32_t      depth;
};

void fb_init(Framebuffer*, Emulator*, Memory*);
void fb_create_window(Framebuffer*, uint32_t width, uint32_t height);
void fb_destroy(Framebuffer*);
void fb_dump(Framebuffer*);
void fb_request(Framebuffer*, uint32_t address);
void fb_write_word(Framebuffer*, uint32_t address, uint16_t data);
void fb_write_dword(Framebuffer*, uint32_t address, uint32_t data);
int  fb_is_buffer(Framebuffer*, uint32_t address);

} /*namespace remu*/

#endif /* REMU_BCM2835_FRAMEBUFFER_H */

