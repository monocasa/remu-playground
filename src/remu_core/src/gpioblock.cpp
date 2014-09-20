#include "remu/gpioblock.h"
#include "remu/gpiolistener.h"

namespace remu {

void GpioBlock::dispatchGpioWrite(uint32_t port, uint32_t value)
{
  for (auto listener : listeners) {
    listener->onGpioWrite(port, value);
  }
}

} /*namespace remu*/

