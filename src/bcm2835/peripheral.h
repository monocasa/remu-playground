#ifndef REMU_BCM2835_PERIPHERAL_H
#define REMU_BCM2835_PERIPHERAL_H

/**
 * Peripherials state
 */
class Peripheral
{
public:
  Peripheral(Ui &ui);
  virtual ~Peripheral() = default;

  void writePort(uint32_t port, uint8_t data);
  uint32_t readPort(uint32_t port);

  /**
   * Checks whether a port is a peripherial or not
   * @param addr  Port to be tested
   * @return      true if port belongs to peripherial
   */
  static bool isPeripheralAddress(uint32_t addr) {
    return addr >= AUX_BASE && addr <= AUX_SPI1_CNTL1_REG;
  }

private:
  /**
   * List of auxiliary peripherial ports
   */
  enum {
    AUX_BASE           = 0x20215000,
    AUX_IRQ            = AUX_BASE + 0x00,
    AUX_ENABLES        = AUX_BASE + 0x04,
    AUX_MU_IO_REG      = AUX_BASE + 0x40,
    AUX_MU_IER_REG     = AUX_BASE + 0x44,
    AUX_MU_IIR_REG     = AUX_BASE + 0x48,
    AUX_MU_LCR_REG     = AUX_BASE + 0x4C,
    AUX_MU_MCR_REG     = AUX_BASE + 0x50,
    AUX_MU_LSR_REG     = AUX_BASE + 0x54,
    AUX_MU_MSR_REG     = AUX_BASE + 0x58,
    AUX_MU_SCRATCH     = AUX_BASE + 0x5C,
    AUX_MU_CNTL_REG    = AUX_BASE + 0x60,
    AUX_MU_STAT_REG    = AUX_BASE + 0x64,
    AUX_MU_BAUD_REG    = AUX_BASE + 0x68,
    AUX_SPI0_CNTL0_REG = AUX_BASE + 0x80,
    AUX_SPI0_CNTL1_REG = AUX_BASE + 0x84,
    AUX_SPI0_STAT_REG  = AUX_BASE + 0x88,
    AUX_SPI0_IO_REG    = AUX_BASE + 0x90,
    AUX_SPI0_PEEK_REG  = AUX_BASE + 0x94,
    AUX_SPI1_CNTL0_REG = AUX_BASE + 0xC0,
    AUX_SPI1_CNTL1_REG = AUX_BASE + 0xC4
  };

  Ui &ui;

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

#endif /* REMU_BCM2835_PERIPHERAL_H */

