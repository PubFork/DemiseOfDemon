#include "Console.h"
#include "DxLib.h"
#include "Device.h"
#include "Server.h"
#include "Framework.h"
#include "Network.h"

//Console
const int PITCH = 20;
const int COLOR = 0xffffff;
const int SCREEN_SIZE_X = 1024;
const int SCREEN_SIZE_Y = 768;
const int FRAME_R = STATUS_NUM + 1;
const int FRAME_X = PITCH;
const int FRAME_Y = PITCH;
const int FRAME_H = 320;
const int FRAME_PLAYER_X = FRAME_X;
const int FRAME_PLAYER_W = 130;
const int FRAME_PLAYER_DEVICE_X = FRAME_PLAYER_X + FRAME_PLAYER_W;
const int FRAME_PLAYER_DEVICE_W = 60;
const int FRAME_DIR_X = FRAME_PLAYER_DEVICE_X + FRAME_PLAYER_DEVICE_W;
const int FRAME_DIR_W = 150;
const int FRAME_BUTTON_X = FRAME_DIR_X + FRAME_DIR_W;
const int FRAME_BUTTON_W = 130;
const int FRAME_STATE_X = FRAME_BUTTON_X + FRAME_BUTTON_W;
const int FRAME_STATE_W = 110;
const int FRAME_WEAPON_X = FRAME_STATE_X + FRAME_STATE_W;
const int FRAME_WEAPON_W = 80;
const int FRAME_LEVEL_LIFE_X = FRAME_WEAPON_X + FRAME_WEAPON_W;
const int FRAME_LEVEL_LIFE_W = 60;
const int FRAME_EXPERIENCE_X = FRAME_LEVEL_LIFE_X + FRAME_LEVEL_LIFE_W;
const int FRAME_EXPERIENCE_W = 60;
const int FRAME_LIFE_X = FRAME_EXPERIENCE_X + FRAME_EXPERIENCE_W;
const int FRAME_LIFE_W = 180;
const int FRAME_W = ( FRAME_LIFE_X + FRAME_LIFE_W ) - FRAME_PLAYER_X;//フレームの幅
const int FRAME_CONNECT_R = MAX_MACHINE + 1;
const int FRAME_CONNECT_X = FRAME_X;
const int FRAME_CONNECT_Y = FRAME_H + PITCH * 2;
const int FRAME_CONNECT_W = 200;
const int FRAME_CONNECT_H = SCREEN_SIZE_Y - FRAME_CONNECT_Y - PITCH;

const int FRAME_DEVICE_X = FRAME_CONNECT_X + FRAME_CONNECT_W + PITCH;
const int FRAME_DEVICE_Y = FRAME_H + PITCH + PITCH;
const int FRAME_DEVICE_W = FRAME_CONNECT_W;
const int FRAME_DEVICE_H = 150;

const int FRAME_BGM_X = FRAME_CONNECT_X + FRAME_CONNECT_W + PITCH;
const int FRAME_BGM_Y = FRAME_H + PITCH * 4;
const int FRAME_BGM_W = FRAME_CONNECT_W;
const int FRAME_BGM_H = 150;

const int FRAME_STAGE_R = STAGE_MAX + 1;
const int FRAME_STAGE_X = FRAME_CONNECT_X + FRAME_CONNECT_W + PITCH;
const int FRAME_STAGE_Y = FRAME_H + PITCH * 6;
const int FRAME_STAGE_W = FRAME_CONNECT_W;
const int FRAME_STAGE_H = 80;

const int FRAME_COMMAND_X = FRAME_DEVICE_X + FRAME_CONNECT_W + PITCH;
const int FRAME_COMMAND_W = SCREEN_SIZE_X - FRAME_COMMAND_X - PITCH;
const int FRAME_COMMAND_H = 30; // 固定値
const int FRAME_COMMAND_Y = SCREEN_SIZE_Y - PITCH - FRAME_COMMAND_H;
const int FRAME_COMMAND_TITLE_W = 80;
const int FRAME_LOG_X = FRAME_COMMAND_X;
const int FRAME_LOG_Y = FRAME_Y + FRAME_H + PITCH;
const int FRAME_LOG_W = SCREEN_SIZE_X - FRAME_LOG_X - PITCH;
const int FRAME_LOG_H = FRAME_COMMAND_Y - FRAME_LOG_Y - PITCH;

