#pragma once

#include "smart_ptr.h"
#include "Task.h"
#include <string>

PTR( Mouse );

class Mouse : public Task {
public:
	static std::string getTag( ) { return "MOUSE"; }
	static MousePtr getTask( );
public:
	Mouse( );
	virtual ~Mouse( );
	void update( );
public:
	int getX( ) const;
	int getY( ) const;
	int getWheel( ) const;
	bool isRight( ) const;
	bool isLeft( ) const;
private:
	int _x;
	int _y;
	int _wheel;
	bool _right;
	bool _left;
};

