#ifndef REMU_KEYLISTENER_H
#define REMU_KEYLISTENER_H

#include <SDL/SDL.h>

namespace remu {

class KeyListener
{
public:
  virtual void onKeyDown(SDLKey key) = 0;
  virtual void onKeyUp(SDLKey key) = 0;
};

} /*namespace remu*/

#endif /*REMU_KEYLISTENER_H*/

