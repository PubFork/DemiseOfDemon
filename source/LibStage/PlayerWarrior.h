#pragma once
#include "Player.h"

PTR( PlayerWarrior );
class PlayerWarrior : public Player {
public:
	PlayerWarrior( Vector pos );
	virtual ~PlayerWarrior( );
private:
	void attack( );
	double getAttackTotalTime( );
	double getAttackTimeBegin( );
	double getAttackTimeEnd( );
};

