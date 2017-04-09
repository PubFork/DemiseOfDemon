#include "Weapon.h"
#include "Framework.h"
#include "Bullet.h"
#include "BulletMissile.h"
#include "BulletLay.h"
#include "BulletSword.h"
#include "BulletArrow.h"
#include "BulletShot.h"
#include "BulletNova.h"

Weapon::Weapon( ) {
	BulletMissile::initialize( );
	BulletLay::initialize( );
	BulletSword::initialize( );
	BulletArrow::initialize( );
	BulletShot::initialize( );
	BulletNova::initialize( );
}


Weapon::~Weapon( ) {
}

void Weapon::update( ) {
	std::list< BulletPtr >::iterator it = _bullet.begin( );
	while ( it != _bullet.end( ) ) {
		BulletPtr bullet = *it;
		if ( bullet->update( ) ) {
			it++;
		} else {
			it = _bullet.erase( it );
		}
	}
}

void Weapon::add( BulletPtr bullet ) {
	_bullet.push_back( bullet );
}
