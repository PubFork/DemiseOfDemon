#include "ViewerUI.h"
#include "ImageBase.h"
#include "Network.h"
#include "Framework.h"
#include "Client.h"
#include "DxLib.h"
#include "Status.h"
#include "Sound.h"

// W = "WIDTH", H = "HEIGHT", I = "INTERVAL"

// DRAW STATUS
const int WINDOW_W = 1280;
const int WINDOW_H = 1024;
const int STATUS_POS_X = 0;
const int STATUS_POS_Y = 0;
const int PLAYER_W = 400;
const int PLAYER_H = 64;
const int PLATER_POS_X = 0;
const int PLAYER_POS_Y = -50;
const int PITCH_X = 30;
const int PITCH_Y = 24;
// DRAW LIFE
const int LIFE_W = 512;
const int LIFE_H = 512;
const int LIFE_POS_X = PITCH_X * 2;
const int LIFE_POS_Y = PLAYER_H + PITCH_Y * 5;
const int LIFE_LEVEL_W = 128;
const int LIFE_LEVEL_H = 128;
const int LIFE_LEVEL_POS_X = LIFE_POS_X + LIFE_W - LIFE_LEVEL_W - 100;
const int LIFE_LEVEL_POS_Y = LIFE_POS_Y + 50;
const int EXP_GAUGE_W = 450;
const int EXP_GAUGE_H = 96;
const int EXP_GAUGE_X = LIFE_POS_X;
const int EXP_GAUGE_Y = LIFE_LEVEL_POS_Y + LIFE_LEVEL_H - 75;
const int CURRENT_LIFE_W = 570;
const int CURRENT_LIFE_H = 144;
const int CURRENT_LIFE_POS_X = LIFE_POS_X;
const int CURRENT_LIFE_POS_Y = EXP_GAUGE_Y + EXP_GAUGE_H + PITCH_Y + 10;
const int CURRENT_LIFE_NUMBER_W = 64;
const int CURRENT_LIFE_NUMBER_H = 128;
const int CURRENT_LIFE_NUMBER_I = 0;
const int CURRENT_LIFE_NUMBER_POS_X = CURRENT_LIFE_POS_X + CURRENT_LIFE_W - ( int )( CURRENT_LIFE_NUMBER_W * 4.5 );
const int CURRENT_LIFE_NUMBER_POS_Y = CURRENT_LIFE_POS_Y;
const int MAX_LIFE_W = 16;
const int MAX_LIFE_H = 64;
const int MAX_LIFE_I = 0;
const int MAX_LIFE_POS_X = LIFE_POS_X + LIFE_W - PITCH_X * 5 - MAX_LIFE_W + 5;
const int MAX_LIFE_POS_Y = CURRENT_LIFE_POS_Y + CURRENT_LIFE_H + PITCH_Y + 30;
// DRAW WEAPON
const int WEAPON_W = 256;
const int WEAPON_H = 300;
const int WEAPON_POS_X = LIFE_POS_X;
const int WEAPON_POS_Y = LIFE_POS_Y + LIFE_H;
const int WEAPON_LEVEL_W = 128;
const int WEAPON_LEVEL_H = 128;
const int WEAPON_LEVEL_POS_X = WEAPON_POS_X + LIFE_W / 2 - WEAPON_LEVEL_W / 2;
const int WEAPON_LEVEL_POS_Y = WEAPON_POS_Y + WEAPON_H / 2 - WEAPON_LEVEL_H / 2;
// DRAW CHARA
const int CHARA_W = 1280;
const int CHARA_H = 896;
const int CHARA_POS_X = STATUS_POS_X;
const int CHARA_POS_Y = STATUS_POS_Y + PITCH_Y * 3;
// DRAW STAGE
const int STAGE_W = 256;
const int STAGE_H = 512;
const int STAGE_I = 62;
const int STAGE_POS_X = WINDOW_W - STAGE_W - PITCH_X * 2;
const int STAGE_POS_Y = WEAPON_H + PLAYER_H + PITCH_Y;
const int STAGE_STATE_POS_X = STAGE_POS_X + PITCH_X * 2 + 5;
const int STAGE_CAMP_Y = STAGE_POS_Y + 330;
const int STAGE_LABYRINTH_Y = STAGE_CAMP_Y - STAGE_I;
const int STAGE_DEATHPARADE_Y = STAGE_LABYRINTH_Y - STAGE_I;
const int STAGE_BOSS_Y = STAGE_DEATHPARADE_Y - STAGE_I;
// DRAW GAUGE
const int GAUGE_W = 1024;
const int GAUGE_H = 1024;

