#include "remu/kvm/KvmContext.h"
#include "remu/EmulationException.h"

#include <linux/kvm.h>

#include <sys/ioctl.h>
#include <sys/mman.h>

#include <fcntl.h>
#include <unistd.h>

#include <cerrno>
#include <cstring>

namespace remu { namespace kvm {

const char *KvmContext::DEVICE_FILE_NAME = "/dev/kvm";

KvmContext::KvmContext()
  : device(openDeviceFile())
  , vm(device.openVmFile())
{ }

KvmContext::FdWrapper::~FdWrapper()
{
  if (-1 != fd)
  {
    close(fd);
  }
}

bool KvmContext::FdWrapper::callIoctl(int request, void *value, int &ret)
{
  if (-1 == fd)
  {
    throw EmulationException("Trying to ioctl closed fd");
  }

  ret = ioctl(fd, request, value);

  return ret >= 0;
}

void* KvmContext::FdWrapper::mmap(size_t size)
{
  return ::mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
}

int KvmContext::DeviceFile::openVmFile()
{
  int vmFd;

  if (!callIoctl(KVM_CREATE_VM, nullptr, vmFd))
  {
    throw EmulationException("Unable to KVM_CREATE_VM");
  }

  return vmFd;
}

int KvmContext::DeviceFile::readVcpuMmapSize()
{
  int mmapSize;

  if (!callIoctl(KVM_GET_VCPU_MMAP_SIZE, nullptr, mmapSize))
  {
    throw EmulationException("Unable to kvm_run size");
  }

  return mmapSize;
}

KvmContext::VmFile::VmFile(int fd)
  : FdWrapper(fd)
{ }

void KvmContext::VmFile::setTssLocation(uintptr_t tssLocation)
{
  if (!callIoctl(KVM_SET_TSS_ADDR, (void*)tssLocation))
  {
    throw EmulationException("Error setting TSS base for VM");
  }
}

int KvmContext::VmFile::allocateVcpu()
{
  int vcpuFd;

  if (!callIoctl(KVM_CREATE_VCPU, nullptr, vcpuFd))
  {
    throw EmulationException("Error creating  vcpu");
  }

  return vcpuFd;
}

void KvmContext::VmFile::setMemRegion(int slot, uint64_t guestBase, size_t memSize, void *hostBase)
{
  struct kvm_userspace_memory_region region;

  region.slot = slot;
  region.flags = 0;
  region.guest_phys_addr = guestBase;
  region.memory_size = memSize;
  region.userspace_addr = reinterpret_cast<unsigned long>(hostBase);

  if (!callIoctl(KVM_SET_USER_MEMORY_REGION, &region))
  {
    throw EmulationException("Unable to map kvm memory region");
  }
}

KvmContext::Cpu::Cpu(int fd, const KvmContext &kvmContext)
  : FdWrapper(fd)
  , kvmContext(kvmContext)
  , kvmRun(mmapKvmRun(kvmContext.getVcpuMmapSize()))
{
  setupRegisters();
  setupSpecialRegisters();
}

void KvmContext::Cpu::setGdt(uint64_t guestAddr, int len)
{
  struct kvm_sregs sregs;

  if (!callIoctl(KVM_GET_SREGS, &sregs))
  {
    throw EmulationException("Couldn't KVM_GET_SREGS");
  }

  sregs.gdt.base = guestAddr;
  sregs.gdt.limit = len;

  if (!callIoctl(KVM_SET_SREGS, &sregs))
  {
    throw EmulationException("Couldn't KVM_SET_SREGS");
  }
}

void KvmContext::Cpu::setIdt(uint64_t guestAddr, int len)
{
  struct kvm_sregs sregs;

  if (!callIoctl(KVM_GET_SREGS, &sregs))
  {
    throw EmulationException("Couldn't KVM_GET_SREGS");
  }

  sregs.idt.base = guestAddr;
  sregs.idt.limit = len;

  if (!callIoctl(KVM_SET_SREGS, &sregs))
  {
    throw EmulationException("Couldn't KVM_SET_SREGS");
  }
}

void KvmContext::Cpu::run()
{
  bool running = true;

  while (running)
  {
    if (!callIoctl(KVM_RUN, nullptr))
    {
      throw EmulationException("Can't KVM_RUN (%d)", kvmRun->exit_reason);
    }

    switch (kvmRun->exit_reason)
    {
      case KVM_EXIT_IO:
      {
        throw EmulationException("Got here");
      }

      default:
      {
        throw EmulationException("other reason (%d)", kvmRun->exit_reason);
      }
    }
  }
}

struct kvm_run* KvmContext::Cpu::mmapKvmRun(size_t kvmRunSize)
{
  void *addr = mmap(kvmRunSize);

