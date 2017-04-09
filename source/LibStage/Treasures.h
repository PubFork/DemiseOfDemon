#pragma once

#include "smart_ptr.h"
#include "mathmatics.h"
#include <string>

PTR( Treasures );
PTR( Treasure )

class Treasures {
public:
	Treasures();
	virtual ~Treasures();
public:
	virtual void update( );
	TreasureConstPtr getTreasure( int index ) const;
	void init( );
	TreasurePtr getTreasure( int index );
	void add( TreasurePtr treasure );
	void create( const Vector& pos );
public:
	static const int MAX_NUM = 200;
private:
	TreasurePtr _treasure[ MAX_NUM ];
};

