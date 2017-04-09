#pragma once

#include "Stage.h"
#include "smart_ptr.h"
#include <string>

PTR( StageTest );

class StageTest : public Stage {
public:
	StageTest( std::string filename );
	virtual ~StageTest( );
private:
	virtual void create( );
private:
	std::string _filename;
};

