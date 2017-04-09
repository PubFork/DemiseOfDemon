#include "PlayerMage.h"
#include "mathmatics.h"
#include "Weapon.h"
#include "Framework.h"
#include "mathmatics.h"
#include "Network.h"
#include "BulletLay.h"
#include "Stage.h"
#include "Sound.h"
#include "Effect.h"

const double LAY_PITCH = 0.3;
const int EFFECT_LAY_RUN_COUNT = 15;
const double ATTACK_TIME = 60;
const double BEGIN_TIME = 24;
const double END_TIME = 33;


PlayerMage::PlayerMage( Vector pos )
: Player( PLAYER_MAGE, pos ){
	_bullet_lay_shot_num = 0;
}

PlayerMage::~PlayerMage( ) {
}

void PlayerMage::attack( ) {
	
	if ( _time >= BEGIN_TIME && _time < END_TIME ) {
		adjustDir( );
		_bullet_lay_shot_num++;
		StagePtr stage = Stage::getTask( );
		Vector dir = Vector( getDirX( ), 0, getDirZ( ) ).normalize( );
		Vector pos = getPos( ) + Vector( 0, 0.85, 0 ) + dir;
		
		WeaponPtr weapon = stage->getWeapon( );
		Matrix mat = Matrix::makeTransformRotation( Vector( 0, -1, 0 ), PI / 2 );
		pos += mat.multiply( dir * 0.35 );
		if ( _bullet_lay_shot_num % EFFECT_LAY_RUN_COUNT == 0 ) {
			// 発射エフェクト
			EffectPtr effect = stage->getEffect( );
			effect->play( Effect::GEM_ALWAYS_KS01, pos + Vector( 0, 0, 0 ), 0.35f, 25 );
		}
		BulletPtr bullet = BulletLayPtr( new BulletLay( pos, dir, _weapon_level ) );
		weapon->add( bullet );
	} else {
		// レイの発射数を初期化
		_bullet_lay_shot_num = 0;
	}
}

double PlayerMage::getAttackTimeBegin( ) {
	return BEGIN_TIME;
}

double PlayerMage::getAttackTimeEnd( ) {
	return END_TIME;
}

double PlayerMage::getAttackTotalTime( ) {
	return ATTACK_TIME;
}