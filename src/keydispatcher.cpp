#include "keydispatcher.h"
#include "keylistener.h"

namespace remu {

void KeyDispatcher::dispatchKeyDown(SDLKey key)
{
  for (auto listener : listeners) {
    listener->onKeyDown(key);
  }
}

void KeyDispatcher::dispatchKeyUp(SDLKey key)
{
  for (auto listener : listeners) {
    listener->onKeyUp(key);
  }
}

} /*namespace remu*/

