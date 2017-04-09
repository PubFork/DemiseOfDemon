#pragma once

#include "Task.h"
#include "Network.h"

PTR( Device );

class Device : public Task {
public:
	static std::string getTag( ) { return "DEVICE"; }
	static DevicePtr getTask( );
public:
	Device( );
	virtual ~Device( );
public:
	virtual void update( );
public:
	char getDirX( int index ) const;
	char getDirY( int index ) const;
	unsigned char getButton( int index ) const;
	int getId( const int num ) const;
	void setDevice( const int playerNum, const int deviceNum );
	int getDeviceNum( );//デバイスの個数を渡す
public:
	void resetup( );
private:
	struct DATA {
		int id;
		char x;
		char y;
		unsigned char button;
	};
private:
	DATA _data[ STATUS_NUM ];
	int _device_num;
};