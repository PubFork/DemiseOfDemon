#include "Status.h"
#include "Framework.h"
#include "ViewerUI.h"
#include "Client.h"
#include "Sound.h"

const int TIMEOUT = 30 * 2;

StatusPtr Status::getTask( ) {
	FrameworkPtr fw = Framework::getInstance( );
	return std::dynamic_pointer_cast< Status >( fw->getTask( Status::getTag( ) ) );
}

Status::Status( int index )
: _index( index ) {
	_ui = ViewerUIPtr( new ViewerUI( index ) );
	_gauge = 0;
	_timeout = 0;
	_sound  = SoundPtr( new Sound );
	_cheak_state = STATE_NONE;
	_cheak_life = 9999;
}

Status::~Status( ) {
}

void Status::update( ) {
	_ui->draw( );
	_sound->update( );
	updateGauge( );
	updateSE( );
}

void Status::updateGauge( ) {
	ClientPtr client = Client::getTask( );
	CLIENTDATA data = client->getClientData( );

	if ( data.user[ _index ].state != STATE_READY ) {
		return;
	}

	if (  data.user[ _index ].button != 0 ) {
		_gauge++;
		if ( _gauge > 100 ) {
			_gauge = 100;
		}
	} else {
		_gauge -= 2;
		if ( _gauge < 0 ) {
			_gauge = 0;
		}
	}

	if ( _gauge >= 100 ) {
		_timeout--;
		if ( _timeout <= 0 ) {
			SERVERDATA netdata;
			netdata.command = COMMAND_STATE;
			netdata.index = _index;
			netdata.value = STATE_CAMP;
			client->send( netdata );
			_timeout = TIMEOUT;
		}
	}
}

int Status::getGauge( ) {
	return _gauge; // 0 - 100
}

void Status::updateSE( ) {
	ClientPtr client = Client::getTask( );
	CLIENTDATA data = client->getClientData( );
	if ( data.user[ _index ].life <= 10  && _cheak_life != data.user[ _index ].life ) {
		_sound->play( Sound::SOUND_PLAYER_COUNT );
	}
	if ( data.user[ _index ].life == 10 ) {
		_sound->play( Sound::SOUND_PLAYER_COUNT_ANOUNCE );
	}
	if ( data.user[ _index ].life == 0 && _cheak_life != 0 ) {
		_sound->play( Sound::SOUND_PLAYER_COUNT_ZERO );
	}
	if ( data.user[ _index ].state == STATE_CLEAR && _cheak_state != STATE_CLEAR ) {
		_sound->play( Sound::SOUND_GAME_CLEAR );
	}
	if ( data.user[ _index ].state == STATE_DEAD && _cheak_state != STATE_DEAD ) {
		_sound->play( Sound::SOUND_GAME_OVER );
	}
	if ( _gauge == 100 && data.user[ _index ].state == STATE_READY ) {
		_sound->play( Sound::SOUND_GAME_START );
	}
	_cheak_state =  data.user[ _index ].state;
	_cheak_life = data.user[ _index ].life;
}