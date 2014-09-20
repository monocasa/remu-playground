#ifndef REMU_RAMREGION_H
#define REMU_RAMREGION_H

#include "host/mem.h"

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
    , buffer(remu::host::allocatePageMem(size))
  { }

  ~RamRegion()
  {
    remu::host::freePageMem(buffer);
  }

private:
  const size_t   size;
  const uint64_t base;
  const bool     readOnly;
  void          *buffer;
};

} /*namespace remu*/

#endif /*REMU_RAMREGION_H*/

