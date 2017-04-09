#include "Treasures.h"
#include "Framework.h"
#include "Network.h"
#include "Item.h"
#include "Field.h"
#include "Treasure.h"
#include "DxLib.h"
#include "Client.h"
#include "Stage.h"

const double RANGE = 5;
const double FIX_RANGE = 2;

Treasures::Treasures( ) {
}

Treasures::~Treasures( ) {
}

void Treasures::init( ) {
	for ( int i = 0; i < MAX_NUM; i++ ) {
		_treasure[ i ].reset( );
	}
	int num = 0;
	// map‚©‚ç¶¬
	StagePtr stage = Stage::getTask( );
	FieldPtr field = stage->getField( );
	for ( int i = 0; i < MAP_SIZE; i++ ) {
		for ( int j = 0; j < MAP_SIZE; j++ ) {
			unsigned char object = field->getObject(i, j);
			if ( object == OBJECT_ITEM_GEM ) {
				int y = field->getHeight( i, j );
				TreasurePtr treasure = TreasurePtr(new Treasure( Vector( i,y,j ), Treasure::LEVEL_UP ) );
				_treasure[ num ] = treasure;
				num++;
				field->setTarget( i, j, treasure );
			}
		}
	}
}

void Treasures::update( ) {
	ClientPtr client = Client::getTask( );
	CLIENTDATA data = client->getClientData( );

	for ( int i = 0; i < MAX_NUM; i++ ) {
		TreasurePtr treasure = _treasure[ i ];
		if ( !treasure ) {
			continue;
		}
		treasure->update( );
		if ( !treasure->isExpired( ) ) {
			_treasure[ i ].reset( );
		}
	}
}

void Treasures::add( TreasurePtr treasure ) {
	for ( int i = 0; i < MAX_NUM; i++ ) {
		TreasurePtr check = _treasure[ i ];
		if ( !check ) {
			_treasure[ i ] = treasure;
			treasure->update( );
			break;
		}
	}
}

void Treasures::create( const Vector& pos ) {
	double angle = ( GetRand( 1000 ) ) * PI2 / 1000;
	double length = ( GetRand( 1000 ) ) * RANGE / 1000 + FIX_RANGE;
	Matrix mat = Matrix::makeTransformRotation( Vector( 0, 1, 0 ), angle );
	Vector vec = mat.multiply( Vector( length, 0, 0 ) );
	Vector p = pos + vec;
	p.x = ( int )p.x + 0.5;
	p.z = ( int )p.z + 0.5;

	StagePtr stage = Stage::getTask( );
	FieldPtr field = stage->getField( );

	if ( field->isEntering( p, true ) ) {
		// ”z’u‚Å‚«‚é
		int n = Treasure::MAX_TYPE + 20;
		int choise = GetRand( n - 1);
		if ( choise >= Treasure::MAX_TYPE ) {
			choise = Treasure::POTION;
		}
		if ( choise == Treasure::LEVEL_UP ) {
			choise = Treasure::POTION_BIG;
		}
		Treasure::TYPE type = ( Treasure::TYPE )choise;
		TreasurePtr treasure = TreasurePtr( new Treasure( p, type ) );
		add( treasure );

		field->setTarget( ( int )p.x, ( int )p.z, treasure );
	}
}

TreasureConstPtr Treasures::getTreasure( int index ) const {
	return _treasure[ index ];
}

TreasurePtr Treasures::getTreasure( int index ) {
	return _treasure[ index ];
}
