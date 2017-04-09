#pragma once

#include "Task.h"
#include "mathmatics.h"

PTR( Boss );
PTR( EnemyBoss );

class Boss : public Task {
public:
	static std::string getTag( ) { return "BOSS"; }
	static BossPtr getTask( );
public:
	enum MOTION {
		MOTION_HIDE,
		MOTION_WAIT,
		MOTION_APPEAR,
		MOTION_EXIT,
		MOTION_ATTACK_CLEAVE,
		MOTION_ATTACK_LOWHIT,
		MOTION_ATTACK_MAGIC,
		MAX_MOTION
	};
public:
	Boss( );
	virtual ~Boss( );
public:
	void update( );
	Vector getDir( );
	MOTION getMotion( );
	double getTime( );
private:
	bool isPassedTime( double time );
	void gameClear( );
private:
	MOTION _motion;
	double _time;
	double _time_store;
	Vector _dir;
	bool _init;
	EnemyBossPtr _enemy[ 10 ];
	int _magic_actuation_time;
};

