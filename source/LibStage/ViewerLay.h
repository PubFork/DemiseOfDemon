#pragma once

#include "smart_ptr.h"
#include "mathmatics.h"
#include <array>

PTR( ViewerLay );
PTR( Model );
PTR( Bullet );

class ViewerLay {
public:
	ViewerLay();
	virtual ~ViewerLay();
public:
	void create( BulletPtr bullet );
	void draw( ); 
private:
	static const int MAX_NUM = 20;
	std::array< ModelPtr, MAX_NUM > _model;
	int _num;
public:
	static void updateScroll( );
private:
	static double _scroll;
};

