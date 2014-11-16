#include <cstdio>

extern "C" 
int printf(const char *format, ...)
{
	(void)format;
	return 0;
}

