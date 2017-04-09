#include "Experiences.h"
#include "Experience.h"
#include "DxLib.h"
#include "Stage.h"
#include "Field.h"
#include "Client.h"
#include "Party.h"
#include "Framework.h"

const double LENGTH = 1;
const double RANGE = 3;

Experiences::Experiences( ) {
	for ( int i = 0; i < MAX_NUM; i++ ) {
		_experience[ i ].reset( );
	}
}


Experiences::~Experiences( ) {
}

void Experiences::init( ) {
	for ( int i = 0; i < MAX_NUM; i++ ) {
		_experience[ i ].reset( );
	}
}

void Experiences::update( ) {
	StagePtr stage = Stage::getTask( );
	PartyPtr party = stage->getParty( );

	for ( int i = 0; i < MAX_NUM; i++ ) {
		ExperiencePtr experience = _experience[ i ];
		if ( !experience ) {
			continue;
		}
		experience->update( party );
		if ( !experience->isExpired( ) ) {
			_experience[ i ].reset( );
		}
	}
}

ExperienceConstPtr Experiences::getExperience( int index ) const {
	return _experience[ index ];
}

ExperiencePtr Experiences::getExperience( int index ) {
	return _experience[ index ];
}

void Experiences::add( ExperiencePtr experience ) {
	for ( int i = 0; i < MAX_NUM; i++ ) {
		ExperiencePtr check = _experience[ i ];
		if ( !check ) {
			_experience[ i ] = experience;
			break;
		}
	}
}

void Experiences::create( const Vector& pos, int num ) {
	StagePtr stage = Stage::getTask( );
	FieldPtr field = stage->getField( );

	const int RATIO = 1000;
	int height = field->getHeight( ( int )pos.x, ( int )pos.z );
	if ( height < 10 ) {
		return;
	}
	for ( int i = 0; i < num; i++ ) {
		Matrix mat = Matrix::makeTransformRotation( Vector( 0, 1, 0 ), PI2 * i / num );
		double len = LENGTH + RANGE * ( rand( ) % RATIO ) / RATIO;
		Vector p = mat.multiply( Vector( len, 0, 0 ) ) + pos;
		p.y = field->getHeight( ( int )p.x, ( int )p.z ) + 1.0;
		ExperiencePtr experience = ExperiencePtr( new Experience( p ) );
		add( experience );
	}
}