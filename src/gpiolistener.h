#ifndef REMU_GPIOLISTENER_H
#define REMU_GPIOLISTENER_H

#include <cstdint>

namespace remu {

class GpioListener
{
public:
	virtual void onGpioWrite(uint32_t port, uint32_t value) = 0;
};

} /*namespace remu*/

#endif /*REMU_GPIOLISTENER_H*/

