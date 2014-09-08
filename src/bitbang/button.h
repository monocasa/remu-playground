#ifndef REMU_BITBANG_BUTTON_H
#define REMU_BITBANG_BUTTON_H

#include "keylistener.h"

namespace remu {
class GpioBlock;
class KeyDispatcher;
} /*namespace remu*/

namespace remu { namespace bitbang {

class Button : public KeyListener
{
public:
  Button(GpioBlock &gpio, KeyDispatcher &kd, int port, SDLKey key);
  virtual ~Button();

  void onKeyDown(SDLKey key) override final;
  void onKeyUp(SDLKey key) override final;

private:
  GpioBlock &gpio;
  KeyDispatcher &kd;

  const int port;
  const SDLKey key;
};

}} /*namespace remu::bitbang*/

#endif /*REMU_BITBANG_H*/

