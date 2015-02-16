#include <cstring>

extern "C" void *memset(void *s, int c, size_t n)
{
	unsigned char *buffer = (unsigned char*)s;

	for( size_t ii = 0; ii < n; ii++ ) {
		buffer[ii] = c;
	}

	return buffer;
}

