#include "bitbang/button.h"
#include "gpioblock.h"
#include "keydispatcher.h"

#include <cstdint>

namespace remu { namespace bitbang {

Button::Button(GpioBlock &gpio, KeyDispatcher &kd, int port, SDLKey key)
  : gpio(gpio)
  , kd(kd)
  , port(port)
  , key(key)
{
  kd.addKeyListener(this);
}

Button::~Button()
{
  kd.removeKeyListener(this);
}

void Button::onKeyDown(SDLKey incomingKey)
{
  if (incomingKey == key) {
    gpio.setPortState(port, 1);
  }
}

void Button::onKeyUp(SDLKey incomingKey)
{
  if (incomingKey == key) {
    gpio.setPortState(port, 0);
  }
}

}} /*namespace remu::bitbang*/

