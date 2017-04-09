#include "Experience.h"
#include "Client.h"
#include "Player.h"
#include "Party.h"
#include "Network.h"

const double RANGE = 5.0;
const double SPEED = 0.3;
const double GET_RANGE = 0.5;

Experience::Experience( const Vector& pos ) {
	_pos = pos;
	_expired = true;
}

Experience::~Experience( ) {
}


void Experience::update( PartyPtr party ) {
	for ( int i = 0; i < PLAYER_NUM; i++ ) {
		PlayerPtr player = party->getPlayer( i );
		if ( !player ) {
			continue;
		}
		Vector vec = player->getPos( ) + Vector( 0, 1.0, 0 ) - _pos;
		Vector v = vec;
		v.y = 0;
		double len = v.getLength2( ); 
		if ( len < RANGE * RANGE ) {
			_pos += vec.normalize( ) * SPEED;
			vec = player->getPos( ) + Vector( 0, 1.0, 0 ) - _pos;
			len = vec.getLength2( );
			if ( len < GET_RANGE * GET_RANGE ) {
				delMyself( );
				ClientPtr client = Client::getTask( );
				SERVERDATA data;
				data.command = COMMAND_EXP;
				data.index = i;
				client->send( data );
			}
			break;
		}
	}
}

Vector Experience::getPos( ) const {
	return _pos;
}

bool Experience::isExpired( ) {
	return _expired;
}

void Experience::delMyself( ) {
	_expired = false;
}