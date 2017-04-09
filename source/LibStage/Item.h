#pragma once

#include "Object.h"
#include "mathmatics.h"

PTR( Item );

class Item : public Object {
public:
	Item( );
	virtual ~Item();
public:
	virtual bool pickup( int index ) = 0;
	bool isExpired( );
protected:
	void delMyself( );
private:
	bool _expired;
};