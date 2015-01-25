#ifndef REMU_UTIL_BITOPS_H
#define REMU_UTIL_BITOPS_H

#include <climits>

namespace remu { namespace util { namespace bitops {

template<typename T>
static inline T rotateRight(const T x, int d)
{
	const int NUM_T_BITS = sizeof(T) * CHAR_BIT;

	return (x >> d) | (x << (NUM_T_BITS - d));
}

template<typename T, unsigned B>
static inline T signExtend(const T x) {
  struct {
    T x:B;
  } s;

  return s.x = x;
}

}}} /*namespace remu:util:bitops*/

#endif /*REMU_UTIL_BITOPS_H*/

