#include "common.h"

#include <algorithm>

/**
 * Checks whether a port is a dma control port
 */
static inline int
dma_is_port(uint32_t addr)
{
  return 0x20007000 <= addr && addr < 0x20007FF4;
}

/**
 * Checks whether a port is a clock control port
 */
static inline int
clock_is_port(uint32_t addr)
{
  return 0x20101000 <= addr && addr < 0x20101FF4;
}

/**
 * Checks whether a port is a pwm control port
 */
static inline int
pwm_is_port(uint32_t addr)
{
  return 0x2020C000 <= addr && addr < 0x2020CFF4;
}

/**
 * Initialises the memory module
 * @param emu      Reference to the emulator structure
 * @param mem_size Size of dram in bytes
 */
Memory::Memory(Emulator *emu, size_t mem_size)
  : emu(emu)
  , mem_size(mem_size)
  , data(new uint8_t[mem_size])
  , gpio(nullptr)
{
  std::fill(data, data + mem_size, 0);
}

/**
 * Clean up Memory
 */
Memory::~Memory()
{
  delete data;
}

/**
 * Prints out the non-zero bytes from memory
 * @param memory Reference to the memory structure
 */
void
memory_dump(Memory* m)
{
  size_t i = 0;
  printf("Non-zero memory:\n");

  for (i = 0; i < m->emu->getMemSize() && i < 65535; i += 4)
  {
    uint32_t data = memory_read_dword_le(m, i);
    if (data != 0)
    {
      printf("0x%08zx: 0x%08x\n", i, data);
    }
  }
}

/**
 * Frees the memory
 * @param memory Reference to the memory structure
 */
void
memory_destroy(Memory* m)
{
  if (!m)
  {
    return;
  }
}

/**
 * Reads a byte from memory
 * @param memory Reference to the memory structure
 * @param addr Memory location
 */
uint8_t
memory_read_byte(Memory* m, uint32_t addr)
{
  addr = addr & 0x3FFFFFFF;

  /* SDRAM */
  if (__builtin_expect(addr < m->emu->getMemSize(), 1))
  {
    return m->data[addr];
  }

  m->emu->error("Out of bounds memory access at address 0x%08x", addr);
  return 0;
}

/**
 * Reads a word from memory (little endian)
 * @param memory Reference to the memory structure
 * @param address Memory location
 */
uint16_t
memory_read_word_le(Memory* m, uint32_t addr)
{
  uint32_t base;
  uint8_t off;

  addr = addr & 0x3FFFFFFF;

  /* SDRAM */
  if (__builtin_expect(addr + 1 < m->emu->getMemSize(), 1))
  {
    base = addr & ~0x01;
    off = addr & 0x01;

    return (m->data[base + ((off + 0) & 0x01)] <<  0) |
           (m->data[base + ((off + 1) & 0x01)] <<  8);
  }

  m->emu->error("Out of bounds memory access at address 0x%08x", addr);
  return 0;
}

/**
 * Reads a double word from memory (little endian)
 * @param memory Reference to the memory structure
 * @param address Memory location
 */
