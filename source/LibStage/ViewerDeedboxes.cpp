#include "ViewerDeedboxes.h"
#include "dxlib.h"
#include "mathmatics.h"
#include "Deedboxes.h"
#include "Deedbox.h"
#include "Stage.h"

ViewerDeedboxes::ViewerDeedboxes( ) {
	init();
}


ViewerDeedboxes::~ViewerDeedboxes( ) {
}

void ViewerDeedboxes::init( ) {
	int form_id = MV1LoadModel( "resource/object/deedbox/deedbox.mv1" );
	if ( form_id < 0 ) {
		return;
	}
	int num = MV1GetMaterialNum( form_id ) ;
	for ( int i = 0; i < num; i++ ) {
		MV1SetMaterialEmiColor( form_id, i, GetColorF( 1.0f, 1.0f, 1.0f, 1.0f ) );
	}

	for ( int i = 0; i < Deedboxes::MAX_NUM; i++ ) {
		_data[ i ].body_id = MV1DuplicateModel( form_id );
		_data[ i ].anim_id = MV1AttachAnim( _data[ i ].body_id, 0, -1, FALSE ) ;
	}
}

void ViewerDeedboxes::draw( ) {
	StagePtr stage = Stage::getTask( );
	DeedboxesPtr deedboxes = stage->getDeedboxes( );

	int num = deedboxes->getNum( );
	for ( int i = 0; i < num; i++ ) {
		DeedboxPtr deedbox = deedboxes->getDeedbox( i );

		double x = deedbox->getX( ) + 0.5;
		double y = deedbox->getY( );
		double z = deedbox->getZ( ) + 0.5;
		double time = deedbox->getTime( );

		// 座標変換
		MATRIX matrix = MGetIdent( );
		matrix = MMult( matrix, MGetScale( VGet( 0.009f, 0.009f, 0.009f ) ) );
		matrix = MMult( matrix, MGetTranslate( VGet( ( float )x, ( float )y, ( float )z ) ) );
		MV1SetMatrix( _data[ i ].body_id, matrix );

		// アニメーション時間設定
		MV1SetAttachAnimTime( _data[ i ].body_id, _data[ i ].anim_id, ( float )time );

		// ３Ｄモデルの描画
		MV1DrawModel( _data[ i ].body_id ) ;
	}
}
