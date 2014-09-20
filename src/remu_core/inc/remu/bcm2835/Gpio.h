#ifndef REMU_BCM2835_GPIO_H
#define REMU_BCM2835_GPIO_H

#include "remu/GpioBlock.h"
#include "remu/IoRegion.h"

#include <cstdint>
#include <cstdlib>
#include <vector>

namespace remu {
class Memory;
class GpioListener;
class Ui;
} /*namespace remu*/

namespace remu { namespace bcm2835 {

/**
 * GPIO emulation
 */
class Gpio : public IoRegion
           , public GpioBlock
{
public:
  Gpio(remu::Ui &ui, remu::Memory &mem);
  virtual ~Gpio();

  /**
   * Sets the bits of a particular gpio port
   */
  void setPortState(int port, uint8_t state) override final {
    ports[port].state = state;
  }

private:
  /** Number of GPIO ports on a bcm2835 */
  static const int GPIO_PORT_COUNT = 54;

  /** The GPIO register bank is 4k in size */
  static const size_t GPIO_REG_BANK_SIZE = 0x1000;

  /** Base GPIO block address as seen by Arm on bcm2835 */
  static const uint32_t GPIO_BASE = 0x20200000;

  /**
   * Struct describing a single gpio port
   */
  typedef struct
  {
    uint8_t func;
    uint8_t state;
  } gpio_port_t;

  /**
   * GPIO registers
   */
  enum
  {
    GPIO_FSEL0  = 0x00,
    GPIO_FSEL1  = 0x04,
    GPIO_FSEL2  = 0x08,
    GPIO_FSEL3  = 0x0C,
    GPIO_FSEL4  = 0x10,
    GPIO_FSEL5  = 0x14,
    GPIO_SET0   = 0x1C,
    GPIO_SET1   = 0x20,
    GPIO_CLR0   = 0x28,
    GPIO_CLR1   = 0x2C,
    GPIO_LEV0   = 0x34,
    GPIO_LEV1   = 0x38,
    GPIO_EDS0   = 0x40,
    GPIO_EDS1   = 0x44,
    GPIO_REN0   = 0x4C,
    GPIO_REN1   = 0x50,
    GPIO_FEN0   = 0x58,
    GPIO_FEN1   = 0x5C,
    GPIO_HEN0   = 0x64,
    GPIO_HEN1   = 0x68,
    GPIO_LEN0   = 0x70,
    GPIO_LEN1   = 0x74,
    GPIO_AREN0  = 0x7C,
    GPIO_AREN1  = 0x80,
    GPIO_AFEN0  = 0x88,
    GPIO_AFEN1  = 0x8C,
    GPIO_PUD    = 0x94,
    GPIO_UDCLK0 = 0x98,
    GPIO_UDCLK1 = 0x9C,
  };

  remu::Ui     &ui;
  remu::Memory &mem;
  std::vector<gpio_port_t> ports;

  uint64_t readIo(uint64_t offset, unsigned int size) override final;
  void writeIo(uint64_t offset, uint64_t val, unsigned int size) override final;
};

}} /*namespace remu::bcm2835*/

#endif /* REMU_BCM2835_GPIO_H */

