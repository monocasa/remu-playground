#ifndef REMU_VMM_OS_LOG_H
#define REMU_VMM_OS_LOG_H

#include "os/Types.h"

namespace os {

int log(const char *format, ...) PRINTF_FN(1,2);

} /*namespace os*/

#endif /*REMU_VMM_OS_PRINT_H*/

