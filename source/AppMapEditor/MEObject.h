#pragma once

#include "smart_ptr.h"
#include "mathmatics.h"

PTR( MEObject );
PTR( Map );

class MEObject {
public:
	MEObject( MapConstPtr map );
	virtual ~MEObject();
public:
	void draw( );
	void setHeight( double height );
private:
	enum MODEL {
		MODEL_DOOR,
		MODEL_CYCLOPS,
		MODEL_DEEDBOX,
		MODEL_GOBLIN,
		MODEL_REDGOBLIN,
		MODEL_GHOST,
		MODEL_ARMOR,
		MODEL_GEM,
		MAX_MODEL,
	};
private:
	void create( MODEL model, const char * filename );
	void drawDoorPX( const Vector& pos );
	void drawDoorMX( const Vector& pos );
	void drawDoorPZ( const Vector& pos );
	void drawDoorMZ( const Vector& pos );
	void drawObject( MODEL model, const Vector& pos, double angle );
private:
	MapConstPtr _map;
	int _id[ MAX_MODEL ];
	double _height;
};

