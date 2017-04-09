#pragma once

#include "smart_ptr.h"
#include "mathmatics.h"
#include <array>

PTR( ViewerSword );
PTR( Model );
PTR( Bullet );

class ViewerSword {
public:
	ViewerSword();
	virtual ~ViewerSword();
public:
	void create( BulletPtr bullet );
	void draw( ); 
private:
	static const int MAX_NUM = 3;
	std::array< ModelPtr, MAX_NUM > _model;
	int _num;
};

