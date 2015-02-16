#include <os/Board.h>

#include <cstdio>
#include <cstdlib>

extern "C" void __cxa_pure_virtual()
{
	printf("__cxa_pure_virtual() called.  Probably a bad build.\n");
	os::board::shutdown();
}

void* operator new(size_t size)
{
	return malloc(size);
}

void* operator new[](size_t size)
{
	return malloc(size);
}

void operator delete(void *ptr)
{
	free(ptr);
}

void operator delete[](void *ptr)
{
	free(ptr);
}

