#pragma once

#include "smart_ptr.h"
#include "Task.h"
#include "IP.h"
#include "Network.h"
#include <string>

PTR( Client );
PTR( Device );
PTR( Sound );

class Client : public Task {
public:
	static std::string getTag( ) { return "CLIENT"; }
	static ClientPtr getTask( );
public:
	Client( );
	virtual ~Client( );
public:
	virtual void update( );
public:
	IP getServerIP( ) const;
	IP getClientIP( ) const;
	std::string getPhase( ) const;
	void send( const SERVERDATA& data );
	CLIENTDATA getClientData( );
	void damage( int index, int power );
	void moveStage ( unsigned char before_state, unsigned char after_state );
	void kill( int index );
private:
	enum PHASE {
		PHASE_READY,
		PHASE_CONNECTING,
	};
private:
	void updateReady( );
	void updateConnecting( );
	void updateOffline( );
	bool load( );
	bool connect( );
	void recieveStatus( );
	void responseOfState( );
	void updateOfflineDevice( );
	void updateOfflineCreate( );
	void updateOfflineLife( );
private:
	PHASE _phase;
	IP _ip;
	int _tcp_handle;
	int _udp_handle;
	int _network_state_time;
	int _count;
	CLIENTDATA _status;
};

