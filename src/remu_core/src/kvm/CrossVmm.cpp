#include "remu/kvm/CrossVmm.h"
#include "remu/loader/Loader.h"
#include "remu/Memory.h"

#include "oshal/Exception.h"

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

void CrossVmm::onOut(KvmContext::Cpu &cpu, int size, uint16_t port, uint64_t data)
{
  (void)size;
  (void)data;

  if (port != 0) {
    throw oshal::Exception("CrossVmm::onOut(size=%d, port=0x%x, data=0x%lx)", size, port, data);
  }

  KvmContext::Cpu::GprSet gprs;

  cpu.getGprs( gprs );

  switch( gprs.rdi )
  {
    /* Exit */
    case 0: {
      cpu.stop();
    }
    break;

    /* Putc */
    case 1: {
      printf("%c", (uint8_t)gprs.rsi);
    }
    break;

    /* Read Env Blob */
    case 2: {
      gprs.rcx = readBlobHypercall(gprs.rsi, gprs.rdx, gprs.rcx);
      cpu.setGprs( gprs );
	}
	break;

    default: {
      throw oshal::Exception("Got here rdi:0x%lx rsi:0x%lx \n", gprs.rdi, gprs.rsi);
    }
  }

}

void CrossVmm::onWrite(int size, uint64_t addr, uint8_t *data)
{
  if( addr < EMU_BASE )
  {
    throw oshal::Exception("MMIO Write below EMU_BASE:  addr=%08lx", addr);
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
      throw oshal::Exception("Unknown size in CrossVmm.onWrite( size=%d )", size);
    }
  }
}

void CrossVmm::onRead(int size, uint64_t addr, uint8_t *data)
{
  if( addr < EMU_BASE )
  {
    throw oshal::Exception("MMIO Write below EMU_BASE:  addr=%08lx", addr);
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
      throw oshal::Exception("Unknown size in CrossVmm.onRead( size=%d )", size);
    }
  }
}

bool CrossVmm::readGuestStr(uint64_t guest_addr, char* host_str, size_t size)
{
  for( size_t ii = 0; ii < size; ii++ ) {
    auto ptr_to_guest = bufferForRegion(guest_addr + ii, 1);
    if( !ptr_to_guest ) {
      return false;
    }
    host_str[ii] = *ptr_to_guest;
    if( host_str[ii] == '\0' ) {
      return true;
    }
  }

  return false;
}

int64_t CrossVmm::readBlobHypercall(uint64_t name_ptr, uint64_t buffer_ptr, uint64_t buffer_size)
{
  char name_buffer[64];

  if( !readGuestStr(name_ptr, name_buffer, sizeof(name_buffer) ) ) {
    printf("Guest gave the host an invalid buffer for readBlob\n" );
    return -1;
  }

  auto buffer = bufferForRegion(buffer_ptr, buffer_size);
  if( !buffer ) {
    printf("Guest gave the host an invalid buffer for readBlob\n");
    return -1;
  }

  if( strcmp(name_buffer, "core0_cmd_line") == 0 ) {
    strncpy((char*)buffer, "arm1176", buffer_size);
    return strlen("arm1176") + 1;
  }
  else {
    printf("Guest tried to receive an unknown blob:  \"%s\"\n", name_buffer);
    return -1;
  }
}

}} /*namespace remu::kvm*/

