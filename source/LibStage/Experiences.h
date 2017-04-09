#pragma once
#include "smart_ptr.h"
#include "mathmatics.h"
#include <array>

PTR( Experiences );
PTR( Experience );

class Experiences {
public:
	Experiences( );
	virtual ~Experiences( );
public:
	void init( );
	virtual void update( );
	ExperienceConstPtr getExperience( int index ) const;
	ExperiencePtr getExperience( int index );
	void create( const Vector& pos, int num );
private:
	void add( ExperiencePtr treasure );
public:
	static const int MAX_NUM = 200;
private:
	std::array< ExperiencePtr, MAX_NUM > _experience;
};