const int BOX_DIR_STRING_Y = 2;
//Command
const char * COM_DEVICE = "device";
const char * COM_IP     = "ip";
const char * COM_STATE = "state";
const char * COM_LVWEAPON = "lvweapon";
const char * COM_LVLIFE= "lvlife";
const char * COM_LIFE = "life";
const char * COM_BGM = "bgm";
const char * COM_LVSTAGE = "stage";

ConsolePtr Console::getTask( ) {
	FrameworkPtr fw = Framework::getInstance( );
	return std::dynamic_pointer_cast< Console >( fw->getTask( Console::getTag( ) ) );
}

Console::Console( ) {
}

Console::~Console( ) {
}

void Console::update( ) {
	// 画面更新
	ScreenFlip( );
	ClearDrawScreen( );
	//Command
	updateCommand( );
	//Console
	drawStatus( );
	drawStatusDevice( );
	drawBgm( );
	drawStage( );
	drawConnect( );
	drawLog( );
	drawCommand( );
}

void Console::drawStatus( ) {
	ServerPtr server = Server::getTask( );
	_data = server->getData( );

	drawStatusFrame( );
	for ( int num = 0; num < STATUS_NUM; num++ ) {
		drawStatusPlayer( num );
		drawStatusPlayerDevice( num );
		drawStatusDir( num );
		drawStatusButton( num );
		drawStatusState( num );
		drawStatusLevelWeapon( num );
		drawStatusLevelLife( num );
		drawStatusExperience( num );
		drawStatusLife( num );
	}
}

void Console::drawStatusFrame( ) {
	// 外枠
	drawBox( FRAME_X, FRAME_Y, FRAME_X + FRAME_W, FRAME_Y + FRAME_H );

	// 横線
	for ( int num = 1; num < FRAME_R; num++ ) {
		int x1 = FRAME_X;
		int y1 = FRAME_Y + FRAME_H * num / FRAME_R;
		int x2 = FRAME_X + FRAME_W;
		int y2 = y1;
		drawLine( x1, y1, x2, y2 );
	}
}

void Console::drawStatusPlayer( int num ) {
	// デバイス
	{	// 区切り線
		int x1 = FRAME_PLAYER_X + FRAME_PLAYER_W;
		int y1 = FRAME_Y;
		int x2 = x1;
		int y2 = y1 + FRAME_H;
		drawLine( x1, y1, x2, y2 );
	}
	{	// タイトル
		int x = FRAME_PLAYER_X + 5;
		int y = FRAME_Y + BOX_DIR_STRING_Y;
		drawString( x, y, "Player" );
	}
	int x = FRAME_PLAYER_X + 5;
	int y = FRAME_Y + FRAME_H * ( num + 1 ) / FRAME_R + BOX_DIR_STRING_Y;
	switch( num ){
		case PLAYER_KNIGHT :
			drawString( x, y, "%d KNIGHT", num);
			break;
		case PLAYER_MAGE :
			drawString( x, y, "%d MAGE", num );
			break;
		case PLAYER_MONK :
			drawString( x, y, "%d MONK", num );
			break;
		case PLAYER_SUMMONER :
			drawString( x, y, "%d SUMMONER" , num );
			break;
		case PLAYER_SHOOTER :
			drawString( x, y, "%d SHOOTER", num );
			break;
		case PLAYER_WARRIOR :
			drawString( x, y, "%d WARRIOR", num );
			break;

	}
}

