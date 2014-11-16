#include <cstdio>

extern "C" 
int sprintf(char *str, const char *format, ...)
{
	(void)format;

	str[0] = 'A';
	str[1] = '\0';

	return 1;
}

