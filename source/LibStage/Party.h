#pragma once

#include "smart_ptr.h"
#include "Network.h"

PTR( Party );
PTR( Player );

class Party {
public:
	Party( );
	virtual ~Party( );
public:
	virtual void update( );
public:
	PlayerConstPtr getPlayer( int index ) const;
public:
	PlayerPtr getPlayer( int index );
	void createPlayer( int index );
	void destroyPlayer( int index );
private:
	PlayerPtr _player[ PLAYER_NUM ];
};

