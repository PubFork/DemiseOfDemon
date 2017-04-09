#include "ViewerImpact.h"
#include "Model.h"
#include "BulletImpact.h"
#include "DxLib.h"

const int TAIL_NUM = 5;
const int IMPACT_NUM = 100;
const int IMPACT_TIME = 60;

ViewerImpact::ViewerImpact( ) {
	int id = LoadGraph( "resource/image/weapon/impact.png" );
	_model = ModelPtr( new Model );
	_model->alloc( IMPACT_NUM * 2 );
	_model->setTexture( id );
}

ViewerImpact::~ViewerImpact( ) {
}

void ViewerImpact::create( BulletPtr bullet ) {
	BulletImpactPtr impact = std::dynamic_pointer_cast< BulletImpact >( bullet );
	_alpha = impact->getAlpha( );
	
	double length = impact->getLength( );
	Vector pos = impact->getPos( );

	Model::VERTEX vertex[ 3 ];
	vertex[ 0 ].u = 0;
	vertex[ 0 ].v = 0;
	vertex[ 1 ].u = 1;
	vertex[ 1 ].v = 0;
	vertex[ 2 ].u = 1;
	vertex[ 2 ].v = 1;

	Vector v[ 3 ];
	v[ 0 ] = Vector( 0, 0.2, length );
	v[ 1 ] = Vector( -0.25, 0, length );
	v[ 2 ] = Vector( 0.25, 0, length );

	Matrix mat = Matrix::makeTransformRotation( Vector( 0, 1, 0 ), PI2 / IMPACT_NUM );
	for ( int i = 0; i < IMPACT_NUM; i++ ) {
		for ( int j = 0; j < 3; j++ ) {
			v[ j ] = mat.multiply( v[ j ] );
			vertex[ j ].pos = v[ j ] + pos;
		}
		_model->set( i * 6 + 0, vertex[ 0 ] );
		_model->set( i * 6 + 1, vertex[ 1 ] );
		_model->set( i * 6 + 2, vertex[ 2 ] );
		_model->set( i * 6 + 3, vertex[ 0 ] );
		_model->set( i * 6 + 4, vertex[ 2 ] );
		_model->set( i * 6 + 5, vertex[ 1 ] );
	}
}

void ViewerImpact::draw( ) {
	SetDrawBlendMode( DX_BLENDMODE_ADD , _alpha );
	_model->draw( );
	SetDrawBlendMode( DX_BLENDMODE_NOBLEND , 0 );
}
