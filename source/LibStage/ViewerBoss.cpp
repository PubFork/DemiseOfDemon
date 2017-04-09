#include "ViewerBoss.h"
#include "Party.h"
#include "Player.h"
#include "Boss.h"
#include "Framework.h"
#include "DxLib.h"

const float SCALE = 0.006f;

ViewerBoss::ViewerBoss( ) {
	createModel( );
	_title_id = LoadGraph( "resource/UI/title.png" );
}


ViewerBoss::~ViewerBoss( ) {
}

void ViewerBoss::draw(  ) {
	BossPtr boss = Boss::getTask( );
	Boss::MOTION motion = boss->getMotion( );

	if ( motion == Boss::MOTION_HIDE ) {
		FrameworkPtr fw = Framework::getInstance( );
		int w = fw->getScreenWidth( );
		int h = fw->getScreenHeight( );
		DrawGraph( ( w - 1900 ) / 2, ( h - 1140 ) / 2, _title_id, TRUE );
		return;
	}

	double time = boss->getTime( );
	Vector dir = boss->getDir( );;
	
	Vector pos( 50, -11, 56 );

	// 座標変換
	MATRIX matrix = MGetIdent( );
	matrix = MMult( matrix, MGetRotVec2( VGet( 0, 0, -1 ), VGet( ( float )dir.x, 0, ( float )dir.z ) ) );
	matrix = MMult( matrix, MGetScale( VGet( SCALE, SCALE, SCALE ) ) );
	matrix = MMult( matrix, MGetTranslate( VGet( ( float )pos.x, ( float )pos.y, ( float )pos.z ) ) );
	MV1SetMatrix( _id[ motion ].body_id, matrix );

	// アニメーション時間設定
	MV1SetAttachAnimTime( _id[ motion ].body_id, _id[ motion ].anim_id, ( float )time );

	// ３Ｄモデルの描画
	MV1DrawModel( _id[ motion ].body_id ) ;
}


void ViewerBoss::create( Boss::MOTION motion, const char * filename ) {
	// body
	int& id = _id[ motion ].body_id;
	id = MV1LoadModel( filename );
	if ( id < 0 ) {
		return;
	}
	int num = MV1GetMaterialNum( id ) ;
	for ( int i = 0; i < num; i++ ) {
		MV1SetMaterialEmiColor( id, i, GetColorF( 1.0f, 1.0f, 1.0f, 1.0f ) );
	}
	int& anim = _id[ motion ].anim_id;
	anim = MV1AttachAnim( id, 0, -1, FALSE ) ;
}

void ViewerBoss::createModel( ) {
	create( Boss::MOTION_WAIT, "resource/character/enemy/boss/boss_wait.mv1");
	create( Boss::MOTION_EXIT, "resource/character/enemy/boss/boss_exit.mv1");
	create( Boss::MOTION_APPEAR, "resource/character/enemy/boss/boss_appear.mv1");
	create( Boss::MOTION_ATTACK_CLEAVE, "resource/character/enemy/boss/boss_attack_cleave.mv1");
	create( Boss::MOTION_ATTACK_LOWHIT, "resource/character/enemy/boss/boss_attack_lowhit.mv1");
	create( Boss::MOTION_ATTACK_MAGIC, "resource/character/enemy/boss/boss_attack_magic.mv1");
}
