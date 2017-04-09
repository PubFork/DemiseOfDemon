#pragma once

#include "smart_ptr.h"
#include "mathmatics.h"

PTR( Model );
PTR( ModelImpl );

class Model {
public:
	Model( );
	virtual ~Model( );
public:
	struct VERTEX {
		Vector pos;
		double u;
		double v;
		VERTEX( ) { }
		VERTEX( 
			Vector pos_,
			double u_,
			double v_ ) {
				pos = pos_;
				u = u_;
				v = v_;
		}
	};
public:
	void draw( bool trans = false ) const;
public:
	void setTexture( int id );
	void alloc( int polygon_num );
	void translate( Vector move );
	void setPolygonNum( int num );
	void set( int n, VERTEX vertex ); // n * 3
private:
	ModelImplPtr _impl;
};

