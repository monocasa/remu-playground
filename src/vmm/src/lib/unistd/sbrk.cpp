#include <os/MemoryManager.h>

extern "C" void *sbrk(intptr_t increment)
{
	return os::mm::sbrk(increment);
}

