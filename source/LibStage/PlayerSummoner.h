#pragma once
#include "Player.h"
#include "mathmatics.h"

PTR( PlayerSummoner );

class PlayerSummoner : public Player{
public:
	PlayerSummoner( Vector pos );
	virtual ~PlayerSummoner( );
private:
	void attack( );
	double getAttackTimeBegin( );
	double getAttackTimeEnd( );
	double getAttackTotalTime( );
};

