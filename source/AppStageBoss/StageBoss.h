#pragma once

#include "Stage.h"
#include "smart_ptr.h"
#include <string>

PTR( StageBoss );

class StageBoss : public Stage {
public:
	StageBoss( );
	virtual ~StageBoss( );
	virtual std::string getMapFilename( ) const;
private:
	virtual void updateManagement( );
	int _time;
};

