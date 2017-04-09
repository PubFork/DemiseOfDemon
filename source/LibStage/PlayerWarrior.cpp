#include "PlayerWarrior.h"
#include "Weapon.h"
#include "Framework.h"
#include "mathmatics.h"
#include "BulletNova.h"
#include "Stage.h"
#include "Network.h"
#include "Sound.h"

const double ATTACK_TIME = 90;
const double BEGIN_TIME = 0;
const double END_TIME = 90;

PlayerWarrior::PlayerWarrior( Vector pos ) 
: Player( PLAYER_WARRIOR, pos ) {
}


PlayerWarrior::~PlayerWarrior( ) {
}


void PlayerWarrior::attack( ) {
	StagePtr stage = Stage::getTask( );
	WeaponPtr weapon = stage->getWeapon( );
	SoundPtr sound = stage->getSound( );
	if ( isPassedTime( 40 ) ) {
		BulletPtr bullet = BulletNovaPtr( new BulletNova( getPos( ) + Vector( 0, 0.05, 0 ), _weapon_level ) );
		weapon->add( bullet );			
		sound->play( Sound::SOUND_PLAYER_NOVA );
	}
}

double PlayerWarrior::getAttackTimeBegin( ) {
	return BEGIN_TIME;
}

double PlayerWarrior::getAttackTimeEnd( ) {
	return END_TIME;
}

double PlayerWarrior::getAttackTotalTime( ) {
	return ATTACK_TIME;
}