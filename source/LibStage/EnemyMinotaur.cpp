#include "EnemyMinotaur.h"
#include "Framework.h"
#include "Player.h"
#include "Client.h"
#include "Stage.h"
#include "Effect.h"
#include "Sound.h"
#include "Experiences.h"

const double TIME[ Enemy::MAX_MOTION ] = {
	60, //MOTION_WAIT,
	48, //MOTION_WALK,
	47, //MOTION_DASH,
	76, //MOTION_DAMAGE,
	66, //MOTION_DEAD,
	68, //MOTION_CLEAVE,
	100, //MOTION_SMASH,
};

EnemyMinotaur::EnemyMinotaur( const Vector& pos )
: Enemy( 
	Enemy::MINOTAUR, 
	Framework::getInstance( )->getSettingValue( "ENEMY_MINOTAUR_SPEED" ),
	Framework::getInstance( )->getSettingValue( "ENEMY_MINOTAUR_RANGE" ),
	Framework::getInstance( )->getSettingValue( "ENEMY_MINOTAUR_LIFE" ),
	pos )
, _power( ( int )Framework::getInstance( )->getSettingValue( "ENEMY_MINOTAUR_POWER" ) ) {
	_is_stone = true;
}

EnemyMinotaur::~EnemyMinotaur( ) {
}

double EnemyMinotaur::getTime( MOTION motion ) const {
	return TIME[ motion  ];
}

void EnemyMinotaur::act( ) {
	if ( !_is_stone ) {
		searchTarget( );
		moveToTarget( );
		attackToTarget( );
	}
	if ( _is_stone ) {
		setAction( ACTION_FALL );
	}
}

void EnemyMinotaur::onAttack( ) {
	PlayerPtr target = getTarget( );
	if ( !target ) {
		return;
	}

	if ( isPassedTime( 50 ) ) {
		ClientPtr client = Client::getTask( );
		StagePtr stage = Stage::getTask( );
		EffectPtr effect = stage->getEffect( );
		SoundPtr sound = stage->getSound( );
		sound->play( Sound::SOUND_PLAYER_DAMAGE );
		effect->play( Effect::BMVOL2_HIT10, target->getPos( ) + Vector( 0, 1.0, 0 ), 0.1f );
		client->damage(target->getIndex(), _power);
	}
}

void EnemyMinotaur::onExp( )  {
	StagePtr stage = Stage::getTask( );
	ExperiencesPtr experiences = stage->getExperiences( );
	Vector vec = Vector( getDirX( ), 0.0, getDirZ( ) ).normalize( ) * -4;
	Vector pos = getPos( ) + vec;
	FrameworkPtr fw = Framework::getInstance( );
	int exp_num = ( int )fw->getSettingValue( "ENEMY_MINOTAUR_POP_EXP" );
	experiences->create( pos , exp_num );
}

void EnemyMinotaur::setStone( bool stoneFlag ) {
	_is_stone = stoneFlag;
}