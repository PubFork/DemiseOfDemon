#include "Party.h"
#include "Player.h"
#include "Framework.h"
#include "Network.h"
#include "Camera.h"
#include "Field.h"
#include "Client.h"
#include "Stage.h"
#include "Sound.h"
#include "Effect.h"
#include "PlayerKnight.h"
#include "PlayerWarrior.h"
#include "PlayerMage.h"
#include "PlayerMonk.h"
#include "PlayerShooter.h"
#include "PlayerSummoner.h"

Party::Party( ) {
	Player::initialize( );
}


Party::~Party( ) {

}

void Party::update( ) {
	ClientPtr client = Client::getTask( );
	CLIENTDATA data = client->getClientData( );
	FrameworkPtr fw = Framework::getInstance( );

	for ( int i = 0; i < PLAYER_NUM; i++ ) {
		PlayerPtr player = _player[ i ];
		if ( !player ) {
			continue;
		}
		bool jump = ( ( data.user[ i ].button & BUTTON_B ) != 0 );
		bool attack = ( ( data.user[ i ].button & BUTTON_A ) != 0 );
		player->input( data.user[ i ].x, data.user[ i ].y, jump, attack );
		player->update( );

	}
}

PlayerConstPtr Party::getPlayer( int index ) const {
	return _player[ index ];
}

PlayerPtr Party::getPlayer( int index ) {
	return _player[ index ];
}

void Party::createPlayer( int index ) {
	StagePtr stage = Stage::getTask( );
	CameraPtr camera = stage->getCamera( );
	FieldPtr field = stage->getField( );
	
	Vector focus = camera->getFocus( );
	int x = ( int )focus.x;
	int z = ( int )focus.z;
	Vector pos = field->getValidPos( x, z, true );

	SoundPtr sound = stage->getSound( );
	sound->play( Sound::SOUND_PLAYER_APPEARANCE );
	EffectPtr effect = stage->getEffect( );
	effect->play( Effect::TESTBLUE, pos, 0.1f );

	switch ( index ) {
	case PLAYER_KNIGHT:
		_player[ index ] = PlayerPtr( new PlayerKnight( pos ) );
		break;
	case PLAYER_WARRIOR:
		_player[ index ] = PlayerPtr( new PlayerWarrior( pos ) );
		break;
	case PLAYER_MAGE:
		_player[ index ] = PlayerPtr( new PlayerMage( pos ) );
		break;
	case PLAYER_MONK:
		_player[ index ] = PlayerPtr( new PlayerMonk( pos ) );
		break;
	case PLAYER_SHOOTER:
		_player[ index ] = PlayerPtr( new PlayerShooter( pos ) );
		break;
	case PLAYER_SUMMONER:
		_player[ index ] = PlayerPtr( new PlayerSummoner( pos ) );
		break;
	default:
		_player[ index ] = PlayerPtr( new Player( index, pos ) );
	}
	
}


void Party::destroyPlayer( int index ) {
	StagePtr stage = Stage::getTask( );
	SoundPtr sound = stage->getSound( );
	sound->play( Sound::SOUND_AREA_WARP );
	EffectPtr effect = stage->getEffect( );
	effect->play( Effect::BMVOL3_WARP, _player[ index ]->getPos( ), 0.1f );

	_player[ index ].reset( );
}