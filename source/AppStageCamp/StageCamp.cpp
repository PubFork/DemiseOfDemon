#include "StageCamp.h"
#include "Client.h"
#include "CameraCamp.h"

const char* FILE_NAME = "camp.map";

StageCamp::StageCamp( )
: Stage( STATE_CAMP ) {

}

StageCamp::~StageCamp( ) {

}

CameraPtr StageCamp::createCamera( ) {
	return CameraPtr( new CameraCamp );
}

std::string StageCamp::getMapFilename( ) const {
	return FILE_NAME;
}

void StageCamp::updateManagement( ) {
}
