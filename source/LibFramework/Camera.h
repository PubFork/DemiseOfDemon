#pragma once

#include "smart_ptr.h"
#include "mathmatics.h"

PTR( Camera );

class Camera {
public:
	Camera( );
	virtual ~Camera( );
public:
	virtual void update( ) = 0;
	virtual void init( ) { }
public:
	Vector getEye( ) const;
	Vector getFocus( ) const;
protected:
	void updateCamera( );
protected:	
	Vector _eye;
	Vector _focus;
};