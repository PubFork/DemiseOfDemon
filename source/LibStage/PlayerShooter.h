#pragma once
#include "Player.h"
#include "mathmatics.h"

PTR( PlayerShooter );

class PlayerShooter : public Player {
public:
	PlayerShooter( Vector pos );
	virtual ~PlayerShooter( );
private:
	void attack( );
	double getAttackTimeBegin( );
	double getAttackTimeEnd( );
	double getAttackTotalTime( );
};

