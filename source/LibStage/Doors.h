#pragma once
#include "smart_ptr.h"
#include <string>

PTR( Doors );
PTR( Door );

class Doors {
public:
	static const int MAX_NUM = 100;
public:
	Doors( );
	virtual ~Doors();
public:
	virtual void update( );
public:
	DoorConstPtr getDoor( int index ) const;
	int getNum( ) const;
	bool isOpening( ) const;
public:
	void init( );
	DoorPtr getDoor( int index );
private:
	DoorPtr _door[ MAX_NUM ];
	int _num;
};
