#pragma once

#include "Bullet.h"
#include "mathmatics.h"

PTR( BulletImpact );

class BulletImpact : public Bullet {
public:
	static void initialize( );
public:
	BulletImpact( const Vector& pos, int level );
	virtual ~BulletImpact( );
public:
	virtual bool update( );
public:
	Vector getPos( ) const;
	double getLength( ) const;
	int getAlpha( ) const;
private:
	const int _level;
	Vector _pos;
	int _count;
};
