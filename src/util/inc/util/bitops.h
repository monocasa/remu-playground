#ifndef REMU_UTIL_BITOPS_H
#define REMU_UTIL_BITOPS_H

namespace remu { namespace util { namespace bitops {

template<typename T, unsigned B>
static inline T signExtend(const T x) {
  struct {
    T x:B;
  } s;

  return s.x = x;
}

}}} /*namespace remu:util:bitops*/

#endif /*REMU_UTIL_BITOPS_H*/

