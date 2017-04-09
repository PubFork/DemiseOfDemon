#include "EnemyRedGoblin.h"
#include "Framework.h"
#include "BulletMissile.h"
#include "Weapon.h"
#include "Player.h"
#include "Client.h"
#include "Stage.h"
#include "Sound.h"
#include "Effect.h"
#include "Experiences.h"

const double TIME[ Enemy::MAX_MOTION ] = {
	60, //MOTION_WAIT,
	60, //MOTION_WALK,
	60, //MOTION_FALL,
	34, //MOTION_DAMAGE,
	60, //MOTION_DEAD,
	55, //MOTION_JUMP,
	42, //MOTION_ATTACK,
};

EnemyRedGoblin::EnemyRedGoblin( const Vector& pos )
: Enemy(
	Enemy::REDGOBLIN, 
	Framework::getInstance( )->getSettingValue( "ENEMY_REDGOBLIN_SPEED" ),
	Framework::getInstance( )->getSettingValue( "ENEMY_REDGOBLIN_RANGE" ),
	Framework::getInstance( )->getSettingValue( "ENEMY_REDGOBLIN_LIFE" ),
	pos )
, _power( ( int )Framework::getInstance( )->getSettingValue( "ENEMY_REDGOBLIN_POWER" ) ) {
}

EnemyRedGoblin::~EnemyRedGoblin( ) {
}


double EnemyRedGoblin::getTime( MOTION motion ) const {
	return TIME[ motion  ];
}

void EnemyRedGoblin::act( ) {
	searchTarget( );
	moveToTarget( );
	attackToTarget( );
}

void EnemyRedGoblin::onAttack( ) {
	PlayerPtr target = getTarget( );
	if ( !target ) {
		return;
	}

	if ( isPassedTime( 10 ) ) {
		ClientPtr client = Client::getTask();
		StagePtr stage = Stage::getTask( );
		SoundPtr sound = stage->getSound( );
		sound->play( Sound::SOUND_PLAYER_DAMAGE );
		EffectPtr effect = stage->getEffect( );
		effect->play( Effect::BMVOL2_HIT05, target->getPos( ) + Vector( 0, 1.0, 0 ), 0.3f );
		client->damage(target->getIndex(), _power);
	}
}

void EnemyRedGoblin::onExp( )  {
	StagePtr stage = Stage::getTask( );
	ExperiencesPtr experiences = stage->getExperiences( );
	Vector vec = Vector( getDirX( ), 0.0, getDirZ( ) ).normalize( ) * -4;
	Vector pos = getPos( ) + vec;
	FrameworkPtr fw = Framework::getInstance( );
	int exp_num = ( int )fw->getSettingValue( "ENEMY_REDGOBLIN_POP_EXP" );
	experiences->create( pos , exp_num );
}