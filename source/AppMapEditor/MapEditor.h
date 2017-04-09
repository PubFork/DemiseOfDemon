#pragma once

#include "smart_ptr.h"
#include "Task.h"
#include <string>

PTR( MapEditor );
PTR( ModuleBase );
PTR( Map );
PTR( MEObject );
PTR( CameraMapEditor );

class MapEditor : public Task {
public:
	static std::string getTag( ) { return "MAPEDITOR"; }
	static MapEditorPtr getTask( );
public:
	MapEditor( );
	virtual ~MapEditor( );
	void update( );
	virtual CameraMapEditorPtr getCamera( );
private:
	enum MODE {
		MODE_NONE,
		MODE_EDITOR,
		MODE_VIEWER,
		MODE_FILESAVE,
		MODE_FILELOAD,
	};

	enum MODULE {
		MODULE_EDITOR,
		MODULE_VIEWER,
	};
private:
	MODE getChangeMode( );
	void changeMode( );
	void resetModule( );
	void file( bool save );
	void saveMapData( );	//saveä÷êî
private:
	MODULE _now;
	ModuleBasePtr _module;
	MapPtr _map;
	MEObjectPtr _object;
	CameraMapEditorPtr _camera;
};

