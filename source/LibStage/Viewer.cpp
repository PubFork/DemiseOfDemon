#include "Viewer.h"
#include "Framework.h"
#include "Client.h"
#include "DxLib.h"
#include "Network.h"
#include "Ip.h"
#include "Party.h"
#include "Model.h"
#include "DxLib.h"
#include "Player.h"
#include "Map.h"
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
#include "Stage.h"
#include "Camera.h"
#include "Viewer.h"

#if _MSC_VER == 1600
#	include "ViewerEffect.h"
#endif

const int FPS = 30;

Viewer::Viewer( TYPE type, MapPtr map ) {
	_party = ViewerPartyPtr( new ViewerParty );
	_weapon = ViewerWeaponPtr( new ViewerWeapon );
	_deedboxes = ViewerDeedboxesPtr( new ViewerDeedboxes );
	_treasures = ViewerTreasuresPtr( new ViewerTreasures );
	_doors = ViewerDoorsPtr( new ViewerDoors );
	_shadow = ViewerShadowPtr( new ViewerShadow ) ;
	_experiences = ViewerExperiencesPtr( new ViewerExperiences );

	_cohort = ViewerCohortPtr( new ViewerCohort );
	_ground = ViewerGroundPtr( new ViewerGround( map, false ) );
	_water = ViewerWaterPtr( new ViewerWater( map ) );
	
#	if _MSC_VER == 1600
		_effect = ViewerEffectPtr( new ViewerEffect );
#	endif

	if (type == BOSS) {
		_boss = ViewerBossPtr(new ViewerBoss);
		_mirror = ViewerMirrorPtr(new ViewerMirror);
		_mirror->setPos(Vector(40, 11.001, 40), 20, 13);
	}
	_count = 0;
}

Viewer::~Viewer( ) {
}

void Viewer::update( ) {
	flip( );

	_ground->draw( );

	if (_mirror) {
		StagePtr stage = Stage::getTask();
		CameraPtr camera = stage->getCamera();
		_mirror->drawBegin( camera->getEye( ), camera->getFocus( ) );
		_ground->draw( );
		if (_boss) {
			_boss->draw();
		}
		_party->drawBody();
		_weapon->draw();
		_mirror->drawEnd();
	}


	if (_boss) {
		_boss->draw();
	}

	_doors->draw( );
	_deedboxes->draw( );

	_party->drawShadow( );
	_shadow->draw( );
	if ( _cohort ) {
		_cohort->draw( );
	}
	_party->drawBody( );
	_weapon->draw( );
	_treasures->draw( );
	_water->draw( );
	_experiences->draw( );

#	if _MSC_VER == 1600
		_effect->draw( );
#	endif

	if ( CheckHitKey( KEY_INPUT_SPACE ) ) {
		drawDebug( );
	}
}

void Viewer::flip( ) {
	if ( _count == 0 ) { //1�t���[���ڂȂ玞�����L��
		_start_time = GetNowCount();
	}
	if ( _count == 60 ){ //60�t���[���ڂȂ畽�ς��v�Z����
		_start_time = GetNowCount();
		_count = 0;
	}
	_count++;

	int took_time = GetNowCount() - _start_time;	//������������
	int wait_time = _count * 1000 / FPS - took_time;	//�҂ׂ�����
	if ( wait_time > 0 ) {
		Sleep( wait_time );	//�ҋ@
	}

	// ��ʍX�V
	ScreenFlip( );
	ClearDrawScreen( );
}

void Viewer::drawDebug( ) {
	_line = 0;

	ClientPtr client = Client::getTask( );
	CLIENTDATA status = client->getClientData( );

	{	// �T�[�o�[IP�\��
		IP ip = client->getServerIP( );
		drawDebugString( "�T�[�o�[IP : %d %d %d %d",
			ip.address[ 0 ], ip.address[ 1 ],ip.address[ 2 ],ip.address[ 3 ] );
	}

	
	{	// �N���C�A���gIP�\��
		IP ip = client->getClientIP( );
		drawDebugString( "�N���C�A���gIP : %d %d %d %d",
			ip.address[ 0 ], ip.address[ 1 ],ip.address[ 2 ],ip.address[ 3 ] );
	}

	{	// ���
		std::string str = client->getPhase( );
		drawDebugString( "��� : %s", str.c_str( ) );
	}
	
	//drawString( 0, 280, "user   dx   dy button   state weapon level life       Area" );
	drawDebugString( "user   dx   dy button   state weapon level life" );
	for ( int i = 0; i < PLAYER_NUM; i++ ) {
		drawDebugString( "% 4d % 4d % 4d %s % 5x % 6d % 5d % 10u",
			i,
			status.user[ i ].x, status.user[ i ].y,
			BTOS( status.user[ i ].button ).c_str( ),
			status.user[ i ].state,
			status.user[ i ].level_weapon,
			status.user[ i ].life );
	}
}

void Viewer::drawDebugString( const char * format, ... ) {
	const int COLOR = 0xffffff;

	va_list args;
	va_start( args, format );
	char str[ 255 ];
	vsprintf_s( str, 255, format, args );
	DrawFormatString( 0, _line * 20, COLOR, str );
	_line++;
}
