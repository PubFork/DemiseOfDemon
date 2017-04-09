#pragma once

#include "Camera.h"

PTR( CameraMapEditor );

class CameraMapEditor : public Camera {
public:
	CameraMapEditor( );
	virtual ~CameraMapEditor( );
	virtual void update( );
	void setFocus( const Vector& focus_pos, const Vector& focus_target );
	void moveHorizontal( double value );
	void moveVertical( double value );
	void moveYaw( double value );
	void movePitch( double value );
	void moveZoom( double value );
	void appearanceFocus( );
	double get_Focus_pos_x( ) const;
	double get_Focus_pos_y( ) const;
	double get_Focus_pos_z( ) const;
	double get_Focus_target_x( ) const;
	double get_Focus_target_y( ) const;
	double get_Focus_target_z( ) const;
protected:
	Vector _focus_pos;
	Vector _focus_target;

};