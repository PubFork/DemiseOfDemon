#pragma once

#include "smart_ptr.h"
#include "mathmatics.h"
#include <array>

PTR( ViewerMissile );
PTR( Bullet );

class ViewerMissile {
public:
	ViewerMissile();
	virtual ~ViewerMissile();
public:
	void create( BulletPtr bullet );
	void draw( ); 
private:
	static const int MAX_NUM = 50;
	std::array< Vector, MAX_NUM > _pos;
	std::array< Vector, MAX_NUM > _vec;
	int _num;
	int _id;
};

