#pragma once

#include "Task.h"
#include "smart_ptr.h"
#include "Ip.h"
#include "Network.h"
#include <array>

PTR( Server );
PTR( Bgm );

class Server : public Task {
public:
	static std::string getTag( ) { return "SERVER"; }
	static ServerPtr getTask( );
public:
	Server( );
	virtual ~Server( );
public:
	virtual void update( );
	void resetIP( );
	std::string getMachineIPStr( int index );
	void sendStatus( const CLIENTDATA& data );
	unsigned char getDeviceStatus( int index );
	void setConecting( int index, int device );
	bool setState( int index, unsigned char state );
	bool setLevelWeapon( int index, int level );
	bool setLevelLife( int index, int level );
	bool setLife( int index, unsigned int life );
	bool healLife( int index, unsigned int heal_life );
	bool damage( int index, unsigned int heal_life );
	CLIENTDATA getData( );
	void updateOriginal( );
	void setDevice( int user_idx, int device_idx );
	void setWait( int index );
	void setNone( int index );
	void setNoneAll( );
	//音
	void setBgmPlay( bool playing );
	bool getBgmPlay( );
	//ステージレベル
	bool setLevelStage( int index, int level );
private:
	void listenForAcceptNewClient( );
	void listenForPackets( );
	void sendCondition( );
	bool getIP( IP& ip );
	bool save( const IP& ip );
	void showTheReceivedLog( int index, const SERVERDATA& data );
	void executeNetData( const SERVERDATA& data );
	void resetDevice( int index = 0 );
	void setDeviceReady( );
	void sendData( );
	void updateStatus( );
	void updateDevice( );
	void updateDeviceRedist( int user_idx );
	void resetState( );
	//音
	void cheakPlayBgm( int state );
	void updateBgm( ) ;
	//経験値によるレベルアップ
	void updateLifeLevelUp( );
private:
	std::array< int, MAX_MACHINE > _machine;
	int _udp_handle;
	int _network_state_time;
	int _send_network_condition_num;
	int _count;
	int _cheak_state;
	int _max_exp[ LIFE_LEVEL_MAX ];
	int _device_restart_time[ STATUS_NUM ];
	BgmPtr _bgm;
	CLIENTDATA _data;
};