#pragma once

#include "Enemy.h"

PTR( EnemyGoblin );

class EnemyGoblin : public Enemy {
public:
	EnemyGoblin( const Vector& pos );
	virtual ~EnemyGoblin();
public:
	virtual void act( );
private:
	virtual double getTime( MOTION motion ) const;
	virtual void onAttack( );
	virtual void onExp( );
private:
	const int _power;
};

