#pragma once
#include "Character.h"

PTR( Enemy );
PTR( Player );

class Enemy : public Character {
public:
	enum MOTION{
		MOTION_WAIT,
		MOTION_WALK,
		MOTION_FALL,
		MOTION_DAMAGE,
		MOTION_DEAD,
		MOTION_JUMP,
		MOTION_ATTACK,
		MAX_MOTION
	};
	enum TYPE {
		GOBLIN,
		REDGOBLIN,
		GHOST,
		ARMOR,
		CYCLOPS,
		MINOTAUR,
		BOSS,
		MAX_TYPE
	};
public:
	Enemy( TYPE type, double speed, double range, double life, const Vector& pos );
	virtual ~Enemy();
public:
	virtual double getHeight( ) const;
	virtual void act( ) = 0;
	MOTION getMotion( ) const ;
	double getTime( ) const ;
	TYPE getType( ) const;
	bool isExpired( ) const;
	virtual void damage( double power );
	virtual bool knockback( const Vector& dir );
protected:
	bool isPassedTime( double time );
	void searchTarget( );
	void moveToTarget( bool jump = true );
	void attackToTarget( );
	PlayerPtr getTarget( );
private:
	virtual double getTime( MOTION motion ) const = 0;
	virtual void updateDerive( );
	virtual void onJump( );
	virtual void onAttack( );
	virtual void onExp( ) { }
	virtual double getJumpPower( );
protected:
	double _time;
	MOTION _motion;
private:
	const TYPE _type;
	const double _speed;
	const double _range;
	bool _expired;
	double _time_store;
	double _life;
	PlayerWeakPtr _target;
	int _research_count;
	static double _endurance;
};

