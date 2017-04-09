#pragma once

#include "Bullet.h"
#include "mathmatics.h"

PTR( BulletMissile );

class BulletMissile : public Bullet {
public:
	static void initialize( );
public:
	BulletMissile( const Vector& pos, const Vector& dir );
	virtual ~BulletMissile( );
public:
	virtual bool update( );
public:
	Vector getPos( ) const;
private:
	const Vector _dir;
private:
	static int _power;
	static double _speed;
	Vector _pos;
};
