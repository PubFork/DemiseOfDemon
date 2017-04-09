#pragma once
#include "Enemy.h"

PTR( EnemyMinotaur );

class EnemyMinotaur : public Enemy {
public:
	EnemyMinotaur( const Vector& pos );
	virtual ~EnemyMinotaur();
public:
	virtual void act( );
	void setStone( bool stoneFlag );
private:
	virtual double getTime( MOTION motion ) const;
	virtual void onAttack( );
	virtual void onExp( );
private:
	const int _power;
	bool _is_stone;
};

