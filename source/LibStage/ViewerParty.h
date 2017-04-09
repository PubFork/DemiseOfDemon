#pragma once

#include "smart_ptr.h"
#include "Network.h"
#include "Player.h"
#include <array>

PTR( ViewerParty );

class ViewerParty {
public:
	ViewerParty( );
	virtual ~ViewerParty( );
public:
	void drawBody( ) const;
	void drawShadow( ) const;
private:
	struct PARTY {
		struct PLAYER {
			struct ID {
				int body;
				int body_anim;
				int shadow;
				int shadow_anim;
			};
			std::array< ID, Player::MAX_MOTION > id;
		};
		std::array< PLAYER, PLAYER_NUM > player;
	};
private:
	void init( );
	void createModel( );
	void create( int index, Player::MOTION motion, const char * filename );
	void drawPlayerBody( int index, PlayerConstPtr player ) const;
	void drawPlayerShadow( int index, PlayerConstPtr player ) const;
	void drawModel( int id, int anim, PlayerConstPtr player ) const;
private:
	PARTY _party;
};

