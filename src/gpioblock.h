#ifndef REMU_GPIOBLOCK_H
#define REMU_GPIOBLOCK_H

#include <cstdint>
#include <list>

namespace remu {

class GpioListener;

class GpioBlock
{
public:
  virtual void setPortState(int port, uint8_t state) = 0;

  void addGpioListener(GpioListener *listener) {
    listeners.push_back(listener);
  }

  void removeGpioListener(GpioListener *listener) {
    listeners.remove(listener);
  }

protected:
  void dispatchGpioWrite(uint32_t port, uint32_t value);

private:
  std::list<GpioListener*> listeners;
};

} /*namespace remu*/

#endif /*REMU_GPIOBLOCK_H*/

