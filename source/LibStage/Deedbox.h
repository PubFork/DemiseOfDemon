#pragma once

#include "Item.h"
#include "mathmatics.h"

PTR( Deedbox );

class Deedbox : public Item {
public:
	Deedbox( int x, int y, int z );
	virtual ~Deedbox();
public:
	int getX( ) const;
	int getY( ) const;
	int getZ( ) const;
	double getTime( ) const;
	void update( );
	virtual bool pickup( int index );
private:
	enum PHASE {
		PHASE_CLOSE,
		PHASE_UNLOCK,
		PHASE_OPEN,
		PHASE_NONE,
	};
private:
	const int _x;
	const int _y;
	const int _z;
	double _time;
	PHASE _phase;
	int _count;
	int _index;
};

