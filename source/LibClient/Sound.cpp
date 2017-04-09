#include "Sound.h"
#include "Framework.h"
#include "DxLib.h"

Sound::Sound( ) {
	// player
	_id[ SOUND_PLAYER_APPEARANCE ]	= LoadSoundMem( "resource/se/Player/appearance.wav" );
	_id[ SOUND_PLAYER_ARROW ]		= LoadSoundMem( "resource/se/Player/arrow.wav" );
	_id[ SOUND_PLAYER_COUNT ]		= LoadSoundMem( "resource/se/Player/count.mp3" );
	_id[ SOUND_PLAYER_COUNT_ANOUNCE ]	= LoadSoundMem( "resource/se/Player/count_anounce.mp3" );
	_id[ SOUND_PLAYER_COUNT_ZERO ]	= LoadSoundMem( "resource/se/Player/count_zero.wav" );
	_id[ SOUND_PLAYER_DAMAGE ]		= LoadSoundMem( "resource/se/Player/damage.wav" );
	_id[ SOUND_PLAYER_DEAD ]		= LoadSoundMem( "resource/se/Player/dead.wav" );
	_id[ SOUND_PLAYER_HEAL ]		= LoadSoundMem( "resource/se/Player/heal.wav" );
	_id[ SOUND_PLAYER_JUMP ]		= LoadSoundMem( "resource/se/Player/jump.wav" );
	_id[ SOUND_PLAYER_LANDING ]		= LoadSoundMem( "resource/se/Player/landing.wav" );
	_id[ SOUND_PLAYER_LAY ]			= LoadSoundMem( "resource/se/Player/lay.wav" );
	_id[ SOUND_PLAYER_NOVA ]		= LoadSoundMem( "resource/se/Player/nova.wav" );
	_id[ SOUND_PLAYER_SHOT ]		= LoadSoundMem( "resource/se/Player/shot.wav" );
	_id[ SOUND_PLAYER_SWORD ]		= LoadSoundMem( "resource/se/Player/sword.wav" );
	
	// UI
	_id[ SOUND_AREA_CLEAR ]			= LoadSoundMem( "resource/se/UI/area_clear.wav" );
	_id[ SOUND_WEAPON_CHANGE ]		= LoadSoundMem( "resource/se/UI/weapon_change.wav" );
	_id[ SOUND_WEAPON_LEVEL_UP ]	= LoadSoundMem( "resource/se/UI/weapon_level_up.wav" );

	// eles
	_id[ SOUND_DOOR_OPEN ]			= LoadSoundMem( "resource/se/else/door_open.wav" );
	_id[ SOUND_DEEDBOX_OPEN ]		= LoadSoundMem( "resource/se/else/deedbox_open.wav" );
	_id[ SOUND_GAME_OVER ]			= LoadSoundMem( "resource/se/else/game_over.mp3" );
	_id[ SOUND_GAME_CLEAR ]			= LoadSoundMem( "resource/se/else/game_clear.mp3" );
	_id[ SOUND_GAME_START ]			= LoadSoundMem( "resource/se/else/game_start.mp3" );
	_id[ SOUND_AREA_WARP ]			= LoadSoundMem( "resource/se/else/area_warp.wav" );
	_id[ SOUND_DEEDBOX_GET_ITEMS ]	= LoadSoundMem( "resource/se/else/deedbox_get_item.wav" );

	//boss
	_id[ SOUND_BOSS_CLEAVE ]		= LoadSoundMem( "resource/se/Boss/cleave.wav" );
	_id[ SOUND_BOSS_APPEAR_RUMBLING ]	= LoadSoundMem( "resource/se/Boss/entry_rumbling.wav" );
	_id[ SOUND_BOSS_APPEAR_ROAR ]		= LoadSoundMem( "resource/se/Boss/entry_roar.wav" );
	_id[ SOUND_BOSS_LOWHIT ]		= LoadSoundMem( "resource/se/Boss/slash.wav" );
	_id[ SOUND_BOSS_CHANT ]		= LoadSoundMem( "resource/se/Boss/chant.wav" );
	_id[ SOUND_BOSS_MAGIC ]		= LoadSoundMem( "resource/se/Boss/magic.wav" );

	for ( int i = 0; i < MAX_SOUND; i++ ) {
		SOUND tag = ( SOUND )i;
		_play[ tag ] = false;
	}
}

Sound::~Sound( ) {
}

void Sound::update( ) {
	for ( int i = 0; i < MAX_SOUND; i++ ) {
		SOUND tag = ( SOUND )i;
		if ( _play[ tag ] ) {
			if ( CheckSoundMem( _id[ tag ] ) == 1 ) {
				StopSoundMem( _id[ tag ] );
			}
			PlaySoundMem( _id[ tag ], DX_PLAYTYPE_BACK );
		}
		_play[ tag ] = false;
	}
}

void Sound::play( SOUND tag ) {
	if ( tag < 0 || tag >= MAX_SOUND ) {
		return;
	}
	_play[ tag ] = true;
}
