#include "ViewerShot.h"
#include "Model.h"
#include "BulletShot.h"
#include "DxLib.h"

const int TAIL_NUM = 5;

ViewerShot::ViewerShot( ) {
	_id = LoadGraph( "resource/image/weapon/Shot.png" );
	_num = 0;

	_model = ModelPtr( new Model );
	_model->alloc( 2 * MAX_NUM );
	_model->setTexture( _id );
}

ViewerShot::~ViewerShot( ) {
}

void ViewerShot::create( BulletPtr bullet ) {
	if ( _num >= MAX_NUM ) {
		return;
	}

	BulletShotPtr shot = std::dynamic_pointer_cast< BulletShot >( bullet );
	_pos[ _num ] = shot->getPos( );
	_vec[ _num ] = shot->getDir( );
	_num++;
}

void ViewerShot::draw( ) {
	Model::VERTEX vertex[ 4 ];
	vertex[ 0 ].u = 0;
	vertex[ 0 ].v = 0;
	vertex[ 1 ].u = 1;
	vertex[ 1 ].v = 1;
	vertex[ 2 ].u = 1;
	vertex[ 2 ].v = 0;
	vertex[ 3 ].u = 0;
	vertex[ 3 ].v = 1;

	Matrix mat = Matrix::makeTransformRotation( Vector( 0, 1, 0 ), PI / 2 );
	for ( int i = 0; i < _num; i++ ) {
		Vector vec = _vec[ i ].normalize( );
		vertex[ 2 ].pos = _pos[ i ] + vec * 0.1;
		vec = mat.multiply( vec );
		vertex[ 0 ].pos = _pos[ i ] + vec * 0.1;
		vec = mat.multiply( vec );
		vertex[ 3 ].pos = _pos[ i ] + vec * 2.0;
		vec = mat.multiply( vec );
		vertex[ 1 ].pos = _pos[ i ] + vec * 0.1;

		_model->set( i * 6 + 0, vertex[ 0 ] );
		_model->set( i * 6 + 1, vertex[ 2 ] );
		_model->set( i * 6 + 2, vertex[ 1 ] );
		_model->set( i * 6 + 3, vertex[ 0 ] );
		_model->set( i * 6 + 4, vertex[ 1 ] );
		_model->set( i * 6 + 5, vertex[ 3 ] );
	}
	_model->setPolygonNum( _num * 2 );

	SetDrawBlendMode( DX_BLENDMODE_ADD , 255 );
	_model->draw( );
	SetDrawBlendMode( DX_BLENDMODE_NOBLEND , 0 ) ;

	_num = 0;
}
