#include "StageLabyrinth.h"
#include "Cohort.h"
#include "Client.h"

const char* FILE_NAME = "labyrinth.map"; 
const int WAIT_TIME = 10;

StageLabyrinth::StageLabyrinth( ) 
: Stage( STATE_LABYRINTH ){
	_time = 0;
}

StageLabyrinth::~StageLabyrinth( ) {
}

std::string StageLabyrinth::getMapFilename( ) const {
	return FILE_NAME;
}

void StageLabyrinth::updateManagement( ) {
	CohortPtr cohort = getCohort( );
	if ( cohort->isAliveCyclops( ) ) {
		return;
	}
	if ( ( _time >= ( WAIT_TIME * 30 ) ) ) {
		ClientPtr client = Client::getTask( );
		client->moveStage( STATE_LABYRINTH, STATE_DEATHPARADE );
	}
	_time++;
}
