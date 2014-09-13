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
                  , public IoRegion
{
public:
  Framebuffer(size_t mem_size, Emulator*, Memory*);
  virtual ~Framebuffer();

  void tick();

  /* Emulator reference */
  Emulator     *emu;
  Memory       *mem;
  Gpio         *gpio;
  const size_t  mem_size;

  void dump();
  void request(uint32_t data);

  bool getError() const {
    return error;
  }

private:
  /* IoRegion overrides */
  uint64_t readIo(uint64_t addr, unsigned int size) override final;
  void writeIo(uint64_t addr, uint64_t val, unsigned int size) override final;

  static void putPixel(SDL_Surface *surface, int x, int y, uint32_t pixel);

  uint32_t getPixel(uint32_t x, uint32_t y);

  /* Framebuffer */
  uint8_t*      framebuffer;
  size_t        fb_bpp;
  size_t        fb_pitch;
  size_t        fb_size;
  uint32_t      fb_address;
  uint16_t      fb_palette[256];

  /* Flag if set if query is malformed */
  bool          error;

  /* Window */
  SDL_Surface*  surface;
  uint32_t      width;
  uint32_t      height;
  uint32_t      depth;
};

} /*namespace remu*/

#endif /* REMU_BCM2835_FRAMEBUFFER_H */

