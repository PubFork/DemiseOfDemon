#pragma once

#include "smart_ptr.h"
#include "Map.h"
#include <string>

PTR( Viewer );
PTR( Model );
PTR( Map );
PTR( ViewerGround );
PTR( ViewerWater );
PTR( ViewerParty );
PTR( ViewerCohort );
PTR( ViewerWeapon );
PTR( ViewerDoors );
PTR( ViewerDeedboxes );
PTR( ViewerTreasures );
PTR( ViewerShadow );
PTR( ViewerBoss );
PTR( ViewerMirror );
PTR( ViewerEffect );
PTR( ViewerExperiences );

class Viewer {
public:
	enum TYPE {
		ENTRY,
		ADV,
		BOSS,
	};
public:
	Viewer( TYPE type, MapPtr map );
	virtual ~Viewer( );
public:
	virtual void update( );
private:
	void flip( );
	void drawDebug( );
	void drawDebugString( const char * format, ... );
private:
	ViewerPartyPtr _party;
	ViewerCohortPtr _cohort;
	ViewerWeaponPtr _weapon;
	ViewerGroundPtr _ground;
	ViewerWaterPtr _water;
	ViewerDoorsPtr _doors;
	ViewerDeedboxesPtr _deedboxes;
	ViewerTreasuresPtr _treasures;
	ViewerShadowPtr _shadow;
	ViewerBossPtr _boss;
	ViewerMirrorPtr _mirror;
	ViewerExperiencesPtr _experiences;

#if _MSC_VER == 1600
		ViewerEffectPtr _effect;
#endif

	int _count;
	int _start_time;
	int _line;
};
