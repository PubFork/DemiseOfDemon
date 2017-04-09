#pragma once

#include "Enemy.h"

PTR( EnemyArmor );

class EnemyArmor : public Enemy {
public:
	EnemyArmor( const Vector& pos );
	virtual ~EnemyArmor( );
public:
	virtual void act( );
private:
	virtual double getTime( MOTION motion ) const;
	virtual void onAttack( );
	virtual void onExp( );
private:
	const int _power;
};

