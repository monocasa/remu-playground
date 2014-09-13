#include "common.h"

#include "emulationexception.h"

namespace remu {

/**
 * Initialises the mailbox
 */
Mbox::Mbox(Emulator *emu, Memory &mem)
  : IoRegion(MBOX_BASE, 0x24)
  , emu(emu)
  , mem(mem)
  , last_channel(0x0)
{
  mem.addRegion(this);
}

Mbox::~Mbox()
{
  mem.removeRegion(this);
}

uint64_t Mbox::readIo(uint64_t addr, unsigned int size)
{
  if (size != sizeof(uint32_t))
  {
    throw EmulationException("Unknown mbox read from addr %08lx:%dB", addr, size);
  }

  /* Check which port is being read */
  switch (addr)
  {
    case MBOX_READ:
    {
      /* Always return 0 + last channel id */
      switch (last_channel)
      {
        case 1:
        {
          /* Return non zero after a failed request */
          return last_channel | (emu->fb.getError() ? ~0xF : 0x0);
        }
        default:
        {
          return last_channel;
        }
      }
      break;
    }
    case MBOX_STATUS:
    {
      /* Bit 31 == 0: ready to receive
         Bit 30 == 0: ready to send */
      return 0;
    }
  }

  emu->error("Mailbox unimplemented 0x%08x", addr);
  return 0;
}

void Mbox::writeIo(uint64_t addr, uint64_t val, unsigned int size)
{
  uint8_t channel;
  uint32_t data;

  if (size != sizeof(uint32_t))
  {
    throw EmulationException("Unknown peripheral write to addr %08lx:%dB <- %lx", addr, size, val);
  }

  /* Retrieve data & channel */
  channel = val & 0xF;
  data = val & ~0xF;

  /* Save the channel of the last request */
  last_channel = channel;

  /* Check which port is being written */
  switch (addr)
  {
    case MBOX_WRITE:
    {
      switch (channel)
      {
        case 1:   /* Framebuffer */
        {
          emu->fb.request(data);
          return;
        }
        default:
        {
          emu->error("Wrong channel 0x%x", channel);
          return;
        }
      }
      break;
    }
  }

  emu->error("Mailbox unimplemented 0x%08x", addr);
}

} /*namespace remu*/

