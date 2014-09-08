#ifndef REMU_BITBANG_BUTTON_H
#define REMU_BITBANG_BUTTON_H

#include "gpiolistener.h"
#include "keylistener.h"

namespace remu { namespace bitbang {

class Button : public GpioListener
             , public KeyListener
{
};

}} /*namespace remu::bitbang*/

#endif /*REMU_BITBANG_H*/

