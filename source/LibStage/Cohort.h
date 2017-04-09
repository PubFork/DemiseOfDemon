#pragma once

#include "smart_ptr.h"
#include <string>

PTR( Cohort );
PTR( Enemy );

class Cohort {
public:
	static const int MAX_NUM = 500;
public:
	Cohort( );
	virtual ~Cohort();
public:
	virtual void update( );
	bool isAliveCyclops( ) const;
	bool isAliveMinotaur( ) const;
	EnemyConstPtr getEnemy( int index ) const;
public:
	void init( );
	EnemyPtr getEnemy( int index );
	void add( EnemyPtr enemy );
private:
	EnemyPtr _enemy[ MAX_NUM ];
	bool _alive_cyclops;
	bool _alive_minotaur;
};

