#include "remu/kvm/CrossVmm.h"
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
  , vcpu(kvmContext.allocateCpu(), kvmContext)
{
  kvmContext.setTssLocation(TSS_LOCATION);

  memcpy(((uint8_t*)wram.getBuffer()) + INITIAL_GDT_LOCATION, INITIAL_GDT, sizeof(INITIAL_GDT));

  kvmContext.setMemRegion(WRAM_SLOT, WRAM_BASE, WRAM_SIZE, wram.getBuffer());

  vcpu.setGdt(INITIAL_GDT_LOCATION, sizeof(INITIAL_GDT));

  ((uint8_t*)wram.getBuffer())[0x00100000] = 0xEE;
}

}} /*namespace remu::kvm*/

