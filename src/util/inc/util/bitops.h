#ifndef REMU_UTIL_BITOPS_H
#define REMU_UTIL_BITOPS_H

#include <climits>
#include <cstdint>

namespace remu { namespace util { namespace bitops {

template<typename T> static inline T byteSwap(T value);

template<>
uint8_t byteSwap<uint8_t>(uint8_t value)
{
	return value;
}

template<>
uint16_t byteSwap<uint16_t>(uint16_t value)
{
  return (value >> 8) | (value << 8);
}

template<>
uint32_t byteSwap<uint32_t>(uint32_t value)
{
  return ((uint32_t)byteSwap<uint16_t>(value) << 16) | byteSwap<uint16_t>(value >> 16);
}

template<>
uint64_t byteSwap<uint64_t>(uint64_t value)
{
  return ((uint64_t)byteSwap<uint32_t>(value) << 32) | byteSwap<uint32_t>(value >> 32);
}

template<typename T>
static inline T rotateLeft(const T x, int d) 
{
	const int NUM_T_BITS = sizeof(T) * CHAR_BIT;
	const int T_BITS_MASK = NUM_T_BITS - 1;
	const int REAL_SH = d & T_BITS_MASK;

	return (x << REAL_SH) | (x >> (NUM_T_BITS - REAL_SH));
}

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

