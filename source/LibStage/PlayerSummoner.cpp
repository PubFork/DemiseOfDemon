#include "PlayerSummoner.h"
#include "Framework.h"
#include "Network.h"
#include "Weapon.h"
#include "mathmatics.h"
#include "BulletArrow.h"
#include "Stage.h"
#include "Sound.h"

const double ATTACK_TIME = 60;
const double BEGIN_TIME = 19;
const double END_TIME = 41;

PlayerSummoner::PlayerSummoner( Vector pos )
: Player( PLAYER_SUMMONER, pos ) {
}


PlayerSummoner::~PlayerSummoner( ) {
}

void PlayerSummoner::attack( ) {

	if ( !isPassedTime( 20 ) ) {
		return;
	}

	StagePtr stage = Stage::getTask( );
	WeaponPtr weapon = stage->getWeapon( );
	SoundPtr sound = stage->getSound( );
	Vector vec( 6 / 4.0, 0, 0 );
	Matrix mat = Matrix::makeTransformRotation( Vector( 0, 1, 0 ), PI2 / ( _weapon_level + 2 ) ); 

	/*if ( _cooltime <= 0 && _time > 40 ) {
		_cooltime = _arrow_cooltime;
		Vector vec( ( 7 -( _weapon_level + 1 ) ) / 4.0, 0, 0 );
		Matrix mat = Matrix::makeTransformRotation( Vector( 0, 1, 0 ), PI2 / ( _weapon_level + 1 ) ); 
		
		bool se_arrow = false;
		for ( int i = 0; i < ( _weapon_level + 1 ); i++ ) {
			vec = mat.multiply( vec );
			BulletPtr bullet = BulletArrowPtr( new BulletArrow( getPos( ), vec ) );
			weapon->add( bullet );
			se_arrow = true;
		}*/
	sound->play( Sound::SOUND_PLAYER_ARROW );
	bool se_arrow = false;
	for ( int i = 0; i < ( _weapon_level + 2 ); i++ ) {
		vec = mat.multiply( vec );
		BulletPtr bullet = BulletArrowPtr( new BulletArrow( getPos( ) + Vector( 0, 1, 0 ), vec ) );
		weapon->add( bullet );
		se_arrow = true;
	}
}

double PlayerSummoner::getAttackTimeBegin( ) {
	return BEGIN_TIME;
}

double PlayerSummoner::getAttackTimeEnd( ) {
	return END_TIME;
}

double PlayerSummoner::getAttackTotalTime( ) {
	return ATTACK_TIME;
}