#include "ViewerLay.h"
#include "Model.h"
#include "BulletLay.h"
#include "DxLib.h"

const int DIV = 8;


double ViewerLay::_scroll = 0;

void ViewerLay::updateScroll( ) {
	_scroll += 0.2;
	if ( _scroll > 1.0 ) {
		_scroll -= 1.0;
	}
}

ViewerLay::ViewerLay( ) {
	int id = LoadGraph( "resource/image/weapon/Lay.png" );
	for ( int i = 0; i < MAX_NUM; i++ ) {
		_model[ i ] = ModelPtr( new Model );
		_model[ i ]->alloc( DIV * 2 );
		_model[ i ]->setTexture( id );
	}
	_num = 0;
}

ViewerLay::~ViewerLay( ) {
}

void ViewerLay::create( BulletPtr bullet ) {
	if ( _num >= MAX_NUM ) {
		return;
	}

	BulletLayPtr lay = std::dynamic_pointer_cast< BulletLay >( bullet );
	
	const Vector pos = lay->getPos( );
	const double length = lay->getLength( );
	const Vector dir = lay->getDir( );
	double radius = lay->getRadius( );
	
	Vector axis = dir.cross( Vector( 0, 0, 1 ) );
	double angle = dir.angle( Vector( 0, 0, 1 ) );
	Matrix mat_dir = Matrix::makeTransformRotation( axis, angle );

	for ( int i = 0; i < DIV; i++ ) {

		std::array< Vector, 4 > p;

		for ( int j = 0; j < 2; j++ ) {
			Matrix mat = Matrix::makeTransformRotation( Vector( 0, 0, 1 ), PI2 * ( i + j ) / DIV );
			Vector pos = mat.multiply( Vector( 0, radius, 0 ) );
			p[ j + 0 ] = pos + Vector( 0, 0, length );
			p[ j + 2 ] = pos + Vector( 0, 0, 1.0 );
		}

		std::array< Model::VERTEX, 4 > vertex;
		for ( int j = 0; j < 4; j++ ) {
			vertex[ j ].pos = mat_dir.multiply( p[ j ] )+ pos;
			vertex[ j ].u = j % 2;
			vertex[ j ].v = j / 2 * length + _scroll;
		}

		_model[ _num ]->set( i * 6 + 0, vertex[ 0 ] );
		_model[ _num ]->set( i * 6 + 1, vertex[ 1 ] );
		_model[ _num ]->set( i * 6 + 2, vertex[ 2 ] );
		_model[ _num ]->set( i * 6 + 3, vertex[ 1 ] );
		_model[ _num ]->set( i * 6 + 4, vertex[ 3 ] );
		_model[ _num ]->set( i * 6 + 5, vertex[ 2 ] );
	}

	_num++;
}

void ViewerLay::draw( ) {
	SetDrawBlendMode( DX_BLENDMODE_ADD , 255 ) ;
	for ( int i = 0; i < _num; i++ ) {
		_model[ i ]->draw( true );
	}
    // 描画ブレンドモードをノーブレンドにする
    SetDrawBlendMode( DX_BLENDMODE_NOBLEND , 0 ) ;

	_num = 0;
}
