#include "Door.h"
#include "Field.h"
#include "Stage.h"
#include "Sound.h"

const double TIME = 40;
const double SPEED = 0.5;

Door::Door( int x, int y, int z, DIR dir )
: _x( x )
, _y( y )
, _z( z )
, _dir( dir ) {
	_time = 0;
	_phase = PHASE_CLOSE;
}

Door::~Door() {
}

void Door::update( ) {
	if ( _phase != PHASE_UNLOCK ) {
		return;
	}
	
	_time += SPEED;
	if ( _time > TIME ) {
		_time = TIME;
		_phase = PHASE_OPEN;
		StagePtr stage = Stage::getTask( );
		FieldPtr field = stage->getField( );
		for ( int k = 0; k < 9; k++ ) {
			int x = _x + k % 3 - 1;
			int z = _z + k / 3 - 1;
			field->delTarget( x, z, getThis( ) );
		}
	}
}

int Door::getX( ) const {
	return _x;
}

int Door::getY( ) const {
	return _y;
}

int Door::getZ( ) const {
	return _z;
}

bool Door::isOpening( ) const {
	return _phase == PHASE_OPEN;
}

Door::DIR Door::getDir( ) const {
	return _dir;
}

double Door::getTime( ) const {
	return _time;
}

bool Door::pickup( int index ) {
	if ( _phase == PHASE_CLOSE ) {
		_phase = PHASE_UNLOCK;
		StagePtr stage = Stage::getTask( );
		FieldPtr field = stage->getField( );
		for ( int k = 0; k < 9; k++ ) {
			int x = _x + k % 3 - 1;
			int z = _z + k / 3 - 1;
			field->delTarget( x, z, getThis( ) );
		}
		SoundPtr sound = stage->getSound( );
		sound->play( Sound::SOUND_DOOR_OPEN );
	}

	return true;
}
