#include "Grid.h"
#include "Map.h"
#include "Framework.h"
#include "Model.h"
#include "ImageBase.h"
#include "DxLib.h"

Grid::Grid( MapConstPtr map ): _map( map ) {
	_image = ImageBasePtr( new ImageBase );
	
	//_model_box作成
	_model_box = ModelPtr( new Model );
	_model_box->alloc( 5 * 2 * MAP_SIZE * MAP_SIZE );
	_model_box->setTexture( _image->load( "resource/image/map/grid.png" ) );
	for ( int i = 0; i < MAP_SIZE; i++ ) {
		for ( int j = 0; j < MAP_SIZE; j++ ) {
			updateBox( i, j );
		} 
	}
	
	// _model_water作成
	_model_water = ModelPtr( new Model );
	_model_water->alloc( 2 * MAP_SIZE * MAP_SIZE );
	_model_water->setTexture( _image->load( "resource/image/map/water.png" ) );
	for ( int i = 0; i < MAP_SIZE; i++ ) {
		for ( int j = 0; j < MAP_SIZE; j++ ) {
			updateWater( i, j );
		}
	}


	_height = 10;
}


Grid::~Grid( ) {
}

void Grid::draw( ) {
	// 描画
	_model_box->draw( );
	_model_water->draw( true );
}

void Grid::setHeight( double height ) {
	_height = height;
	for ( int i = 0; i < MAP_SIZE; i++ ) {
		for ( int j = 0; j < MAP_SIZE; j++ ) {
			updateBox( i, j );
			updateWater( i, j );
		}
	}
}

void Grid::updateWater( int x, int z ) {
	if ( x < 0 || x >= MAP_SIZE ) {
		return; 
	}
	if ( z < 0 || z >= MAP_SIZE ) {
		return;
	}


	int h = _map->getWaterHeight( x, z );
	if ( h == 0 ) {
		h = -10;
	}

	Vector pos[ 6 ];
	pos[ 0 ] = Vector( 0.0f, h, 0.0f );//4 00
	pos[ 1 ] = Vector( 0.0f, h, 1.0f );//6 01
	pos[ 2 ] = Vector( 1.0f, h, 1.0f );//7 11
	pos[ 3 ] = Vector( 1.0f, h, 1.0f );//7 11
	pos[ 4 ] = Vector( 1.0f, h, 0.0f );//5 10
	pos[ 5 ] = Vector( 0.0f, h, 0.0f );//4 00

	for ( int i = 0; i < 6; i++ ) {
		pos[ i ] += Vector( x, 0, z );
	}

	double u[ 6 ] = { 0, 0, 1, 1, 1, 0 };
	double v[ 6 ] = { 0, 1, 1, 1, 0, 0 };

	// 頂点設定
	for ( int i = 0; i < 6; i++ ) {
		int n = ( x + z * MAP_SIZE ) * 6 + i;

		Model::VERTEX vertex;
		vertex.pos = pos[ i ] + Vector( 0, -_height, 0 );
		vertex.u = u[ i ];
		vertex.v = v[ i ];
		_model_water->set( n, vertex );
	}
}