void Console::drawStatusPlayerDevice( int num ) {
	DevicePtr device = Device::getTask( );
	ServerPtr server = Server::getTask( );

	// デバイス
	{	// 区切り線
		int x1 = FRAME_PLAYER_DEVICE_X + FRAME_PLAYER_DEVICE_W;
		int y1 = FRAME_Y;
		int x2 = x1;
		int y2 = y1 + FRAME_H;
		drawLine( x1, y1, x2, y2 );
	}
	{	// タイトル
		int x = FRAME_PLAYER_DEVICE_X + 5;
		int y = FRAME_Y + BOX_DIR_STRING_Y;
		drawString( x, y, "Device" );
	}
	{
		int x = FRAME_PLAYER_DEVICE_X + 10;
		int y = FRAME_Y + FRAME_H * ( num + 1 ) / FRAME_R + BOX_DIR_STRING_Y;
		unsigned char device = server->getDeviceStatus( num );
		if ( device < STATUS_NUM ) {
			drawString( x + FRAME_PLAYER_DEVICE_W / 2, y, "%d", device );
		}
		if ( device == DEVICE_WAIT ) {
			drawString( x, y, "WAIT" );
		}
		if ( device == DEVICE_READY ) {
			drawString( x, y, "READY" );
		}
		if ( device == DEVICE_NONE ) {
			drawString( x, y, "NONE" );
		}
	}
}

void Console::drawStatusDir( int num ) {
	// 入力方向
	{ // 区切り線
		int x1 = FRAME_DIR_X + FRAME_DIR_W;
		int y1 = FRAME_Y;
		int x2 = x1;
		int y2 = y1 + FRAME_H;
		drawLine( x1, y1, x2, y2 );
	}
	{	// タイトル
		int x = FRAME_DIR_X + 5;
		int y = FRAME_Y + BOX_DIR_STRING_Y;
		drawString( x, y, "Dir" );
	}
	{
		int x = FRAME_DIR_X + 5;
		int y = FRAME_Y + FRAME_H * ( num + 1 ) / FRAME_R + BOX_DIR_STRING_Y;
		drawString( x, y, "% 4d % 4d", _data.user[ num ].x, _data.user[ num ].y );
	}
}

void Console::drawStatusButton( int num ) {
	// ボタン
	{ // 区切り線
		int x1 = FRAME_BUTTON_X + FRAME_BUTTON_W;
		int y1 = FRAME_Y;
		int x2 = x1;
		int y2 = y1 + FRAME_H;
		drawLine( x1, y1, x2, y2 );
	}
	{	// タイトル
		int x = FRAME_BUTTON_X + 5;
		int y = FRAME_Y + BOX_DIR_STRING_Y;
		drawString( x, y, "Button" );
	}

	{
		int x = FRAME_BUTTON_X + 5;
		int y = FRAME_Y + FRAME_H * ( num + 1 ) / FRAME_R + BOX_DIR_STRING_Y;
		drawString( x, y, "%s", BTOS( _data.user[ num ].button ).c_str( ) );
	}
}

void Console::drawStatusState( int num ) {
	// ステート
	{ // 区切り線
		int x1 = FRAME_STATE_X + FRAME_STATE_W;
		int y1 = FRAME_Y;
		int x2 = x1;
		int y2 = y1 + FRAME_H;
		drawLine( x1, y1, x2, y2 );
	}
	{	// タイトル
		int x = FRAME_STATE_X + 5;
		int y = FRAME_Y + BOX_DIR_STRING_Y;
		drawString( x, y, "State" );
	}

	{
		int x = FRAME_STATE_X + 5;
		int y = FRAME_Y + FRAME_H * ( num + 1 ) / FRAME_R + BOX_DIR_STRING_Y;
		std::string str;
		switch ( _data.user[ num ].state ) {
		case STATE_CAMP:
			str = "Camp";
			break;
		case STATE_LABYRINTH:
			str = "Labyrinth";
			break;
		case STATE_DEATHPARADE:
			str = "Deathparade";
			break;
		case STATE_BOSS:
			str = "Boss";
			break;
		case STATE_READY:
			str = "Ready";
			break;
		case STATE_DEAD:
			str = "Dead";
			break;
		case STATE_CLEAR:
			str = "Clear";
			break;
		case STATE_NONE:
			str = "none";
			break;
		}

		drawString( x, y, "%s", str.c_str( ) );
	}
}

