#pragma once

#include "Item.h"

PTR( Treasure );

class Treasure : public Item {
public:
	enum TYPE {
		LEVEL_UP,
		POTION,
		POTION_BIG,
		MAX_TYPE,
	};
public:
	Treasure( const Vector& pos, TYPE type );
	virtual ~Treasure();
public:
	virtual bool pickup( int index );
	void update( );
	TYPE getType( ) const;
	Vector getPos( ) const;
private:
	const TYPE _type;
	Vector _pos;
	double _vec_y;
	bool _standing;
	int _heal_life;
	int _heal_life_big;
};

