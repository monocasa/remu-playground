#include <lib/vfnprintf.h>

#include <cstdio>

namespace {

class SnprintfByteExporter : public lib_helper::ByteExporter
{
public:
	SnprintfByteExporter(char *buffer)
	  : _buffer( buffer )
	  , _bytes_written(0)
	{ }

	void finish() {
		_buffer[_bytes_written] = '\0';
	}

	size_t getBytesWritten() const {
		return _bytes_written;
	}
	
private:
	char *_buffer;
	size_t _bytes_written;

	void onByte(char c) override final {
			_buffer[_bytes_written++] = c;
	}
};

} /*anonymous namespace*/

extern "C" 
int sprintf(char *str, const char *format, ...)
{
	SnprintfByteExporter exp(str);

	va_list ap;

	va_start(ap, format);
	vfnprintf(format, exp, 10, ap);
	va_end(ap);

	exp.finish();

	return exp.getBytesWritten();
}

