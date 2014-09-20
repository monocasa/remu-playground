#ifndef REMU_RAMREGION_H
#define REMU_RAMREGION_H

#include "remu/host/mem.h"

#include <cstdint>
#include <cstdlib>

namespace remu {

class RamRegion
{
public:
  RamRegion(size_t size, uint64_t base, bool readOnly)
    : size(size)
    , base(base)
    , readOnly(readOnly)
    , wasAllocatedHere(true)
    , buffer(remu::host::allocatePageMem(size))
  { }

  RamRegion(size_t size, uint64_t base, bool readOnly, void *buffer)
    : size(size)
    , base(base)
    , readOnly(readOnly)
    , wasAllocatedHere(false)
    , buffer(buffer)
  { }

  ~RamRegion()
  {
    if (wasAllocatedHere)
    {
      remu::host::freePageMem(buffer);
    }
  }

private:
  const size_t   size;
  const uint64_t base;
  const bool     readOnly;
  const bool     wasAllocatedHere;
  void          *buffer;
};

} /*namespace remu*/

#endif /*REMU_RAMREGION_H*/

