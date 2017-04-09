#include "Status.h"
#include "Framework.h"
#include "Network.h"
#include "client.h"

void main( ) {
	TaskPtr status = StatusPtr( new Status( PLAYER_WARRIOR ) );
	TaskPtr client = ClientPtr( new Client( ) );

	FrameworkPtr fw = Framework::getInstance( );
	fw->addTask( Status::getTag( ), status );
	fw->addTask( Client::getTag( ), client );
}