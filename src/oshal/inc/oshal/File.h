#ifndef REMU_OSHAL_FILE_H
#define REMU_OSHAL_FILE_H

#include "oshal/Exception.h"

#include "util/bitops.h"

#include <cstdio>
#include <string>

namespace remu { namespace oshal {

class File
{
public:
	enum Endian {
		BIG,
		LITTLE
	};

	File( const std::string &path )
	  : path( path )
	  , file( fopen(path.c_str(), "rb") )
	  , endian( SYS_ENDIAN )
	{ }

	~File() {
		if( file ) {
			fclose( file );
		}
	}

	bool isOpen() const {
		return (nullptr != file);
	}

	const std::string& name() const {
		return path;
	}

	void setEndian( Endian endian ) {
		this->endian = endian;
	}

	void seek( uint64_t offset ) {
		fseek(file, offset, SEEK_SET);
	}

	void readBuffer( uint8_t *buffer, const size_t size ) {
		if( 1 != fread(buffer, size, 1, file) ) {
			throw oshal::Exception( "Attempting to read beyond end of file:	%s", path.c_str() );
		}
	}

	template<typename T>
	T read() {
		T value;

		if( 1 != fread(&value, sizeof(T), 1, file) ) {
			throw oshal::Exception( "Attempting to read beyond end of file:	%s", path.c_str() );
		}

		if( endian != SYS_ENDIAN ) {
			value = util::bitops::byteSwap<T>( value );
		}

		return value;
	}

	size_t size() {
		long current = ftell( file );

		fseek( file, 0, SEEK_END );

		long last = ftell( file );

		fseek( file, current, SEEK_SET );

		return last;
	}

private:
	static const Endian SYS_ENDIAN = LITTLE;

	std::string path;
	FILE* file;
	Endian endian;
};

}} /*namespace remu::oshal*/

#endif /*REMU_OSHAL_FILE_H*/

