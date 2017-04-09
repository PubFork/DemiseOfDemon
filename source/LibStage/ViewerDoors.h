#pragma once

#include "Doors.h"
#include <array>

PTR( ViewerDoors );

class ViewerDoors {
public:
	ViewerDoors( );
	virtual ~ViewerDoors( );
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
	std::array< DATA, Doors::MAX_NUM > _data;
};

