#include "StageTest.h"
#include "Framework.h"
#include "Network.h"
#include "Party.h"
#include "Weapon.h"
#include "Field.h"
#include "Doors.h"
#include "Deedboxes.h"
#include "Treasures.h"
#include "CameraAdv.h"
#include "Cohort.h"
#include "Client.h"
#include "Experiences.h"
#include "Sound.h"
#include "Effect.h"

StageTest::StageTest( std::string filename ) 
: Stage( STATE_TEST )
, _filename( filename ) {
	_viewer = ViewerTestPtr( new ViewerTest );
}

StageTest::~StageTest( ) {
}

void StateTest::create( ) {
	MapPtr map = MapPtr( new Map( _filename ) );

	_camera      = CameraPtr( new CameraTest );
	_field       = FieldPtr( new Field( map ) );
	_cohort      = CohortPtr( new Cohort );
	_doors       = DoorsPtr( new Doors );
	_deedboxes   = DeedboxesPtr( new Deedboxes );
	_party       = PartyPtr( new Party );
	_weapon      = WeaponPtr( new Weapon );
	_treasures   = TreasuresPtr( new Treasures );
	_experiences = ExperiencesPtr( new Experiences );
}

