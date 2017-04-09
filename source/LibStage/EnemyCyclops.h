#pragma once
#include "Enemy.h"

PTR( EnemyCyclops );

class EnemyCyclops : public Enemy {
public:
	EnemyCyclops( const Vector& pos );
	virtual ~EnemyCyclops( );
public:
	void act( );
private:
	virtual double getTime( MOTION motion ) const;
	virtual void onAttack( );
	virtual void onExp( );
	virtual double getHeight( ) const;
private:
	const int _power;
};