void Console::drawStatusLevelWeapon( int num ) {
	// ステート
	{ // 区切り線
		int x1 = FRAME_WEAPON_X + FRAME_WEAPON_W;
		int y1 = FRAME_Y;
		int x2 = x1;
		int y2 = y1 + FRAME_H;
		drawLine( x1, y1, x2, y2 );
	}
	{	// タイトル
		int x = FRAME_WEAPON_X + 5;
		int y = FRAME_Y + BOX_DIR_STRING_Y;
		drawString( x, y, "LvWeapon" );
	}

	{
		int x = FRAME_WEAPON_X + FRAME_WEAPON_W / 2;
		int y = FRAME_Y + FRAME_H * ( num + 1 ) / FRAME_R + BOX_DIR_STRING_Y;

		char buf[ 10 ];
		_itoa_s( _data.user[ num ].level_weapon, buf, 10, 10 );
		
		drawString( x, y, "%s", buf );
	}
}

void Console::drawStatusLevelLife( int num ) {
	// ライフレベル
	{ // 区切り線
		int x1 = FRAME_LEVEL_LIFE_X + FRAME_LEVEL_LIFE_W;
		int y1 = FRAME_Y;
		int x2 = x1;
		int y2 = y1 + FRAME_H;
		drawLine( x1, y1, x2, y2 );
	}
	{	// タイトル
		int x = FRAME_LEVEL_LIFE_X + 5;
		int y = FRAME_Y + BOX_DIR_STRING_Y;
		drawString( x, y, "LvLife" );
	}

	{
		int x = FRAME_LEVEL_LIFE_X + FRAME_LEVEL_LIFE_W / 2;
		int y = FRAME_Y + FRAME_H * ( num + 1 ) / FRAME_R + BOX_DIR_STRING_Y;
		drawString( x, y, "%u", _data.user[ num ].level_life );
	}
}

void Console::drawStatusExperience( int num ) {
	{ // 区切り線
		int x1 = FRAME_EXPERIENCE_X + FRAME_EXPERIENCE_W;
		int y1 = FRAME_Y;
		int x2 = x1;
		int y2 = y1 + FRAME_H;
		drawLine( x1, y1, x2, y2 );
	}
	{	// タイトル
		int x = FRAME_EXPERIENCE_X + 5;
		int y = FRAME_Y + BOX_DIR_STRING_Y;
		drawString( x, y, "EXP" );
	}

	{
		int x = FRAME_EXPERIENCE_X + FRAME_EXPERIENCE_W / 2;
		int y = FRAME_Y + FRAME_H * ( num + 1 ) / FRAME_R + BOX_DIR_STRING_Y;
		drawString( x, y, "%u", _data.user[ num ].exp );
	}
}

void Console::drawStatusLife( int num ) {
	{ // 区切り線
		int x1 = FRAME_LIFE_X + FRAME_LIFE_W;
		int y1 = FRAME_Y;
		int x2 = x1;
		int y2 = y1 + FRAME_H;
		drawLine( x1, y1, x2, y2 );
	}
	{	// タイトル
		int x = FRAME_LIFE_X + 5;
		int y = FRAME_Y + BOX_DIR_STRING_Y;
		drawString( x, y, "Life" );
	}

	{
		int x = FRAME_LIFE_X + 5;
		int y = FRAME_Y + FRAME_H * ( num + 1 ) / FRAME_R + BOX_DIR_STRING_Y;
		drawString( x, y, "%10u", _data.user[ num ].life );
	}
}

