#pragma once

#include "smart_ptr.h"
#include <string>

PTR( Deedboxes );
PTR( Deedbox );
PTR( Map );

class Deedboxes {
public:
	static const int MAX_NUM = 100;
public:
	Deedboxes( );
	virtual ~Deedboxes();
public:
	virtual void update( );
public:
	DeedboxConstPtr getDeedbox( int index ) const;
	int getNum( ) const;
public:
	void init( );
	DeedboxPtr getDeedbox( int index );
private:
	DeedboxPtr _deedbox[ MAX_NUM ];
	int _num;
};
