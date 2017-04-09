#pragma once

#include "Character.h"

PTR( Player );

class Player : public Character {
public:
	enum MOTION {
		MOTION_WAIT,
		MOTION_WALK,
		MOTION_ATTACK,
		MOTION_USE,
		MOTION_DAMAGE,
		MOTION_DEAD,
		MOTION_JUMP,
		MAX_MOTION
	};
public:
	static void initialize( );
	Player( int index, const Vector& pos );
	virtual ~Player( );
	void input( int dir_x, int dir_y, bool jump, bool attack );
	MOTION getMotion( ) const;
	double getTime( ) const;
	int getIndex( ) const;
	void use( );
protected:
	bool isPassedTime( double time );
private:
	virtual void updateDerive( );
	virtual void onAttack( );
	virtual void onJump( );
	virtual void attack( );
	virtual double getJumpPower( );
	bool pickupItem( );
	void replace( );
	virtual double getAttackTimeBegin( );
	virtual double getAttackTimeEnd( );
	virtual double getAttackTotalTime( );
protected: // private‚É‚·‚×‚« by okuty
	double _time;
	unsigned char _weapon_level;
private:
	const int _index;
	MOTION _motion;
	double _time_store;
};

