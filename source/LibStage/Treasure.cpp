#include "Treasure.h"
#include "Field.h"
#include "Party.h"
#include "Client.h"
#include "Player.h"
#include "Stage.h"
#include "Sound.h"
#include "Framework.h"
#include "Effect.h"

const double POWER = 0.3;
const double GRAVITY = 0.05;

Treasure::Treasure( const Vector& pos, TYPE type )
: _type( type ) {
	_pos = pos;
	_vec_y = POWER;
	_standing = false;

	FrameworkPtr fw = Framework::getInstance( );
	_heal_life = ( int )fw->getSettingValue( "ITEM_HEAL_LIFE" );
	_heal_life_big = ( int )fw->getSettingValue( "ITEM_HEAL_LIFE_BIG" );
}


Treasure::~Treasure( ) {
}

bool Treasure::pickup( int index ) {
	delMyself( );
	
	if ( _type == LEVEL_UP ) {
		// Playerのモーション
		StagePtr stage = Stage::getTask( );
		PartyPtr party = stage->getParty( );
		PlayerPtr player = party->getPlayer( index );
		if ( player ) {
			player->use( );
		}
	}

	// アイテムの変化
	ClientPtr client = Client::getTask( );
	
	//client->useItem( index, _type );
	StagePtr stage = Stage::getTask( );
	PartyPtr party = stage->getParty( );
	PlayerPtr player = party->getPlayer( index );
	SoundPtr sound = stage->getSound( );
	EffectPtr effect = stage->getEffect( );
	if ( _type == Treasure::POTION || _type == Treasure::POTION_BIG ) {
		SERVERDATA netdata;
		netdata.command = COMMAND_HEAL;
		netdata.index = index;
		sound->play( Sound::SOUND_PLAYER_HEAL );
		effect->play( Effect::BMVOL1_3_RECOVERY, player->getPos( ), 0.015f );
		
		if ( _type == Treasure::POTION ) {
			netdata.value = _heal_life;
		}
		if ( _type == Treasure::POTION_BIG ){
			netdata.value = _heal_life_big;
		}
		client->send( netdata );
	} else {
		CLIENTDATA data = client->getClientData( );
		unsigned char& level_weapon = data.user[ index ].level_weapon;

		if ( _type == LEVEL_UP ) {
			sound->play( Sound::SOUND_WEAPON_LEVEL_UP );
			effect->play( Effect::BMVOL2_HIT09, player->getPos( ) + Vector( 0.0, 1.0, 0.0 ), 0.2f );
			level_weapon++;
		}
		
		if ( level_weapon <= 5 ) {
			SERVERDATA netdata;
			netdata.command = COMMAND_LEVEL;
			netdata.index = index;
			netdata.value = level_weapon;
			client->send( netdata );
		}
	} 

	return true;
}

Treasure::TYPE Treasure::getType( ) const {
	return _type;
}

void Treasure::update( ) {
	if ( _standing ) {
		return;
	}
	// 重力
	_vec_y -= GRAVITY;
	_pos.y += _vec_y;
	StagePtr stage = Stage::getTask( );
	FieldPtr field = stage->getField( );
	int height = field->getHeight( ( int )_pos.x, ( int )_pos.z );
	if ( height > _pos.y ) {
		_pos.y = height;
		_standing = true;
	}
}

Vector Treasure::getPos( ) const {
	return _pos;
}
