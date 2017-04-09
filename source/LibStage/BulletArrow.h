#pragma once

#include "Bullet.h"
#include "mathmatics.h"
#include <array>

PTR( BulletArrow );
PTR( Enemy );

class BulletArrow : public Bullet {
public:
	static void initialize( );
public:
	BulletArrow( const Vector& center, const Vector& pos );
	virtual ~BulletArrow( );
public:
	virtual bool update( );
public:
	Vector getPos( ) const;
private:
private:
	Vector _pos;
	Vector _vec;
	int _count;
	EnemyWeakPtr _target;
	Vector _center;
	static double _power;
	static int _range;
};
