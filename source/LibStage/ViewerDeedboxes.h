#pragma once

#include "Deedboxes.h"
#include <array>

PTR( ViewerDeedboxes );

class ViewerDeedboxes {
public:
	ViewerDeedboxes( );
	virtual ~ViewerDeedboxes( );
public:
	void draw( );
private:
	struct DATA {
		int body_id;
		int anim_id;
		double time;
	};
private:
	void init( );
private:
	std::array< DATA, Deedboxes::MAX_NUM > _data;
};

