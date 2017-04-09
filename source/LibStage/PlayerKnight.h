#pragma once
#include "Player.h"
#include "mathmatics.h"
#include <list>

PTR( PlayerKnight );
PTR( Enemy );

class PlayerKnight : public Player {
public:
	PlayerKnight( Vector pos );
	virtual ~PlayerKnight( );
private:
	void attack( );
	double getAttackTimeBegin( );
	double getAttackTimeEnd( );
	double getAttackTotalTime( );
private:
	std::list< EnemyPtr > _targets;
};

