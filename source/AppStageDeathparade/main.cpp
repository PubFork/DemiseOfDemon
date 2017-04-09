#include "Framework.h"
#include "Viewer.h"
#include "Client.h"
#include "StageDeathparade.h"



void main() {

	TaskPtr stage = StageDeathparadePtr (new StageDeathparade( ) );
	TaskPtr client = ClientPtr ( new Client );

	FrameworkPtr fw = Framework::getInstance( );
	fw->addTask( Stage::getTag( ), stage );
	fw->addTask( Client::getTag( ), client );
}