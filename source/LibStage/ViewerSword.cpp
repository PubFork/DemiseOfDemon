#include "ViewerSword.h"
#include "Model.h"
#include "BulletSword.h"
#include "DxLib.h"

const int ACCURACY = 10;

ViewerSword::ViewerSword( ) {
	int id = LoadGraph( "resource/image/weapon/sword.png" );
	for ( int i = 0; i < MAX_NUM; i++ ) {
		_model[ i ] = ModelPtr( new Model );
		_model[ i ]->alloc( ACCURACY );
		_model[ i ]->setTexture( id );
	}
	_num = 0;
}

ViewerSword::~ViewerSword( ) {
}

void ViewerSword::create( BulletPtr bullet ) {
	if ( _num >= MAX_NUM ) {
		return;
	}

	BulletSwordPtr sword = std::dynamic_pointer_cast< BulletSword >( bullet );
	
	const Vector pos = sword->getPos( );


	const Vector begin_dir = sword->getBeginDir( );
	const Vector end_dir = sword->getEndDir( );
	const double length = sword->getLength( );

	Model::VERTEX origin;
	origin.pos = pos;
	origin.u = 1;
	origin.v = 1;
	
	Model::VERTEX vertex0;
	Model::VERTEX vertex1;
	double angle = begin_dir.angle( end_dir ) / ACCURACY;
	Matrix mat = Matrix::makeTransformRotation( Vector( 0, -1, 0 ), angle );

	vertex0.pos = begin_dir.normalize( ) * length + pos;
	vertex0.u = 0;
	vertex0.v = 0;
	for ( int i = 0; i < ACCURACY; i++ ) {
		vertex1.pos = mat.multiply( vertex0.pos - pos ) + pos;
		vertex1.u = 1.0 * ( i + 1 ) / ACCURACY;
		vertex1.v = 0;

		_model[ _num ]->set( i * 3 + 0, origin );
		_model[ _num ]->set( i * 3 + 1, vertex0 );
		_model[ _num ]->set( i * 3 + 2, vertex1 );

		vertex0 = vertex1;
	}

	_num++;
}

void ViewerSword::draw( ) {
    SetDrawBlendMode( DX_BLENDMODE_ADD , 255 ) ;
	for ( int i = 0; i < _num; i++ ) {
		_model[ i ]->draw( true );
	}
    // 描画ブレンドモードをノーブレンドにする
    SetDrawBlendMode( DX_BLENDMODE_NOBLEND , 0 ) ;

	_num = 0;
}
