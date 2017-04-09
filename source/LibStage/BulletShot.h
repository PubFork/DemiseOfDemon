#pragma once

#include "Bullet.h"
#include "mathmatics.h"

PTR( BulletShot );

class BulletShot : public Bullet {
public:
	static void initialize( );
public:
	BulletShot( const Vector& pos, const Vector& dir );
	virtual ~BulletShot( );
public:
	virtual bool update( );
public:
	Vector getPos( ) const;
	Vector getDir( ) const;
private:
	const Vector _dir;
private:
	Vector _pos;
	static double _power;
	static double _speed;
};
