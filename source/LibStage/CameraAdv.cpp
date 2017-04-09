#include "CameraAdv.h"
#include "Framework.h"
#include "Party.h"
#include "Player.h"
#include "Field.h"
#include "Stage.h"

CameraAdv::CameraAdv( const Vector& eye, const Vector& focus )
: _init_x( ( int )focus.x )
, _init_z( ( int )focus.z ) {
	_eye = eye;
	_vec = eye - focus;
}

CameraAdv::~CameraAdv( ) {

}

void CameraAdv::init( ) {
	FrameworkPtr fw = Framework::getInstance( );
	StagePtr stage = Stage::getTask( );
	FieldPtr field = stage->getField( );
	int y = field->getHeight( _init_x, _init_z );
	_focus = Vector( _init_x + 0.5, y + 0.5, _init_z + 0.5 );
}

void CameraAdv::update( ) {
	trackingOfPlayers( );
	updateEye( );
	updateCamera( );
}

void CameraAdv::trackingOfPlayers( ) {
	StagePtr stage = Stage::getTask( );
	FieldPtr field = stage->getField( );
	PartyPtr party = stage->getParty( );

	if ( !party ) {
		return;
	}

	Vector sum;
	int player_num = 0;
	for ( int i = 0; i < PLAYER_NUM; i++ ) {
		PlayerConstPtr player = party->getPlayer( i );
		if ( player  ) {
			sum += player->getStandPos( );
			player_num++;
		}
	}

	if ( player_num == 0 ) {
		return;
	}

	Vector target = sum * ( 1.0 / player_num );
	_focus += ( target - _focus ) * 0.05; 
}

void CameraAdv::updateEye( ) {
	_eye = _focus + _vec; 
}
