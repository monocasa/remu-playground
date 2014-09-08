#ifndef REMU_BCM2835_MBOX_H
#define REMU_BCM2835_MBOX_H

namespace remu {

class Emulator;

/**
 * List of mailbox register addresses
 */
typedef enum
{
  MBOX_BASE   = 0x2000b880,
  MBOX_READ   = MBOX_BASE + 0x00,
  MBOX_POLL   = MBOX_BASE + 0x10,
  MBOX_SENDER = MBOX_BASE + 0x14,
  MBOX_STATUS = MBOX_BASE + 0x18,
  MBOX_CONFIG = MBOX_BASE + 0x1c,
  MBOX_WRITE  = MBOX_BASE + 0x20
} mbox_ports_t;

/**
 * Mailbox structure
 * Mailbox emulation is not completely accurate as all requests
 * are serviced immediately, so the status bits are always set
 * to ready
 */
class Mbox
{
public:
  Mbox(Emulator *emu);
  virtual ~Mbox() = default;

  Emulator   *emu;
  uint8_t     last_channel;
};

uint32_t mbox_read(Mbox *mbox, uint32_t addr);
void     mbox_write(Mbox *mbox, uint32_t addr, uint32_t val);

/**
 * Checks whether a given address is a mailbox port.
 * It seems that the CPU ignores the last 4 bits of the address, so
 * all writes are aligned on a 4 byte boundary.
 */
static inline int
mbox_is_port(uint32_t addr)
{
  addr &= ~0x3;
  return MBOX_BASE <= addr && addr <= MBOX_WRITE;
}

} /*namespace remu*/

#endif /*REMU_BCM2835_MBOX_H*/

