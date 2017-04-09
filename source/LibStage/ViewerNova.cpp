#include "ViewerNova.h"
#include "Model.h"
#include "BulletNova.h"
#include "DxLib.h"

const double THICK = 5.0;

ViewerNova::ViewerNova( ) {
	int id = LoadGraph( "resource/image/weapon/nova.png" );
	for ( int i = 0; i < MAX_NUM; i++ ) {
		_model[ i ] = ModelPtr( new Model );
		_model[ i ]->alloc( BulletNova::DIV * 2 );
		_model[ i ]->setTexture( id );
	}
	_num = 0;
}

ViewerNova::~ViewerNova( ) {
}

void ViewerNova::create( BulletPtr bullet ) {
	if ( _num >= MAX_NUM ) {
		return;
	}

	BulletNovaPtr nova = std::dynamic_pointer_cast< BulletNova >( bullet );
	
	const Vector pos = nova->getPos( );
	const double length = nova->getLength( );

	double inner = length - THICK;
	if ( inner < 0 ) {
		inner = 0;
	}

	for ( int i = 0; i < BulletNova::DIV; i++ ) {
		std::array< Vector, 4 > p;
		if ( nova->isExpired( i ) ) {
			Matrix mat0 = Matrix::makeTransformRotation( Vector( 0, -1, 0 ), PI2 * i / BulletNova::DIV );
			p[ 0 ] = mat0.multiply( Vector( length, 0, 0 ) );
			p[ 2 ] = p[ 0 ].normalize( ) * inner;
			Matrix mat1 = Matrix::makeTransformRotation( Vector( 0, -1, 0 ), PI2 * ( i + 1 ) / BulletNova::DIV );
			p[ 1 ] = mat1.multiply( Vector( length, 0, 0 ) );
			p[ 3 ] = p[ 1 ].normalize( ) * inner;
		}
		std::array< Model::VERTEX, 4 > vertex;
		for ( int j = 0; j < 4; j++ ) {
			vertex[ j ].pos = p[ j ] + pos;
			vertex[ j ].u = j % 2;
			vertex[ j ].v = j / 2;
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

void ViewerNova::draw( ) {
	SetDrawBlendMode( DX_BLENDMODE_ADD , 255 ) ;
	for ( int i = 0; i < _num; i++ ) {
		_model[ i ]->draw( true );
	}
    // 描画ブレンドモードをノーブレンドにする
    SetDrawBlendMode( DX_BLENDMODE_NOBLEND , 0 ) ;

	_num = 0;
}
