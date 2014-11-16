#include <os/Board.h>
#include <os/Types.h>
#include <os/Vfnprintf.h>

namespace {

class LogByteExporter : public os::ByteExporter
{
private:
	void onByte(char c) override final {
		os::board::putc(c);
	}
};

} /*anonymous namespace*/

namespace os {

void log(const char *format, ...)
{
	LogByteExporter exp;

	va_list ap;

	va_start(ap, format);
	vfnprintf(format, exp, 10, ap);
	va_end(ap);
}

} /*namespace os*/

