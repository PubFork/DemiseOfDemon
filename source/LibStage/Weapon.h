#pragma once

#include "smart_ptr.h"
#include <list>

PTR( Weapon );
PTR( Bullet );

class Weapon {
friend class ViewerWeapon;
public:
	Weapon( );
	virtual ~Weapon( );
public:
	virtual void update( );
public:
	void add( BulletPtr bullet );
private:
	std::list< BulletPtr > _bullet;
};

