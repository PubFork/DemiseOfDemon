#include "Deedbox.h"
#include "Field.h"
#include "Treasures.h"
#include "Stage.h"
#include "Effect.h"
#include "Sound.h"

const double TIME = 40;
const double SPEED = 0.5;
const int ITEM_INTERVAL = 10;
const int ITEM_NUM = 10;
const double THROW_HEIGHT = 5;

Deedbox::Deedbox( int x, int y, int z )
: _x( x )
, _y( y )
, _z( z ) {
	_time = 0;
	_phase = PHASE_CLOSE;
	_count = 0;
	_index = -1;
}

Deedbox::~Deedbox() {
}

void Deedbox::update( ) {
	StagePtr stage = Stage::getTask( );
	switch ( _phase  ) {
	case PHASE_UNLOCK:
		_time += SPEED;
		if ( _time > TIME ) {
			_time = TIME;
			_phase = PHASE_OPEN;

			_index = -1;
			EffectPtr effect = stage->getEffect( );
			effect->play( Effect::BMVOL3_CHANT, Vector( _x, _y, _z ) + Vector( 0, 0.8, 0 ), 0.1f );
			SoundPtr sound = stage->getSound( );
			sound->play( Sound::SOUND_DEEDBOX_GET_ITEMS );
		}
		break;
	case PHASE_OPEN:
		_count++;
		if ( _count < ITEM_NUM * ITEM_INTERVAL ) {
			if ( _count % ITEM_INTERVAL == 0 ) {
				
				TreasuresPtr treasures = stage->getTreasures( );
				Vector pos( _x + 0.5, _y + THROW_HEIGHT, _z + 0.5 );
				treasures->create( pos );
				
			}
		} else {
			_phase = PHASE_NONE;
		}
		break;
	}
}

int Deedbox::getX( ) const {
	return _x;
}

int Deedbox::getY( ) const {
	return _y;
}

int Deedbox::getZ( ) const {
	return _z;
}

double Deedbox::getTime( ) const {
	return _time;
}

bool Deedbox::pickup( int index ) {
	if ( _phase == PHASE_CLOSE ) {
		_phase = PHASE_UNLOCK;
		_index = index;
		StagePtr stage = Stage::getTask( );

		SoundPtr sound = stage->getSound( );
		sound->play( Sound::SOUND_DEEDBOX_OPEN);
		return true;
	}

	return false;
}
