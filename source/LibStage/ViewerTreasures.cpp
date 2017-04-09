#include "ViewerTreasures.h"
#include "Treasures.h"
#include "Treasure.h"
#include "DxLib.h"
#include "Stage.h"

const float PARTITION = 60.0f;

ViewerTreasures::ViewerTreasures( ) {
	create( Treasure::LEVEL_UP ,"resource/object/item/item_level_up.mv1" );
	create( Treasure::POTION, "resource/object/item/item_potion.mv1" );
	create( Treasure::POTION_BIG, "resource/object/item/item_potion_big.mv1" );
	_timer = 0;
}

ViewerTreasures::~ViewerTreasures( ) {
}

void ViewerTreasures::draw( ) {
	StagePtr stage = Stage::getTask( );
	TreasuresPtr treasures = stage->getTreasures( );

	for ( int i = 0; i < Treasures::MAX_NUM; i++ ) {
		TreasureConstPtr treasure = treasures->getTreasure( i );
		if ( !treasure ) {
			continue;
		}
		drawTreasure( treasure );
	}
	_timer++;
	if ( _timer % ( int )PARTITION == 0 ) {
		_timer = 0;
	}
}

void ViewerTreasures::drawTreasure( TreasureConstPtr treasure ) {
	int id = _id[ treasure->getType( ) ];
	Vector pos = treasure->getPos( );
	// À•W•ÏŠ·
	MATRIX matrix = MGetIdent( );
	if ( treasure->getType( ) != Treasure::LEVEL_UP ){
		matrix = MMult( matrix, MGetScale( VGet( 0.008f, 0.008f, 0.008f ) ) );
	}
	if ( treasure->getType( ) == Treasure::LEVEL_UP ) {
		matrix = MMult( matrix, MGetRotY( ( float )PI2 * ( _timer / PARTITION ) ) );
	}
	matrix = MMult( matrix, MGetTranslate( VGet( ( float )pos.x, ( float )pos.y, ( float )pos.z ) ) );
	MV1SetMatrix( id, matrix );
	MV1DrawModel( id );
}


void ViewerTreasures::create( Treasure::TYPE item_id, const char * filename ) {
	{
		_id[ item_id ] = MV1LoadModel( filename );
		if ( _id[ item_id ] < 0 ) {
			return;
		}
		int num = MV1GetMaterialNum( _id[ item_id ] ) ;
		for ( int i = 0; i < num; i++ ) {
			MV1SetMaterialEmiColor( _id[ item_id ], i, GetColorF( 1.0f, 1.0f, 1.0f, 1.0f ) );
		}
	}
}
