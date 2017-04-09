#pragma once

#include "Enemy.h"

PTR( EnemyRedGoblin );

class EnemyRedGoblin : public Enemy {
public:
	EnemyRedGoblin( const Vector& pos );
	virtual ~EnemyRedGoblin();
public:
	virtual void act( );
private:
	virtual double getTime( MOTION motion ) const;
	virtual void onAttack( );
	virtual void onExp( );
private:
	const int _power;
};

