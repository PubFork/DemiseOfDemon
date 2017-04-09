#include "ViewerArrow.h"
#include "Model.h"
#include "BulletArrow.h"
#include "DxLib.h"
#include "Stage.h"
#include "Camera.h"

const int DIV = 20;
const int TIME = 60;
const double GRAVITY = 0.001;
const double POWER = 0.02;

ViewerArrow::ViewerArrow( ) {
	_id = LoadGraph( "resource/image/weapon/arrow.png" );
	_id_particle = LoadGraph( "resource/image/weapon/arrow_particle.png" );
	_num = 0;
	_idx = 0;

	for ( int i = 0; i < PARTICLE_NUM; i++ ) {
		_particle[ i ].count = 0;
	}
}

ViewerArrow::~ViewerArrow( ) {
}

void ViewerArrow::create( BulletPtr bullet ) {
	if ( _num >= MAX_NUM ) {
		return;
	}

	StagePtr stage = Stage::getTask( );
	CameraPtr camera = stage->getCamera( );

	BulletArrowPtr arrow = std::dynamic_pointer_cast< BulletArrow >( bullet );
	_data[ _num ].pos = arrow->getPos( );
	_data[ _num ].priority = ( _data[ _num ].pos - camera->getEye( ) ).getLength( );

	_particle[ _idx ].count = TIME;
	Vector v(
		rand( ) % 10000 / 10000.0 - 0.5,
		rand( ) % 10000 / 10000.0 * 0.5,
		rand( ) % 10000 / 10000.0 - 0.5 );
	_particle[ _idx ].vec = v.normalize( ) * POWER;
	_particle[ _idx ].pos = _data[ _num ].pos;
		
	_num++;
	_idx = ( _idx + 1 ) % PARTICLE_NUM;

	for ( int i = 0; i < _num - 1; i++ ) {
		for ( int j = i + 1; j < _num; j++ ) {
			if ( _data[ i ].priority < _data[ j ].priority ) {
				DATA temp = _data[ i ];
				_data[ i ] = _data[ j ];
				_data[ j ] = temp;
			}
		}
	}
}

void ViewerArrow::draw( ) {
	for ( int i = 0; i < PARTICLE_NUM; i++ ) {
		if ( _particle[ i ].count <= 0 ) {
			continue;
		}
		_particle[ i ].count--;
		_particle[ i ].vec.y -= GRAVITY;
		_particle[ i ].pos += _particle[ i ].vec;
	}


	for ( int i = 0; i < PARTICLE_NUM; i++ ) {
		if ( _particle[ i ].count <= 0 ) {
			continue;
		}
		Vector pos = _particle[ i ].pos;
		SetDrawBlendMode( DX_BLENDMODE_ADD, 255 * _particle[ i ].count / TIME );
		DrawBillboard3D(
			VGet( ( float )pos.x, ( float )pos.y, ( float )pos.z ),
			0.5f, 0.5f, 0.05f, 0, _id_particle, TRUE ) ;
	}
	
	SetDrawBlendMode( DX_BLENDMODE_ADD, 255 );
	for ( int i = 0; i < _num; i++ ) {
		Vector pos = _data[ i ].pos;
		DrawBillboard3D(
			VGet( ( float )pos.x, ( float )pos.y, ( float )pos.z ),
			0.5f, 0.5f, 0.5f, 0, _id, TRUE ) ;
	}
	SetDrawBlendMode( DX_BLENDMODE_NOBLEND , 0 ) ;

	_num = 0;
}
