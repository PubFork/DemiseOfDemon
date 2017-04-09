#pragma once

#include "smart_ptr.h"

PTR( ViewerWeapon );
PTR( ViewerSword );
PTR( ViewerNova );
PTR( ViewerShot );
PTR( ViewerLay );
PTR( ViewerArrow );
PTR( ViewerImpact );
PTR( ViewerMissile );

class ViewerWeapon {
public:
	ViewerWeapon( );
	virtual ~ViewerWeapon( );
public:
	void draw( );
private:
	ViewerSwordPtr _sword;
	ViewerNovaPtr _nova;
	ViewerShotPtr _shot;
	ViewerLayPtr _lay;
	ViewerArrowPtr _arrow;
	ViewerImpactPtr _impact;
	ViewerMissilePtr _missile;
};

