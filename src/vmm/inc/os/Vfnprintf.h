#ifndef REMU_VMM_OS_VFNPRINTF_H
#define REMU_VMM_OS_VFNPRINTF_H

#include "os/Types.h"

namespace os {

typedef void (*printf_putc_fn)(int, void*);

int vfnprintf(char const *fmt, printf_putc_fn fn, void *arg, int radix, 
              va_list ap);

} /*namespace os*/

#endif /*REMU_VMM_OS_VFNPRINTF_H*/

