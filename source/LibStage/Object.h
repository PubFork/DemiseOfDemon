#pragma once

#include "smart_ptr.h"

PTR( Object );

class Object : public std::enable_shared_from_this < Object > {
public:
	Object( );
	virtual ~Object( );
protected:
	ObjectPtr getThis( );
};
