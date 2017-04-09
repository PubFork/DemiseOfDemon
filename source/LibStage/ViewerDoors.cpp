#include "ViewerDoors.h"
#include "mathmatics.h"
#include "Doors.h"
#include "Door.h"
#include "Framework.h"
#include "dxlib.h"
#include "Stage.h"

ViewerDoors::ViewerDoors( ) {
	init();
}


ViewerDoors::~ViewerDoors( ) {
}

void ViewerDoors::init( ) {
	int form_id = MV1LoadModel( "resource/object/door/door.mv1" );
	if ( form_id < 0 ) {
		return;
	}
	int num = MV1GetMaterialNum( form_id ) ;
	for ( int i = 0; i < num; i++ ) {
		MV1SetMaterialEmiColor( form_id, i, GetColorF( 1.0f, 1.0f, 1.0f, 1.0f ) );
	}

	for ( int i = 0; i < Doors::MAX_NUM; i++ ) {
		_data[ i ].body_id = MV1DuplicateModel( form_id );
		_data[ i ].anim_id = MV1AttachAnim( _data[ i ].body_id, 0, -1, FALSE ) ;
	}
}

void ViewerDoors::draw( ) {
	FrameworkPtr fw = Framework::getInstance( );
	StagePtr stage = Stage::getTask( );
	DoorsPtr doors = stage->getDoors( );

	int num = doors->getNum( );
	for ( int i = 0; i < num; i++ ) {
		DoorPtr door = doors->getDoor( i );

		double x = door->getX( ) + 0.5;
		double y = door->getY( );
		double z = door->getZ( ) + 0.5;
		double time = door->getTime( );
		Door::DIR dir = door->getDir( );

		if ( fw->isOutterOfScreen( Vector( x, y, z ) ) ) {
			continue;
		}

		float angle = 0;
		switch ( dir ) {
		case Door::DIR_MZ:
			angle = DX_PI_F * 0 / 2;
			break;
		case Door::DIR_PX:
			angle = DX_PI_F * 1 / 2;
			break;
		case Door::DIR_PZ:
			angle = DX_PI_F * 2 / 2;
			break;
		case Door::DIR_MX:
			angle = DX_PI_F * 3 / 2;
			break;
		}
		MATRIX mat_rot = MGetRotY( angle );

		// 座標変換
		MATRIX matrix = MGetIdent( );
		matrix = MMult( matrix, mat_rot );
		matrix = MMult( matrix, MGetScale( VGet( 0.009f, 0.009f, 0.009f ) ) );
		matrix = MMult( matrix, MGetTranslate( VGet( ( float )x, ( float )y, ( float )z ) ) );
		MV1SetMatrix( _data[ i ].body_id, matrix );

		// アニメーション時間設定
		MV1SetAttachAnimTime( _data[ i ].body_id, _data[ i ].anim_id, ( float )time );

		// ３Ｄモデルの描画
		MV1DrawModel( _data[ i ].body_id ) ;
	}
}
