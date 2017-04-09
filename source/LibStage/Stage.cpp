#include "Stage.h"
#include "Party.h"
#include "Weapon.h"
#include "Field.h"
#include "Doors.h"
#include "Deedboxes.h"
#include "Treasures.h"
#include "CameraAdv.h"
#include "Cohort.h"
#include "Framework.h"
#include "Viewer.h"
#include "Client.h"
#include "Enemy.h"
#include "Experiences.h"
#include "Sound.h"
#include "Effect.h"
#include "ViewerGround.h"
#include "ViewerWater.h"
#include "ViewerParty.h"
#include "ViewerCohort.h"
#include "ViewerWeapon.h"
#include "ViewerDoors.h"
#include "ViewerDeedboxes.h"
#include "ViewerTreasures.h"
#include "ViewerShadow.h"
#include "ViewerBoss.h"
#include "ViewerMirror.h"
#include "ViewerExperiences.h"

StagePtr Stage::getTask( ) {
	FrameworkPtr fw = Framework::getInstance( );
	return std::dynamic_pointer_cast< Stage >( fw->getTask( Stage::getTag( ) ) );
}

CameraPtr Stage::getCamera( ) {
	return _camera;
}

CohortPtr Stage::getCohort( ) {
	return _cohort;
}

DoorsPtr Stage::getDoors( ) {
	return _doors;
}

DeedboxesPtr Stage::getDeedboxes( ) {
	return _deedboxes;
}

FieldPtr Stage::getField( ) {
	return _field;
}

PartyPtr Stage::getParty( ) {
	return _party;
}

WeaponPtr Stage::getWeapon( ) {
	return _weapon;
}

TreasuresPtr Stage::getTreasures( ) {
	return _treasures;
}

EffectPtr Stage::getEffect( ) {
	return _effect;
}

ExperiencesPtr Stage::getExperiences( ){
	return _experiences;
}

SoundPtr Stage::getSound( ) {
	return _sound;
}

Stage::Stage( const unsigned char state )
: _state( state ) {
	_phase = PHASE_INIT;
	_effect = EffectPtr( new Effect );
	_sound  = SoundPtr( new Sound );
}

Stage::~Stage( ) {
}

void Stage::update( ) {
	manage( );
	updateStatus( );
	updateObjects( );
	draw( );
}

void Stage::updateObjects( ) {
	if ( _camera      ) _camera->update( );
	if ( _cohort      ) _cohort->update( );
	if ( _doors       ) _doors->update( );
	if ( _deedboxes   ) _deedboxes->update( );
	if ( _field       ) _field->update( );
	if ( _party       ) _party->update( );
	if ( _weapon      ) _weapon->update( );
	if ( _treasures   ) _treasures->update( );
	if ( _experiences ) _experiences->update( );
					  
	if ( _viewer      ) _viewer->update( );
					  
	if ( _effect      ) _effect->update( );
	if ( _sound       ) _sound->update( );
}


void Stage::updateStatus( ) {
	switch ( _phase ) {
	case PHASE_INIT:
		phaseInit( );
		break;
	case PHASE_READY:
		phaseReady( );
		break;
	case PHASE_PLAY:
		phasePlay( );
		break;
	}
}

void Stage::phaseInit( ) {
	create( );
	_phase = PHASE_READY;
}

void Stage::phaseReady( ) {
	ClientPtr client = Client::getTask( );
	CLIENTDATA data = client->getClientData( );

	bool expire = false;
	for ( int i = 0; i < PLAYER_NUM; i++ ) {
		if ( data.user[ i ].state == _state  ) {
			expire = true;
			break;
		}
	}

	if ( expire ) {
		_phase = PHASE_PLAY;
	}
}

void Stage::phasePlay( ) {
	ClientPtr client = Client::getTask( );
	CLIENTDATA data = client->getClientData( );

	StagePtr stage = Stage::getTask( );
	PartyPtr party = stage->getParty( );
	
	bool expire = false;
	for ( int i = 0; i < PLAYER_NUM; i++ ) {
		if ( party->getPlayer( i ) ) {
			expire = true;
			if ( data.user[ i ].state != _state  ) {
				party->destroyPlayer( i );
			}
		} else {
			if ( data.user[ i ].state == _state  ) {
				party->createPlayer( i );
				expire = true;
			}
		}
	}

	if ( !expire ) {
		_phase = PHASE_INIT;
	}
}

void Viewer::draw( ) {
	if ( _count == 0 ) { //1フレーム目なら時刻を記憶
		_start_time = GetNowCount();
	}
	if ( _count == 60 ){ //60フレーム目なら平均を計算する
		_start_time = GetNowCount();
		_count = 0;
	}
	_count++;

	int took_time = GetNowCount() - _start_time;	//かかった時間
	int wait_time = _count * 1000 / FPS - took_time;	//待つべき時間
	if ( wait_time > 0 ) {
		Sleep( wait_time );	//待機
	}

	// 画面更新
	ScreenFlip( );
	ClearDrawScreen( );


	_viewer_ground->draw( );

	if (_mirror ) {
		_viewer_mirror->drawBegin( _camera->getEye( ), _camera->getFocus( ) );
		_viewer_ground->draw( );
		if ( _viewer_boss ) _viewer_boss->draw( );
		_viewer_party->drawBody();
		_viewer_weapon->draw();
		_viewer_mirror->drawEnd();
	}

	if ( _viewer_boss      ) _viewer_boss->draw( );
	if ( _viewer_doors     ) _viewer_doors->draw( );
	if ( _viewer_deedboxes ) _viewer_deedboxes->draw( );

	_viewer_party->drawShadow( );
	_viewer_shadow->draw( );

	if ( _viewer_cohort ) _viewer_cohort->draw( );

	_viewer_party->drawBody( );
	_viewer_weapon->draw( );

	if ( _viewer_treasures   ) _viewer_treasures->draw( );
	if ( _viewer_water       ) _viewer_water->draw( );
	if ( _viewer_experiences ) _viewer_experiences->draw( );

#	if _MSC_VER == 1600
		_viewer_effect->draw( );
#	endif

	if ( CheckHitKey( KEY_INPUT_SPACE ) ) {
		drawDebug( );
	}
}
