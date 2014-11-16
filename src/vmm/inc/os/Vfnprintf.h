#ifndef REMU_VMM_OS_VFNPRINTF_H
#define REMU_VMM_OS_VFNPRINTF_H

#include <os/Types.h>

namespace os {

typedef void (*printf_putc_fn)(int, void*);

class ByteExporter
{
public:
	virtual void onByte( char c ) = 0;
};

int vfnprintf(char const *fmt, ByteExporter &exp, int radix, 
              va_list ap);

} /*namespace os*/

#endif /*REMU_VMM_OS_VFNPRINTF_H*/