  if (MAP_FAILED == addr)
  {
    throw EmulationException("Unable to mmap kvm_run");
  }

  return reinterpret_cast<struct kvm_run*>(addr);
}

void KvmContext::Cpu::setupRegisters()
{
  struct kvm_regs regs;

  if (!callIoctl(KVM_GET_REGS, &regs))
  {
    throw EmulationException("Couldn't KVM_GET_REGS");
  }

  regs.rax = 0x00000000;
  regs.rbx = 0x00000000;
  regs.rcx = 0x00000000;
  regs.rdx = 0x00000000;
  regs.rdi = 0x00000000;
  regs.rsi = 0x00000000;
  regs.rbp = 0x00000000;
  regs.rsp = 0x00003000 - 16;

  regs.rip = 0x00100000;
  regs.rflags = 0x00000046;

  if (!callIoctl(KVM_SET_REGS, &regs))
  {
    throw EmulationException("Couldn't KVM_SET_REGS");
  }
}

void KvmContext::Cpu::setupSpecialRegisters()
{
  struct kvm_sregs sregs;

  if (!callIoctl(KVM_GET_SREGS, &sregs))
  {
    throw EmulationException("Couldn't KVM_GET_SREGS");
  }

  sregs.cr0 = 0x60000011;
  sregs.cr2 = 0x00000000;
  sregs.cr3 = 0x00000000;
  sregs.cr4 = 0x00000000;
  sregs.efer = 0x00000000;
  sregs.apic_base = 0xFEE00000;

  setSegmentFlags(&sregs.cs,  0x0008, 0x00000000, 0xFFFFFFFF, 0x00C09B00, 0);
  setSegmentFlags(&sregs.ds,  0x0010, 0x00000000, 0xFFFFFFFF, 0x00C09300, 0);
  setSegmentFlags(&sregs.es,  0x0010, 0x00000000, 0xFFFFFFFF, 0x00C09300, 0);
  setSegmentFlags(&sregs.fs,  0x0000, 0x00000000, 0x0000FFFF, 0x00009300, 0);
  setSegmentFlags(&sregs.gs,  0x0000, 0x00000000, 0x0000FFFF, 0x00009300, 0);
  setSegmentFlags(&sregs.ss,  0x0010, 0x00000000, 0xFFFFFFFF, 0x00C09300, 0);
  setSegmentFlags(&sregs.ldt, 0x0000, 0x00000000, 0x0000FFFF, 0x00008200, 0);
  setSegmentFlags(&sregs.tr,  0x0000, 0x00000000, 0x0000FFFF, 0x00008B00, 0);

  sregs.gdt.base = 0;
  sregs.gdt.limit = 0;

  sregs.idt.base = 0;
  sregs.idt.limit = 0;

  if (!callIoctl(KVM_SET_SREGS, &sregs))
  {
    throw EmulationException("Couldn't KVM_SET_SREGS");
  }
}

void KvmContext::Cpu::setSegmentFlags(struct kvm_segment *segment, uint16_t selector, uint32_t base,
                                      uint32_t limit, uint32_t flags, int unusable)
{
  segment->selector = selector;

  segment->base = base;

  segment->limit = limit;

  segment->type    = (flags >> 8) & 0xf;
  segment->present = (flags >> 15) & 1;
  segment->dpl     = (flags >> 13) & 3;
  segment->db      = (flags >> 22) & 1;
  segment->s       = (flags >> 12) & 1;
  segment->l       = (flags >> 21) & 1;
  segment->g       = (flags >> 23) & 1;
  segment->avl     = (flags >> 20) & 1;

  segment->unusable = unusable;
}

int KvmContext::openDeviceFile()
{
  int fd = open(DEVICE_FILE_NAME, O_RDWR);

  if (-1 == fd )
  {
    throw EmulationException("Unable to open kvm device file:  %s", strerror(errno));
  }

  return fd;
}

}} /*namesapce remu::kvm*/

