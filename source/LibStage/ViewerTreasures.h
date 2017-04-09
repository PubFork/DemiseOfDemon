#pragma once

#include "Treasure.h"
#include <array>

PTR( ViewerTreasures );
PTR( Treasure );

class ViewerTreasures {
public:
	ViewerTreasures( );
	virtual ~ViewerTreasures( );
public: 
	void draw( );
private:
	void drawTreasure( TreasureConstPtr treasure );
	void create( Treasure::TYPE item_id, const char * filename );
private:
	std::array< int, Treasure::MAX_TYPE > _id;
	int _timer;
};

