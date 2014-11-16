#ifndef REMU_VMM_LIB_VFNPRINTF_H
#define REMU_VMM_LIB_VFNPRINTF_H

#include <os/Types.h>

namespace lib_helper {

class ByteExporter
{
public:
	virtual void onByte( char c ) = 0;
};

int vfnprintf(char const *fmt, ByteExporter &exp, int radix, 
              va_list ap);

} /*namespace lib_helper*/

#endif /*REMU_VMM_LIB_VFNPRINTF_H*/

