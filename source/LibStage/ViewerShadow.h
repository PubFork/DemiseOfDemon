#pragma once

#include "smart_ptr.h"
#include "Model.h"

PTR( ViewerShadow );

class ViewerShadow : public Model {
public:
	ViewerShadow( );
	virtual ~ViewerShadow( );
public:
	void draw( );
private:
	void makeForm( );
	void addPlayerShadow( );
	void addEnemyShadow( );
	void add( const Vector& pos );
private:
	static const int MAX_NUM = 30;
	int _num;
};

