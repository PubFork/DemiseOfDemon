#include "Framework.h"
#include "Console.h"
#include "Task.h"
#include "Network.h"
#include "Device.h"
#include "Server.h"

bool main( ) {
	
	FrameworkPtr fw = Framework::getInstance( );
	fw->changeServerWindow( );

	TaskPtr console = ConsolePtr( new Console );
	fw->addTask( Console::getTag( ), console );

	TaskPtr device = DevicePtr( new Device );
	fw->addTask( Device::getTag( ), device );

	TaskPtr server = ServerPtr( new Server );
	fw->addTask( Server::getTag( ), server );

	return true;
}