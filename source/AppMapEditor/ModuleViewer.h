#pragma once

#include "Modulebase.h"

PTR( Map );
PTR( ViewerGround );
PTR( ViewerWater );
PTR( MEObject);

class ModuleViewer : public ModuleBase {
public:
	ModuleViewer( MapPtr map, MEObjectPtr object );
	virtual ~ModuleViewer( );
	void update( );
private:
	int _last_mouse_x;
	int _last_mouse_y;
	ViewerGroundPtr _ground;
	ViewerWaterPtr _water;
	MEObjectPtr _object;
};