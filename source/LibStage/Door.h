#pragma once

#include "Item.h"
#include "mathmatics.h"

PTR( Door );

class Door : public Item {
public:
	enum DIR {
		DIR_PZ,
		DIR_MZ,
		DIR_PX,
		DIR_MX,
	};
public:
	Door( int x, int y, int z, DIR dir );
	virtual ~Door();
public:
	int getX( ) const;
	int getY( ) const;
	int getZ( ) const;
	double getTime( ) const;
	DIR getDir( ) const;
	void update( );
	virtual bool pickup( int index );
	bool isOpening( ) const;
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
	const DIR _dir;
	double _time;
	PHASE _phase;
};

