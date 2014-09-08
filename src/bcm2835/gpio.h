#ifndef REMU_BCM2835_GPIO_H
#define REMU_BCM2835_GPIO_H

#include "gpioblock.h"

#include <cstdint>
#include <vector>

namespace remu {

class Emulator;
class Memory;
class GpioListener;
class Ui;

/**
 * GPIO emulation
 */
class Gpio : public IoRegion
           , public GpioBlock
{
public:
  Gpio(Ui &ui, Memory &mem);
  virtual ~Gpio();

  uint64_t readIo(uint64_t offset, unsigned int size) override final;
  void writeIo(uint64_t offset, uint64_t val, unsigned int size) override final;

  /**
   * Checks whether a given address is a GPIO port
   */
  static bool isGpioAddress(uint32_t addr) {
    addr &= ~0x3;
    if (addr < GPIO_BASE || GPIO_UDCLK1 < addr)
    {
      return false;
     }

    switch (addr)
    {
      case GPIO_FSEL0 ... GPIO_FSEL5:
      case GPIO_SET0 ... GPIO_SET1:
      case GPIO_CLR0 ... GPIO_CLR1:
      case GPIO_LEV0 ... GPIO_LEV1:
      case GPIO_EDS0 ... GPIO_EDS1:
      case GPIO_REN0 ... GPIO_REN1:
      case GPIO_FEN0 ... GPIO_FEN1:
      case GPIO_HEN0 ... GPIO_HEN1:
      case GPIO_LEN0 ... GPIO_LEN1:
      case GPIO_AREN0 ... GPIO_AREN1:
      case GPIO_AFEN0 ... GPIO_AFEN1:
      case GPIO_PUD:
      case GPIO_UDCLK0 ... GPIO_UDCLK1:
        return true;
      default:
        return false;
    }
  }

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
    GPIO_FSEL0  = GPIO_BASE + 0x00,
    GPIO_FSEL1  = GPIO_BASE + 0x04,
    GPIO_FSEL2  = GPIO_BASE + 0x08,
    GPIO_FSEL3  = GPIO_BASE + 0x0C,
    GPIO_FSEL4  = GPIO_BASE + 0x10,
    GPIO_FSEL5  = GPIO_BASE + 0x14,
    GPIO_SET0   = GPIO_BASE + 0x1C,
    GPIO_SET1   = GPIO_BASE + 0x20,
    GPIO_CLR0   = GPIO_BASE + 0x28,
    GPIO_CLR1   = GPIO_BASE + 0x2C,
    GPIO_LEV0   = GPIO_BASE + 0x34,
    GPIO_LEV1   = GPIO_BASE + 0x38,
    GPIO_EDS0   = GPIO_BASE + 0x40,
    GPIO_EDS1   = GPIO_BASE + 0x44,
    GPIO_REN0   = GPIO_BASE + 0x4C,
    GPIO_REN1   = GPIO_BASE + 0x50,
    GPIO_FEN0   = GPIO_BASE + 0x58,
    GPIO_FEN1   = GPIO_BASE + 0x5C,
    GPIO_HEN0   = GPIO_BASE + 0x64,
    GPIO_HEN1   = GPIO_BASE + 0x68,
    GPIO_LEN0   = GPIO_BASE + 0x70,
    GPIO_LEN1   = GPIO_BASE + 0x74,
    GPIO_AREN0  = GPIO_BASE + 0x7C,
    GPIO_AREN1  = GPIO_BASE + 0x80,
    GPIO_AFEN0  = GPIO_BASE + 0x88,
    GPIO_AFEN1  = GPIO_BASE + 0x8C,
    GPIO_PUD    = GPIO_BASE + 0x94,
    GPIO_UDCLK0 = GPIO_BASE + 0x98,
    GPIO_UDCLK1 = GPIO_BASE + 0x9C,
  };

  Ui     &ui;
  Memory &mem;
  std::vector<gpio_port_t> ports;
};

} /*namespace remu*/

#endif /* REMU_BCM2835_GPIO_H */