void Console::drawConnect( ) {
	ServerPtr server = Server::getTask( );

	// 外の枠組み
	{
		int x1 = FRAME_CONNECT_X;
		int y1 = FRAME_CONNECT_Y;
		int x2 = x1 + FRAME_CONNECT_W;
		int y2 = y1 + FRAME_CONNECT_H;
		drawBox( x1, y1, x2, y2 );
	}

	// 中の枠組み
	for ( int i = 0; i < FRAME_CONNECT_R - 1; i++ ) {
		int x1 = FRAME_CONNECT_X;
		int y1 = FRAME_CONNECT_Y + FRAME_CONNECT_H * ( i + 1 ) / FRAME_CONNECT_R;
		int x2 = x1 + FRAME_CONNECT_W;
		int y2 = y1;
		drawLine( x1, y1, x2, y2 );
	}

	// タイトル
	{
		int x = FRAME_CONNECT_X + 2;
		int y = FRAME_CONNECT_Y + BOX_DIR_STRING_Y;
		drawString( x, y, "Connect" );
	}

	// IP
	{
		for ( int i = 0; i < MAX_MACHINE; i++ ) {
			std::string str = server->getMachineIPStr( i );
			int x = FRAME_CONNECT_X + 2;
			int y = FRAME_CONNECT_Y + FRAME_CONNECT_H * ( i + 1 ) / FRAME_CONNECT_R + BOX_DIR_STRING_Y;
			drawString( x, y, "%s", str.c_str( ) );
		}
	}
}

void Console::drawStatusDevice( ) {
	DevicePtr device = Device::getTask( );
	ServerPtr server = Server::getTask( );
	// デバイス
	//外の枠組み
	{
		int x1 = FRAME_DEVICE_X;
		int y1 = FRAME_DEVICE_Y;
		int x2 = x1 + FRAME_DEVICE_W;
		int y2 = y1 + PITCH;
		drawBox( x1, y1, x2, y2 );
	}
	{	// 区切り線
		int x1 = FRAME_DIR_X + FRAME_DIR_W;
		int y1 = FRAME_DEVICE_Y;
		int x2 = x1;
		int y2 = y1 + PITCH;
		drawLine( x1, y1, x2, y2 );
	}
	{	// タイトル
		int x = FRAME_DEVICE_X + 5;
		int y = FRAME_DEVICE_Y + BOX_DIR_STRING_Y;
		drawString( x, y, "Device" );
	}
		//デバイスの数
	{
		int x = FRAME_DIR_X + FRAME_DIR_W + ( FRAME_DIR_W / 4 );
		int y = FRAME_DEVICE_Y + BOX_DIR_STRING_Y;
		drawString( x, y, "%d", device->getDeviceNum( ) );
	}
}

void Console::drawBgm( ) {
	ServerPtr server = Server::getTask( );
	//外の枠組み
	{
		int x1 = FRAME_BGM_X;
		int y1 = FRAME_BGM_Y;
		int x2 = x1 + FRAME_BGM_W;
		int y2 = y1 + PITCH;
		drawBox( x1, y1, x2, y2 );
	}
	{	// 区切り線
		int x1 = FRAME_DIR_X + FRAME_DIR_W;
		int y1 = FRAME_BGM_Y;
		int x2 = x1;
		int y2 = y1 + PITCH;
		drawLine( x1, y1, x2, y2 );
	}
	{	// タイトル
		int x = FRAME_BGM_X + 5;
		int y = FRAME_BGM_Y + BOX_DIR_STRING_Y;
		drawString( x, y, "BGM" );
	}
		//BGMのON、OFF
	{
		int x = FRAME_DIR_X + FRAME_DIR_W + ( FRAME_DIR_W / 5 );
		int y = FRAME_BGM_Y + BOX_DIR_STRING_Y;
		if ( server->getBgmPlay( ) ) {
			drawString( x, y, "ON" );
		} else {
			drawString( x, y, "OFF" );
		}
	}

}

