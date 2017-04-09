#pragma once
#include "Stage.h"
#include "smart_ptr.h"
#include <string>

PTR( StageCamp );
PTR( Camera );

class StageCamp : public Stage {
public:
	StageCamp( );
	virtual ~StageCamp();
	virtual std::string getMapFilename( ) const;
private:
	virtual void updateManagement( );
	virtual CameraPtr createCamera( );
};

