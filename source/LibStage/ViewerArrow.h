#pragma once

#include "smart_ptr.h"
#include "mathmatics.h"
#include "BulletArrow.h"
#include <array>

PTR( ViewerArrow );
PTR( Bullet );

class ViewerArrow {
public:
	ViewerArrow();
	virtual ~ViewerArrow();
public:
	void create( BulletPtr bullet );
	void draw( ); 
private:
	static const int MAX_NUM = 100;
	struct DATA {
		Vector pos;
		double priority;
	};
	static const int PARTICLE_NUM = 500;
	struct PARTICLE {
		Vector pos;
		Vector vec;
		int count;
	};

	std::array< DATA, MAX_NUM > _data;
	std::array< PARTICLE, PARTICLE_NUM > _particle;
	int _idx;
	int _num;
	int _id;
	int _id_particle;
};

