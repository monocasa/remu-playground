#include "common.h"
#include "emulationexception.h"

namespace remu {

Framebuffer::Framebuffer(size_t mem_size, Emulator *emu, Memory *mem, Mbox &mbox)
  : IoRegion(0, 0)
  , Channel(mbox, FRAMEBUFFER_CHANNEL_NUM)
  , emu(emu)
  , mem(mem)
  , mem_size(mem_size)
  , framebuffer(nullptr)
  , fb_bpp(0)
  , fb_pitch(0)
  , fb_size(0)
  , fb_address(0)
  , fb_palette()
  , error(false)
  , surface(nullptr)
  , width(640)
  , height(480)
  , depth(32)
{
  mem->addRegion(this);

  /* Create the window */
  SDL_Init(SDL_INIT_EVERYTHING);
  surface = SDL_SetVideoMode(width, height, depth, SDL_SWSURFACE);

  /* Set the window caption */
  SDL_WM_SetCaption("Remu", "Remu");
}

/**
 * Cleans up memory used by the framebuffer
 */
Framebuffer::~Framebuffer()
{
  /* Destroy SDL */
  if (surface)
  {
    SDL_FreeSurface(surface);
  }
  SDL_Quit();

  /* Free framebuffer */
  if (framebuffer)
  {
    free(framebuffer);
  }
}

/*
 * Set the pixel at (x, y) to the given value
 * NOTE: The surface must be locked before calling this!
 */
void Framebuffer::putPixel(SDL_Surface *surface, int x, int y, uint32_t pixel)
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
 * @param x X position
 * @param y Y position
 */
uint32_t Framebuffer::getPixel(uint32_t x, uint32_t y)
{
  if (!framebuffer)
  {
    return SDL_MapRGB(surface->format, 0xff, 0x00, 0xff);
  }

  switch (fb_bpp)
  {
    case 1: // 1 byte per pixel - 8 bit colour
    {
      uint8_t key = framebuffer[y * fb_pitch + x * fb_bpp];
      uint16_t value = fb_palette[key];

      /* Format: RRRRRGGGGGGBBBBB */
      uint32_t r = (value >> 11) & 0x1F;
      uint32_t g = (value >> 5) & 0x3F;
      uint32_t b = value & 0x1F;

      /* Map to [0-255] */
      r = (r * 255) / 31;
      g = (g * 255) / 63;
      b = (b * 255) / 31;

      /* Return colour */
      return SDL_MapRGB(surface->format, r, g, b);
    }
    case 2: // 2 bytes per pixel - R5G6B5
    {
      uint16_t value =
        framebuffer[y * fb_pitch + x * fb_bpp] +
        (framebuffer[y * fb_pitch + x * fb_bpp + 1] << 8);

      /* Format: BBBBBGGGGGGRRRRR */
      uint32_t r = value & 0x1F;
      uint32_t g = (value >> 5) & 0x3F;
      uint32_t b = (value >> 11) & 0x1F;

      /* Map to [0-255] */
      r = (r * 255) / 31;
      g = (g * 255) / 63;
      b = (b * 255) / 31;

      /* Return colour */
      return SDL_MapRGB(surface->format, r, g, b);
    }
    case 3: // 3 bytes per pixel - RGB8
    case 4: // 4 bytes per pixel - XRGB8
    {
      uint32_t value =
        framebuffer[y * fb_pitch + x * fb_bpp] +
        (framebuffer[y * fb_pitch + x * fb_bpp + 1] << 8) +
        (framebuffer[y * fb_pitch + x * fb_bpp + 2] << 16) +
        (framebuffer[y * fb_pitch + x * fb_bpp + 3] << 24);

      /* Format: BBBBBBBBGGGGGGGGRRRRRRRR */
      return SDL_MapRGB(surface->format,
        value & 0xFF, (value >> 8) & 0xFF, (value >> 16) & 0xFF);
    }
    default:
    {
      emu->error("Unsupported pixel format");
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
      putPixel(surface, x, y, getPixel(x, y));
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
 * @param addr Address received from the mailbox
 */
void Framebuffer::request(uint32_t addr)
{
  size_t i;
  framebuffer_req_t req;

  /* Clear error flag */
  error = false;

  /* Graphic flag must be set*/
  if (!emu->isGraphicsEnabled())
  {
    emu->error("Graphic mode must be enabled for framebuffer");
    error = true;
    return;
  }

  /* Check whether address is valid */
  if (addr < 0x40000000)
  {
    emu->error("Invalid framebuffer address");
    error = true;
    return;
  }

  /* Respond to the framebuffer request */
  addr -= 0x40000000;
  for (i = 0; i < sizeof(req.data) / sizeof(req.data[0]); ++i)
  {
    req.data[i] = mem->readDwordLe(addr + (i << 2));
  }

  /* Free old framebuffer */
  if (framebuffer)
  {
    free(framebuffer);
  }

  /* Read palette if 8 bit colour
   * We're assuming that the palette comes immediately after the request */
  if (req.fb.depth == 8)
  {
    for (i = 0; i < 256; ++i)
    {
      fb_palette[i] = mem->readWordLe(addr + sizeof(req) + i * 2);
    }
  }

  /* Allocate a nice frame buffer, placed after the main memory */
  fb_bpp = req.fb.depth >> 3;
  fb_pitch = req.fb.virt_width * fb_bpp;
  req.fb.pitch = fb_pitch + (4 - (fb_pitch % 4)) % 4;
  req.fb.size = fb_size = fb_pitch * req.fb.virt_height;
  framebuffer = static_cast<uint8_t*>( malloc(fb_size) );
  req.fb.addr = fb_address = mem_size;
  width = req.fb.virt_width;
  height = req.fb.virt_height;

  base = fb_address;
  length = fb_size;

  memset(framebuffer, 0, fb_size);

  /* Write back structure into memory */
  for (i = 0; i < sizeof(req.data) / sizeof(req.data[0]); ++i)
  {
    mem->writeDwordLe(addr + (i << 2), req.data[i]);
  }

  /* Change the window caption */
  SDL_WM_SetCaption("Remu", "Remu");

  /* Change the window size */
  surface = SDL_SetVideoMode(width, height, depth, SDL_SWSURFACE);
}

uint64_t Framebuffer::readIo(uint64_t addr, unsigned int size)
{
  throw EmulationException("Read unimplemented for framebuffer @ %08llx:%xB", addr, size);
}

void Framebuffer::writeIo(uint64_t addr, uint64_t val, unsigned int size)
{
  switch(size) {
    case sizeof(uint16_t): {
      framebuffer[addr + 0] = (val >> 0) & 0xFF;
      framebuffer[addr + 1] = (val >> 8) & 0xFF;
      return;
    }

    case sizeof(uint32_t): {
      framebuffer[addr + 0] = (val >> 0) & 0xFF;
      framebuffer[addr + 1] = (val >> 8) & 0xFF;
      framebuffer[addr + 2] = (val >> 16) & 0xFF;
      framebuffer[addr + 3] = (val >> 24) & 0xFF;
      return;
    }

    default: {
      throw EmulationException("Write unimplemented for framebuffer @ %08lx:%xB <- %lx", addr, size, val);
    }
  }
}

} /*namespace remu*/

