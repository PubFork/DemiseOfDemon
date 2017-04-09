#include "EnemyGhost.h"
#include "Framework.h"
#include "BulletMissile.h"
#include "Weapon.h"
#include "Player.h"
#include "Stage.h"
#include "Experiences.h"

const double TIME[ Enemy::MAX_MOTION ] = {
	50, //MOTION_WAIT,
	12, //MOTION_WALK,
	12, //MOTION_FALL,
   100, //MOTION_DAMAGE,
	70, //MOTION_DEAD,
	12, //MOTION_JUMP,
	90, //MOTION_ATTACK,
};

EnemyGhost::EnemyGhost( const Vector& pos )
: Enemy(
	Enemy::GHOST, //ゴーストに変更
	Framework::getInstance( )->getSettingValue( "ENEMY_GHOST_SPEED" ),
	Framework::getInstance( )->getSettingValue( "ENEMY_GHOST_RANGE" ),
	Framework::getInstance( )->getSettingValue( "ENEMY_GHOST_LIFE" ),
	pos )
, _max_cooltime( ( int )Framework::getInstance( )->getSettingValue( "ENEMY_GHOST_COOLTIME" ) ) {
}

EnemyGhost::~EnemyGhost( ) {
}


double EnemyGhost::getTime( MOTION motion ) const {
	return TIME[ motion  ];
}

void EnemyGhost::act( ) {
	_cooltime--;

	if ( _cooltime < 0 ) {
		searchTarget( );
		moveToTarget( );
		attackToTarget( );
	}
}

void EnemyGhost::onAttack( ) {
	PlayerPtr target = getTarget( );
	if ( !target ) {
		return;
	}
	
	Vector pos = getPos( ) + Vector( 0, 1.0, 0 );
	Vector dir = target->getPos( ) + Vector( 0, 1, 0 ) - pos;
	setDir( dir.x, dir.z );

	if ( isPassedTime( 40 ) ) {
		_cooltime = _max_cooltime;
		BulletPtr bullet = BulletMissilePtr( new BulletMissile( pos + dir.normalize( ), dir ) );
		StagePtr stage = Stage::getTask( );
		WeaponPtr weapon =  stage->getWeapon( );
		weapon->add( bullet );
	}
}

void EnemyGhost::onExp( )  {
	StagePtr stage = Stage::getTask( );
	ExperiencesPtr experiences = stage->getExperiences( );
	Vector vec = Vector( getDirX( ), 0.0, getDirZ( ) ).normalize( ) * -4;
	Vector pos = getPos( ) + vec;
	FrameworkPtr fw = Framework::getInstance( );
	int exp_num = ( int )fw->getSettingValue( "ENEMY_GHOST_POP_EXP" );
	experiences->create( pos , exp_num );
}