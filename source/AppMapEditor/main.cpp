#include "DxLib.h"
#include "Framework.h"
#include "Keyboard.h"
#include "mouse.h"
#include "CameraMapEditor.h"
#include "MapEditor.h"

void main( ) {
	FrameworkPtr fw = Framework::getInstance( );

	fw->addTask( Keyboard::getTag( ), TaskPtr( new Keyboard ) );
	fw->addTask( Mouse::getTag( ), TaskPtr( new Mouse ) );
	fw->addTask( MapEditor::getTag( ), TaskPtr( new MapEditor ) );

	SetCameraNearFar( 50.0f, 400.0f );
}