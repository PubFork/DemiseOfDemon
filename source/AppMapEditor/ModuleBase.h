#pragma once

#include "smart_ptr.h"

PTR( ModuleBase );
PTR( Map );

class ModuleBase {
public:
	ModuleBase( MapPtr map ) : _map( map ) { }
	virtual ~ModuleBase( ) { }
	virtual void update( ) = 0;
protected:
	MapPtr _map;
};

