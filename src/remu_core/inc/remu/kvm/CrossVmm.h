#ifndef REMU_KVM_CROSSVMM_H
#define REMU_KVM_CROSSVMM_H

#include "remu/kvm/KvmContext.h"
#include "remu/RamRegion.h"

#include <cstdint>
#include <cstdlib>
#include <list>

namespace remu {
class Memory;
} /*namespace remu */

namespace remu { namespace kvm {

class CrossVmm
{
public:
  CrossVmm(Memory &emuPhysMem);

  void run() {
    vcpu.run();
  }

private:
  static const size_t WRAM_SIZE = 512 * 1024 * 1024;
  static const uint64_t WRAM_BASE = 0x00000000;
  static const int WRAM_SLOT = 0;

  static const uintptr_t TSS_LOCATION = 0xE0000000;

  static const uint64_t EMU_BASE = 0x100000000UL;

  static const uint64_t INITIAL_GDT_LOCATION = 0x1000;

  static const uint8_t INITIAL_GDT[24];

  Memory         &emuPhysMem;
  RamRegion       wram;
  KvmContext      kvmContext;
  KvmContext::Cpu vcpu;
};

}} /*namespace remu::kvm*/

#endif /*REMU_KVM_CROSSVMM_H*/

