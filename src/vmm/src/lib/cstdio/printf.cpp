#include <lib/vfnprintf.h>

#include <os/Board.h>

#include <cstdio>

namespace {

class PrintfByteExporter : public lib_helper::ByteExporter
{
private:
	void onByte(char c) override final {
		os::board::putc(c);
	}
};

} /*anonymous namespace*/

extern "C"
int printf(const char *format, ...)
{
	int bytes_written;

	PrintfByteExporter exp;

	va_list ap;

	va_start(ap, format);
	bytes_written = vfnprintf(format, exp, 10, ap);
	va_end(ap);

	return bytes_written;
}

