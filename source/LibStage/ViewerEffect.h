#pragma once

#include "smart_ptr.h"

PTR( ViewerEffect );

class ViewerEffect {
public:
	ViewerEffect( );
	virtual ~ViewerEffect( );
public:
	void draw( );
private:
	void initEffects( );
	void addEffects( );
	void updateEffects( );
	void load( int index );
private:
	bool _init;
	bool _loadfile[ 72 ];	// ロードのフラグ
};

