#include "Deedboxes.h"
#include "Framework.h"
#include "Deedbox.h"
#include "Map.h"
#include "Field.h"
#include "Stage.h"

Deedboxes::Deedboxes( ) {

}

Deedboxes::~Deedboxes( ) {
}

void Deedboxes::update( ) {
	FrameworkPtr fw = Framework::getInstance( );
	for ( int i = 0; i < _num; i++ ) {
		_deedbox[ i ]->update();
	}
}

int Deedboxes::getNum( ) const {
	return _num;
}

void Deedboxes::init( ) {
	// èâä˙âª
	for ( int i = 0; i < MAX_NUM; i++ ) {
		_deedbox[ i ].reset( );
	}
	_num = 0;

	// mapÇ©ÇÁê∂ê¨
	StagePtr stage = Stage::getTask( );
	FieldPtr field = stage->getField( );
	for ( int i = 0; i < MAP_SIZE; i++ ) {
		for ( int j = 0; j < MAP_SIZE; j++ ) {
			unsigned char object = field->getObject(i, j);
			if ( object == OBJECT_DEEDBOX ) {
				int y = field->getHeight( i, j );
				DeedboxPtr deedbox = DeedboxPtr(new Deedbox( i, y, j ) );
				_deedbox[ _num ] = deedbox;
				_num++;

				field->setTarget( i, j, deedbox );
			}
		}
	}
}

DeedboxConstPtr Deedboxes::getDeedbox( int index ) const {
	return _deedbox[ index ];
}

DeedboxPtr Deedboxes::getDeedbox( int index ) {
	return _deedbox[ index ];
}
