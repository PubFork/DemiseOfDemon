#include "Mouse.h"
#include "Framework.h"
#include "DxLib.h"

MousePtr Mouse::getTask( ) {
	FrameworkPtr fw = Framework::getInstance( );
	return std::dynamic_pointer_cast< Mouse >( fw->getTask( Mouse::getTag( ) ) );
}

Mouse::Mouse( ) {
	_wheel = 0;
	_right = false;
	_left = false;
	_x = 0;
	_y = 0;
}

Mouse::~Mouse( ) {
}

void Mouse::update( ) {
	GetMousePoint( &_x, &_y );
	_wheel = GetMouseWheelRotVol( );
	int n = GetMouseInput( );
	_right = ( ( n & MOUSE_INPUT_RIGHT ) != 0 );
	_left  = ( ( n & MOUSE_INPUT_LEFT  ) != 0 );
}

int Mouse::getX( ) const {
	return _x;
}

int Mouse::getY( ) const {
	return _y;
}

int Mouse::getWheel( ) const {
	return _wheel;
}

bool Mouse::isRight( ) const {
	return _right;
}

bool Mouse::isLeft( ) const {
	return _left;
}

