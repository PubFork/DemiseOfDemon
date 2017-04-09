#pragma once

#include "Camera.h"

PTR( CameraAdv );

class CameraAdv : public Camera {
public:
	CameraAdv( const Vector& eye, const Vector& focus );
	virtual ~CameraAdv( );
public:
	virtual void init( );
	virtual void update( );
private:
	void trackingOfPlayers( );
	void updateEye( );
private:
	const int _init_x;
	const int _init_z;
	Vector _vec;
};

