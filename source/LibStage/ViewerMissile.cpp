#include "ViewerMissile.h"
#include "BulletMissile.h"
#include "DxLib.h"

const int TAIL_NUM = 5;

ViewerMissile::ViewerMissile( ) {
	_id = LoadGraph( "resource/image/weapon/missile.png" );
	_num = 0;
}

ViewerMissile::~ViewerMissile( ) {
}

void ViewerMissile::create( BulletPtr bullet ) {
	if ( _num >= MAX_NUM ) {
		return;
	}

	BulletMissilePtr missile = std::dynamic_pointer_cast< BulletMissile >( bullet );
	_pos[ _num ] = missile->getPos( );
	_num++;
}

void ViewerMissile::draw( ) {
	for ( int i = 0; i < _num; i++ ) {
		DrawBillboard3D(
			VGet( ( float )_pos[ i ].x, ( float )_pos[ i ].y, ( float )_pos[ i ].z ),
			0.5f, 0.5f, 0.5f, 0, _id, TRUE ) ;
	}

	_num = 0;
}
