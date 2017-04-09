#pragma once

#include "Modulebase.h"

PTR( Map );
PTR( Grid );
PTR( CameraMapEditor );
PTR( MEObject );

class ModuleEditor : public ModuleBase {
public:
	ModuleEditor( MapPtr map, MEObjectPtr obejct );
	virtual ~ModuleEditor( );
	void update( );
private:
	enum MODE {
		MODE_HEIGHT,
		MODE_ATTRIBUTE,
		MODE_WATER,
	};
private:
	void modifiSector( int mx, int mz );
	void changeInterface( );
	void changeCamera( );
	void removeWildObject( int mx, int mz );
private:
	int _last_mouse_x;
	int _last_mouse_y;
	GridPtr _grid;	MODE _mode;
	int _height;
	int _range;
	int _attribute;
	double _grid_height;
	MEObjectPtr _object;
};

