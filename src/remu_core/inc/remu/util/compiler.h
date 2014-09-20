#ifndef REMU_UTIL_COMPILER_H
#define REMU_UTIL_COMPILER_H

#ifdef __GNUC__
#  define UNUSED(x) UNUSED_ ## x __attribute__((__unused__))
#  define UNUSED_FUNCTION(x) __attribute__((__unused__)) UNUSED_ ## x
#else
#  error Unknown compiler in util/compiler.h
#endif

#endif /*REMU_UTIL_COMPILER_H*/

