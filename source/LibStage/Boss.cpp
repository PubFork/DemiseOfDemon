#include "Boss.h"
#include "Framework.h"
#include "Party.h"
#include "Player.h"
#include "Client.h"
#include "Enemy.h"
#include "Field.h"
#include "Network.h"
#include "Stage.h"
#include "Sound.h"
#include "DxLib.h"

static const double LIFE = 50000;
static const int ATTACK_EFFECT_TIME = 20; 
static const int SOUND_CLEAVE_TIME = 100;
static const int SOUND_ROAR_TIME = 200;
static const int SOUND_LOWHIT_TIME = 150;
static const int EFFECT_MAGIC_TIME = 350;

const double TIME[ Boss::MAX_MOTION ] = {
	0, //MOTION_HIDE,
	338, //MOTION_WAIT,
	750, //MOTION_APPEAR,
	350, //MOTION_EXIT,
	352, //MOTION_ATTACK_CLEAVE,
	390, //MOTION_ATTACK_LOWHIT,
	762, //MOTION_ATTACK_MAGIC,
};

static double g_life = LIFE;

PTR( EnemyBoss );

class EnemyBoss : public Enemy {
public:
	EnemyBoss( const Vector& pos ) : Enemy( BOSS, 0, 0, 10000, pos ) { }
	virtual ~EnemyBoss( ) { }
public:
	void act( ) { }
private:
	double getTime( MOTION motion ) const { return 0; }
	virtual void damage( double power ) { g_life -= power; }
	virtual double getHeight( ) const { return 4; }
};

BossPtr Boss::getTask( ) {
	FrameworkPtr fw = Framework::getInstance( );
	return std::dynamic_pointer_cast< Boss >( fw->getTask( Boss::getTag( ) ) );
}

Boss::Boss(  ) {
	_dir = Vector( 0, 0, -1 );
	_time = 0;
	_motion = MOTION_APPEAR;
	_init = true;
	_magic_actuation_time = 0;
}

Boss::~Boss( ) {
}

double Boss::getTime( ) {
	return _time;
}

Boss::MOTION Boss::getMotion( ) {
	return _motion;
}

Vector Boss::getDir( ) {
	return _dir;
}

void Boss::update( ) {

	_time_store = _time;
	_time += 1;

	switch ( _motion ) {
	case MOTION_HIDE:
		{
			_time = 0;
		}
		break;
	case MOTION_WAIT:
		if ( _time > TIME[ _motion ] ) {
			_time = 0;
			switch ( GetRand( 2 ) ) {
			case 0: _motion = MOTION_ATTACK_CLEAVE; break;
			case 1: _motion = MOTION_ATTACK_LOWHIT; break;
			case 2: _motion = MOTION_ATTACK_MAGIC; break;
			}
		}
		if ( g_life <= 0 ) {
			_motion = MOTION_EXIT;
		}
		break;
	case MOTION_EXIT:
		if ( _time > TIME[ _motion ] ) {
			_motion = MOTION_HIDE;
			_time = 0;
		} 
		break;
	case MOTION_APPEAR:
		if ( _time > TIME[ _motion ] ) {
			_motion = MOTION_WAIT;
			_time = 0;
		} 

		break;
	case MOTION_ATTACK_CLEAVE:
		if ( _time > TIME[ _motion ] ) {
			_motion = MOTION_WAIT;
			_time = 0;
		} 
		break;
	case MOTION_ATTACK_LOWHIT:
		if ( _time > TIME[ _motion ] ) {
			_motion = MOTION_WAIT;
			_time = 0;
		} 
		break;
	case MOTION_ATTACK_MAGIC:
		/*StagePtr stage = Stage::getTask( );
		SoundPtr sound = stage->getSound( );
		sound->play( Sound::SOUND_BOSS_CHANT );*/
		if ( _time > TIME[ _motion ] ) {
			_motion = MOTION_WAIT;
			_time = 0;
		} 
		break;
	}
}

bool Boss::isPassedTime( double time ) {
	return _time >= time && _time_store < time;	
}