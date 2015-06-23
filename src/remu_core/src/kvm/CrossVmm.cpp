#include "remu/kvm/CrossVmm.h"
#include "remu/loader/Loader.h"
#include "remu/EmulationException.h"
#include "remu/Memory.h"

#include <cstring>

namespace remu { namespace kvm {

const uint8_t CrossVmm::INITIAL_GDT[24] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xFF, 0xFF, 0x00, 0x00, 0x00, 0x9B, 0xCF, 0x00,
  0xFF, 0xFF, 0x00, 0x00, 0x00, 0x93, 0xCF, 0x00,
};

CrossVmm::CrossVmm(Memory &emuPhysMem)
  : emuPhysMem(emuPhysMem)
  , wram(WRAM_SIZE, WRAM_BASE, false)
  , kvmContext()
  , vcpu(kvmContext.allocateCpu(), kvmContext, *this, *this)
{
  uint64_t entry = 0;
  int cur_slot = 0;

  kvmContext.setTssLocation(TSS_LOCATION);

  memcpy(((uint8_t*)wram.getBuffer()) + INITIAL_GDT_LOCATION, INITIAL_GDT, sizeof(INITIAL_GDT));

  kvmContext.setMemRegion(cur_slot++, WRAM_BASE, WRAM_SIZE, wram.getBuffer());

  for(auto region : emuPhysMem.getRamRegions()) {
    kvmContext.setMemRegion(cur_slot++,
                            region->getBase() + EMU_BASE,
                            region->getSize(),
                            region->getBuffer());
  }

  vcpu.setGdt(INITIAL_GDT_LOCATION, sizeof(INITIAL_GDT));

  remu::loader::loadElf(*this, "bin/vmm.bin", entry);

  vcpu.setPc(entry);
}

uint8_t* CrossVmm::bufferForRegion(uint64_t base_addr, size_t size)
{
  if ((base_addr + size) > WRAM_SIZE) {
    return nullptr;
  }

  return ((uint8_t*)wram.getBuffer()) + base_addr;
}

void CrossVmm::onOut(int size, uint16_t port, uint64_t data)
{
  switch (port)
  {
    /* exit */
    case 0:
    {
      vcpu.stop();
      break;
    }

    /* putc */
    case 1:
    {
      printf("%c", (uint8_t)data);
      break;
    }

    default:
    {
      throw EmulationException("CrossVmm::onOut(size=%d, port=0x%x, data=0x%lx)", size, port, data);
    }
  }
}

void CrossVmm::onWrite(int size, uint64_t addr, uint8_t *data)
{
  if( addr < EMU_BASE )
  {
    throw EmulationException("MMIO Write below EMU_BASE:  addr=%08lx", addr);
  }

  addr -= EMU_BASE;

  switch( size )
  {
    case 4:
    {
      uint32_t *data_ptr = (uint32_t*)data;
      emuPhysMem.writeDwordLe(addr, *data_ptr);
      break;
    }

    default:
    {
      throw EmulationException("Unknown size in CrossVmm.onWrite( size=%d )", size);
    }
  }
}

void CrossVmm::onRead(int size, uint64_t addr, uint8_t *data)
{
  if( addr < EMU_BASE )
  {
    throw EmulationException("MMIO Write below EMU_BASE:  addr=%08lx", addr);
  }

  addr -= EMU_BASE;

  switch( size )
  {
    case 4:
    {
      uint32_t *data_ptr = (uint32_t*)data;
      *data_ptr = emuPhysMem.readDwordLe(addr);
      break;
    }

    default:
    {
      throw EmulationException("Unknown size in CrossVmm.onRead( size=%d )", size);
    }
  }
}

}} /*namespace remu::kvm*/

