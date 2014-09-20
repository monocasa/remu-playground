#ifndef REMU_HOST_MEM_H
#define REMU_HOST_MEM_H

#include <cstdlib>

namespace remu { namespace host {

void* allocatePageMem(size_t size);
void freePageMem(void *pageMem);

}} /*namespace remu::host*/

#endif /*REMU_HOST_MEM_H*/

