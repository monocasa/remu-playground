#include "remu/Emulator.h"
#include "remu/Memory.h"
#include "remu/RamRegion.h"

#include <algorithm>

namespace remu {

/**
 * Initialises the memory module
 * @param emu      Reference to the emulator structure
 * @param mem_size Size of dram in bytes
 */
Memory::Memory(Emulator *emu, size_t mem_size)
  : emu(emu)
  , mem_size(mem_size)
  , data(new uint8_t[mem_size])
  , ramRegion(new RamRegion(mem_size, 0, false, data))
{
  std::fill(data, data + mem_size, 0);

  ramList.push_back(ramRegion);
}

/**
 * Clean up Memory
 */
Memory::~Memory()
{
  delete ramRegion;
  delete data;
}

/**
 * Prints out the non-zero bytes from memory
 */
void Memory::dump()
{
  size_t i = 0;
  printf("Non-zero memory:\n");

  for (i = 0; i < mem_size && i < 65535; i += 4)
  {
    uint32_t data = readDwordLe(i);
    if (data != 0)
    {
      printf("0x%08zx: 0x%08x\n", i, data);
    }
  }
}

/**
 * Reads a byte from memory
 * @param addr Memory location
 */
uint8_t Memory::readByte(uint32_t addr)
{
  addr = addr & 0x3FFFFFFF;

  /* SDRAM */
  if (__builtin_expect(addr < mem_size, 1))
  {
    return data[addr];
  }

  emu->error("Out of bounds memory access at address 0x%08x", addr);
  return 0;
}

/**
 * Reads a word from memory (little endian)
 * @param memory Reference to the memory structure
 */
uint16_t Memory::readWordLe(uint32_t addr)
{
  uint32_t base;
  uint8_t off;

  addr = addr & 0x3FFFFFFF;

  /* SDRAM */
  if (__builtin_expect(addr + 1 < mem_size, 1))
  {
    base = addr & ~0x01;
    off = addr & 0x01;

    return (data[base + ((off + 0) & 0x01)] <<  0) |
           (data[base + ((off + 1) & 0x01)] <<  8);
  }

  emu->error("Out of bounds memory access at address 0x%08x", addr);
  return 0;
}

/**
 * Reads a double word from memory (little endian)
 * @param address Memory location
 */
uint32_t Memory::readDwordLe(uint32_t addr)
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
  if (__builtin_expect(addr + 3 < mem_size, 1))
  {
    base = addr & ~0x03;
    off = addr & 0x03;

    return (data[base + ((off + 0) & 0x03)] <<  0) |
           (data[base + ((off + 1) & 0x03)] <<  8) |
           (data[base + ((off + 2) & 0x03)] << 16) |
           (data[base + ((off + 3) & 0x03)] << 24);
  }

  /* Grab relevant IoRegion and call it if registered */
  IoRegion *ioregion = iomap.getRegionForAddr(addr);
  if (ioregion) {
    return ioregion->readIo(addr - ioregion->base, sizeof(uint32_t));
  }

  emu->error("Out of bounds memory access at address 0x%08x", addr);
  return 0;
}

/**
 * Writes a single byte to memory
 * @param addr  Memory location
 * @param value Data to be written
 */
void Memory::writeByte(uint32_t addr, uint8_t value)
{
  addr = addr & 0x3FFFFFFF;

  /* SDRAM */
  if (__builtin_expect(addr < mem_size, 1))
  {
    data[addr] = value;
    return;
  }

  emu->error("Out of bounds memory access at address 0x%08x", addr);
}

/**
 * Writes a word to memory
 * @param addr Memory location
 * @param data Data to be written
 */
void Memory::writeWordLe(uint32_t addr, uint16_t value)
{
  addr = addr & 0x3FFFFFFF;

  /* SDRAM */
  if (__builtin_expect(addr + 1 < mem_size, 1))
  {
    data[addr + 0] = (value >> 0) & 0xFF;
    data[addr + 1] = (value >> 8) & 0xFF;
    return;
  }

  /* Grab relevant IoRegion and call it if registered */
  IoRegion *ioregion = iomap.getRegionForAddr(addr);
  if (ioregion) {
    ioregion->writeIo(addr, value, sizeof(uint16_t));
    return;
  }

  emu->error("Out of bounds memory access at address 0x%08x", addr);
}

/**
 * Writes a double word to memory
 * @param addr  Memory location
 * @param value Data to be written
 */
void Memory::writeDwordLe(uint32_t addr, uint32_t value)
{
  addr = addr & 0x3FFFFFFF;

  /* SDRAM */
  if (__builtin_expect(addr + 3 < mem_size, 1))
  {
    data[addr + 0] = (value >>  0) & 0xFF;
    data[addr + 1] = (value >>  8) & 0xFF;
    data[addr + 2] = (value >> 16) & 0xFF;
    data[addr + 3] = (value >> 24) & 0xFF;
    return;
  }

  /* Grab relevant IoRegion and call it if registered */
  IoRegion *ioregion = iomap.getRegionForAddr(addr);
  if (ioregion) {
    ioregion->writeIo(addr - ioregion->base, value, sizeof(uint32_t));
    return;
  }

  emu->error("Out of bounds memory access at address 0x%08x", addr);
}

} /*namespace remu*/

