#pragma once

#include "smart_ptr.h"
#include <array>

PTR( Sound );

class Sound {
public:
	enum SOUND {
		SOUND_PLAYER_APPEARANCE,
		SOUND_PLAYER_ARROW,
		SOUND_PLAYER_COUNT,
		SOUND_PLAYER_COUNT_ANOUNCE,
		SOUND_PLAYER_COUNT_ZERO,
		SOUND_PLAYER_DAMAGE,
		SOUND_PLAYER_DEAD,
		SOUND_PLAYER_HEAL,
		SOUND_PLAYER_JUMP,
		SOUND_PLAYER_LANDING,
		SOUND_PLAYER_LAY,
		SOUND_PLAYER_NOVA,
		SOUND_PLAYER_SHOT,
		SOUND_PLAYER_SWORD,
		SOUND_AREA_CLEAR,
		SOUND_WEAPON_CHANGE,
		SOUND_WEAPON_LEVEL_UP,
		SOUND_DOOR_OPEN,
		SOUND_DEEDBOX_OPEN,
		SOUND_GAME_OVER,
		SOUND_GAME_CLEAR,
		SOUND_GAME_START,
		SOUND_AREA_WARP,
		SOUND_DEEDBOX_GET_ITEMS,
		SOUND_BOSS_CLEAVE,
		SOUND_BOSS_APPEAR_RUMBLING,
		SOUND_BOSS_APPEAR_ROAR,
		SOUND_BOSS_LOWHIT,
		SOUND_BOSS_CHANT,
		SOUND_BOSS_MAGIC,
		MAX_SOUND,
	};
public:
	Sound( );
	virtual ~Sound( );
public:
	void update( );
	void play( SOUND tag );
protected:
	std::array< bool, MAX_SOUND > _play;
	int _id[ MAX_SOUND ];
};