//Fade wait time
const int FADE_TIME = 60;

ViewerUI::ViewerUI( int index ) //IMAGE_FILEを呼び込む
: _index( index ) {
	_image = ImageBasePtr( new ImageBase );
	// WINDOW
	_window[ 0 ] = _image->load( "resource/status/statewindow1.png" );
	_window[ 1 ] = _image->load( "resource/status/statewindow2.png" );
	_window[ 2 ] = _image->load( "resource/status/statewindow3.png" );
	_window[ 3 ] = _image->load( "resource/status/statewindow4.png" );
	_window[ 4 ] = _image->load( "resource/status/statewindow5.png" );
	_window[ 5 ] = _image->load( "resource/status/statewindow6.png" );

	// BASE
	_base[ 0 ] = _image->load( "resource/status/statebase1.png" );
	_base[ 1 ] = _image->load( "resource/status/statebase2.png" );
	_base[ 2 ] = _image->load( "resource/status/statebase3.png" );
	_base[ 3 ] = _image->load( "resource/status/statebase4.png" );
	_base[ 4 ] = _image->load( "resource/status/statebase5.png" );
	_base[ 5 ] = _image->load( "resource/status/statebase6.png" );

	// PLAYER & IMAGE
	switch( index ) {
		case PLAYER_KNIGHT:
			_player = _image->load( "resource/status/knight.png" );
			_player_img= _image->load( "resource/status/knightchara.png" );
			break;
		case PLAYER_WARRIOR:
			_player = _image->load( "resource/status/warrior.png" );
			_player_img= _image->load( "resource/status/warriorchara.png" );
			break;
		case PLAYER_MAGE:
			_player = _image->load( "resource/status/mage.png" );
			_player_img= _image->load( "resource/status/magechara.png" );
			break;
		case PLAYER_MONK:
			_player = _image->load( "resource/status/monk.png" );
			_player_img= _image->load( "resource/status/monkchara.png" );
			break;
		case PLAYER_SHOOTER:
			_player = _image->load( "resource/status/shooter.png" );
			_player_img= _image->load( "resource/status/shooterchara.png" );
			break;
		case PLAYER_SUMMONER:
			_player = _image->load( "resource/status/summoner.png" );
			_player_img= _image->load( "resource/status/summonerchara.png" );
			break;
		default:
		break; 
	}

	// STAGE
	_stage[ STAGE_WINDOW ] = _image->load( "resource/status/stagewindow.png" );
	_stage[ STAGE_ICON ] = _image->load( "resource/status/stageicon.png" );

	// LEVEL
	_level[ LEVEL_0 ] = _image->load( "resource/status/lv0.png" );
	_level[ LEVEL_1 ] = _image->load( "resource/status/lv1.png" );
	_level[ LEVEL_2 ] = _image->load( "resource/status/lv2.png" );
	_level[ LEVEL_3 ] = _image->load( "resource/status/lv3.png" );
	_level[ LEVEL_4 ] = _image->load( "resource/status/lv4.png" );
	_level[ LEVEL_5 ] = _image->load( "resource/status/lv5.png" );
	_level[ LEVEL_6 ] = _image->load( "resource/status/lv6.png" );
	_level[ LEVEL_7 ] = _image->load( "resource/status/lv7.png" );
	_level[ LEVEL_8 ] = _image->load( "resource/status/lv8.png" );
	_level[ LEVEL_9 ] = _image->load( "resource/status/lv9.png" );

	// NUMBER
	_number[ A ][ NUMBER_0 ] = _image->load( "resource/status/lifenumber0a.png" );
	_number[ A ][ NUMBER_1 ] = _image->load( "resource/status/lifenumber1a.png" );
	_number[ A ][ NUMBER_2 ] = _image->load( "resource/status/lifenumber2a.png" );
	_number[ A ][ NUMBER_3 ] = _image->load( "resource/status/lifenumber3a.png" );
	_number[ A ][ NUMBER_4 ] = _image->load( "resource/status/lifenumber4a.png" );
	_number[ A ][ NUMBER_5 ] = _image->load( "resource/status/lifenumber5a.png" );
	_number[ A ][ NUMBER_6 ] = _image->load( "resource/status/lifenumber6a.png" );
	_number[ A ][ NUMBER_7 ] = _image->load( "resource/status/lifenumber7a.png" );
	_number[ A ][ NUMBER_8 ] = _image->load( "resource/status/lifenumber8a.png" );
	_number[ A ][ NUMBER_9 ] = _image->load( "resource/status/lifenumber9a.png" );
															  
	_number[ B ][ NUMBER_0 ] = _image->load( "resource/status/lifenumber0b.png" );
	_number[ B ][ NUMBER_1 ] = _image->load( "resource/status/lifenumber1b.png" );
	_number[ B ][ NUMBER_2 ] = _image->load( "resource/status/lifenumber2b.png" );
	_number[ B ][ NUMBER_3 ] = _image->load( "resource/status/lifenumber3b.png" );
	_number[ B ][ NUMBER_4 ] = _image->load( "resource/status/lifenumber4b.png" );
	_number[ B ][ NUMBER_5 ] = _image->load( "resource/status/lifenumber5b.png" );
	_number[ B ][ NUMBER_6 ] = _image->load( "resource/status/lifenumber6b.png" );
	_number[ B ][ NUMBER_7 ] = _image->load( "resource/status/lifenumber7b.png" );
	_number[ B ][ NUMBER_8 ] = _image->load( "resource/status/lifenumber8b.png" );
	_number[ B ][ NUMBER_9 ] = _image->load( "resource/status/lifenumber9b.png" );

	// LIFE
	_life[ LIFE_WINDOW ] = _image->load( "resource/status/lifewindow.png" );

	// WEAPON
	_weapon[ WEAPON_WINDOW ] = _image->load( "resource/status/weaponwindow.png" );
	_weapon[ WEAPON_ICON ] = _image->load( "resource/status/weaponicon.png" );

	// EXP
	_exp[ EXP_GAUGE ] = _image->load( "resource/status/expgauge.png" );

	// CURRENT LIFE
	_current_life[ CURRENT_LIFE_BASE ] = _image->load( "resource/status_dummy/numberabase.png" ); //We need to change filename
	_current_life[ CURRENT_LIFE_WINDOW ] = _image->load( "resource/status_dummy/numberawindow.png" ); //We need to change filename

	//WELCOME
	_welcome = _image->load( "resource/status_dummy/ready.png" );

	// GAUGE
	_gauge_img = _image->load( "resource/status_dummy/summon.png" );

	// CLEAR
	_clear = _image->load( "resource/status_dummy/clear.png" );

	// DEAD
	_dead = _image->load( "resource/status_dummy/dead.png" );

	FrameworkPtr fw = Framework::getInstance( );
	_max_exp[ 0 ] = ( int )fw->getSettingValue( "PLAYER_MAX_EXP_Lv0" );
	_max_exp[ 1 ] = ( int )fw->getSettingValue( "PLAYER_MAX_EXP_Lv1" );
	_max_exp[ 2 ] = ( int )fw->getSettingValue( "PLAYER_MAX_EXP_Lv2" );
	_max_exp[ 3 ] = ( int )fw->getSettingValue( "PLAYER_MAX_EXP_Lv3" );
	_max_exp[ 4 ] = ( int )fw->getSettingValue( "PLAYER_MAX_EXP_Lv4" );
	_max_exp[ 5 ] = ( int )fw->getSettingValue( "PLAYER_MAX_EXP_Lv5" );
	_max_exp[ 6 ] = ( int )fw->getSettingValue( "PLAYER_MAX_EXP_Lv6" );
	_max_exp[ 7 ] = ( int )fw->getSettingValue( "PLAYER_MAX_EXP_Lv7" );
	_max_exp[ 8 ] = ( int )fw->getSettingValue( "PLAYER_MAX_EXP_Lv8" );
}

