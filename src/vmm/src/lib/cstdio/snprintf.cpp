#include <cstdio>

extern "C" 
int snprintf(char *str, size_t size, const char *format, ...)
{
	(void)size;
	(void)format;

	str[0] = 'A';
	str[1] = '\0';

	return 1;
}

