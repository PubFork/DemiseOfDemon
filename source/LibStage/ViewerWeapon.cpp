#include "ViewerWeapon.h"
#include "ViewerSword.h"
#include "ViewerNova.h"
#include "ViewerShot.h"
#include "ViewerLay.h"
#include "ViewerArrow.h"
#include "ViewerImpact.h"
#include "ViewerMissile.h"
#include "Weapon.h"
#include "Bullet.h"
#include "Stage.h"

ViewerWeapon::ViewerWeapon( ) {
	_sword = ViewerSwordPtr( new ViewerSword );
	_nova  = ViewerNovaPtr( new ViewerNova );
	_shot  = ViewerShotPtr( new ViewerShot );
	_lay = ViewerLayPtr( new ViewerLay );
	_arrow = ViewerArrowPtr( new ViewerArrow );
	_impact = ViewerImpactPtr( new ViewerImpact );
	_missile = ViewerMissilePtr( new ViewerMissile );
}


ViewerWeapon::~ViewerWeapon( ) {
}

void ViewerWeapon::draw( ) {
	// •`‰æî•ñİ’è
	StagePtr stage = Stage::getTask( );
	WeaponPtr weapon = stage->getWeapon( );
	std::list< BulletPtr >::iterator it = weapon->_bullet.begin( );
	while ( it != weapon->_bullet.end( ) ) {
		BulletPtr bullet = *it;
		switch ( bullet->getType( ) ) {
		case Bullet::SWORD:
			_sword->create( bullet );
			break;
		case Bullet::SHOT:
			_shot->create( bullet );
			break;
		case Bullet::LAY:
			_lay->create( bullet );
			break;
		case Bullet::ARROW:
			_arrow->create( bullet );
			break;
		case Bullet::NOVA:
			_nova->create( bullet );
			break;
		case Bullet::IMPACT:
			_impact->create( bullet );
			break;
		case Bullet::MISSILE:
			_missile->create( bullet );
			break;
		}
		it++;
	}

	// LayƒAƒjƒ[ƒVƒ‡ƒ“
	ViewerLay::updateScroll( );

	// •`‰æ
	_sword->draw( );
	_nova->draw( );
	_shot->draw( );
	_lay->draw( );
	_impact->draw( );
	_arrow->draw( );
	_missile->draw( );
}

