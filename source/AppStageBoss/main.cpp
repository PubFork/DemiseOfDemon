#include "Framework.h"
#include "Viewer.h"
#include "Client.h"
#include "StageBoss.h"
#include "Boss.h"

void main() {
	TaskPtr stage = StageBossPtr( new StageBoss( ) );
	TaskPtr client = ClientPtr( new Client );
	TaskPtr boss = BossPtr(new Boss);

	FrameworkPtr fw = Framework::getInstance( );
	fw->addTask( Stage::getTag( ), stage );
	fw->addTask(Client::getTag(), client);
	fw->addTask(Boss::getTag(), boss);
}