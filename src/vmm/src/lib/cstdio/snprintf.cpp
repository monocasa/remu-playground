#include <lib/vfnprintf.h>

#include <cstdio>

namespace {

class SnprintfByteExporter : public lib_helper::ByteExporter
{
public:
	SnprintfByteExporter(char *buffer, size_t size)
	  : _buffer( buffer )
	  , _size( size )
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
	const size_t _size;
	size_t _bytes_written;

	void onByte(char c) override final {
		if( _bytes_written < (_size - 1) ) {
			_buffer[_bytes_written++] = c;
		}
	}
};

} /*anonymous namespace*/

extern "C" 
int snprintf(char *str, size_t size, const char *format, ...)
{
	SnprintfByteExporter exp(str, size);

	va_list ap;

	va_start(ap, format);
	vfnprintf(format, exp, 10, ap);
	va_end(ap);

	exp.finish();

	return exp.getBytesWritten();
}

