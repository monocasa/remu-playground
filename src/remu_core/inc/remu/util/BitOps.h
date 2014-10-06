#ifndef REMU_UTIL_BITOPS_H
#define REMU_UTIL_BITOPS_H

#include <cstdint>

namespace remu { namespace util {

template<typename T> static inline T byteSwap(T value);

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

template<typename T> constexpr T rotateLeft( T value, int sh );

template<>
constexpr uint32_t rotateLeft<uint32_t>(uint32_t value, int sh) {
  return (value << (sh & 0x1F)) | (value >> (32 - (sh & 0x1F)));
}

template<typename T, unsigned B>
static inline T signExtend(const T x) {
  struct {
    T x:B;
  } s;

  return s.x = x;
}

}} /*namespace remu::util*/

#endif /*REMU_UTIL_BITOPS_H*/

