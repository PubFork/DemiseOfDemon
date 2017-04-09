#pragma once
#include "smart_ptr.h"

PTR( Grid );
PTR( Map );
PTR( Model );
PTR( ImageBase );

class Grid  {
public:
	Grid( MapConstPtr map );
	virtual ~Grid( );
public:
	void draw( );
	void drawCursor( int x, int z, int h, int range, const int mode );
	void updateBox( int x, int z );
	void updateWater( int x, int z );
	void setHeight( double height );
private:
	void drawSelected( int x, int z, int h );
private:
	MapConstPtr _map;
	ModelPtr _model_box;
	ModelPtr _model_water;
	ImageBasePtr _image;
	double _height;
};