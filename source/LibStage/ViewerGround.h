#pragma once

#include "smart_ptr.h"
#include "Model.h"

PTR( ViewerGround );
PTR( Map );
PTR( ImageGround );

class ViewerGround : public Model {
public:
	ViewerGround( MapConstPtr map, bool outter );
	virtual ~ViewerGround( );
};