uint32_t
memory_read_dword_le(Memory* m, uint32_t addr)
{
  uint32_t base;
  uint8_t off;

  /* Apparently, SDRAM and IO peripherials are mapped to 4 different address
   * ranges by the VideoCore MMU. We can replicate that behaviour by ignoring
   * the two most significant bits of the address. We can do this thanks to
   * the fact that we ignore cacheing
   */
  addr = addr & 0x3FFFFFFF;

  /* SDRAM Read */
  if (__builtin_expect(addr + 3 < m->emu->getMemSize(), 1))
  {
    base = addr & ~0x03;
    off = addr & 0x03;

    return (m->data[base + ((off + 0) & 0x03)] <<  0) |
           (m->data[base + ((off + 1) & 0x03)] <<  8) |
           (m->data[base + ((off + 2) & 0x03)] << 16) |
           (m->data[base + ((off + 3) & 0x03)] << 24);
  }

  /* System Timer */
  if (addr == 0x20003004)
  {
    uint64_t timer_value = m->emu->getSystemTimer();
    return (uint32_t)(timer_value & 0xffffffff);
  }
  else if (addr == 0x20003008)
  {
    uint64_t timer_value = m->emu->getSystemTimer();
    return (uint32_t)((timer_value >> 32) & 0xffffffff);
  }

  /* GPIO registers */
  if (Gpio::isGpioAddress(addr))
  {
    return m->gpio->readPort(addr);
  }

  /* Mailbox interface */
  if (mbox_is_port(addr))
  {
    return mbox_read(&m->emu->mbox, addr);
  }

  /* Peripherals */
  if (Peripheral::isPeripheralAddress(addr))
  {
    return m->emu->pr->readPort(addr);
  }

  /* DMA - just ignore it */
  if (dma_is_port(addr))
  {
    return 0;
  }

  /* CLOCK - just ignore it */
  if (clock_is_port(addr))
  {
    return 0;
  }

  /* PWM - just ignore it */
  if (pwm_is_port(addr))
  {
    return 0;
  }

  m->emu->error("Out of bounds memory access at address 0x%08x", addr);
  return 0;
}

/**
 * Writes a single byte to memory
 * @param m    Reference to the memory structure
 * @param addr Memory location
 * @param data Data to be written
 */
void
memory_write_byte(Memory* m, uint32_t addr, uint8_t data)
{
  addr = addr & 0x3FFFFFFF;

  /* SDRAM */
  if (__builtin_expect(addr < m->emu->getMemSize(), 1))
  {
    m->data[addr] = data;
    return;
  }

  m->emu->error("Out of bounds memory access at address 0x%08x", addr);
}

/**
 * Writes a word to memory
 * @param m    Reference to the memory structure
 * @param addr Memory location
 * @param data Data to be written
 */
void
memory_write_word_le(Memory* m, uint32_t addr, uint16_t data)
{
  addr = addr & 0x3FFFFFFF;

  /* SDRAM */
  if (__builtin_expect(addr + 1 < m->emu->getMemSize(), 1))
  {
    m->data[addr + 0] = (data >> 0) & 0xFF;
    m->data[addr + 1] = (data >> 8) & 0xFF;
    return;
  }

  /* Framebuffer */
  if (fb_is_buffer(&m->emu->fb, addr))
  {
    fb_write_word(&m->emu->fb, addr, data);
    return;
  }

  m->emu->error("Out of bounds memory access at address 0x%08x", addr);
}

/**
 * Writes a double word to memory
 * @param m    Reference to the memory structure
 * @param addr Memory location
 * @param data Data to be written
 */
void
memory_write_dword_le(Memory* m, uint32_t addr, uint32_t data)
{
  addr = addr & 0x3FFFFFFF;

  /* SDRAM */
  if (__builtin_expect(addr + 3 < m->emu->getMemSize(), 1))
  {
    m->data[addr + 0] = (data >>  0) & 0xFF;
    m->data[addr + 1] = (data >>  8) & 0xFF;
    m->data[addr + 2] = (data >> 16) & 0xFF;
    m->data[addr + 3] = (data >> 24) & 0xFF;
    return;
  }

  /* GPIO registers */
  if (Gpio::isGpioAddress(addr))
  {
    m->gpio->writePort(addr, data);
    return;
  }

  /* Mailbox interface */
  if (mbox_is_port(addr))
  {
    mbox_write(&m->emu->mbox, addr, data);
    return;
  }

  /* Framebuffer */
  if (fb_is_buffer(&m->emu->fb, addr))
  {
    fb_write_dword(&m->emu->fb, addr, data);
    return;
  }

  /* Peripherals */
  if (Peripheral::isPeripheralAddress(addr))
  {
    m->emu->pr->writePort(addr, data);
    return;
  }

  /* DMA - just ignore it */
  if (dma_is_port(addr))
  {
    return;
  }

  /* CLOCK - just ignore it */
  if (clock_is_port(addr))
  {
    return;
  }

  /* PWM - just ignore it */
  if (pwm_is_port(addr))
  {
    return;
  }

  m->emu->error("Out of bounds memory access at address 0x%08x", addr);
}

