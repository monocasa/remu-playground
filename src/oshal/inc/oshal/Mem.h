#ifndef REMU_OSHAL_MEM_H
#define REMU_OSHAL_MEM_H

#include <cstdlib>

namespace remu { namespace oshal {

void* allocatePageMem(size_t size);
void freePageMem(void *pageMem);

}} /*namespace remu::oshal*/

#endif /*REMU_OSHAL_MEM_H*/

