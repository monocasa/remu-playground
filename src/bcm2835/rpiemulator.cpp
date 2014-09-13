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

void RPiEmulator::tick()
{
  Emulator::tick();

  /* When graphics are emulated, we execute a screen refresh after 34ms has
   * passed (30 frames per second) */
  uint32_t frame_time = 20;
  if (isGraphicsEnabled())
  {
    uint64_t now = getTime();
    if ((now - last_refresh) > frame_time)
    {
      fb.tick();
      last_refresh = now;
    }
  }
}

} /*namespace remu*/
 
