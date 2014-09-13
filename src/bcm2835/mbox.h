#ifndef REMU_BCM2835_MBOX_H
#define REMU_BCM2835_MBOX_H

#include "ioregion.h"

namespace remu {

class Emulator;
class Memory;

/**
 * Mailbox structure
 * Mailbox emulation is not completely accurate as all requests
 * are serviced immediately, so the status bits are always set
 * to ready
 */
class Mbox : public IoRegion
{
public:
  class Channel
  {
  public:
    Channel(Mbox& mbox, int channelNum)
      : mbox(mbox)
      , channelNum(channelNum)
    {
      mbox.addChannel(this);
    }

    virtual ~Channel()
    {
      mbox.removeChannel(this);
    }

    int getChannelNum() const {
      return channelNum;
    }

    virtual void request(uint32_t data) = 0;
    virtual bool getError() const = 0;

  private:
    Mbox &mbox;
    int channelNum;
  };

  Mbox(Emulator *emu, Memory &mem);
  virtual ~Mbox();

  void addChannel(Channel *channel);
  void removeChannel(Channel *channel);

private:
  uint64_t readIo(uint64_t addr, unsigned int size) override final;
  void writeIo(uint64_t addr, uint64_t val, unsigned int size) override final;

  static const uint32_t MBOX_BASE = 0x2000b880;

  static const int NUM_CHANNELS = 16;

  /**
   * List of mailbox register addresses
   */
  typedef enum
  {
    MBOX_READ   = 0x00,
    MBOX_POLL   = 0x10,
    MBOX_SENDER = 0x14,
    MBOX_STATUS = 0x18,
    MBOX_CONFIG = 0x1c,
    MBOX_WRITE  = 0x20,
  } mbox_ports_t;

  Emulator   *emu;
  Memory     &mem;
  uint8_t     last_channel;
  Channel    *channels[NUM_CHANNELS];
};

} /*namespace remu*/

#endif /*REMU_BCM2835_MBOX_H*/

