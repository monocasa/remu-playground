#include <cstring>

extern "C"
char* strcpy(char *dest, const char *src)
{
	char *original_dest = dest;

	while(*src) {
		*dest = *src;
		dest++;
		src++;
	}

	return original_dest;
}

