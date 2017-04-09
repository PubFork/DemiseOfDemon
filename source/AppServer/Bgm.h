#pragma once

#include <array>
#include "smart_ptr.h"

PTR( Bgm ); 

class Bgm {
public:
	enum BGM {
		BGM_NONE,
		BGM_WAIT,
		BGM_CAMP_NORMAL,
		BGM_CAMP_LAST,
		BGM_LABYRINTH_1,
		BGM_LABYRINTH_2,
		BGM_LABYRINTH_3,
		BGM_LABYRINTH_LAST,
		BGM_MINOTAUR_1,
		BGM_MINOTAUR_2,
		BGM_MINOTAUR_3,
		BGM_BOSS,
		MAX_BGM,
	};
public:
	Bgm( );
	virtual ~Bgm( );
public:
	void update( );
public:
	void play( BGM tag );
	void stop( );
	void bgmPlayOn( );
	bool getBgmPlaying( );
private:
	int _id[ MAX_BGM ];
	bool _isplaying;
	BGM _play;
};

