#pragma once
#include "Stage.h"
#include "smart_ptr.h"
#include <string>

PTR( StageLabyrinth );

class StageLabyrinth : public Stage {
public:
	StageLabyrinth( );
	virtual ~StageLabyrinth( );
	virtual std::string getMapFilename( ) const;
private:
	virtual void updateManagement( );
	int _time;
};

