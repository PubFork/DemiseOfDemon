#pragma once

#include "smart_ptr.h"
#include "mathmatics.h"
#include <array>

PTR( ViewerNova );
PTR( Model );
PTR( Bullet );

class ViewerNova {
public:
	ViewerNova();
	virtual ~ViewerNova();
public:
	void create( BulletPtr bullet );
	void draw( ); 
private:
	static const int MAX_NUM = 3;
	std::array< ModelPtr, MAX_NUM > _model;
	int _num;
};

