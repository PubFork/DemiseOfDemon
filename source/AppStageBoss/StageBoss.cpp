#include "StageBoss.h"
#include "Network.h"
#include "Cohort.h"
#include "Client.h"


const char * FILENAME = "boss.map";
const int WAIT_TIME = 10;

StageBoss::StageBoss( ) 
: Stage( STATE_BOSS ) {
	_time = 0;
}

StageBoss::~StageBoss( ) {
}

std::string StageBoss::getMapFilename( ) const {
	return FILENAME;
}

void StageBoss::updateManagement( ) {
	CohortPtr cohort = getCohort( );
	if ( cohort->isAliveMinotaur( ) ) {
		return;
	}
	if ( ( _time >= ( WAIT_TIME * 30 ) ) ) {
		ClientPtr client = Client::getTask( );
		client->moveStage( STATE_BOSS ,STATE_CAMP );
	}
	_time++;
}

/*

	if ( _state == STATE_BOSS ) {
		ClientPtr client = Client::getTask( );
		CLIENTDATA data = client->getClientData( );
		for ( int i = 0; i < 3 - ( data.stage[ STAGE_BOSS ] - 1 ); i++ ) {
			CohortPtr cohort = getCohort( );
			Vector pos = Vector( 40 + ( data.stage[STAGE_BOSS] ) * 5 + i * 5, 20, 50 );
			FieldPtr field = getField( );
			pos.y = field->getHeight( ( int )pos.x, ( int )pos.z );
			EnemyMinotaurPtr enemy = EnemyMinotaurPtr( new EnemyMinotaur( pos ) );
			if ( i == 0 ) {
				enemy->setStone( false );
			}
			cohort->add( EnemyPtr( enemy ) );
		}
	}
*