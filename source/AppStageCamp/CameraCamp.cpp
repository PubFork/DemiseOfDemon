#include "CameraCamp.h"

CameraCamp::CameraCamp( ) {
	_eye = Vector( 0, 10, 0 );
	_focus = Vector( 10, 0, 10 );
}

CameraCamp::~CameraCamp( ) {

}

void CameraCamp::update( ) {
	updateCamera( );
}