ViewerUI::~ViewerUI( ) {
}

void ViewerUI::draw( ) {
	ScreenFlip( );
	ClearDrawScreen( );

	ClientPtr client = Client::getTask( );
	CLIENTDATA data = client->getClientData( );

	//data.user[ _index ].state = STATE_CAMP;
	// 状態
	switch ( data.user[ _index ].state ) {
	case STATE_NONE:
		drawStateNone( );
		break;
	case STATE_READY:
		_timer = 0;
		drawStateReady( data );
		break;
	case STATE_CLEAR:
		drawStateClear( data );
		break;
	case STATE_DEAD:
		drawStateDead( data );
		break;
	default:
		drawStatePlay( data );
	}
}

void ViewerUI::drawDebugString( const char * format, ... ) {
	const int COLOR = 0xffffff;

	va_list args;
	va_start( args, format );
	char str[ 255 ];
	vsprintf_s( str, 255, format, args );
	DrawFormatString( 0, _line * 20, COLOR, str );
	_line++;
}

void ViewerUI::drawStateNone( ) {
	ClientPtr client = Client::getTask( );
	CLIENTDATA status = client->getClientData( );

	 _line = 0;

	{	// サーバーIP表示
		IP ip = client->getServerIP( );
		drawDebugString( "サーバーIP : %d %d %d %d",
			ip.address[ 0 ], ip.address[ 1 ],ip.address[ 2 ],ip.address[ 3 ] );
	}

	
	{	// クライアントIP表示
		IP ip = client->getClientIP( );
		drawDebugString( "クライアントIP : %d %d %d %d",
			ip.address[ 0 ], ip.address[ 1 ],ip.address[ 2 ],ip.address[ 3 ] );
	}

	{	// 状態
		std::string str = client->getPhase( );
		drawDebugString( "状態 : %s", str.c_str( ) );
	}
	
	drawDebugString( "user   dx   dy button   state level life" );
	for ( int i = 0; i < PLAYER_NUM; i++ ) {
		drawDebugString( "% 4d % 4d % 4d %s % 5x % 5d % 10u",
			i,
			status.user[ i ].x, status.user[ i ].y,
			BTOS( status.user[ i ].button ).c_str( ),
			status.user[ i ].state,
			status.user[ i ].level_weapon,
			status.user[ i ].life );
	}
}


