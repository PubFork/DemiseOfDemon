#pragma once

#include "Enemy.h"

PTR( EnemyGhost );

class EnemyGhost : public Enemy {
public:
	EnemyGhost( const Vector& pos );
	virtual ~EnemyGhost();
public:
	virtual void act( );
private:
	virtual double getTime( MOTION motion ) const;
	virtual void onAttack( );
	virtual void onExp( );
private:
	const int _max_cooltime;
	int _cooltime;
};

