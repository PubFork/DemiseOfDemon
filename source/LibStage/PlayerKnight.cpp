#include "PlayerKnight.h"
#include "Player.h"
#include "Weapon.h"
#include "Network.h"
#include "BulletSword.h"
#include "BulletNova.h"
#include "BulletShot.h"
#include "BulletLay.h"
#include "BulletArrow.h"
#include "Stage.h"
#include "Sound.h"
#include "Effect.h"
#include "mathmatics.h"

const double ATTACK_TIME = 50;
const double BEGIN_TIME = 0;
const double END_TIME = 50;

PlayerKnight::PlayerKnight( Vector pos )
: Player( PLAYER_KNIGHT, pos ){
	
}

PlayerKnight::~PlayerKnight( ) {
}


void PlayerKnight::attack( ) {
	StagePtr stage = Stage::getTask( );
	if ( isPassedTime( 15 ) ) {
		WeaponPtr weapon = stage->getWeapon( );
		SoundPtr sound = stage->getSound( );
		sound->play( Sound::SOUND_PLAYER_SWORD );
		BulletPtr bullet = BulletSwordPtr( new BulletSword( getPos( ) +  Vector( 0, 0.5, 0 ), getDirX( ), getDirZ( ), _weapon_level ) );
		weapon->add( bullet );

		
	}
	
}

double PlayerKnight::getAttackTimeBegin( ) {
	return BEGIN_TIME;
}

double  PlayerKnight::getAttackTimeEnd( ) {
	return END_TIME;
}

double PlayerKnight::getAttackTotalTime( ) {
	return ATTACK_TIME;
}