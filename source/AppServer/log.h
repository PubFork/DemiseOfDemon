#pragma once

#include "Task.h"
#include <array>
#include <string>

PTR( Log );

class Log : public Task {
public:
	static std::string getTag( ) { return "LOG"; }
	static LogPtr getTask( );
public:
	Log();
	virtual ~Log();
public:
	virtual void update( );
public:
	static const int MESSAGE_COUNT = 10;
public:
	void send( const char * format, ... );
	std::string get( int index );
private:
	std::array< std::string, MESSAGE_COUNT > _message;
};

