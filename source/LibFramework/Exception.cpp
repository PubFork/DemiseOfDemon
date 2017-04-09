#include "Exception.h"

#include <stdio.h>
#include <stdarg.h>

void exception( const char * fmt, ... ) {

	char str[ 1024 ];

    va_list arg;

    va_start( arg, fmt );
    vsprintf_s( str, 1024, fmt, arg );
    va_end( arg );

	throw ( Exception( str ) );
}
