#include "Log.h"
#include "Framework.h"
#include <stdarg.h>

LogPtr Log::getTask( ) {
	FrameworkPtr fw = Framework::getInstance( );
	return std::dynamic_pointer_cast< Log >( fw->getTask( Log::getTag( ) ) );
}

Log::Log( ) {
}

Log::~Log( ) {
}

void Log::update( ) {
	// なにもしない
}

std::string Log::get( int index ) {
	return _message[ index ];
}

void Log::send( const char * format, ... ) {
	// スクロール
	for ( int i = 0; i < MESSAGE_COUNT - 1; i++ ) {
		_message[ i ] = _message[ i + 1 ];
	}

	// 文字列を加える
	va_list args;
	va_start( args, format );
	char str[ 255 ];
	vsprintf_s( str, 255, format, args );
	_message[ MESSAGE_COUNT - 1 ] = str;
}