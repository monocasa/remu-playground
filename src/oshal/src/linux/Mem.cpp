#include "oshal/Exception.h"
#include "oshal/Mem.h"

namespace {
const int PAGE_SIZE = 4096;
} /*anonymous namespace*/

namespace remu { namespace oshal {

void* allocatePageMem(size_t size)
{
  void *memptr;

  const int status = posix_memalign(&memptr, ::PAGE_SIZE, size);
  switch (status)
  {
    case 0:
      return memptr;

    case EINVAL:
      throw Exception("Page size of 4096 is somehow not a valid alignment of posix_memalign");

    case ENOMEM:
      throw Exception("No memory left for remu::host::allocatePageMem(size=%ld)", size);

    default:
      throw Exception("Unknown status %d throw by posix_memalign in remu::host::allocatePageMem(size=%ld)", status, size);
  }
}

void freePageMem(void *pageMem)
{
  if (pageMem)
  {
    free(pageMem);
  }
}

}} /*namespace remu::oshal*/

