#pragma once

#include "smart_ptr.h"
#include "DxLib.h"
#include "mathmatics.h"

PTR( ViewerMirror );

class ViewerMirror {
public:
	ViewerMirror( );
	virtual ~ViewerMirror( );
public:
	void setPos( Vector pos, int width, int depth );
	void drawBegin( const Vector eye, const Vector focus );
	void drawEnd( );
private:
	void setupCamera( VECTOR CameraEyePos, VECTOR CameraTargetPos );
	void render( );
private:
	VECTOR _eye;
	VECTOR _focus;
	int _pixel_shader;
	int _vertex_shader;
	int _handle;
	VECTOR _world_pos[ 4 ];
	FLOAT4 _screen_pos[ 4 ];
	VERTEX3DSHADER _vertex[ 6 ];
};

