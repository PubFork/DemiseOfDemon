#pragma once

#include "Camera.h"

PTR( CameraCamp );

class CameraCamp : public Camera {
public:
	CameraCamp( );
	virtual ~CameraCamp( );
private:
	virtual void update( );
};

