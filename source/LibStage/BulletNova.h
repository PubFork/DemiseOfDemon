#pragma once

#include "Bullet.h"
#include "mathmatics.h"
#include <array>

PTR( BulletNova );

class BulletNova : public Bullet {
public:
	static void initialize( );
public:
	static const int DIV = 50;
public:
	BulletNova( const Vector& pos, int level );
	virtual ~BulletNova( );
public:
	virtual bool update( );
public:
	Vector getPos( ) const;
	double getLength( ) const;
	bool isExpired( int num );
private:
	const Vector _pos;
	std::array< bool, DIV > _expired;
	const int _level;
private:
	double _ratio;
	static double _power;
	static double _speed;
};
