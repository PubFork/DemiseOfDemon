#pragma once

#include "Bullet.h"
#include "mathmatics.h"

PTR( BulletSword );

class BulletSword : public Bullet { 
public:
	static void initialize( );
public:
	BulletSword( const Vector& pos, double dir_x, double dir_z, int level );
	virtual ~BulletSword( );
public:
	virtual bool update( );
public:
	Vector getPos( ) const;
	Vector getBeginDir( ) const;
	Vector getEndDir( ) const;
	double getLength( ) const;
private:
	const Vector _pos;
	const Vector _dir;
	const int _level;
private:
	double _ratio;
	static double _power;
	static double _speed;
};