void ViewerUI::drawStateReady( const CLIENTDATA& data ) {
	StatusPtr status = Status::getTask( );
	drawStatus( data );
	int now_gauge = ( int )( GAUGE_H * ( status->getGauge( ) / 100.0 ) );
	if ( data.user[ _index ].button == 0 ) {
		DrawGraph( 0, 0, _welcome, TRUE );
	} else {
		DrawRectGraph( 0, GAUGE_H - now_gauge, 0, GAUGE_H - now_gauge, GAUGE_W, now_gauge, _gauge_img, TRUE, FALSE );
	}
}


void ViewerUI::drawStateClear( const CLIENTDATA& data ) {
	StatusPtr status = Status::getTask( );
	drawStatus( data );
	int alpha = 255;
	if ( _timer <= FADE_TIME ) {
		alpha = 255 * _timer / FADE_TIME;
	}

	SetDrawBlendMode( DX_BLENDMODE_ALPHA, alpha );
	DrawGraph( 0, 0, _clear, TRUE );
	SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 0 );
	_timer ++;
}

void ViewerUI::drawStateDead( const CLIENTDATA& data ) {
	StatusPtr status = Status::getTask( );
	drawStatus( data );
	int alpha = 255;
	if ( _timer <= FADE_TIME ) {
		alpha = 255 * _timer / FADE_TIME;
	}

	SetDrawBlendMode( DX_BLENDMODE_ALPHA, alpha );
	DrawGraph( 0, 0, _dead, TRUE );
	SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 0 );
	_timer ++;
}

void ViewerUI::drawStatePlay( const CLIENTDATA& data ) {
	drawStatus( data );
	drawLife( data );
	drawWeapon( data );
	drawStage( data );
}

//全体画面 WINDOW & BASE & PLAYER NAME & CHARA IMAGE
void ViewerUI::drawStatus( const CLIENTDATA& data ) {
	DrawGraph( STATUS_POS_X, STATUS_POS_Y, _base[ data.user[ _index ].level_weapon ], TRUE );
	drawChara( data );
	DrawGraph( STATUS_POS_X, STATUS_POS_Y, _window[ data.user[ _index ].level_weapon ], TRUE );
	DrawGraph( PLATER_POS_X, PLAYER_POS_Y, _player, TRUE );
}

//CHARA IMAGE in 全体画面
void ViewerUI::drawChara( const CLIENTDATA& data ) {
	DrawGraph( CHARA_POS_X, CHARA_POS_Y, _player_img, TRUE );
}

//LIFE 画面
void ViewerUI::drawLife( const CLIENTDATA& data ) {
	//LIFE
	DrawGraph( LIFE_POS_X, LIFE_POS_Y, _life[ LIFE_WINDOW ], TRUE );
	//LIFE LEVEL
	DrawGraph( LIFE_LEVEL_POS_X, LIFE_LEVEL_POS_Y, _level[ data.user[ _index ].level_life ], TRUE );
	//EXP
	drawExp( data );
	//CURRENT LIFE
	drawCurrentLife( data );
	//MAX LIFE
	drawMaxLife( data );
}