void Console::drawStage( ) {
	ServerPtr server = Server::getTask( );

	// 外の枠組み
	{
		int x1 = FRAME_STAGE_X;
		int y1 = FRAME_STAGE_Y;
		int x2 = x1 + FRAME_STAGE_W;
		int y2 = y1 + FRAME_STAGE_H;
		drawBox( x1, y1, x2, y2 );
	}

	// 中の枠組み
	for ( int i = 0; i < FRAME_STAGE_R - 1; i++ ) {
		int x1 = FRAME_STAGE_X;
		int y1 = FRAME_STAGE_Y + FRAME_STAGE_H * ( i + 1 ) / FRAME_STAGE_R;
		int x2 = x1 + FRAME_STAGE_W;
		int y2 = y1;
		drawLine( x1, y1, x2, y2 );
	}

	{	// 区切り線
		int x1 = FRAME_DIR_X + FRAME_DIR_W;
		int y1 = FRAME_STAGE_Y;
		int x2 = x1;
		int y2 = y1 + FRAME_STAGE_H;
		drawLine( x1, y1, x2, y2 );
	}

	// タイトル
	{
		int x = FRAME_STAGE_X + 2;
		int y = FRAME_STAGE_Y + BOX_DIR_STRING_Y;
		drawString( x, y, "Stage" );
	}

	{
		int x = FRAME_DIR_X + FRAME_DIR_W + ( FRAME_DIR_W / 5 );
		int y = FRAME_STAGE_Y + BOX_DIR_STRING_Y;
		drawString( x, y, "Lv" );

	}

	// ステージ
	{
		for ( int i = 0; i < STAGE_MAX; i++ ) {
			int x = FRAME_STAGE_X + 2;
			int y = FRAME_STAGE_Y + FRAME_STAGE_H * ( i + 1 ) / FRAME_STAGE_R + BOX_DIR_STRING_Y;
			switch ( i ) {
				case STAGE_LABYRINTH: 
					drawString( x, y, "Labyrinth" );
					break;
				case STAGE_DEATHPARADE:
					drawString( x, y, "Deathparade" );
					break;
				case STAGE_BOSS:
					drawString( x, y, "Boss" );
					break;
			}
		}
	}
	{
		for ( int i = 0; i < STAGE_MAX; i++ ) {
			int x = FRAME_DIR_X + FRAME_DIR_W + ( FRAME_DIR_W / 4 );
			int y = FRAME_STAGE_Y + FRAME_STAGE_H * ( i + 1 ) / FRAME_STAGE_R + BOX_DIR_STRING_Y;
			drawString( x, y, "%d", _data.stage[ i ] );
		}
	}
}

void Console::drawLog( ) {
	{
		int x1 = FRAME_LOG_X;
		int y1 = FRAME_LOG_Y;
		int x2 = x1 + FRAME_LOG_W;
		int y2 = y1 + FRAME_LOG_H;
		drawBox( x1, y1, x2, y2 );
	}

	// ログ
	for ( int i = 0; i < MESSAGE_COUNT; i++ ) {
		int x = FRAME_LOG_X + 10;
		int y = FRAME_LOG_Y + FRAME_LOG_H / MESSAGE_COUNT * i;
		drawString( x, y, "%s", get( i ).c_str( ) );
	}
}

void Console::drawCommand( ) {
	{
		int x1 = FRAME_COMMAND_X;
		int y1 = FRAME_COMMAND_Y;
		int x2 = x1 + FRAME_COMMAND_W;
		int y2 = y1 + FRAME_COMMAND_H;
		drawBox( x1, y1, x2, y2 );
	}

	// 中の枠
	{
		int x1 = FRAME_COMMAND_X + FRAME_COMMAND_TITLE_W;
		int y1 = FRAME_COMMAND_Y;
		int x2 = x1;
		int y2 = y1 + FRAME_COMMAND_H;
		drawLine( x1, y1, x2, y2 );
	}

	// タイトル
	{
		int x = FRAME_COMMAND_X + 3;
		int y = FRAME_COMMAND_Y + 5;
		drawString( x, y, "Command" );
	}

	// コマンド
	{
		int x = FRAME_COMMAND_X + FRAME_COMMAND_TITLE_W + 3;
		int y = FRAME_COMMAND_Y + 5;
		drawString( x, y, get( ).c_str( ) );
	}
}

