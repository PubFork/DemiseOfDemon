#include "ModuleViewer.h"
#include "framework.h"
#include "ViewerGround.h"
#include "Map.h"
#include "CameraMapEditor.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "MEObject.h"
#include "MapEditor.h"
#include "ViewerWater.h"

ModuleViewer::ModuleViewer( MapPtr map, MEObjectPtr object )
: ModuleBase( map ) {
	_ground = ViewerGroundPtr( new ViewerGround( _map, false ) );
	_object = object;
	_object->setHeight( 0 );

	_last_mouse_x = 0;
	_last_mouse_y = 0;

	_water = ViewerWaterPtr( new ViewerWater( _map ) );
}

ModuleViewer::~ModuleViewer( ) {

}

void ModuleViewer::update( ) {
	KeyboardPtr keyboard = Keyboard::getTask( );
	MousePtr mouse = Mouse::getTask( );
	MapEditorPtr editor = MapEditor::getTask( );
	CameraMapEditorPtr camera = editor->getCamera( );

	if ( keyboard->isPushKey( "F" ) ) {
		camera->setFocus( _map->getCameraPos( ), _map->getCameraTarget( ) );
	}
	if ( keyboard->isHoldKey( "ARROW_LEFT" ) ) {
		camera->moveHorizontal( -1.0 );
	}
	if ( keyboard->isHoldKey( "ARROW_RIGHT" ) ) {
		camera->moveHorizontal(  1.0 );
	}
	if ( keyboard->isHoldKey( "ARROW_UP" ) ) {
		camera->moveVertical(  1.0 );
	}
	if ( keyboard->isHoldKey( "ARROW_DOWN" ) ) {
		camera->moveVertical( -1.0 );
	}
	if ( mouse->getWheel( ) != 0 ) {
		camera->moveZoom( mouse->getWheel( ) * 2 );
	}
	if ( mouse->isLeft( ) ) {
		camera->moveHorizontal( ( mouse->getX( ) - _last_mouse_x ) * -0.1 );
		camera->moveVertical( ( mouse->getY( )- _last_mouse_y ) * 0.1 );
	}
	if ( mouse->isRight( ) ) {
		camera->moveYaw( ( mouse->getX( ) - _last_mouse_x ) * 0.005 );
		camera->movePitch( ( mouse->getY( )- _last_mouse_y ) * 0.005 );
	}

	_last_mouse_x = mouse->getX( );
	_last_mouse_y = mouse->getY( );
	camera->update( );
	_ground->draw( );
	_water->draw( );
}
