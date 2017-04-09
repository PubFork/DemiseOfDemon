#pragma once

#include "Task.h"
#include <string>

PTR( Stage );
PTR( Camera );
PTR( Cohort );
PTR( Doors );
PTR( Deedboxes );
PTR( Viewer );
PTR( Field );
PTR( Party );
PTR( Weapon );
PTR( Treasures );
PTR( Sound );
PTR( Effect );
PTR( Experiences );
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

class Stage : public Task {
public:
	static std::string getTag( ) { return "STAGE"; }
	static StagePtr getTask( );
public:
	Stage( const unsigned char state );
	virtual ~Stage( );
public:
	CameraPtr      getCamera( );
	CohortPtr      getCohort( );
	DoorsPtr       getDoors( );
	DeedboxesPtr   getDeedboxes( );
	FieldPtr       getField( );    
	PartyPtr       getParty( );   
	WeaponPtr      getWeapon( );  
	TreasuresPtr   getTreasures( );
	EffectPtr      getEffect( );
	ExperiencesPtr getExperiences( );
	SoundPtr       getSound( );
public:
	void update( );
protected:
	CameraPtr      _camera;    
	PartyPtr       _party;   
	WeaponPtr      _weapon;  
	CohortPtr      _cohort;   
	ExperiencesPtr _experiences;
	FieldPtr       _field;   
	DoorsPtr       _doors;    
	DeedboxesPtr   _deedboxes;
	TreasuresPtr   _treasures;
	EffectPtr      _effect;  
	SoundPtr       _sound;
	ViewerPartyPtr       _viewer_party;
	ViewerCohortPtr      _viewer_cohort;
	ViewerWeaponPtr      _viewer_weapon;
	ViewerGroundPtr      _viewer_ground;
	ViewerWaterPtr       _viewer_water;
	ViewerDoorsPtr       _viewer_doors;
	ViewerDeedboxesPtr   _viewer_deedboxes;
	ViewerTreasuresPtr   _viewer_treasures;
	ViewerShadowPtr      _viewer_shadow;
	ViewerBossPtr        _viewer_boss;
	ViewerMirrorPtr      _viewer_mirror;
	ViewerExperiencesPtr _viewer_experiences;
private:
	enum PHASE {
		PHASE_INIT,
		PHASE_READY,
		PHASE_PLAY,
	};
private:
	virtual void create( ) = 0;
	virtual void manage( ) { }
private:
	void updateObjects( );
	void updateStatus( );
	void phaseInit( );
	void phaseReady( );
	void phasePlay( );
private:
	const unsigned char _state;
	PHASE _phase;
};

