#include "Framework.h"
#include "Viewer.h"
#include "Client.h"
#include "StageCamp.h"

void main( ) {
	TaskPtr stage = StageCampPtr ( new StageCamp ( ) );
	TaskPtr client = ClientPtr ( new Client );

	FrameworkPtr fw = Framework::getInstance( );
	fw->addTask( Stage::getTag( ), stage );
	fw->addTask( Client::getTag( ), client );
}