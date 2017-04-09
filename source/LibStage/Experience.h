#pragma once
#include "smart_ptr.h"
#include "mathmatics.h"

PTR( Experience );
PTR( Party );

class Experience {
public:
	Experience( const Vector& pos );
	virtual ~Experience( );
public:
	Vector getPos( ) const;
	bool isExpired( );
	void update( PartyPtr data );
private:
	void delMyself( );
private:
	Vector _pos;
	bool _expired;
};

