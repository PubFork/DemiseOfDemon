#pragma once

#include "Stage.h"
#include "smart_ptr.h"
#include <string>

PTR( StageDeathparade );

class StageDeathparade:public Stage {
public:
	StageDeathparade();
	virtual ~StageDeathparade();
	virtual std::string getMapFilename( ) const;
private:
	void updateManagement( );
};

