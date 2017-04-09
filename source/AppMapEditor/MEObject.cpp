#include "MEObject.h"
#include "Framework.h"
#include "Map.h"
#include "DxLib.h"

MEObject::MEObject( MapConstPtr map ) 
	: _map( map ) {

	create( MODEL_DOOR, "resource/object/door/door.mv1" );  
	create( MODEL_DEEDBOX, "resource/object/deedbox/deedbox.mv1" ); 
	create( MODEL_CYCLOPS, "resource/character/enemy/cyclops/cyclops_wait.mv1" );
	create( MODEL_GOBLIN, "resource/character/enemy/goblin/enemy0_wait.mv1" );
	create( MODEL_REDGOBLIN, "resource/character/enemy/red_goblin/enemy1_wait.mv1" );
	create( MODEL_GHOST, "resource/character/enemy/ghost/enemy_ghost_wait.mv1" );
	create( MODEL_ARMOR, "resource/character/enemy/armor/enemy_armor_wait.mv1" );
	create( MODEL_GEM, "resource/object/item/item_level_up.mv1" );
}


MEObject::~MEObject( ) {
}

void MEObject::create( MODEL model, const char * filename ) {
	_id[ model ] = MV1LoadModel( filename );
	if ( _id[ model ] < 0 ) {
		return;
	}
	int num = MV1GetMaterialNum( _id[ model ] ) ;
	for ( int i = 0; i < num; i++ ) {
		MV1SetMaterialEmiColor( _id[ model ], i, GetColorF( 1.0f, 1.0f, 1.0f, 1.0f ) );

	}
}

void MEObject::setHeight( double height ) {
	_height = height;
}

void MEObject::draw( ) {
	for ( int i = 0; i < MAP_SIZE; i++ ) {
		for ( int j = 0; j < MAP_SIZE; j++ ) {
			unsigned char obejct = _map->getObject( i, j );
			if ( obejct == OBJECT_NONE ) {
				continue;
			}

			double height = _map->getHeight( i, j ) - _height;
			Vector pos( i + 0.5, height, j + 0.5 );

			switch ( obejct ) {
			case OBJECT_DOOR_PX:
				drawDoorPX( pos );
				break;
			case OBJECT_DOOR_MX:
				drawDoorMX( pos );
				break;
			case OBJECT_DOOR_PZ:
				drawDoorPZ( pos );
				break;
			case OBJECT_DOOR_MZ:
				drawDoorMZ( pos );
				break;
			case OBJECT_ENEMY_CYCLOPS:
				drawObject( MODEL_CYCLOPS, pos, 0 );
				break;
			case OBJECT_DEEDBOX:
				drawObject( MODEL_DEEDBOX, pos, 0 );
				break;
			case OBJECT_ENEMY_GOBLIN:
				drawObject( MODEL_GOBLIN, pos, 0 );
				break;
			case OBJECT_ENEMY_REDGOBLIN:
				drawObject( MODEL_REDGOBLIN, pos, 0 );
				break;
			case OBJECT_ENEMY_GHOST:
				drawObject( MODEL_GHOST, pos, 0 );
				break;
			case OBJECT_ENEMY_ARMOR:
				drawObject( MODEL_ARMOR, pos, 0 );
				break;
			case OBJECT_ITEM_GEM:
				drawObject( MODEL_GEM, pos, 0 );
				break;
			}
		}
	}
}

void MEObject::drawDoorPX( const Vector& pos ) {
	drawObject( MODEL_DOOR, pos, PI2 * 1 / 4 );
}

void MEObject::drawDoorMX( const Vector& pos ) {
	drawObject( MODEL_DOOR, pos, PI2 * 3 / 4 );
}

void MEObject::drawDoorPZ( const Vector& pos ) {
	drawObject( MODEL_DOOR, pos, PI2 * 2 / 4 );
}

void MEObject::drawDoorMZ( const Vector& pos ) {
	drawObject( MODEL_DOOR, pos, PI2 * 0 / 4 );
}

void MEObject::drawObject( MODEL model, const Vector& pos , double angle ) {
	
	int id = _id[ model ];
	if ( id < 0 ) {
		return;
	}
	float scale = 0.008f;
	if ( model == MODEL_GEM ) {
		scale = 1.0f;
	}
	switch ( model ) {
	case MODEL_DEEDBOX:
		scale = 0.009f;
		break;
	case MODEL_DOOR:
		scale = 0.009f;
		break;
	}

	// À•W•ÏŠ·
	MATRIX matrix = MGetIdent( );
	matrix = MMult( matrix, MGetRotY( ( float )angle ) );
	matrix = MMult( matrix, MGetScale( VGet( scale, scale, scale ) ) );
	matrix = MMult( matrix, MGetTranslate( VGet( ( float )pos.x, ( float )pos.y, ( float )pos.z ) ) );
	MV1SetMatrix( id, matrix );

	// ‚R‚cƒ‚ƒfƒ‹‚Ì•`‰æ
	MV1DrawModel( id ) ;
}
