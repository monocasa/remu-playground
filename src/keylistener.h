#ifndef REMU_KEYLISTENER_H
#define REMU_KEYLISTENER_H

#include <SDL/SDL.h>

class KeyListener
{
public:
  virtual void onKeyDown(SDLKey key) = 0;
  virtual void onKeyUp(SDLKey key) = 0;
};

#endif /*REMU_KEYLISTENER_H*/

