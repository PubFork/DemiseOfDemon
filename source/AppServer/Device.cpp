#include "Device.h"
#include "mathmatics.h"
#include "Framework.h"
#include "Server.h"
#include "DxLib.h"

const int MAX_JOYPAD_USE_NUM = STATUS_NUM;
const int JOYPADKEY[ MAX_JOYPAD_USE_NUM ] = {
	DX_INPUT_PAD1,
	DX_INPUT_PAD2,
	DX_INPUT_PAD3,
	DX_INPUT_PAD4,
	DX_INPUT_PAD5,
	DX_INPUT_PAD6,
	DX_INPUT_PAD7,
	DX_INPUT_PAD8,
	DX_INPUT_PAD9,
	DX_INPUT_PAD10,
	DX_INPUT_PAD11,
	DX_INPUT_PAD12,
	DX_INPUT_PAD13,
	DX_INPUT_PAD14,
	DX_INPUT_PAD15,
	DX_INPUT_PAD16,
};

DevicePtr Device::getTask( ) {
	FrameworkPtr fw = Framework::getInstance( );
	return std::dynamic_pointer_cast< Device >( fw->getTask( Device::getTag( ) ) );
}


Device::Device( ) {
	for ( int i = 0; i < STATUS_NUM; i++ ) {
		_data[ i ].x = 0;
		_data[ i ].y = 0;
		_data[ i ].button = 0;
		_data[ i ].id = i;
	}
}

Device::~Device( ) {
}

int Device::getId( const int num ) const {
	return _data[ num ].id;
}

void Device::resetup( ) {
	ReSetupJoypad( );
}

char Device::getDirX( int index ) const {
	return _data[ index ].x;
}

char Device::getDirY( int index ) const {
	return _data[ index ].y;
}

unsigned char Device::getButton( int index ) const {
	return _data[ index ].button;
}

void Device::update( ) {
	_device_num = GetJoypadNum( );
	for ( int i = 0; i < MAX_JOYPAD_USE_NUM; i++ ) {
		
		int key = GetJoypadInputState( JOYPADKEY[ i ] );
		Vector vec;
		int x = 0, y = 0;
		GetJoypadAnalogInput( &x, &y, JOYPADKEY[ i ] );
		vec.x = x;
		vec.y = y;
		vec.x += +( ( key & PAD_INPUT_RIGHT ) != 0 );
		vec.x += -( ( key & PAD_INPUT_LEFT  ) != 0 );
		vec.y += +( ( key & PAD_INPUT_DOWN  ) != 0 );
		vec.y += -( ( key & PAD_INPUT_UP    ) != 0 );
		vec = vec.normalize( ) * 100;

		const int INDEX = _data[ i ].id;

		_data[ INDEX ].x = ( char )vec.x;
		_data[ INDEX ].y = ( char )vec.y;

		_data[ INDEX ].button = 0;
		if ( ( key & PAD_INPUT_A ) != 0 ) { 
			_data[ INDEX ].button |= BUTTON_A;
		}
		if ( ( key & PAD_INPUT_B ) != 0 ) { 
			_data[ INDEX ].button |= BUTTON_B;
		}
		if ( ( key & PAD_INPUT_C ) != 0 ) { 
			_data[ INDEX ].button |= BUTTON_C;
		}
		if ( ( key & PAD_INPUT_X ) != 0 ) { 
			_data[ INDEX ].button |= BUTTON_D;
		}
	}
}

//Device‚ÌƒZƒbƒg
void Device::setDevice( const int playerNum, const int deviceNum ) {
	ServerPtr server = Server::getTask( );
	server->setDevice( playerNum, deviceNum );
}

int Device::getDeviceNum( ) {
	return _device_num;
}