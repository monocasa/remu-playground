#include "common.h"
#include "emulationexception.h"

namespace remu {

Framebuffer::Framebuffer(size_t mem_size)
  : emu(nullptr)
  , mem_size(mem_size)
  , framebuffer(nullptr)
  , fb_bpp(0)
  , fb_pitch(0)
  , fb_size(0)
  , fb_address(0)
  , fb_palette()
  , error(0)
  , surface(nullptr)
  , width(0)
  , height(0)
  , depth(0)
{ }

/**
 * Initialises the framebuffer interface
 * @param fb  Reference to the framebuffer structure
 * @param emu Reference to the emulator structure
 */
void
fb_init(Framebuffer* fb, Emulator* emu, Memory *mem)
{
  assert(fb);
  assert(emu);
  assert(mem);

  fb->emu = emu;
  fb->mem = mem;

  mem->addRegion(fb);

  /* Create the window */
  SDL_Init(SDL_INIT_EVERYTHING);
  fb->width = 640;
  fb->height = 480;
  fb->depth = 32;
  fb->surface =
    SDL_SetVideoMode(fb->width, fb->height, fb->depth, SDL_SWSURFACE);

  /* Set the window caption */
  SDL_WM_SetCaption("Remu", "Remu");
}

/**
 * Cleans up memory used by the framebuffer
 * @param fb  Reference to the framebuffer structure
 */
void
fb_destroy(Framebuffer* fb)
{
  if (!fb || !fb->emu->isGraphicsEnabled())
  {
    return;
  }

  /* Destroy SDL */
  SDL_FreeSurface(fb->surface);
  SDL_Quit();
  fb->surface = 0;

  /* Free framebuffer */
  if (fb->framebuffer)
  {
    free(fb->framebuffer);
    fb->framebuffer = NULL;
  }
}

/*
 * Set the pixel at (x, y) to the given value
 * NOTE: The surface must be locked before calling this!
 */
void
put_pixel(SDL_Surface *surface, int x, int y, uint32_t pixel)
{
  int bpp = surface->format->BytesPerPixel;

  /* Here p is the address to the pixel we want to set */
  uint8_t* p = (uint8_t*)surface->pixels + y * surface->pitch + x * bpp;

  switch(bpp)
  {
    case 1:
    {
      *p = pixel;
      break;
    }
    case 2:
    {
      *(uint16_t*)p = pixel;
      break;
    }
    case 3:
    {
      if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
      {
        p[0] = (pixel >> 16) & 0xff;
        p[1] = (pixel >> 8) & 0xff;
        p[2] = pixel & 0xff;
      }
      else
      {
        p[0] = pixel & 0xff;
        p[1] = (pixel >> 8) & 0xff;
        p[2] = (pixel >> 16) & 0xff;
      }
      break;
    }
    case 4:
    {
      *(uint32_t*)p = pixel;
      break;
    }
  }
}

/**
 * Gets the specified pixel colour at a particular location and converts
 * it to the output format.
 *
 * @param fb Reference to the framebuffer structure
 * @param x X position
 * @param y Y position
 */
uint32_t fb_get_pixel(Framebuffer* fb, uint32_t x, uint32_t y)
{
  assert(fb);

  if (!fb->framebuffer)
  {
    return SDL_MapRGB(fb->surface->format, 0xff, 0x00, 0xff);
  }

  switch (fb->fb_bpp)
  {
    case 1: // 1 byte per pixel - 8 bit colour
    {
      uint8_t key = fb->framebuffer[y * fb->fb_pitch + x * fb->fb_bpp];
      uint16_t value = fb->fb_palette[key];

      /* Format: RRRRRGGGGGGBBBBB */
      uint32_t r = (value >> 11) & 0x1F;
      uint32_t g = (value >> 5) & 0x3F;
      uint32_t b = value & 0x1F;

      /* Map to [0-255] */
      r = (r * 255) / 31;
      g = (g * 255) / 63;
      b = (b * 255) / 31;

      /* Return colour */
      return SDL_MapRGB(fb->surface->format, r, g, b);
    }
    case 2: // 2 bytes per pixel - R5G6B5
    {
      uint16_t value =
        fb->framebuffer[y * fb->fb_pitch + x * fb->fb_bpp] +
        (fb->framebuffer[y * fb->fb_pitch + x * fb->fb_bpp + 1] << 8);

      /* Format: BBBBBGGGGGGRRRRR */
      uint32_t r = value & 0x1F;
      uint32_t g = (value >> 5) & 0x3F;
      uint32_t b = (value >> 11) & 0x1F;

      /* Map to [0-255] */
      r = (r * 255) / 31;
      g = (g * 255) / 63;
      b = (b * 255) / 31;

      /* Return colour */
      return SDL_MapRGB(fb->surface->format, r, g, b);
    }
    case 3: // 3 bytes per pixel - RGB8
    case 4: // 4 bytes per pixel - XRGB8
    {
      uint32_t value =
        fb->framebuffer[y * fb->fb_pitch + x * fb->fb_bpp] +
        (fb->framebuffer[y * fb->fb_pitch + x * fb->fb_bpp + 1] << 8) +
        (fb->framebuffer[y * fb->fb_pitch + x * fb->fb_bpp + 2] << 16) +
        (fb->framebuffer[y * fb->fb_pitch + x * fb->fb_bpp + 3] << 24);

      /* Format: BBBBBBBBGGGGGGGGRRRRRRRR */
      return SDL_MapRGB(fb->surface->format,
        value & 0xFF, (value >> 8) & 0xFF, (value >> 16) & 0xFF);
    }
    default:
    {
      fb->emu->error("Unsupported pixel format");
      break;
    }
  }

  return 0;
}

/**
 * Updates the display, displaying the framebuffer and handling events
 */
void Framebuffer::tick()
{
  assert(emu->isGraphicsEnabled());

  /* Handle all SDL events */
  SDL_Event event;
  while (SDL_PollEvent(&event))
  {
    if (event.type == SDL_QUIT)
    {
      emu->terminate();
    }

    /* Route keyboard presses to the NES module if enabled */
    if (event.type == SDL_KEYDOWN)
    {
      dispatchKeyDown(event.key.keysym.sym);
    }
    else if (event.type == SDL_KEYUP)
    {
      dispatchKeyUp(event.key.keysym.sym);
    }
  }

  /* Lock the surface */
  if (SDL_MUSTLOCK(surface))
  {
    SDL_LockSurface(surface);
  }

  /* Copy the pixels to SDL */
  for (uint32_t y = 0; y < height; ++y)
  {
    for (uint32_t x = 0; x < width; ++x)
    {
      put_pixel(surface, x, y, fb_get_pixel(this, x, y));
    }
  }

  /* Unlock the surface */
  if (SDL_MUSTLOCK(surface))
  {
    SDL_UnlockSurface(surface);
  }

  /* Display to screen */
  SDL_Flip(surface);
}

/**
 * Handles a framebuffer request received through the mailbox interface
 *
 * @param fb   Framebuffer structure
 * @param addr Address received from the mailbox
 */
void
fb_request(Framebuffer *fb, uint32_t addr)
{
  size_t i;
  framebuffer_req_t req;

  assert(fb);

  /* Clear error flag */
  fb->error = 0;

  /* Graphic flag must be set*/
  if (!fb->emu->isGraphicsEnabled())
  {
    fb->emu->error("Graphic mode must be enabled for framebuffer");
    fb->error = 1;
    return;
  }

  /* Check whether address is valid */
  if (addr < 0x40000000)
  {
    fb->emu->error("Invalid framebuffer address");
    fb->error = 1;
    return;
  }

  /* Respond to the framebuffer request */
  addr -= 0x40000000;
  for (i = 0; i < sizeof(req.data) / sizeof(req.data[0]); ++i)
  {
    req.data[i] = fb->mem->readDwordLe(addr + (i << 2));
  }

  /* Free old framebuffer */
  if (fb->framebuffer)
  {
    free(fb->framebuffer);
    fb->framebuffer = NULL;
  }

  /* Read palette if 8 bit colour
   * We're assuming that the palette comes immediately after the request */
  if (req.fb.depth == 8)
  {
    for (i = 0; i < 256; ++i)
    {
      fb->fb_palette[i] = fb->mem->readWordLe(addr + sizeof(req) + i * 2);
    }
  }

  /* Allocate a nice frame buffer, placed after the main memory */
  fb->fb_bpp = req.fb.depth >> 3;
  fb->fb_pitch = req.fb.virt_width * fb->fb_bpp;
  req.fb.pitch = fb->fb_pitch + (4 - (fb->fb_pitch % 4)) % 4;
  req.fb.size = fb->fb_size = fb->fb_pitch * req.fb.virt_height;
  fb->framebuffer = static_cast<uint8_t*>( malloc(fb->fb_size) );
  req.fb.addr = fb->fb_address = fb->mem_size;
  fb->width = req.fb.virt_width;
  fb->height = req.fb.virt_height;

  fb->base = fb->fb_address;
  fb->length = fb->fb_size;

  assert(fb->framebuffer);
  memset(fb->framebuffer, 0, fb->fb_size);

  /* Write back structure into memory */
  for (i = 0; i < sizeof(req.data) / sizeof(req.data[0]); ++i)
  {
    fb->mem->writeDwordLe(addr + (i << 2), req.data[i]);
  }

  /* Change the window caption */
  SDL_WM_SetCaption("Remu", "Remu");

  /* Change the window size */
  fb->surface = SDL_SetVideoMode(fb->width, fb->height, fb->depth, SDL_SWSURFACE);
}

uint64_t Framebuffer::readIo(uint64_t addr, unsigned int size)
{
  throw EmulationException("Read unimplemented for framebuffer @ %08llx:%xB", addr, size);
}

void Framebuffer::writeIo(uint64_t address, uint64_t val, unsigned int size)
{
  switch(size) {
    case sizeof(uint16_t): {
      const uint32_t addr = address - fb_address;
      framebuffer[addr + 0] = (val >> 0) & 0xFF;
      framebuffer[addr + 1] = (val >> 8) & 0xFF;
      return;
    }

    case sizeof(uint32_t): {
      const uint32_t addr = address - fb_address;
      framebuffer[addr + 0] = (val >> 0) & 0xFF;
      framebuffer[addr + 1] = (val >> 8) & 0xFF;
      framebuffer[addr + 2] = (val >> 16) & 0xFF;
      framebuffer[addr + 3] = (val >> 24) & 0xFF;
      return;
    }

    default: {
      throw EmulationException("Read unimplemented for framebuffer @ %08lx:%xB", address, size);
    }
  }
}

} /*namespace remu*/

