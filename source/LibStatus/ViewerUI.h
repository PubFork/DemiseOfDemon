#pragma once
#include "smart_ptr.h"
#include "Network.h"

PTR( ViewerUI );
PTR( ImageBase );

class ViewerUI {
public:
	ViewerUI( int index );
	virtual ~ViewerUI( );
public:
	void draw( );
private:

	static const int MAX_WEAPON_LEVEL = 5;

	enum NUMBER_PATTERN {
		A,
		B,
		MAX_NUMBER_PATTERN,
	};

	enum PLAYER {
		KNIGHT,
		WARRIOR,
		MAGE,
		MONK,
		SHOOTER,
		SUMMONER,
		MAX_PLAYER,
	};

	enum STAGE {
		STAGE_WINDOW,
		STAGE_ICON,
		MAX_STAGE,
	};

	enum WEAPON {
		WEAPON_WINDOW,
		WEAPON_ICON,
		MAX_WEAPON,
	};

	enum LIFE {
		LIFE_WINDOW,
		MAX_LIFE,
	};

	enum EXP {
		EXP_GAUGE,
		MAX_EXP,
	};

	enum CURRENT_LIFE {
		CURRENT_LIFE_BASE,
		CURRENT_LIFE_WINDOW,
		MAX_CURRENT_LIFE,
	};
	
	enum LEVEL {
		LEVEL_0,
		LEVEL_1,
		LEVEL_2,
		LEVEL_3,
		LEVEL_4,
		LEVEL_5,
		LEVEL_6,
		LEVEL_7,
		LEVEL_8,
		LEVEL_9,
		MAX_LEVEL,
	};

	enum NUMBER {
		NUMBER_0,
		NUMBER_1,
		NUMBER_2,
		NUMBER_3,
		NUMBER_4,
		NUMBER_5,
		NUMBER_6,
		NUMBER_7,
		NUMBER_8,
		NUMBER_9,
		MAX_NUMBER,
	};
private:
	void drawStateNone( );
	void drawStateReady( const CLIENTDATA& data );
	void drawStateClear( const CLIENTDATA& data );
	void drawStateDead( const CLIENTDATA& data );
	void drawStatePlay( const CLIENTDATA& data );
	void drawStatus( const CLIENTDATA& data );
	void drawWeapon( const CLIENTDATA& data );
	void drawLife( const CLIENTDATA& data );
	void drawExp( const CLIENTDATA& data );
	void drawChara( const CLIENTDATA& data );
	void drawStage( const CLIENTDATA& data );
	void drawCurrentLife( const CLIENTDATA& data );
	void drawMaxLife( const CLIENTDATA& data );
	void drawDebugString( const char * format, ... );
private:
	const int _index;
	ImageBasePtr _image;
	int _player;
	int _player_img;
	int _level[ MAX_LEVEL ];
	int _number[ MAX_NUMBER_PATTERN ][ MAX_NUMBER ];
	int _stage[ MAX_STAGE ];
	int _life[ MAX_LIFE ];
	int _weapon[ MAX_WEAPON ];
	int _exp[ MAX_EXP ];
	int _current_life[ MAX_CURRENT_LIFE ];
	int _window[ MAX_WEAPON_LEVEL + 1 ];
	int _base[ MAX_WEAPON_LEVEL + 1 ];
	int _max_exp[ LIFE_LEVEL_MAX ];
	int _now_exp;
	int _welcome;
	int _gauge_img;
	int _line;
	int _clear;
	int _dead;
	int _timer;
};