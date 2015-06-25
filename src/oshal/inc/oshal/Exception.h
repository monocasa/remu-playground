#ifndef REMU_OSHAL_EXCEPTION_H
#define REMU_OSHAL_EXCEPTION_H

#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stdexcept>

namespace remu { namespace oshal {

class Exception : public std::exception
{
public:
	Exception( const char *fmt, ... )
	  : str( nullptr )
	  , result( 0 )
	{
		va_list va;

		va_start( va, fmt );
		result = vasprintf( &str, fmt, va );
		va_end( va );
	}

	virtual ~Exception()
	{
		if( str ) {
			free(str);
		}
	}

	Exception( const Exception &other )
	  : str( strdup(other.str) )
	{ }

	Exception( Exception &&other )
	  : str( other.str )
	{
		other.str = nullptr;
	}

	Exception& operator=( const Exception &other )
	{
		if( this != &other ) {
			if( str ) {
				free( str );
			}

			str = strdup( other.str );
		}

		return *this;
	}

	Exception& operator=( Exception &&other )
	{
		if( this != &other ) {
			if( str ) {
				free( str );
			}

			str = other.str;

			other.str = nullptr;
		}

		return *this;
	}

private:
	char *str;
	int result;

	const char* what() const noexcept override {
		return str;
	}
};

}} /*namespace remu::oshal*/

#endif /*REMU_OSHAL_EXCEPTION_H*/