//経験値 in LIFE 画面
void ViewerUI::drawExp( const CLIENTDATA& data ) {
	
	if ( data.user[ _index ].level_life == LIFE_LEVEL_MAX ) {
		_now_exp = EXP_GAUGE_W;
	} else {
		_now_exp = EXP_GAUGE_W * data.user[ _index ].exp / _max_exp[ data.user[ _index ].level_life ];
	}
	DrawRectGraph( EXP_GAUGE_X, EXP_GAUGE_Y, 0, 0, _now_exp, EXP_GAUGE_H, _exp[ EXP_GAUGE ], TRUE, FALSE );
}

//残りHP in LIFE 画面
void ViewerUI::drawCurrentLife( const CLIENTDATA& data ) {
	unsigned int life = data.user[ _index ].life;
		if ( life > 0 ) {
			int digit = ( int )log10( ( double )life ) + 1;
			for ( int i = 0; i < digit; i++ ) { 
				int num = life % 10;
				life /= 10;
				DrawGraph( CURRENT_LIFE_NUMBER_POS_X - ( CURRENT_LIFE_NUMBER_W + CURRENT_LIFE_NUMBER_I ) * i, CURRENT_LIFE_NUMBER_POS_Y, _number[ A ][ num ], TRUE );
			}
		} else {
			DrawGraph( CURRENT_LIFE_NUMBER_POS_X, CURRENT_LIFE_NUMBER_POS_Y, _number[ A ][ NUMBER_0 ], TRUE );
	}
}

//最大HP in LIFE 画面
void ViewerUI::drawMaxLife( const CLIENTDATA& data ) {
	unsigned int life = data.user[ _index ].level_life * 1111;
		if ( life > 0 ) {
			int digit = ( int )log10( ( double )life ) + 1;
			for ( int i = 0; i < digit; i++ ) { 
				int num = life % 10;
				life /= 10;
				DrawGraph( MAX_LIFE_POS_X - ( MAX_LIFE_W + MAX_LIFE_I ) * i, MAX_LIFE_POS_Y, _number[ B ][ num ], TRUE );
			}
		} else {
			DrawGraph( MAX_LIFE_POS_X, MAX_LIFE_POS_Y, _number[ B ][ NUMBER_0 ], TRUE );
	}
}

//ウェポンレベル描画
void ViewerUI::drawWeapon( const CLIENTDATA& data ) {
	DrawGraph( WEAPON_POS_X, WEAPON_POS_Y, _weapon[ WEAPON_WINDOW ], TRUE );

	int level = data.user[ _index ].level_weapon;
	for ( int i = 0; i < level; i++  ) {
		int x = WEAPON_POS_X + WEAPON_LEVEL_W / 2 + PITCH_X + WEAPON_W / ( level + 1 ) * ( i + 1 );
		int y = WEAPON_LEVEL_POS_Y + PITCH_Y;
		DrawGraph( x, y, _weapon[ WEAPON_ICON ], TRUE );
	}

	DrawGraph( WEAPON_LEVEL_POS_X, WEAPON_LEVEL_POS_Y, _level[ data.user[ _index ].level_weapon ], TRUE );
}


//STAGE 画面
void ViewerUI::drawStage( const CLIENTDATA& data ) {
	DrawGraph( STAGE_POS_X, STAGE_POS_Y, _stage[ STAGE_WINDOW ], TRUE );

	// プレイ中のエリア
	switch ( data.user[ _index ].state ) {
		case STATE_CAMP:
			DrawGraph( STAGE_STATE_POS_X, STAGE_CAMP_Y, _stage[ STAGE_ICON ], TRUE );
			break;
		case STATE_LABYRINTH:
			DrawGraph( STAGE_STATE_POS_X, STAGE_LABYRINTH_Y, _stage[ STAGE_ICON ], TRUE );
			break;
		case STATE_DEATHPARADE:
			DrawGraph( STAGE_STATE_POS_X, STAGE_DEATHPARADE_Y, _stage[ STAGE_ICON ], TRUE );
			break;
		case STATE_BOSS:
			DrawGraph( STAGE_STATE_POS_X, STAGE_BOSS_Y, _stage[ STAGE_ICON ], TRUE );
			break;
		default:
			break;
	}
}