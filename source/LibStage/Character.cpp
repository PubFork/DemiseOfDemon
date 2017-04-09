#include "Character.h"
#include "Framework.h"
#include "Field.h"
#include "Stage.h"

const double GRAVITY = 0.06;

Character::Character( const Vector& pos, bool ban )
: _ban_on_glass( ban ) {
	_pos = pos;
	_vec_x = 0.0;
	_vec_y = 0.0;
	_vec_z = 0.0;
	_dir_x = 0;
	_dir_z = -1;
	_jump = false;
	_use = false;
	_attack = false;
	_action = ACTION_WAIT;
	_standing_pos = _pos;
}

Character::~Character( ) {
}

void Character::setPos( const Vector& pos ) {
	_pos = pos;
}

Character::ACTION Character::getAction( ) const {
	return _action;
}

Vector Character::getPos( ) const {
	return _pos;
}

double Character::getDirX( ) const {
	return _dir_x;
}

double Character::getDirZ( ) const {
	return _dir_z;
}

void Character::setJump( ) {
	_jump = true;
}

void Character::setDir( double dir_x, double dir_z ) {
	_dir_x = dir_x;
	_dir_z = dir_z;
}

void Character::setUse( ) {
	_use = true;
}

void Character::setAttack( ) {
	_attack = true;
}

void Character::adjustDir( ) {
	if ( _vec_x != 0 || _vec_z != 0 ) {
		_dir_x = _vec_x;
		_dir_z = _vec_z;
	}
}

void Character::setAction( ACTION action ) {
	_action = action;
}

void Character::move( double vec_x, double vec_z ) {
	_vec_x = vec_x;
	_vec_z = vec_z;
}

void Character::update( ) {
	updateTrancelating( );
	updateFalling( );
	updateAction( );
	updateDerive( );

	_jump = false;
	_attack = false;
	_use = false;
}

void Character::updateTrancelating( ) {
	if ( _action != ACTION_WALK &&
		 _action != ACTION_FLOAT ) {
		return;
	}

	// à⁄ìÆèàóù
	StagePtr stage = Stage::getTask( );
	FieldPtr field = stage->getField( );
	field->delTarget( ( int )_pos.x, ( int )_pos.z, getThis( ) );
	field->translate( _pos, _vec_x, _vec_z, _ban_on_glass );
	field->setTarget( ( int )_pos.x, ( int )_pos.z, getThis( ) );
}

bool Character::knockback( const Vector& dir ) {
	StagePtr stage = Stage::getTask( );
	FieldPtr field = stage->getField( );
	field->delTarget( ( int )_pos.x, ( int )_pos.z, getThis( ) );
	bool ret = field->knockback( _pos, dir.x, dir.z );
	field->setTarget( ( int )_pos.x, ( int )_pos.z, getThis( ) );

	return ret;
}

void Character::updateFalling( ) {
	StagePtr stage = Stage::getTask( );
	FieldPtr field = stage->getField( );
	
	// èdóÕ
	_vec_y -= GRAVITY;
	_pos.y += _vec_y;
	int x = ( int )_pos.x;
	int z = ( int )_pos.z;
	int y = ( int )_pos.y;
	if ( _pos.y < 0 ) {
		y = -( ( int )_pos.y + 1 );
	}
	int height = field->getHeight( x, z );

	// ìﬁóé
	_fall = false;
	if ( height < 10 ) {
		_fall = true;
	}

	// ínñ Ç…óßÇ¡ÇƒÇ¢ÇÈÇ©ÅH
	_before_standing = _standing;
	_standing = false;
	if ( height > y ) {
		_pos.y = height + GRAVITY / 2;
		_vec_y = 0;
		_standing = true;
	}

	{
		double y = _standing_pos.y;
		_standing_pos = _pos;
		if ( !_standing && _pos.y > y ) {
			_standing_pos.y = y;
		}
	}
}

void Character::updateAction( ) {
	if ( _action == ACTION_DEAD ) {
		return;
	}

	if ( _fall || _action == ACTION_FALL ) {
		_action = ACTION_FALL;
		return;
	}
	
	if ( _action != ACTION_USE ) {
		if ( _standing ) {
			if ( _attack || ( _action == ACTION_ATTACK && !_finish_attack ) ) {
				onAttack( );
				_action = ACTION_ATTACK;
				_first_attack = false;
			} else {
				_finish_attack = false;
				_first_attack = true;
				_action = ACTION_WAIT;
				if ( _vec_x != 0 || _vec_z != 0 ) {
					_action = ACTION_WALK;
				}
				if ( _jump ) {
					_vec_y = getJumpPower( );
					onJump( );
				}
			}
		} else {
			_action = ACTION_FLOAT;
		}
		if ( _use ) {
			_action = ACTION_USE;
		}
	}
}

Vector Character::getStandPos( ) const {
	return _standing_pos;
}

bool Character::isFirstAttack( ) const {
	return _first_attack;
}
