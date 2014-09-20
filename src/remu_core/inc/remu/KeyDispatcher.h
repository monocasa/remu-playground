#ifndef REMU_KEYDISPATCHER_H
#define REMU_KEYDISPATCHER_H

#include <SDL/SDL.h>

#include <list>

namespace remu {

class KeyListener;

class KeyDispatcher
{
public:
  void addKeyListener(KeyListener *listener) {
    listeners.push_back(listener);
  }

  void removeKeyListener(KeyListener *listener) {
    listeners.remove(listener);
  }

protected:
  void dispatchKeyDown(SDLKey key);
  void dispatchKeyUp(SDLKey key);

private:
  std::list<KeyListener*> listeners;
};

} /*namespace remu*/

#endif /*REMU_KEYDISPATCHER*/

