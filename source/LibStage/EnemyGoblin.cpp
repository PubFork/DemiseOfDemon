#include "EnemyGoblin.h"
#include "Framework.h"
#include "Player.h"
#include "Client.h"
#include "Stage.h"
#include "Effect.h"
#include "Sound.h"
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

EnemyGoblin::EnemyGoblin( const Vector& pos )
: Enemy( 
	Enemy::GOBLIN, 
	Framework::getInstance( )->getSettingValue( "ENEMY_GOBLIN_SPEED" ),
	Framework::getInstance( )->getSettingValue( "ENEMY_GOBLIN_RANGE" ),
	Framework::getInstance( )->getSettingValue( "ENEMY_GOBLIN_LIFE" ),
	pos )
, _power( ( int )Framework::getInstance( )->getSettingValue( "ENEMY_GOBLIN_POWER" ) ) {

}

EnemyGoblin::~EnemyGoblin( ) {
}

double EnemyGoblin::getTime( MOTION motion ) const {
	return TIME[ motion  ];
}

void EnemyGoblin::act( ) {
	searchTarget( );
	moveToTarget( );
	attackToTarget( );
}

void EnemyGoblin::onAttack( ) {
	PlayerPtr target = getTarget( );
	if ( !target ) {
		return;
	}

	if ( isPassedTime( 10 ) ) {
		ClientPtr client = Client::getTask( );
		StagePtr stage = Stage::getTask( );
		EffectPtr effect = stage->getEffect( );
		SoundPtr sound = stage->getSound( );
		sound->play( Sound::SOUND_PLAYER_DAMAGE );
		effect->play( Effect::BMVOL2_HIT05, target->getPos( ) + Vector( 0, 1.0, 0 ), 0.5 );
		client->damage(target->getIndex(), _power);
	}
}

void EnemyGoblin::onExp( )  {
	StagePtr stage = Stage::getTask( );
	ExperiencesPtr experiences = stage->getExperiences( );
	Vector vec = Vector( getDirX( ), 0.0, getDirZ( ) ).normalize( ) * -4;
	Vector pos = getPos( ) + vec;
	FrameworkPtr fw = Framework::getInstance( );
	int exp_num = ( int )fw->getSettingValue( "ENEMY_GOBLIN_POP_EXP" );
	experiences->create( pos , exp_num );
}