#include "bcm2835/mbox.h"
#include "emulationexception.h"
#include "memory.h"
#include "ui.h"

namespace remu {

/**
 * Initialises the mailbox
 */
Mbox::Mbox(Ui &ui, Memory &mem)
  : IoRegion(MBOX_BASE, 0x24)
  , ui(ui)
  , mem(mem)
  , last_channel(0x0)
{
  mem.addRegion(this);

  for(auto &channel : channels)
  {
    channel = nullptr;
  }
}

Mbox::~Mbox()
{
  mem.removeRegion(this);
}

void Mbox::addChannel(Channel *channel)
{
  if (!channel)
  {
    throw EmulationException("Attempt to add null channel to Mbox");
  }

  const int channelNum = channel->getChannelNum();

  if (channels[channelNum])
  {
    throw EmulationException("Attempt to double add a channel to Mbox:  channelNum=%d", 
                             channelNum);
  }

  channels[channelNum] = channel;
}

void Mbox::removeChannel(Channel *channel)
{
  if (!channel)
  {
    throw EmulationException("Attempt to remove null channel from Mbox");
  }

  const int channelNum = channel->getChannelNum();

  if (!channels[channelNum])
  {
    throw EmulationException("Attempt to remove a null channel from Mbox:  channelNum=%d", 
                             channelNum);
  }

  channels[channelNum] = nullptr;
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
      Channel *channel = channels[last_channel];
      if (channel)
      {
        /* Return non zero after a failed request */
        return last_channel | (channel->getError() ? ~0xF : 0x0);
      }
      else
      {
        return last_channel;
      }
    }

    case MBOX_STATUS:
    {
      /* Bit 31 == 0: ready to receive
         Bit 30 == 0: ready to send */
      return 0;
    }

    default:
    {
      ui.error("Mailbox unimplemented 0x%08x", addr);
      return 0;
    }
  }
}

void Mbox::writeIo(uint64_t addr, uint64_t val, unsigned int size)
{
  uint8_t channelNum;
  uint32_t data;

  if (size != sizeof(uint32_t))
  {
    throw EmulationException("Unknown peripheral write to addr %08lx:%dB <- %lx", addr, size, val);
  }

  /* Retrieve data & channel */
  channelNum = val & 0xF;
  data = val & ~0xF;

  /* Save the channel of the last request */
  last_channel = channelNum;

  /* Check which port is being written */
  switch (addr)
  {
    case MBOX_WRITE:
    {
      Channel *channel = channels[channelNum];
      if (channel)
      {
        channel->request(data);
      }
      else
      {
        ui.error("Wrong channel 0x%x", channel);
      }
    }
    break;

    default:
    {
      ui.error("Mailbox unimplemented 0x%08x", addr);
    }
    break;
  }
}

} /*namespace remu*/

