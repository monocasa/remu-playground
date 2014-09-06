#ifndef REMU_BCM2835_GPIO_H
#define REMU_BCM2835_GPIO_H

#include <cstdint>
#include <vector>

class Emulator;
class GpioListener;

/**
 * GPIO emulation
 */
class Gpio
{
public:
  Gpio(Emulator &emulator);
  ~Gpio() = default;

  uint32_t readPort(uint32_t addr);
  void writePort(uint32_t addr, uint32_t val);

  /**
   * Sets the GPIO write listener
   * Gpio does not own this pointer
   */
  void setListener(GpioListener *listener) {
    this->listener = listener;
  }

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
  void setPortState(int port, uint8_t state) {
    ports[port].state = state;
  }

private:
  /**
   * Number of GPIO ports on a bcm2835
   */
  static const int GPIO_PORT_COUNT = 54;

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
    GPIO_BASE   = 0x20200000,
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
    GPIO_UDCLK1 = GPIO_BASE + 0x9C
  };

  Emulator &emu;
  std::vector<gpio_port_t> ports;
  GpioListener *listener;
};

#endif /* REMU_BCM2835_GPIO_H */

