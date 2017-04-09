#pragma once

#include "Player.h"
#include "mathmatics.h"
#include <list>
PTR( PlayerMonk );
PTR( Enemy );
PTR( Model );

class PlayerMonk : public Player {
public:
	PlayerMonk( Vector pos );
	virtual ~PlayerMonk( );
private:
	void attack( );
	double getAttackTimeBegin( );
	double getAttackTimeEnd( );
	double getAttackTotalTime( );
	std::list< EnemyPtr > _targets;
	Vector knockback_dir;
private:
	ModelPtr _model;
};

