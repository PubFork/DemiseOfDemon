#include "Server.h"
#include "Status.h"
#include "Framework.h"
#include "Console.h"
#include "Network.h"
#include "Device.h"
#include "DxLib.h"
#include "Bgm.h"

const int SETTING_NUM = 1;

ServerPtr Server::getTask( ) {
	FrameworkPtr fw = Framework::getInstance( );
	return std::dynamic_pointer_cast< Server >( fw->getTask( Server::getTag( ) ) );
}

Server::Server( ) {
	for ( int i = 0; i < MAX_MACHINE; i++ ) {
		_machine[ i ] = -1;
	}
	_network_state_time = 0;
	_send_network_condition_num = 0;

	PreparationListenNetWork( TCP_PORT_NUM );
	_udp_handle = MakeUDPSocket( -1 );

	//STATUS
	FrameworkPtr fw = Framework::getInstance( );
	for ( int i = 0; i < STATUS_NUM; i++ ) {
		_data.user[ i ].state = STATE_NONE;
		_data.user[ i ].level_weapon = 1;
		_data.user[ i ].level_life = 1;
		_data.user[ i ].life = 0;
		_data.user[ i ].exp = 0;
		_data.user[ i ].button = 0;
		//Device初期化
		_data.user[ i ].device = DEVICE_NONE;	
		_device_restart_time[ i ] = 0;
	}
	for ( int i = 0; i < STAGE_MAX; i++ ) {
		_data.stage[ i ] = 1;
	}

	_count = 0;
	_bgm = BgmPtr( new Bgm );
	_cheak_state = STATE_READY;

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

Server::~Server( ) {
	_bgm->stop( );
	DeleteUDPSocket( _udp_handle );
	StopListenNetWork( );
}

void Server::resetDevice( int index ) {
	if ( index == 0 ) {
		return;
	}
	for ( int i = 0; i < index; i++ ) {
		_data.user[ i ].device = DEVICE_WAIT;
	}
	setDeviceReady( );
}

void Server::setDeviceReady( ) {
	for ( int i = 0; i < STATUS_NUM; i++ ) {
		if ( _data.user[ i ].device == DEVICE_READY ) {
			return;
		}
	}
	for ( int i = 0; i < STATUS_NUM; i++ ) {
		if ( _data.user[ i ].device == DEVICE_WAIT ) {
			_data.user[ i ].device = DEVICE_READY;
			break;
		}
	}
}

void Server::update( ) {
	listenForAcceptNewClient( );
	listenForPackets( );
	sendCondition( );
	updateOriginal( );
	resetState( );
}

std::string Server::getMachineIPStr( int index ) {
	std::string str;
	if ( _machine[ index ] >= 0 ) {
		IPDATA ip;
		GetNetWorkIP( _machine[ index ], &ip );
		str += std::to_string( ( _ULonglong  )ip.d1 );
		str += ".";
		str += std::to_string( ( _ULonglong )ip.d2 );
		str += ".";
		str += std::to_string( ( _ULonglong )ip.d3 );
		str += ".";
		str += std::to_string( ( _ULonglong )ip.d4 );
	}
	return str;
}

void Server::listenForAcceptNewClient( ) {
	int handle = GetNewAcceptNetWork( );
	if ( handle == -1 ) {
		return;
	}
	
	for ( int i = 0; i < MAX_MACHINE; i++ ) {
		if ( _machine[ i ] == -1 ) {
			_machine[ i ] = handle;
			break;
		}
	}
}

void Server::listenForPackets( ) {
	int handle = GetLostNetWork( );
	for ( int i = 0; i < MAX_MACHINE; i++ ) {
		if ( _machine[ i ] == -1 ) {
			continue;
		}

		if ( handle == _machine[ i ] ) {
			CloseNetWork( _machine[ i ] );
			_machine[ i ] = -1;
			continue;
		}

		if ( GetNetWorkDataLength( _machine[ i ] ) <= 0 ) {
			continue;
		}

		// 受信
		SERVERDATA data;
		if ( NetWorkRecv( _machine[ i ], &data, sizeof( data ) ) == 0 ) {
			// 受け取ったデータ表示
			showTheReceivedLog( i, data );

			// データにあわせて処理
			executeNetData( data );
		}
	}
}

void Server::executeNetData( const SERVERDATA& data ) {
	FrameworkPtr fw = Framework::getInstance( );

	switch ( data.command ) {
	case COMMAND_STATE:
		{
			ServerPtr server = Server::getTask( );
			server->setState( data.index, data.value );
		}
		break;
	case COMMAND_HEAL:
		{
			ServerPtr server = Server::getTask( );
			server->healLife( data.index, data.value );
		}
		break;
	case COMMAND_CREATE:
		{
			ServerPtr server = Server::getTask( );
			server->setState( data.index, STATE_CAMP );
			server->setLife( data.index, ( int )fw->getSettingValue( "PLAYER_LIFE" ) );
		}
		break;
	case COMMAND_DAMAGE:
		{
			ServerPtr server = Server::getTask( );
			server->damage( data.index, data.value );
		}
		break;
	
	case COMMAND_LEVEL:
		{
			ServerPtr server = Server::getTask( );
			server->setLevelWeapon( data.index, data.value );
		}
		break;
	/*
	case COMMAND_SE:
		{
			SoundManagerPtr sound_manager  = SoundManager::getTask( );
			sound_manager->setPlaySe( data.index, data.value );
		}
		break;*/
	case COMMAND_EXP:
		{
			_data.user[ data.index ].exp++;
		}
		break;
	}
}

void Server::resetIP( ) {
	ConsolePtr console = Console::getTask( );

	// IP取得
	IP ip;
	if ( !getIP( ip ) ) {
		return;
	}
	console->send( "IP : %d.%d.%d.%d", ip.address[ 0 ], ip.address[ 1 ], ip.address[ 2 ], ip.address[ 3 ] );

	// ファイル書き込み
	if ( save( ip ) ) {
		console->send( "IP.iniファイルの出力に失敗しました" );
		return;
	}
	console->send( "IP.iniファイルを出力しました" );
}

bool Server::getIP( IP& ip ) {
	IPDATA data;
	if ( GetMyIPAddress( &data ) ) {
		return false;
	}

	ip.address[ 0 ] = data.d1;
	ip.address[ 1 ] = data.d2;
	ip.address[ 2 ] = data.d3;
	ip.address[ 3 ] = data.d4;

	return true;
}

bool Server::save( const IP& ip ) {
	FILE* fp;

	if ( fopen_s( &fp, "IP.ini", "wb" ) ) {
		return false;	// エラー
	}

	if ( fwrite( &ip, sizeof( IP ), 1, fp ) != errno ) {
		fclose( fp );
		return false;
	}
	
	fclose( fp );
	return true;
}

void Server::showTheReceivedLog( int index, const SERVERDATA& data ) {

	std::string com;
	switch ( data.command ) {
	case COMMAND_DUMMY:
		// ダミー
		com = "DUMMY";
		break;
	case COMMAND_CONDITION:
		// 表示しない
		return;
	case COMMAND_STATE:
		// ダミー
		com = "STATE";
		break;
	default:
		com = "Unknown";
		break;
	}
	
	ConsolePtr console = Console::getTask( );
	console->send( "<receive> IP:%s COM:%s", getMachineIPStr( index ).c_str( ), com.c_str( ) );
}

void Server::sendStatus( const CLIENTDATA& data ) {
	for ( int i = 0; i < MAX_MACHINE; i++ ) {
		if ( _machine[ i ] < 0 ) {
			continue;
		}
		IPDATA ip;
		GetNetWorkIP( _machine[ i ], &ip );
		NetWorkSendUDP( _udp_handle, ip, UDP_PORT_NUM, &data, sizeof( CLIENTDATA ) ) ;
	}
}

void Server::sendCondition( ) {
	// 一定時間
	_network_state_time++;
	if ( _network_state_time < NETWORK_STATE_TIME ) {
		return;
	}
	_network_state_time = 0;

	// 送信
	for ( int i = 0; i < MAX_MACHINE; i++ ) {
		if ( _machine[ i ] < 0 ) {
			continue;
		}

		SERVERDATA data;
		data.command = COMMAND_CONDITION;
		if ( NetWorkSend( _machine[ i ], &data, sizeof( SERVERDATA ) ) < 0 ) {
			CloseNetWork( _machine[ i ] );
			_machine[ i ] = -1;
		}
	}
}

CLIENTDATA Server::getData( ) {
	return _data;
}
//server
void Server::updateOriginal( ) {
	updateStatus( );
	updateBgm( );
	updateDevice( );
	sendData( );
	updateLifeLevelUp( );
}

void Server::updateStatus( ) {
	_count++;
	for ( int i = 0; i < STATUS_NUM; i++ ) {
		if ( _data.user[ i ].state == STATE_NONE ||
			_data.user[ i ].state == STATE_DEAD ||
			_data.user[ i ].state == STATE_CLEAR ||
			_data.user[ i ].state == STATE_READY ) {
			_data.user[ i ].life = 123456789;
			continue;
		}
		if ( _data.user[ i ].life == 0 ) {
			continue;
		}
		if ( _data.user[ i ].life > 10 ) {
			if ( _count % 5 == 0 ) {// ライフ減少
				FrameworkPtr fw = Framework::getInstance( );
				_data.user[ i ].life -= ( int )fw->getSettingValue( fw->getSettingTag( SETTING_NUM + _data.user[ i ].level_weapon ) );
			}
		} else {
			if ( _count % 60 == 0 ) {
				_data.user[ i ].life--;
				if ( _data.user[ i ].state == STATE_DEAD ) {
					return;
				}
			}
		}
	}
}

void Server::updateDevice( ) {
	DevicePtr device = Device::getTask( );
	for ( int i = 0; i < STATUS_NUM; i++ ) {
		int device_data =_data.user[ i ].device;

		if ( device_data < STATUS_NUM ) {
			_data.user[ i ].x = device->getDirX( device_data );
			_data.user[ i ].y = device->getDirY( device_data );
			_data.user[ i ].button = device->getButton( device_data );
		} else {
			_data.user[ i ].x = 0;
			_data.user[ i ].y = 0;
			_data.user[ i ].button = 0;
		}

		if ( device_data == DEVICE_READY ) {
			updateDeviceRedist( i );
		}
	}
}

void Server::updateDeviceRedist( int user_idx ) {
	DevicePtr device = Device::getTask( );
	for ( int i = 0; i < STATUS_NUM; i++ ) {
		int button = device->getButton( i );
		if ( button == ( BUTTON_A | BUTTON_B | BUTTON_C | BUTTON_D ) ) {
			bool found = false;
			for ( int j = 0; j < STATUS_NUM; j++ ) {
				if ( _data.user[ j ].device == ( unsigned char )i ) {
					found = true;
				}
			}
			if ( !found ) {
				setDevice( user_idx, i );
				break;
			}
		}
	}
}

void Server::setDevice( int user_idx, int device_idx ) {
	for ( int i = 0; i < STATUS_NUM; i++ ) {
		if ( _data.user[ i ].device == device_idx ) {
			_data.user[ i ].device =  DEVICE_NONE;
		}
	}
	_data.user[ user_idx ].device = device_idx;
	setDeviceReady( );
}

void Server::sendData( ) {
	sendStatus( _data );
}

bool Server::setState( int index, unsigned char state ) {
	if ( index < 0 || index >= STATUS_NUM ) {
		return false;
	}

	if ( state == STATE_ERROR ) {
		return false;
	}

	_data.user[ index ].state = state;
	return true;
}

bool Server::healLife( int index, unsigned int heal_life ) {
	if ( index < 0 || index >= STATUS_NUM ) {
		return false;
	}

	_data.user[ index ].life += heal_life;

	return true;
}

bool Server::damage( int index, unsigned int power ) {
	if ( index < 0 || index >= STATUS_NUM ) {
		return false;
	}
	
	int life = _data.user[ index ].life;
	if ( life <= 10 ) {
		return false;
	}
	life -= power;
	if ( life < 10 ) {
		life = 10;
	}
	_data.user[ index ].life = life;

	return true;
}

bool Server::setLevelLife( int index,  int level ) {
	if ( index < 0 || index >= STATUS_NUM ) {
		return false;
	}

	if ( level < 1 || level > LIFE_LEVEL_MAX ) {
		return false;
	}

	_data.user[ index ].level_life = level;

	return true;
}

bool Server::setLevelWeapon( int index,  int level ) {
	if ( index < 0 || index >= STATUS_NUM ) {
		return false;
	}
	//マジックナンバー
	if ( level < 0 || level > 5 ) {
		return false;
	}

	_data.user[ index ].level_weapon = level;

	return true;
}

bool Server::setLife( int index, unsigned int life ) {
	if ( index < 0 || index >= STATUS_NUM ) {
		return false;
	}
	
	_data.user[ index ].life = life;
	
	return true;
}

unsigned char Server::getDeviceStatus( int index ) {
	return _data.user[ index ].device;
}

void Server::setWait( int index ) {
	_data.user[ index ].device = DEVICE_WAIT;
	setDeviceReady( );
}

void Server::setNone( int index ) {
	_data.user[ index ].device = DEVICE_NONE;
	setDeviceReady( );
}

void Server::setNoneAll( ) {
	for ( int i = 0; i < STATUS_NUM; i++  ) {
		_data.user[ i ].device = DEVICE_NONE;
	}
}
//音
void Server::setBgmPlay( bool playing ) {
	if ( playing ) {
		_bgm->bgmPlayOn( );
		cheakPlayBgm( _cheak_state );
	} else {
		_bgm->stop( );
	}
}

bool Server::getBgmPlay( ) {
	return _bgm->getBgmPlaying( );
}

bool Server::setLevelStage( int index, int level ) {
	if ( index < 0 || index >= STAGE_MAX ) {
		return false;
	}
	if ( level <= 0 ) {
		return false;
	}
	_data.stage[ index ] = level;
	return true;
}

void Server::resetState( ) {
	DevicePtr device = Device::getTask( );
	for ( int i = 0; i < STATUS_NUM; i++ ) {
		int button = _data.user[ i ].button;
		
		if ( button == ( BUTTON_A | BUTTON_B | BUTTON_C | BUTTON_D ) ) {
			_device_restart_time[ i ]++;
		} else {
			_device_restart_time[ i ] = 0;
		}
		if ( ( _device_restart_time[ i ] / 60 ) == 1 ) {
			
			_data.user[ i ].state = STATE_READY;
			_data.user[ i ].exp = 0; 
			_data.user[ i ].level_life = 0; 
			_data.user[ i ].level_weapon = 0;
			_device_restart_time[ i ] = 0;
		}

	}
}

//音
void Server::cheakPlayBgm( int state ) {

	Bgm::BGM play_title = Bgm::BGM_WAIT;
	_cheak_state = state;
	if ( _cheak_state == STATE_CAMP ) {
		if ( _data.stage[ STAGE_BOSS ] == 3 ) {
			play_title = Bgm::BGM_CAMP_LAST;
		} else {
			play_title = Bgm::BGM_CAMP_NORMAL;
		}
	}
	if ( _cheak_state == STATE_LABYRINTH ) {
		if ( _data.stage[ STAGE_LABYRINTH ] == 1 ) {
			play_title = Bgm::BGM_LABYRINTH_1;
		}
		if ( _data.stage[ STAGE_LABYRINTH ] == 2 ) {
			play_title = Bgm::BGM_LABYRINTH_2;
		}
		if ( _data.stage[ STAGE_LABYRINTH ] == 3 ) {
			play_title = Bgm::BGM_LABYRINTH_3;
		} else {
			play_title = Bgm::BGM_LABYRINTH_LAST;
		}

	}
	if ( _cheak_state == STATE_DEATHPARADE ) {
		if ( _data.stage[ STATE_DEATHPARADE ] == 1 ) {
			play_title = Bgm::BGM_LABYRINTH_1;
		}
		if ( _data.stage[ STATE_DEATHPARADE ] == 2 ) {
			play_title = Bgm::BGM_LABYRINTH_2;
		}
		if ( _data.stage[ STATE_DEATHPARADE ] == 3 ) {
			play_title = Bgm::BGM_LABYRINTH_3;
		} else {
			play_title = Bgm::BGM_LABYRINTH_LAST;
		}
	}
	if ( _cheak_state == STATE_BOSS ) {
		if ( _data.stage[ STAGE_BOSS ] == 1 ) {
			play_title = Bgm::BGM_MINOTAUR_1;
		}
		if ( _data.stage[ STAGE_BOSS ] == 2 ) {
			play_title = Bgm::BGM_MINOTAUR_2;
		}
		if ( _data.stage[ STAGE_BOSS ] == 3 ) {
			play_title = Bgm::BGM_MINOTAUR_3;
		} else {
			play_title = Bgm::BGM_BOSS;
		}
	}
	_bgm->play( play_title );
}

void Server::updateBgm( ) {
	if ( !_bgm->getBgmPlaying( ) ) {
		return;
	}
	int now_state = STATE_NONE;
	for ( int i = 0; i < STATUS_NUM; i++ ) {
		if ( now_state <= _data.user[ i ].state  ) {
			now_state = _data.user[ i ].state;
		}
	}
	_bgm->update( );
	if( now_state == _cheak_state ) {
		return;
	}
	cheakPlayBgm( now_state );
	
}

void Server::updateLifeLevelUp( ) {
	FrameworkPtr fw = Framework::getInstance( );
	StatusPtr status = Status::getTask( );

	for ( int i = 0; i < STATUS_NUM; i++ ) {
		if ( _data.user[ i ].level_life >= LIFE_LEVEL_MAX ) {
			continue;
		}
	if ( _max_exp[ _data.user[ i ].level_life ] == _data.user[ i ].exp ) {
			setLevelLife( i, _data.user[ i ].level_life + 1 );
			if ( _data.user[ i ].level_life < LIFE_LEVEL_MAX ) {
				_data.user[ i ].exp = 0;
			}
		}
	}
}