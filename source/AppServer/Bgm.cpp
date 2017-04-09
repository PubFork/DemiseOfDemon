#include "Bgm.h"
#include "DxLib.h"

Bgm::Bgm( ) {
	_play = BGM_NONE;

	_id[ BGM_NONE	] = -1;
	_id[ BGM_WAIT ]			  = LoadSoundMem( "resource/bgm/bgm_wait.wav" );
	_id[ BGM_CAMP_NORMAL ]	  = LoadSoundMem( "resource/bgm/bgm_camp_normal.mp3" );
	_id[ BGM_CAMP_LAST ]	  = LoadSoundMem( "resource/bgm/bgm_camp_last.mp3" );
	_id[ BGM_LABYRINTH_1 ]	  = LoadSoundMem( "resource/bgm/bgm_labyrinth_1.mp3" );
	_id[ BGM_LABYRINTH_2 ]	  = LoadSoundMem( "resource/bgm/bgm_labyrinth_2.mp3" );
	_id[ BGM_LABYRINTH_3 ]	  = LoadSoundMem( "resource/bgm/bgm_labyrinth_3.mp3" );
	_id[ BGM_LABYRINTH_LAST ] = LoadSoundMem( "resource/bgm/bgm_labyrinth_last.mp3" );
	_id[ BGM_MINOTAUR_1 ]	  = LoadSoundMem( "resource/bgm/bgm_minotaur_1.mp3" );
	_id[ BGM_MINOTAUR_2 ]	  = LoadSoundMem( "resource/bgm/bgm_minotaur_2.mp3" );
	_id[ BGM_MINOTAUR_3 ]	  = LoadSoundMem( "resource/bgm/bgm_minotaur_3.mp3" );
	_id[ BGM_BOSS	]		  = LoadSoundMem( "resource/bgm/bgm_boss.mp3" );
	_isplaying = false;
}

Bgm::~Bgm( ) {
	stop( );
	InitSoundMem( );
}

void Bgm::update( ) {
	if ( _play == BGM_NONE ) {
		return;
	}

	stop( );

	PlaySoundMem( _id[ _play ], DX_PLAYTYPE_LOOP );
	_play = BGM_NONE;
	_isplaying = true;
}

void Bgm::play( BGM tag ) {
	_play = tag;
}

void Bgm::stop( ) {
	for ( int i = 0; i < MAX_BGM; i++ ) {
		if ( CheckSoundMem( _id[ i ] ) == 1 ) {
			StopSoundMem( _id[ i ] );
			_isplaying = false;
		}
	}
}

void Bgm::bgmPlayOn( ) {
	_isplaying = true;
}

bool Bgm::getBgmPlaying( ) {
	return _isplaying;
}