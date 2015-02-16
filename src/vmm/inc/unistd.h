#ifndef VMM_UNISTD_H
#define VMM_UNISTD_H

#include <os/Types.h>

extern "C" void* sbrk(intptr_t increment);

#endif /*VMM_UNISTD_H*/

