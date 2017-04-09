#pragma once

#include <string>

class Exception {
public:
	Exception( std::string message ) {
		_message = message;
	}
	virtual ~Exception( ) { }
	const char * getMessageCPtr( ) const {
		return _message.c_str( );
	}
private:
	std::string _message;
};

void exception( const char * fmt, ... );