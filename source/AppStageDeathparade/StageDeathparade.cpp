#include "StageDeathparade.h"
#include "Network.h"

const char* FILE_NAME = "deathparade.map";

StageDeathparade::StageDeathparade( )
: Stage( STATE_DEATHPARADE ) {

}

StageDeathparade::~StageDeathparade() {

}

std::string StageDeathparade::getMapFilename( ) const {
	return FILE_NAME;
}

void StageDeathparade::updateManagement( ) {

}
