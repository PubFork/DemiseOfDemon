#pragma once
#include "Player.h"
#include "mathmatics.h"

PTR( PlayerMage );

class PlayerMage : public Player {
public:
	PlayerMage( Vector pos );
	virtual ~PlayerMage( );
private:
	void attack( );
	double getAttackTimeBegin( );
	double getAttackTimeEnd( );
	double getAttackTotalTime( );
private:
	int _bullet_lay_shot_num;
};

