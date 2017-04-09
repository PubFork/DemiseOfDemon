#include "Client.h"
#include "Ip.h"
#include "Framework.h"
#include "DxLib.h"
#include "Treasure.h"
#include "Exception.h"
#include "Sound.h"

ClientPtr Client::getTask( ) {
	FrameworkPtr fw = Framework::getInstance( );
	return std::dynamic_pointer_cast< Client >( fw->getTask( Client::getTag( ) ) );
}

Client::Client( ) {
	FrameworkPtr fw = Framework::getInstance( );

	_phase = PHASE_READY;

	// Server IP 読み込み
	load( );

	_network_state_time = 0;
	_tcp_handle = -1;
	_udp_handle = MakeUDPSocket( UDP_PORT_NUM );
	_count = 0;

	//_status初期化
	_status.stage[ 0 ] = 1;
	_status.stage[ 1 ] = 1;
	_status.stage[ 2 ] = 1;
	for ( int i = 0; i < STATUS_NUM; i++ ) {
		_status.user[ i ].button = 0; 
		_status.user[ i ].exp = 0; 
		_status.user[ i ].level_life = 0; 
		_status.user[ i ].level_weapon = 0; 
		_status.user[ i ].state = STATE_NONE; 
		_status.user[ i ].life = 123456789; 
	}
}

Client::~Client( ) {
}

void Client::moveStage ( unsigned char before_state, unsigned char after_state ) {
	for ( int i = 0; i < STATUS_NUM; i++ ) {
		if ( _status.user[ i ].state != before_state ) {
			continue;
		}
		SERVERDATA netdata;
		netdata.command = COMMAND_STATE;
		netdata.index = i;
		netdata.value = after_state;
		send( netdata );
	}
}


void Client::damage( int index, int power ) {
	SERVERDATA netdata;
	netdata.command = COMMAND_DAMAGE;
	netdata.index = index;
	netdata.value = power;
	send( netdata );
}

void Client::kill( int index ) {
	SERVERDATA netdata;
	netdata.command = COMMAND_STATE;
	netdata.index = index;
	netdata.value = STATE_DEAD;
	send( netdata );
}

void Client::update( ) {
	switch( _phase ) {
	case PHASE_READY:
		updateReady( );
		break;
	case PHASE_CONNECTING:
		updateConnecting( );
		break;
	}
}

void Client::updateReady( ) {
	// 接続
	if ( connect( ) ) {
		_phase = PHASE_CONNECTING;
	}
}

void Client::updateConnecting( ) {
	if ( _tcp_handle < 0 ) {
		_phase = PHASE_READY;
		return;
	}

	if ( GetLostNetWork( ) == _tcp_handle ) {
		CloseNetWork( _tcp_handle );	// 接続を断つ
		_tcp_handle = -1;
	}
	
	recieveStatus( );
	responseOfState( );
}

bool Client::load( ) {
	int fh = FileRead_open( "IP.ini" );
	if ( fh == 0 ) {
		exception( "IP.iniがみつかりません" );
	}

	if ( FileRead_read( &_ip, sizeof( IP ), fh ) == -1 ) {
		FileRead_close( fh ) ;
		return false;
	}

    FileRead_close( fh ) ;
	return true;
}

IP Client::getServerIP( ) const {
	return _ip;
}

IP Client::getClientIP( ) const {
	IP ip = { 0, 0, 0, 0};
	IPDATA data;
	GetMyIPAddress( &data );

	ip.address[ 0 ] = data.d1;
	ip.address[ 1 ] = data.d2;
	ip.address[ 2 ] = data.d3;
	ip.address[ 3 ] = data.d4;

	return ip;
}

std::string Client::getPhase( ) const {
	std::string str;
	switch( _phase ) {
	case PHASE_READY:
		return str = "PHASE_READY";
		break;
	case PHASE_CONNECTING:
		return str = "PHASE_CONNECTING";
		break;
	default:
		return str = "PHASE_NONE";
		break;
	}
}

bool Client::connect( ) {

	IPDATA dip;
	dip.d1 = _ip.address[ 0 ];
	dip.d2 = _ip.address[ 1 ];
	dip.d3 = _ip.address[ 2 ];
	dip.d4 = _ip.address[ 3 ];

	_tcp_handle = ConnectNetWork( dip, TCP_PORT_NUM );

	return _tcp_handle >= 0;
}

void Client::send( const SERVERDATA& data ) {
	if ( _tcp_handle < 0 ) {
		return;
	}

	NetWorkSend( _tcp_handle, &data, sizeof( SERVERDATA ) );
}

CLIENTDATA Client::getClientData( ) {
	return _status;
}

void Client::recieveStatus( ) {
	while ( CheckNetWorkRecvUDP( _udp_handle ) == TRUE ) {
		NetWorkRecvUDP( _udp_handle, NULL, NULL, &_status, sizeof( CLIENTDATA ), FALSE );
	}
}

void Client::responseOfState( ) {
	if ( _tcp_handle < 0 ) {
		return;
	}

	if ( GetNetWorkDataLength( _tcp_handle ) <= 0 ) {
		return;
	}

	// 受信
	SERVERDATA data;
	NetWorkRecv( _tcp_handle, &data, sizeof( data ) );

	// データ処理
	switch ( data.command ) {
	case COMMAND_CONDITION:
		break;
	}
}
