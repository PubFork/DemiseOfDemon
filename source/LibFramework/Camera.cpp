#include "Camera.h"
#include "Framework.h"
#include "DxLib.h"

Camera::Camera( ) {
	_focus = Vector( 50, 10, 50 );
	_eye = Vector( 0, 20, 0 );
}

Camera::~Camera( ) {
}

Vector Camera::getEye( ) const {
	return _eye;
}

Vector Camera::getFocus( ) const {
	return _focus;
}

void Camera::updateCamera( ) {
	FrameworkPtr fw = Framework::getInstance( );
	fw->setCamera( _eye, _focus );
}
