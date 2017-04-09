#pragma once
#include "mathmatics.h"
#include "smart_ptr.h"

PTR( Bullet );

class Bullet {
public:
	enum TYPE {
		SHOT,
		NOVA,
		ARROW,
		LAY,
		SWORD,
		IMPACT,
		MISSILE,
	};
public:
	Bullet( TYPE type );
	virtual ~Bullet();
public:
	virtual bool update( ) = 0;
public:
	TYPE getType( ) const;
protected:
	bool attackEnemy( const Vector& pos, double power );
	bool isDoor( int x, int z );
private:
	const TYPE _type;
};

