#include "EnemyArmor.h"
#include "Framework.h"
#include "Player.h"
#include "Client.h"
#include "Stage.h"
#include "Experiences.h"
#include "Effect.h"
#include "Sound.h"

const double TIME[ Enemy::MAX_MOTION ] = {
	50, //MOTION_WAIT,
	32, //MOTION_WALK,
	32, //MOTION_FALL,
	54, //MOTION_DAMAGE,
	70, //MOTION_DEAD,
	32, //MOTION_JUMP,
	87, //MOTION_ATTACK,
};

EnemyArmor::EnemyArmor( const Vector& pos )
: Enemy( 
	Enemy::ARMOR, 
	Framework::getInstance( )->getSettingValue( "ENEMY_ARMOR_SPEED" ),
	Framework::getInstance( )->getSettingValue( "ENEMY_ARMOR_RANGE" ),
	Framework::getInstance( )->getSettingValue( "ENEMY_ARMOR_LIFE" ),
	pos )
, _power( ( int )Framework::getInstance( )->getSettingValue( "ENEMY_ARMOR_POWER" ) ) {

}

EnemyArmor::~EnemyArmor( ) {
}

double EnemyArmor::getTime( MOTION motion ) const {
	return TIME[ motion  ];
}

void EnemyArmor::act( ) {
	searchTarget( );
	moveToTarget( );
	attackToTarget( );
}

void EnemyArmor::onAttack( ) {
	PlayerPtr target = getTarget( );
	if ( !target ) {
		return;
	}
	StagePtr stage = Stage::getTask( );
	SoundPtr sound = stage->getSound( );

	EffectPtr effect = stage->getEffect( );
	
	if ( isPassedTime( 45 ) ) {
		ClientPtr client = Client::getTask();
		client->damage(target->getIndex(), _power);
		sound->play( Sound::SOUND_PLAYER_DAMAGE );
		effect->play( Effect::BMVOL2_HIT05, target->getPos( ) + Vector( 0, 1.0, 0 ), 0.3f );
	}
}

void EnemyArmor::onExp( )  {
	StagePtr stage = Stage::getTask( );
	ExperiencesPtr experiences = stage->getExperiences( );
	Vector vec = Vector( getDirX( ), 0.0, getDirZ( ) ).normalize( );
	Vector pos = getPos( ) + vec; 
	FrameworkPtr fw = Framework::getInstance( );
	int exp_num = ( int )fw->getSettingValue( "ENEMY_ARMOR_POP_EXP" );
	experiences->create( pos , exp_num );
}