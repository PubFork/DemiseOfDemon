#include "ViewerExperiences.h"
#include "Experiences.h"
#include "Experience.h"
#include "DxLib.h"
#include "Stage.h"
#include "Framework.h"

const int WAIT_TIME = 1;
const int COUNT_TIME = 30;

ViewerExperiences::ViewerExperiences( ) {
	_id = LoadGraph( "resource/image/exp/exp.png" );
	for ( int i = 0; i < PARTICLE_NUM; i++ ) {
		_particle_count[ i ] = 0;
	}
	_index = 0;
	_wait = WAIT_TIME;
}

ViewerExperiences::~ViewerExperiences( ) {
}

void ViewerExperiences::draw( ) {
	SetDrawBlendMode( DX_BLENDMODE_ADD, 255 );
	FrameworkPtr fw = Framework::getInstance( );
	StagePtr stage = Stage::getTask( );
	ExperiencesPtr experiences = stage->getExperiences( );

	_wait = ( _wait + 1 ) % WAIT_TIME;

	for ( int i = 0; i < Experiences::MAX_NUM; i++ ) {
		ExperienceConstPtr experience = experiences->getExperience( i );
		if ( !experience ) {
			continue;
		}
		if ( fw->isOutterOfScreen( experience->getPos( ) ) ) {
			continue;
		}
		drawExperience( experience );

		if ( _wait == 0 ) {
			createParticle( experience->getPos( ) );
		}
	}

	drawParticle( );

	SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 0 );
}

void ViewerExperiences::drawExperience( ExperienceConstPtr experience ) {
	Vector p = experience->getPos( );
	DxLib::VECTOR pos = VGet( ( float )p.x, ( float )p.y, ( float )p.z );
	DrawBillboard3D( pos, 0.5f, 0.5f, 0.5f, 0, _id, TRUE );
}

void ViewerExperiences::createParticle( const Vector& center ) {
	_particle_count[ _index ] = COUNT_TIME;
	Matrix mat = Matrix::makeTransformRotation( Vector( 0, 1, 0 ), PI2 * ( rand( ) % 1000 ) / 1000 );
	Vector vec = mat.multiply( Vector( 0.20, 0, 0 ) );
	_particle_pos[ _index ] = center + vec;
	_index = ( _index + 1 ) % PARTICLE_NUM;
}

void ViewerExperiences::drawParticle( ) {
	for ( int i = 0; i < PARTICLE_NUM; i++ ) {
		if ( _particle_count[ i ] <= 0 ) {
			continue;
		}
		_particle_count[ i ]--;

		double ay = ( COUNT_TIME - _particle_count[ i ] );
		ay *= ay;
		ay /= 10000.0;
		_particle_pos[ i ].y -= ay;
		Vector p = _particle_pos[ i ];
		DxLib::VECTOR pos = VGet( ( float )p.x, ( float )p.y, ( float )p.z );
		
		SetDrawBlendMode( DX_BLENDMODE_ADD, _particle_count[ i ] * 255 / COUNT_TIME );
		DrawBillboard3D( pos, 0.5f, 0.5f, 0.1f, 0, _id, TRUE );
	}
}
