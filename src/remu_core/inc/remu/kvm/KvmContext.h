#ifndef REMU_KVM_KVMCONTEXT_H
#define REMU_KVM_KVMCONTEXT_H

#include <cstdint>
#include <cstdlib>

struct kvm_run;
struct kvm_segment;

namespace remu { namespace kvm {

class KvmContext
{
public:
  KvmContext();

  ~KvmContext() = default;

  void setTssLocation(uintptr_t tssLocation) {
    vm.setTssLocation(tssLocation);
  }

  int allocateCpu() {
    return vm.allocateVcpu();
  }

  void setMemRegion(int slot, uint64_t guestBase, size_t memSize, void *hostBase) {
    vm.setMemRegion(slot, guestBase, memSize, hostBase);
  }

private:
  static const char *DEVICE_FILE_NAME;

  class FdWrapper
  {
  protected:
    FdWrapper(int fd)
      : fd(fd)
    {}

    virtual ~FdWrapper();

    bool callIoctl(int request, void *value, int &ret);

    bool callIoctl(int request, void *value) {
      int ret;
      return callIoctl(request, value, ret);
    }

    void *mmap(size_t size);

  private:
    int fd;
  };

  class DeviceFile : public FdWrapper
  {
  public:
    DeviceFile(int fd)
      : FdWrapper(fd)
      , vcpuMmapSize(readVcpuMmapSize())
    { }

    virtual ~DeviceFile() = default;

    int openVmFile();

    int getVcpuMmapSize() const {
      return vcpuMmapSize;
    }

  private:
    int readVcpuMmapSize();

    int vcpuMmapSize;
  };

  class VmFile : public FdWrapper
  {
  public:
    VmFile(int fd);

    virtual ~VmFile() = default;

    void setTssLocation(uintptr_t tssBase);

    int allocateVcpu();

    void setMemRegion(int slot, uint64_t guestBase, size_t memSize, void *hostBase);
  };

public:
  class Cpu : public FdWrapper
  {
  public:
    class IoPortHandler
    {
    public:
      virtual void onOut(int size, uint16_t port, uint64_t data) = 0;
    };

    class MmioHandler
    {
    public:
      virtual void onRead(int size, uint64_t addr, uint8_t *data) = 0;
    };

    Cpu(int fd, const KvmContext &kvmContext, IoPortHandler &portHandler,
        MmioHandler &mmioHandler);

    virtual ~Cpu() = default;

    void setGdt(uint64_t guestAddr, int size);
    void setIdt(uint64_t guestAddr, int size);
    void setPc(uint64_t pc);

    void run();

    void stop();

  private:
    const KvmContext &kvmContext;
    IoPortHandler    &portHandler;
    MmioHandler      &mmioHandler;
    struct kvm_run  *kvmRun;
    bool             running;

    kvm_run* mmapKvmRun(size_t size);

    void setupRegisters();
    void setupSpecialRegisters();

    static void setSegmentFlags(struct kvm_segment *segment, uint16_t selector, uint32_t base,
                                uint32_t limit, uint32_t flags, int usuable);

    void dump();
  };

private:
  static int openDeviceFile();

  int getVcpuMmapSize() const {
    return device.getVcpuMmapSize();
  }

  DeviceFile device;
  VmFile     vm;
};

}} /*namespace remu::kvm*/

#endif /*REMU_KVM_KVMCONTEXT_H*/

