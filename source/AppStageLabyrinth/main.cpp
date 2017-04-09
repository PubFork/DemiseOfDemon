#include "Framework.h"
#include "Viewer.h"
#include "Client.h"
#include "StageLabyrinth.h"

void main() {
	TaskPtr stage = StageLabyrinthPtr (new StageLabyrinth ( ) );
	TaskPtr client = ClientPtr ( new Client );

	FrameworkPtr fw = Framework::getInstance( );
	fw->addTask( Stage::getTag( ), stage );
	fw->addTask( Client::getTag( ), client );
}