void Grid::updateBox( int x, int z ) {
	if ( x < 0 || x >= MAP_SIZE ) {
		return; 
	}
	if ( z < 0 || z >= MAP_SIZE ) {
		return;
	}

	Vector box_pos[ 8 ];
	box_pos[ 0 ] = Vector( 0.0f, 0.0f, 0.0f );
	box_pos[ 1 ] = Vector( 1.0f, 0.0f, 0.0f );
	box_pos[ 2 ] = Vector( 0.0f, 0.0f, 1.0f );
	box_pos[ 3 ] = Vector( 1.0f, 0.0f, 1.0f );

	int h = _map->getHeight( x, z );
	if ( h == 0 ) {
		h = -10;
	}
	for ( int i = 0; i < 4; i++ ) {
		box_pos[ i + 4 ] = Vector( box_pos[ i ].x, box_pos[ i ].y +( float )h ,box_pos[ i ].z );
	}
	
	for ( int i = 0; i < 8; i++ ) {
		box_pos[ i ] = Vector( box_pos[ i ].x +( float )x , box_pos[ i ].y, box_pos[ i ].z + ( float )z );
	}

	Vector pos[ 30 ]; // 5面 = 30頂点
	// TOP PLANE
	pos[  0 ] = box_pos[ 4 ];
	pos[  1 ] = box_pos[ 6 ];
	pos[  2 ] = box_pos[ 7 ];
	pos[  3 ] = box_pos[ 7 ];
	pos[  4 ] = box_pos[ 5 ];
	pos[  5 ] = box_pos[ 4 ];
	// LEFT PLANE			
	pos[  6 ] = box_pos[ 2 ];
	pos[  7 ] = box_pos[ 6 ];
	pos[  8 ] = box_pos[ 4 ];
	pos[  9 ] = box_pos[ 4 ];
	pos[ 10 ] = box_pos[ 0 ];
	pos[ 11 ] = box_pos[ 2 ];
	// RIGHT PLANE			
	pos[ 12 ] = box_pos[ 0 ];
	pos[ 13 ] = box_pos[ 4 ];
	pos[ 14 ] = box_pos[ 5 ];
	pos[ 15 ] = box_pos[ 5 ];
	pos[ 16 ] = box_pos[ 1 ];
	pos[ 17 ] = box_pos[ 0 ];
	// Back LEFT PLANE		
	pos[ 18 ] = box_pos[ 3 ];
	pos[ 19 ] = box_pos[ 7 ];
	pos[ 20 ] = box_pos[ 6 ];
	pos[ 21 ] = box_pos[ 6 ];
	pos[ 22 ] = box_pos[ 2 ];
	pos[ 23 ] = box_pos[ 3 ];
	// Back RIGHT PLANE		
	pos[ 24 ] = box_pos[ 1 ];
	pos[ 25 ] = box_pos[ 5 ];
	pos[ 26 ] = box_pos[ 7 ];
	pos[ 27 ] = box_pos[ 7 ];
	pos[ 28 ] = box_pos[ 3 ];
	pos[ 29 ] = box_pos[ 1 ];

	double u[ 6 ] = { 0, 0, 1, 1, 1, 0 };
	double v[ 6 ] = { 1, 0, 0, 0, 1, 1 };
	int l[ 5 ] = { 0, 0, 1, 1, 0 };
	int r[ 5 ] = { 0, 1, 0, 0, 1 };

	// 頂点設定
	for ( int i = 0; i < 30; i++ ) {
		int n = ( x + z * MAP_SIZE ) * 30 + i;

		Model::VERTEX vertex;
		vertex.pos = pos[ i ] + Vector( 0, -_height, 0 );
		vertex.u = u[ i % 6 ] / 2 + l[ i / 6 ] * 0.5;
		vertex.v = v[ i % 6 ] / 2 + r[ i / 6 ] * 0.5;
		if ( _map->getAttribute( x, z ) == ATTRIBUTE_GRASS ) {
			vertex.u += 0.5;
			vertex.v += 0.5;
		}

		_model_box->set( n, vertex );
	}
}

void Grid::drawCursor( int x, int z, int h, int r, const int mode ) {
	float fx = ( float )x;
	float fz = ( float )z;
	float fh = ( float )h;
	float fr = ( float )r;

	Vector pos[ 12 ];
	pos[  0 ] = Vector( 0.0f, 0.0f, 0.0f );
	pos[  1 ] = Vector(   fr, 0.0f, 0.0f );
	pos[  2 ] = Vector( 0.0f, 0.0f,   fr );
	pos[  3 ] = Vector(   fr, 0.0f,   fr );
	pos[  4 ] = Vector( 0.0f,    fh, 0.0f );
	pos[  5 ] = Vector(   fr,    fh, 0.0f );
	pos[  6 ] = Vector( 0.0f,    fh,   fr );
	pos[  7 ] = Vector(   fr,    fh,   fr );
	pos[  8 ] = Vector( 0.0f, 30.0f, 0.0f );
	pos[  9 ] = Vector(   fr, 30.0f, 0.0f );
	pos[ 10 ] = Vector( 0.0f, 30.0f,   fr );
	pos[ 11 ] = Vector(   fr, 30.0f,   fr );

	for ( int i = 0; i < 12; i++ ) {
		pos[ i ] = Vector( pos[ i ].x + fx, pos[ i ].y - _height, pos[ i ].z + fz );
	}
	
	FrameworkPtr fw = Framework::getInstance( );

	// 面
	if ( h >= 0 ) {
		Vector plane_pos[ 6 ];
		plane_pos[  0 ] = pos[ 4 ];
		plane_pos[  1 ] = pos[ 6 ];
		plane_pos[  2 ] = pos[ 7 ];
		plane_pos[  3 ] = pos[ 4 ];
		plane_pos[  4 ] = pos[ 7 ];
		plane_pos[  5 ] = pos[ 5 ];
		
		int cr = 200;
		int cg = 200;
		int cb = 0;
		if ( mode == 2 ) {
			cr = 200;
			cg = 200;
			cb = 200;
		}
		for( int i = 0; i < 2; i++ ) {
			fw->polygon(
				plane_pos[ i * 3 + 0 ],
				plane_pos[ i * 3 + 1 ],
				plane_pos[ i * 3 + 2 ], cr, cg, cb );
		}
	}

	// ライン
	int cr = 150;
	int cb = 0;
	int cg = 0;
	if ( mode == 1 ) {
		cr = 0;
		cg = 0;
		cb = 150;
	}
	//水
	if ( mode == 2 ) {
		cr = 0;
		cg = 150;
		cb = 0;
	}
	for ( int i = 0; i < 4; i++) {
		fw->line( pos[ i ], pos[ i + 8 ], cr, cg, cb );
	}
}