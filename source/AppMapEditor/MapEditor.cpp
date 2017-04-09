#include "MapEditor.h"
#include "Framework.h"
#include "ModuleEditor.h"
#include "Map.h"
#include "Keyboard.h"
#include "ModuleViewer.h"
#include "CameraMapEditor.h"
#include "Model.h"
#include "MEObject.h"
#include "DxLib.h"
#include "LoadMapData.h"

MapEditorPtr MapEditor::getTask( ) {
	FrameworkPtr fw = Framework::getInstance( );
	return std::dynamic_pointer_cast< MapEditor >( fw->getTask( MapEditor::getTag( ) ) );
}


MapEditor::MapEditor( ) {
	_map = MapPtr( new Map );
	
	_object = MEObjectPtr( new MEObject( _map ) );

	_camera = CameraMapEditorPtr( new CameraMapEditor );
	_camera->setFocus( _map->getCameraPos( ), _map->getCameraTarget( ) );

	_now = MODULE_EDITOR;
	resetModule( );
}

MapEditor::~MapEditor( ) {

}

CameraMapEditorPtr MapEditor::getCamera( ) {
	return _camera;
}

void MapEditor::update( ) {
	changeMode( );
	_module->update( );
	_object->draw( );

	ScreenFlip( );
	ClearDrawScreen( );
}

MapEditor::MODE MapEditor::getChangeMode( ) {
	MODE mode = MODE_NONE;
	KeyboardPtr keyboard = Keyboard::getTask( );
	if ( keyboard->isPushKey( "F3" ) ) {
		mode = MODE_FILESAVE;
	}
	if ( keyboard->isPushKey( "F4" ) ) {
		mode = MODE_FILELOAD;
	}
	if ( keyboard->isPushKey( "F5" ) ) {
		mode = MODE_EDITOR;
	}
	if ( keyboard->isPushKey( "F6" ) ) {
		mode = MODE_VIEWER;
	}
	return mode;
}

void MapEditor::file( bool save ) {
	if ( save ) {
		saveMapData( );
	} else {
		LoadMapDataPtr loadMapData = LoadMapDataPtr( new LoadMapData );
		if ( loadMapData->selectMapData( ) ) {
			_map->load( loadMapData->getMapFileName( ) );
		} 
	}
}

void MapEditor::changeMode( ) {
	switch ( getChangeMode( ) ) {
	case MODE_FILESAVE:
		file( true );
		_now = MODULE_EDITOR;
		resetModule( );
		break;
	case MODE_FILELOAD:
		file( false );
		_now = MODULE_EDITOR;
		resetModule( );
		break;
	case MODE_EDITOR:
		if ( _now != MODULE_EDITOR ) {
			_now = MODULE_EDITOR;
			resetModule( );
		}
		break;
	case MODE_VIEWER:
		if ( _now != MODULE_VIEWER ) {
			_now = MODULE_VIEWER;
			resetModule( );
		}
		break;
	}
}

void MapEditor::resetModule( ) {
	_module.reset( );
	switch ( _now ) {
	case MODULE_VIEWER:
		_module = ModuleBasePtr( new ModuleViewer( _map, _object ) );
		break;
	case MODULE_EDITOR:
		_module = ModuleBasePtr( new ModuleEditor( _map, _object ) );
		break;
	}
}

void MapEditor::saveMapData( ) {
	char filename[ 256 ];
	DrawString( 0, 0, "ファイル保存", 0xffff );
	DrawString( 15, 70, "ファイル名:", 0xffff );
	bool savePossible = KeyInputSingleCharString( 115, 70, 256 , (TCHAR * )filename, TRUE ) == 1;
	if ( savePossible ) {
		_map->save( filename );
	}
}