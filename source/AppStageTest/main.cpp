#include "Framework.h"
#include "Client.h"
#include "StageTest.h"
#include "loadMapData.h"
#include "Exception.h"

void main( ) {
	FrameworkPtr fw = Framework::getInstance( );
	fw->addTask( Keyboard::getTag( ), TaskPtr( new Keyboard ) );

	LoadMapDataPtr loadMapData = LoadMapDataPtr( new LoadMapData );
	if ( !loadMapData->selectMapData( ) ) {
		exception( "マップが選ばれなかったので終了します" );	
	}
	
	const char* MAP_NAME = loadMapData->getMapFileName( );
	
	StagePtr stage = StageTestPtr( new StageTest( MAP_NAME ) );
	TaskPtr client = ClientPtr( new Client );

	fw->addTask( Stage::getTag( ), stage );
	fw->addTask( Client::getTag( ), client );
}