void Console::drawBox( int x1, int y1, int x2, int y2 ) const {
	DrawBox( x1 , y1 , x2 , y2 , COLOR, FALSE );
}

void Console::drawLine( int x1, int y1, int x2, int y2 ) const {
	DrawLine( x1, y1, x2, y2, COLOR );
}

void Console::drawString( int x, int y, const char * format, ... ) const {
	va_list args;
	va_start( args, format );
	char str[ 255 ];
	vsprintf_s( str, 255, format, args );
	DrawFormatString( x, y, COLOR, str );
}

//Log
std::string Console::get( int index ) {
	return _message[ index ];
}

void Console::send( const char * format, ... ) {
	// スクロール
	for ( int num = 0; num < MESSAGE_COUNT - 1; num++ ) {
		_message[ num ] = _message[ num + 1 ];
	}

	// 文字列を加える
	va_list args;
	va_start( args, format );
	char str[ 255 ];
	vsprintf_s( str, 255, format, args );
	_message[ MESSAGE_COUNT - 1 ] = str;
}

//Command
std::string Console::get( ) {
	return _command;
}

void Console::parser( ) {
	_word.clear( );
	
	std::string command = _command;
	command += " ";
	std::transform( command.cbegin( ), command.cend( ), command.begin( ), tolower );
	_command.clear( );

	int pos = 0;
	while ( true ) {
		int p = command.find( ' ', pos );
		std::string tmp = command.substr( pos, ( p - pos ) );

		if ( tmp.empty( ) ) {
			break;
		}

		_word.push_back( tmp );
		pos = p + 1;
	}
}

