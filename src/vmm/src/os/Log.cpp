#include <os/Board.h>
#include <os/Types.h>
#include <os/Vfnprintf.h>

namespace {

void putchar(int c, void* UNUSED(arg))
{
	os::board::putc(c);
}

} /*anonymous namespace*/

namespace os {

void log(const char *format, ...)
{
	va_list ap;

	va_start(ap, format);
	vfnprintf(format, putchar, nullptr, 10, ap);
	va_end(ap);
}

} /*namespace os*/

