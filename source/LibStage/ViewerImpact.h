#pragma once

#include "smart_ptr.h"
#include "mathmatics.h"
#include <array>

PTR( ViewerImpact );
PTR( Bullet );
PTR( Model );

class ViewerImpact {
public:
	ViewerImpact();
	virtual ~ViewerImpact();
public:
	void create( BulletPtr bullet );
	void draw( ); 
private:
	ModelPtr _model;
	int _alpha;
};

