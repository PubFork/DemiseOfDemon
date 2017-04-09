#include "Player.h"
#include "Framework.h"
#include "Camera.h"
#include "Field.h"
#include "Network.h"
#include "Weapon.h"
#include "BulletSword.h"
#include "BulletNova.h"
#include "BulletShot.h"
#include "BulletLay.h"
#include "BulletArrow.h"
#include "Item.h"
#include "Client.h"
#include "Stage.h"
#include "Sound.h"
#include "Effect.h"
#include "Experiences.h"
#include "Experience.h"

const double TIME_SPEED = 1.6;
const double SPEED = 0.2;
const double LAY_PITCH = 0.3;
const double LAY_TIME = 19.5;
const int EFFECT_LAY_RUN_COUNT = 15;

const double TIME[ Player::MAX_MOTION ] = {
	20, // MOTION_WAIT,
	25, // MOTION_WALK,
	50, // MOTION_ATTACK,
	60, // MOTION_USE,
	40, // MOTION_DAMAGE,
	90, // MOTION_DEAD,
	60, // MOTION_JUMP
};

void Player::initialize( ) {
	
}

Player::Player( int index, const Vector& pos )
: Character( pos, true )
, _index( index ) {
	_motion = MOTION_WAIT;
	_time = 0;
	_time_store = 0;
}

Player::~Player( ) {
}

Player::MOTION Player::getMotion( ) const {
	return _motion;
}

double Player::getTime( ) const {
	return _time;
}

int Player::getIndex( ) const {
	return _index;
}

void Player::use( ) {
	setUse( );
}

void Player::input( int dir_x, int dir_y, bool jump, bool attack ) {
	// ˆÚ“®
	StagePtr stage = Stage::getTask( );
	CameraPtr camera = stage->getCamera( );

	Vector camera_vec = camera->getFocus( ) - camera->getEye( );
	camera_vec.y = 0;
	double angle = camera_vec.angle( Vector( 0, 0, 1 ) );
	Vector axis = camera_vec.cross( Vector( 0, 0, 1 ) );
	Matrix mat = Matrix::makeTransformRotation( axis, angle );
	Vector input_vec( dir_x, 0, -dir_y );
	Vector vec = mat.multiply( input_vec ).normalize( ) * SPEED;
	move( vec.x, vec.z );

	// ƒWƒƒƒ“ƒv—\–ñ
	if ( jump ) {
		setJump( );
	}

	// UŒ‚—\–ñ
	if ( attack ) {
		setAttack( );
	}

	// ’…’n
	if ( _standing && !_before_standing ) {
		SoundPtr sound = stage->getSound( );
		sound->play( Sound::SOUND_PLAYER_LANDING );
	}

}

void Player::onJump( ) {
	StagePtr stage = Stage::getTask( );
	SoundPtr sound = stage->getSound( );
	sound->play( Sound::SOUND_PLAYER_JUMP );
}

void Player::onAttack( ) {
	bool pickingup = false;
	if ( isFirstAttack( ) ) {
		if ( pickupItem( ) ) {
			finishAttack( );
			pickingup = true;
		}
	}

	if ( !pickingup ) {
		attack( );
	}
}

bool Player::pickupItem( ) {
	StagePtr stage = Stage::getTask( );
	FieldPtr field = stage->getField( );
	Vector pos = getPos( );
	int px = ( int )pos.x;
	int pz = ( int )pos.z;
	ExperiencesPtr experiences = stage->getExperiences( );

	for ( int i = 0; i < 9; i++ ) {
		int x = px + i % 3 - 1;
		int z = pz + i / 3 - 1;
		ObjectPtr object = field->getTarget( x, z );
		ItemPtr item = std::dynamic_pointer_cast< Item >( object );
		if ( item ) {
			if ( item->pickup( getIndex( ) ) ) {
				return true;
			}
		}
		
	}

	return false;
}

void Player::replace( ) {
	StagePtr stage = Stage::getTask( );
	FieldPtr field = stage->getField( );
	Vector pos = getPos( );
	field->delTarget( ( int )pos.x, ( int )pos.z, getThis( ) );
	CameraPtr camera = stage->getCamera( );
	pos = camera->getFocus( );
	pos = field->getValidPos( ( int )pos.x, ( int )pos.z, true );
	setPos( pos );
	field->setTarget( ( int )pos.x, ( int )pos.z, getThis( ) );
}

void Player::attack( ) {

}

void Player::updateDerive( ) {
	ClientPtr client = Client::getTask( );
	CLIENTDATA data = client->getClientData( );
	FrameworkPtr fw = Framework::getInstance( );
	StagePtr stage = Stage::getTask( );
	SoundPtr sound = stage->getSound( );
	EffectPtr effect = stage->getEffect( );
	_weapon_level = data.user[ _index ].level_weapon;

	if ( data.user[ _index ].life == 0 ) {
		setAction( Character::ACTION_DEAD );
	}
	
	if ( getAction( ) != Character::ACTION_DEAD ) {
		if ( fw->isOutterOfScreen( getPos( ) ) ) {
			replace( );
		}
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
		/*
			if ( _motion == MOTION_ATTACK ) {
				adjustDir( );
			}
		*/
		break;
	case ACTION_USE:
		if ( _motion != MOTION_USE ) {
			_motion = MOTION_USE ;
			_time = 0;
		}
		break;
	case ACTION_DEAD:
		if ( _motion != MOTION_DEAD ) {
			_motion = MOTION_DEAD;
			_time = 0;
			sound->play( Sound::SOUND_PLAYER_DEAD );
			effect->play( Effect::BMVOL3_POISON, getPos( ), 0.075f );
		}
		break;
	default:
		_motion = MOTION_WAIT;
		break;
	}
	
	switch ( _motion ) {
	case MOTION_WAIT:
	case MOTION_WALK:
		if ( _time > TIME[ _motion ] ) {
			_time = 0;
		}
		break;
	case MOTION_DAMAGE:
		if ( _time > TIME[ _motion ] ) {	
			_time = TIME[ _motion ];
		}
		break;
	case MOTION_DEAD:
		if ( _time > TIME[ _motion ] ) {
			_time = TIME[ _motion ];

			ClientPtr client = Client::getTask( );
			client->kill( getIndex( ) );
		}
		break;
	case MOTION_USE:
		if ( _time > TIME[ _motion ] ) {
			_time = TIME[ _motion ];
			setAction( ACTION_WAIT );
		}
		break;
	case MOTION_ATTACK:
		if ( isAttacking( ) ) {
			if ( _time > getAttackTimeEnd( ) ) {
				_time = getAttackTimeBegin( );
			}
		} else {
			if ( _time >= getAttackTotalTime( ) ) {
				_time = getAttackTotalTime( );
				finishAttack( );
			}
		}
		break;
	}
}

double Player::getJumpPower( ) {
	return 0.6;
}

bool Player::isPassedTime( double time ) {
	return _time >= time && _time_store < time && _time > _time_store;	
}

double Player::getAttackTimeBegin( ) {
	return 0;
}

double Player::getAttackTimeEnd( ) {
	return TIME[ _motion ];
}

double Player::getAttackTotalTime( ) { 
	return 0;
}