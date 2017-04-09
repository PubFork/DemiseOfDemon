#include "Enemy.h"
#include "Camera.h"
#include "Party.h"
#include "Player.h"
#include "Field.h"
#include "Stage.h"
#include "Experiences.h"
#include "Effect.h"

const double TIME_SPEED = 40.0 / 60.0;
const double HEIGHT = 2.0;
const int RESEARCH_TIME = 180;

Enemy::Enemy( TYPE type, double speed, double range, double life, const Vector& pos )
: Character( pos )
, _type( type )
, _speed( speed )
, _range( range ) {
	_time = 0;
	_expired = true;
	_life = life;
	_research_count = 0;
}

Enemy::~Enemy( ) {
}

bool Enemy::isExpired( ) const {
	return _expired;
}

void Enemy::damage( double power ) {
	_life -= power * _endurance;

	// エフェクトサンプル
	StagePtr stage = Stage::getTask( );
	EffectPtr effect = stage->getEffect( );
	effect->play( Effect::BMVOL2_HIT01, getPos( ) + Vector( 0, 1.0, 0 ), 0.1f );
}

Enemy::MOTION Enemy::getMotion( ) const {
	return _motion;
}

double Enemy::getTime( ) const {
	return _time;
}

Enemy::TYPE Enemy::getType( ) const {
	return _type;
}

void Enemy::onAttack( ) {
}

void Enemy::onJump( ) {

}

void Enemy::updateDerive( ) {
	if ( getPos( ).y <= 1 ) {
		_expired = false;
	}

	if ( _life <= 0 && getAction( ) != Character::ACTION_DEAD ) {
		setAction( Character::ACTION_DEAD );
		Vector pos = getPos( );
		StagePtr stage = Stage::getTask( );
		FieldPtr field = stage->getField( );
		field->delTarget( ( int )pos.x, ( int )pos.z, getThis( ) );
		
	}
	
	_time_store = _time;
	_time += TIME_SPEED;

	switch ( getAction( ) ) {
	case Character::ACTION_WAIT:
		if ( _motion != MOTION_WAIT ) {
			_motion = MOTION_WAIT;
			_time = 0;
		}
		break;
	case Character::ACTION_FALL:
		if ( _motion != MOTION_FALL ) {
			_motion = MOTION_FALL;
			//ミノのみなので変更
			_time = getTime( _motion );
		}
		break;
	case Character::ACTION_WALK:
		adjustDir( );
		if ( _motion != MOTION_WALK ) {
			_motion = MOTION_WALK;
			_time = 0;
		}
		break;
	case Character::ACTION_ATTACK:
		if ( _motion != MOTION_ATTACK ) {
			_motion = MOTION_ATTACK;
			_time = 0;
		}
		break;
	case Character::ACTION_DEAD:
		{
			if ( _motion != MOTION_DEAD ) {
				_motion = MOTION_DEAD;
				_time = 0;
			}
		}
		break;
	default:
		_motion = MOTION_WAIT;
		break;
	}
	
	switch ( _motion ) {
	case MOTION_WAIT:
	case MOTION_WALK:
		break;
	case MOTION_FALL:
		if ( _time > getTime( _motion ) ) {
			_time = getTime( _motion );
		}
		break;
	case MOTION_ATTACK:
		if ( _time > getTime( _motion ) ) {
			_time = 0;
			finishAttack( );
		} 
		break;
	case MOTION_DAMAGE:
		if ( _time > getTime( _motion ) ) {
			_time = getTime( _motion );
		}
		break;
	case MOTION_JUMP:
		if ( _time > getTime( _motion ) ) {
			_time = getTime( _motion );
		}
		break;
	case MOTION_DEAD:
		if ( _time > getTime( _motion ) ) {
			_time = getTime( _motion );
			_expired = false;
			onExp( );
		}
		break;
	}
}

bool Enemy::isPassedTime( double time ) {
	return _time >= time && _time_store < time;	
}

double Enemy::getHeight( ) const {
	return HEIGHT;
}

PlayerPtr Enemy::getTarget( ) {
	return _target.lock( );
}

void Enemy::searchTarget( ) {
	_research_count--;

	// ターゲット済みか？
	PlayerPtr target = _target.lock( );
	if ( target || _research_count > 0 ) {
		return;
	}
	_research_count = RESEARCH_TIME;

	// ターゲットを探す
	double min_length = 1000;
	StagePtr stage = Stage::getTask( );
	PartyPtr party = stage->getParty( );
	for ( int i = 0; i < PLAYER_NUM; i++ ) {
		PlayerPtr player = party->getPlayer( i );
		if ( player ) {
			Vector vec = player->getPos( ) - getPos( );
			double length = vec.getLength( );
			if ( length < min_length ) {
				min_length = length;
				_target = player;
			}
		}
	}
}

void Enemy::moveToTarget( bool jump  ) {
	PlayerPtr target = _target.lock( );
	if ( !target ) {
		move( 0, 0 );
		return;
	}

	Vector pos = getPos( );
	Vector vec = target->getPos( ) - pos;
	vec.y = 0;
	vec = vec.normalize( );
	Vector dir = vec * _speed;
	move( dir.x, dir.z );

	StagePtr stage = Stage::getTask( );
	FieldPtr field = stage->getField( );
	double height_here = field->getHeight( ( int )pos.x, ( int )pos.z );
	double height_dst  = field->getHeight( ( int )( pos.x + vec.x ), ( int )( pos.z + vec.z ) );
	double height = height_dst - height_here;
	if ( jump && height > 0 && height < 3 ) {
		setJump( );
	}
}
double Enemy::getJumpPower( ) {
	return 0.8;
}

void Enemy::attackToTarget( ) {
	PlayerPtr target = _target.lock( );
	if ( !target ) {
		return;
	}
	Vector vec = target->getPos( ) - getPos( );
	double length = vec.getLength( );
	if ( length < _range ) {
		move( 0, 0 );
		setAttack( );
	}
}

bool Enemy::knockback( const Vector& dir ) {
	if ( _type == CYCLOPS ||
		 _type == BOSS ||
		 _type == MINOTAUR) {
		return false;
	}

	return Character::knockback( dir );
}