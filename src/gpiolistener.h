#ifndef REMU_GPIOLISTENER_H
#define REMU_GPIOLISTENER_H

class GpioListener
{
public:
	virtual void onGpioWrite(uint32_t port, uint32_t value) = 0;
};

#endif /*REMU_GPIOLISTENER_H*/

