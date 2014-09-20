#include "host/mem.h"
#include "emulationexception.h"

namespace remu { namespace host {

namespace linux_internal {
const int PAGE_SIZE = 4096;
}

void* allocatePageMem(size_t size)
{
  void *memptr;

  const int status = posix_memalign(&memptr, linux_internal::PAGE_SIZE, size);
  switch (status)
  {
    case 0:
      return memptr;

    case EINVAL:
      throw EmulationException("Page size of 4096 is somehow not a valid alignment of posix_memalign");

    case ENOMEM:
      throw EmulationException("No memory left for remu::host::allocatePageMem(size=%ld)", size);

    default:
      throw EmulationException("Unknown status %d throw by posix_memalign in remu::host::allocatePageMem(size=%ld)", status, size);
  }
}

void freePageMem(void *pageMem)
{
  if (pageMem)
  {
    free(pageMem);
  }
}

}} /*namespace remu::host*/
