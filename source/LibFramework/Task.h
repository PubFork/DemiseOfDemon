#pragma once

#include "smart_ptr.h"
#include <string>

PTR( Task );

class Task {
public:
	Task( );
	virtual ~Task( );
public:
	virtual void update( ) = 0;
};