void Console::execute( ) {
	ServerPtr server = Server::getTask( );

	std::string com = _command;

	parser( );

	if ( _word.empty( ) ) {
		_word.push_back( "" );
	}

	// デバイス再取得
	if ( _word[ 0 ] == COM_DEVICE && _word.size( ) == 1 ) {
		DevicePtr device = Device::getTask( );
		device->resetup( );
		server->setNoneAll( );
		send( "[SUCCESS] %s", com.c_str( ) );
		return;
	}

	if (  _word[ 0 ] == COM_DEVICE && _word.size( ) == 3 ) {
		// デバイス対応
		DevicePtr device = Device::getTask( );
		int player_num = atoi( _word[ 1 ].c_str( ) );
		int device_num = atoi( _word[ 2 ].c_str( ) );
		
		//数字文字判定
		if ( device_num == 0 ) {
			if ( !isdigit( _word[ 2 ].c_str( )[ 0 ] ) ) {
				device_num = -1;
			}
		}

		//文字である時の処理
		if ( device_num == -1 ) {
			if ( _word[ 2 ] == "none" || _word[ 2 ] == "n" ) {
				//指定した所がNONEになる
				server->setNone( player_num );
				send( "[SUCCESS]%s", com.c_str( ) );
				return;
			}
			if ( _word[ 2 ] == "redist" || _word[ 2 ] == "r" ) {
				//指定したとこがWAITになる
				server->setWait( player_num );
				send( "[SUCCESS]%s", com.c_str( ) );
				return;
			}
		}

		//数字である時の処理
		if ( device_num >= 0 ) {
			unsigned int device_num = ( unsigned int )atoi( _word[ 2 ].c_str( ) );
			device->setDevice( player_num, device_num );
			send( "[SUCCESS]%s", com.c_str( ) );
			return;
		}
	}

	// IPアドレス取得
	if ( _word[ 0 ] == COM_IP ) {
		ServerPtr server = Server::getTask( );
		server->resetIP( );
		send( "[SUCCESS] %s", com.c_str( ) );
		return;
	}
	
	// プレイヤー作成
	if ( _word[ 0 ] == COM_STATE && _word.size( ) == 3 ) {
		int index = atoi( _word[ 1 ].c_str( ) );
		unsigned char state = STATE_ERROR;
		if ( _word[ 2 ] == "camp" ) {
			state = STATE_CAMP;
		}
		if ( _word[ 2 ] == "labyrinth" || _word[ 2 ] == "l" ) {
			state = STATE_LABYRINTH;
		}
		if ( _word[ 2 ] == "deathparade" || _word[ 2 ] == "d" ) {
			state = STATE_DEATHPARADE;
		}
		if ( _word[ 2 ] == "ready" || _word[ 2 ] == "r" ) {
			state = STATE_READY;
		}
		if ( _word[ 2 ] == "boss" || _word[ 2 ] == "b" ) {
			state = STATE_BOSS;
		}
		if ( _word[ 2 ] == "dead" ) {
			state = STATE_DEAD;
		}
		if ( _word[ 2 ] == "clear" ) {
			state = STATE_CLEAR;
		}
		if ( _word[ 2 ] == "test" ) {
			state = STATE_TEST;
		}
		if ( server->setState( index, state ) ) {
			send( "[SUCCESS] %s", com.c_str( ) );
			return;
		}
	}
	
	
	// 武器レベル
	if ( _word[ 0 ] == COM_LVWEAPON && _word.size( ) == 3 ) {
		int index = atoi( _word[ 1 ].c_str( ) );
		int level = atoi( _word[ 2 ].c_str( ) );
		if ( server->setLevelWeapon( index, level ) ) {
			send( "[SUCCESS] %s", com.c_str( ) );
			return;
		}
	}

	// ライフレベル
	if ( _word[ 0 ] == COM_LVLIFE && _word.size( ) == 3 ) {
		int index = atoi( _word[ 1 ].c_str( ) );
		int level = atoi( _word[ 2 ].c_str( ) );
		if ( server->setLevelLife( index, level ) ) {
			send( "[SUCCESS] %s", com.c_str( ) );
			return;
		}
	}

	// ライフ
	if (  _word[ 0 ] == COM_LIFE && _word.size( ) == 3 ) {
		int index = atoi( _word[ 1 ].c_str( ) );
		unsigned int life = ( unsigned int )atoi( _word[ 2 ].c_str( ) );
		if ( server->setLife( index, life ) ) {
			send( "[SUCCESS] %s", com.c_str( ) );
			return;
		}
	}
	//BGM
	if (  _word[ 0 ] == COM_BGM && _word.size( ) == 2 ) {
		if ( _word[ 1 ] == "on" ) {
			server->setBgmPlay( true );
			send( "[SUCCESS] %s", com.c_str( ) );
			return;
		}
		if ( _word[ 1 ] == "off" ) {
			server->setBgmPlay( false );
			send( "[SUCCESS] %s", com.c_str( ) );
			return;
		}
	}

	// ステージレベル
	if ( _word[ 0 ] == COM_LVSTAGE && _word.size( ) == 3 ) {
		int stage = STAGE_MAX;
		if ( _word[ 1 ] == "labyrinth" || _word[ 1 ] == "l" ) {
			stage = STAGE_LABYRINTH;
		}
		if ( _word[ 1 ] == "deathparade" || _word[ 1 ] == "d" ) {
			stage = STAGE_DEATHPARADE;
		}
		if ( _word[ 1 ] == "boss" || _word[ 1 ] == "b" ) {
			stage = STAGE_BOSS;
		}
		int level = atoi( _word[ 2 ].c_str( ) );
		if ( server->setLevelStage( stage, level ) ) {
			send( "[SUCCESS] %s", com.c_str( ) );
			return;
		}
	}

	// コマンドエラー
	send( "#ERROR# %s", com.c_str( ) );
}

void Console::updateCommand( ) {
	char single_ch = GetInputChar( TRUE );

	if ( single_ch == 0 ) {
		return;
	}

	if ( single_ch == CTRL_CODE_CR ) {
		execute( );
	} else if ( single_ch == CTRL_CODE_BS && !_command.empty( ) ) {
		_command.pop_back( );
	} else {
		_command += single_ch;
	}
}