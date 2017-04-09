#include "Doors.h"
#include "Framework.h"
#include "Door.h"
#include "Map.h"
#include "Field.h"
#include "Stage.h"

Doors::Doors( ) {

}

Doors::~Doors( ) {
}

void Doors::update( ) {
	FrameworkPtr fw = Framework::getInstance( );
	for ( int i = 0; i < _num; i++ ) {
		_door[i]->update();
	}
}

int Doors::getNum( ) const {
	return _num;
}

bool Doors::isOpening( ) const {
	for ( int i = 0; i < _num; i++ ) {
		if ( _door[ i ]->isOpening( ) ) {
			return true;
		}
	}

	return false;
}

void Doors::init( ) {
	// èâä˙âª
	for ( int i = 0; i < MAX_NUM; i++ ) {
		_door[ i ].reset( );
	}
	_num = 0;

	// mapÇ©ÇÁê∂ê¨
	StagePtr stage = Stage::getTask( );
	FieldPtr field = stage->getField( );
	for ( int i = 0; i < MAP_SIZE; i++ ) {
		for ( int j = 0; j < MAP_SIZE; j++ ) {
			unsigned char object = field->getObject( i, j );
			DoorPtr door;
			int y = field->getHeight( i, j );
			switch (object){
			case OBJECT_DOOR_PX:
				door = DoorPtr(new Door( i, y, j, Door::DIR_PX ) );
				break;
			case OBJECT_DOOR_MX:
				door = DoorPtr(new Door( i, y, j, Door::DIR_MX ) );
				break;
			case OBJECT_DOOR_PZ:
				door = DoorPtr(new Door( i, y, j, Door::DIR_PZ ) );
				break;
			case OBJECT_DOOR_MZ:
				door = DoorPtr(new Door( i, y, j, Door::DIR_MZ ) );
				break;
			}
			if ( door ) {
				_door[ _num ] = door;
				_num++;

				for ( int k = 0; k < 9; k++ ) {
					int x = i + k % 3 - 1;
					int z = j + k / 3 - 1;
					field->setTarget( x, z, door );
				}
			}
		}
	}
}

DoorConstPtr Doors::getDoor( int index ) const {
	return _door[ index ];
}

DoorPtr Doors::getDoor( int index ) {
	return _door[ index ];
}
