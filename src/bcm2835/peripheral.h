#ifndef REMU_BCM2835_PERIPHERAL_H
#define REMU_BCM2835_PERIPHERAL_H

#include "ioregion.h"

namespace remu {

class Memory;
class Ui;

/**
 * Peripherials state
 */
class Peripheral : public IoRegion
{
public:
  Peripheral(Ui &ui, Memory &mem);
  virtual ~Peripheral();

  uint64_t readIo(uint64_t addr, unsigned int size) override final;
  void writeIo(uint64_t port, uint64_t data, unsigned int size) override final;

  /**
   * Checks whether a port is a peripherial or not
   * @param addr  Port to be tested
   * @return      true if port belongs to peripherial
   */
  static bool isPeripheralAddress(uint32_t addr) {
    return addr >= AUX_BASE && addr <= AUX_SPI1_CNTL1_REG;
  }

private:
  static const uint32_t AUX_BASE = 0x20215000;
  /**
   * List of auxiliary peripherial ports
   */
  enum {
    AUX_IRQ            = 0x00,
    AUX_ENABLES        = 0x04,
    AUX_MU_IO_REG      = 0x40,
    AUX_MU_IER_REG     = 0x44,
    AUX_MU_IIR_REG     = 0x48,
    AUX_MU_LCR_REG     = 0x4C,
    AUX_MU_MCR_REG     = 0x50,
    AUX_MU_LSR_REG     = 0x54,
    AUX_MU_MSR_REG     = 0x58,
    AUX_MU_SCRATCH     = 0x5C,
    AUX_MU_CNTL_REG    = 0x60,
    AUX_MU_STAT_REG    = 0x64,
    AUX_MU_BAUD_REG    = 0x68,
    AUX_SPI0_CNTL0_REG = 0x80,
    AUX_SPI0_CNTL1_REG = 0x84,
    AUX_SPI0_STAT_REG  = 0x88,
    AUX_SPI0_IO_REG    = 0x90,
    AUX_SPI0_PEEK_REG  = 0x94,
    AUX_SPI1_CNTL0_REG = 0xC0,
    AUX_SPI1_CNTL1_REG = 0xC4
  };

  Ui     &ui;
  Memory &mem;

  /* True if SPI 1 module is enabled */
  bool spi1_enable;
  /* True if SPI 2 module is enabled */
  bool spi2_enable;

  /* Transmit interrupt */
  int irq_tx;
  /* Receive interrupt */
  int irq_rx;

  /* True if uart is enabled */
  int uart_enable;
  /* Uart bits */
  int uart_bits;
  /* Baud rate counter */
  int uart_baud_rate_counter;
  /* Baud rate */
  int uart_baud_rate;
  /* UART DLAB */
  int uart_dlab;
};

} /*namespace remu*/

#endif /* REMU_BCM2835_PERIPHERAL_H */

