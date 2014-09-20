#ifndef REMU_BCM2835_FRAMEBUFFER_H
#define REMU_BCM2835_FRAMEBUFFER_H

#include "remu/bcm2835/Mbox.h"
#include "remu/KeyDispatcher.h"

#include <vector>

namespace remu {
class Emulator;
class Memory;
class KeyListener;
} /*namespace remu*/

namespace remu { namespace bcm2835 {

class Gpio;
class Mbox;

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
                  , public Mbox::Channel
{
public:
  Framebuffer(size_t mem_size, remu::Emulator&, remu::Memory&, Mbox&);
  virtual ~Framebuffer();

  void tick();

private:
  static const int FRAMEBUFFER_CHANNEL_NUM = 1;

  /* IoRegion overrides */
  uint64_t readIo(uint64_t addr, unsigned int size) override final;
  void writeIo(uint64_t addr, uint64_t val, unsigned int size) override final;

  /* Mbox::Channel overrides */
  void request(uint32_t data) override final;
  bool getError() const override final{
    return error;
  }

  static void putPixel(SDL_Surface *surface, int x, int y, uint32_t pixel);

  uint32_t getPixel(uint32_t x, uint32_t y);

  void copyFramebuffer();

  /* Emulator reference */
  remu::Emulator &emu;
  remu::Memory   &mem;
  const size_t    mem_size;

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

}} /*namespace remu::bcm2835*/

#endif /* REMU_BCM2835_FRAMEBUFFER_H */

