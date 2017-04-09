#pragma once

#include "smart_ptr.h"
#include "mathmatics.h"
#include "Map.h"
#include <array>

PTR( Field );
PTR( Map );
PTR( Object );

class Field {
public:
	Field( MapPtr map );
	virtual~Field( );
public:
	bool isEntering( const Vector& pos, bool ban_on_glass = false ) const;
	int getHeight( int x, int z ) const;
	Vector getValidPos( int x, int z, bool ban_on_glass = false, int height = -1 ) const;
	unsigned char getObject( int x, int z ) const;
	int getAttribute( int x, int z ) const;
public:
	void init( );
	ObjectPtr getTarget( int x, int z );
	void translate( Vector& pos, double& vec_x, double& vec_z, bool bad_on_glass );
	bool knockback( Vector& pos, double vec_x, double vec_z );
	void update( );
	bool isThroughAttacking( const Vector& pos );
	void delTarget( int x, int z, ObjectPtr object );
	void setTarget( int x, int z, ObjectPtr object );
private:
	void translateHorizontal( Vector& pos, double& vec_x, bool ban_on_glass ) const;
	void translateVertical( Vector& pos, double& vec_z, bool ban_on_glass ) const;
	bool isExpired( int x, int z ) const;
	int getIndexTargetBoard( int x, int z ) const;
private:
	MapConstPtr _map;
	std::array< ObjectWeakPtr, MAP_SIZE * MAP_SIZE > _target_board;
};

