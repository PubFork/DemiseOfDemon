#pragma once

#include "Object.h"
#include "mathmatics.h"

PTR( Character );

class Character : public Object {
public:
	enum ACTION {
		ACTION_WAIT,
		ACTION_WALK,
		ACTION_FLOAT,
		ACTION_ATTACK,
		ACTION_DAMAGE,
		ACTION_USE,
		ACTION_DEAD,
		ACTION_FALL,
		MAX_ACTION
	};
public:
	Character( const Vector& pos, bool ban = false );
	virtual ~Character( );
public:
	Vector getPos( ) const;
	double getDirX( ) const;
	double getDirZ( ) const;
public:
	virtual void update( );
	virtual bool knockback( const Vector& dir );
	Vector getStandPos( ) const;
	void setAction( ACTION action );
protected:
	bool isFirstAttack( ) const;
	ACTION getAction( ) const;
	void move( double vec_x, double vec_z );
	void setJump( );
	void setAttack( );
	void setUse( );
	void adjustDir( );
	void setDir( double dir_x, double dir_z );
	void setPos( const Vector& pos );
	bool isAttacking( ) const { return _attack; }
	void finishAttack( ) { _finish_attack = true; }
private:
	virtual void onJump( ) { }
	virtual void onAttack( ) { }
	virtual void updateDerive( ) { }
	virtual double getJumpPower( ) = 0;
private:
	void updateTrancelating( );
	void updateFalling( );
	void updateAction( );
protected:
	bool _standing;
	bool _before_standing;
private:
	const bool _ban_on_glass;
	Vector _pos;
	double _vec_x;
	double _vec_y;
	double _vec_z;
	double _dir_x;
	double _dir_z;
	bool _jump;
	bool _attack;
	bool _use;
	bool _fall;
	ACTION _action;
	Vector _standing_pos;
	bool _first_attack;
	bool _finish_attack;
};

