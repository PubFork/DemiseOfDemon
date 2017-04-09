#include "PlayerShooter.h"
#include "Network.h"
#include "Weapon.h"
#include "mathmatics.h"
#include "BulletShot.h"
#include "Stage.h"
#include "Sound.h"

const double ATTACK_TIME = 60;
const double BEGIN_TIME = 20;
const double END_TIME = 44;

PlayerShooter::PlayerShooter( Vector pos )
: Player( PLAYER_SHOOTER, pos ){
}

PlayerShooter::~PlayerShooter( ) {
}

void PlayerShooter::attack( ) {
	StagePtr stage = Stage::getTask( );
	WeaponPtr weapon = stage->getWeapon( );
	SoundPtr sound = stage->getSound( );

	if ( isPassedTime( 21 ) ) {
		int num = _weapon_level + 3;
		sound->play( Sound::SOUND_PLAYER_SHOT );
		for ( int i = 0; i < num; i++ ) {
			Matrix mat = Matrix::makeTransformRotation( Vector( 0, 1, 0 ), PI / 60 * ( i - ( num - 1 ) / 2.0 ) );
			Vector dir = mat.multiply( Vector( getDirX( ), 0, getDirZ( ) ) ).normalize( );
			BulletPtr bullet = BulletShotPtr( new BulletShot( getPos( ) + dir * 2.0 + Vector( 0, 1.0, 0 ), dir ) );
			weapon->add( bullet );
		}
	}
	
}

double PlayerShooter::getAttackTimeBegin( ) {
	return BEGIN_TIME;
}

double PlayerShooter::getAttackTimeEnd( ) {
	return END_TIME;
}

double PlayerShooter::getAttackTotalTime( ) {
	return ATTACK_TIME;
}