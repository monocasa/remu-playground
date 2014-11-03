#ifndef REMU_VMM_OS_COMPILER_H
#define REMU_VMM_OS_COMPILER_H

#ifdef __GNUC__

#	define va_list __builtin_va_list
#	define va_start __builtin_va_start
#	define va_arg __builtin_va_arg
#	define va_end __builtin_va_end

#	define UNUSED(x) x __attribute__((unused))
#	define PRINTF_FN(f,a) __attribute__((format(printf, f , a )))

#else /*!__GNUC__*/

#  error Unknown compiler in vmm/inc/os/Compiler.h

#endif /*!__GNUC__*/

#endif /*REMU_VMM_OS_COMPILER_H*/

