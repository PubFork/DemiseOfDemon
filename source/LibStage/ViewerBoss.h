#pragma once

#include "smart_ptr.h"
#include "Boss.h"
#include <array>

PTR( ViewerBoss );

class ViewerBoss {
public:
	ViewerBoss( );
	virtual ~ViewerBoss( );
public:
	void draw( );
private:
	struct ID {
		int anim_id;
		int body_id;
	};
private:
	void createModel( );
	void create( Boss::MOTION motion, const char * filename );
private:
	std::array< ID, Boss::MAX_MOTION > _id;	
	int _title_id;
};

