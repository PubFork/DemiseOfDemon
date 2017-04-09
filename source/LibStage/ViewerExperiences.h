#pragma once

#include "Experience.h"
#include <array>

PTR( ViewerExperiences );
PTR( Experience );

class ViewerExperiences {
public:
	ViewerExperiences( );
	virtual ~ViewerExperiences( );
public: 
	void draw( );
private:
	static const int PARTICLE_NUM = 200;
private:
	void drawExperience( ExperienceConstPtr experience );
	void createParticle( const Vector& center );
	void drawParticle( );
private:
	int _id;
	std::array< Vector, PARTICLE_NUM > _particle_pos;
	std::array< int, PARTICLE_NUM > _particle_count;
	int _index;
	int _wait;
};

