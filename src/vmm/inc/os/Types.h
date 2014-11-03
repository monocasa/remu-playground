#ifndef REMU_VMM_OS_TYPES_H
#define REMU_VMM_OS_TYPES_H

#include "os/Compiler.h"

static const int CHAR_BITS = 8;

typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int   uint32_t;
typedef unsigned long  uint64_t;

static_assert(sizeof(uint8_t)  == 1, "sizeof(uint8_t)  != 1");
static_assert(sizeof(uint16_t) == 2, "sizeof(uint16_t) != 2");
static_assert(sizeof(uint32_t) == 4, "sizeof(uint32_t) != 4");
static_assert(sizeof(uint64_t) == 8, "sizeof(uint64_t) != 8");

typedef signed char  int8_t;
typedef signed short int16_t;
typedef signed int   int32_t;
typedef signed long  int64_t;

static_assert(sizeof(int8_t)  == 1, "sizeof(int8_t)  != 1");
static_assert(sizeof(int16_t) == 2, "sizeof(int16_t) != 2");
static_assert(sizeof(int32_t) == 4, "sizeof(int32_t) != 4");
static_assert(sizeof(int64_t) == 8, "sizeof(int64_t) != 8");

#endif /*REMU_VMM_OS_TYPES_H*/

