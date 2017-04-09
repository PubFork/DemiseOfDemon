#pragma once

#include "smart_ptr.h"
#include "mathmatics.h"
#include <array>

PTR( ViewerShot );
PTR( Bullet );
PTR( Model );

class ViewerShot {
public:
	ViewerShot();
	virtual ~ViewerShot();
public:
	void create( BulletPtr bullet );
	void draw( ); 
private:
	static const int MAX_NUM = 100;
	std::array< Vector, MAX_NUM > _pos;
	std::array< Vector, MAX_NUM > _vec;
	ModelPtr _model;
	int _num;
	int _id;
};

