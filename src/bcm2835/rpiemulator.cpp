#include "bcm2835/rpiemulator.h"

namespace remu {

RPiEmulator::RPiStubRegion::RPiStubRegion(uint32_t base, uint32_t length, Memory &mem)
  : StubRegion(base, length)
  , mem(mem)
{
  mem.addRegion(this);
}

RPiEmulator::RPiStubRegion::~RPiStubRegion()
{
  mem.removeRegion(this);
}

} /*namespace remu*/
 
