#pragma once

#include "smart_ptr.h"
#include "mathmatics.h"
#include <string>

PTR( Map );
PTR( Character );

const int MAP_SIZE = 100;
const int ATTRIBUTE_STONE    = 1;
const int ATTRIBUTE_GRASS    = 2;
const int EVENT_ROAD_H = 10;
const int EVENT_ROAD_W =  6;
const int EVENT_ROAD_V = 10;
const unsigned char OBJECT_NONE				= 0x00;
const unsigned char OBJECT_DOOR_PX			= 0x10;
const unsigned char OBJECT_DOOR_MX			= 0x11;
const unsigned char OBJECT_DOOR_PZ			= 0x12;
const unsigned char OBJECT_DOOR_MZ			= 0x13;
const unsigned char OBJECT_ENEMY_GOBLIN		= 0x20;
const unsigned char OBJECT_ENEMY_REDGOBLIN  = 0x21;
const unsigned char OBJECT_ENEMY_GHOST		= 0x22;
const unsigned char OBJECT_ENEMY_ARMOR		= 0x23;
const unsigned char OBJECT_ENEMY_CYCLOPS	= 0x30;
const unsigned char OBJECT_DEEDBOX			= 0x40;
const unsigned char OBJECT_ITEM_GEM			= 0x50;

class Map {
public:
	Map( std::string filename );
	Map( );
	virtual ~Map( );
public:
	int    getHeight( int x, int z ) const;
	int    getWaterHeight( int x, int z ) const;
	Vector getCameraPos( ) const;
	Vector getCameraTarget( ) const;
	int    getAttribute( int x, int z ) const;
	unsigned char getObject( int x, int z ) const;
	bool   isGround( int x, int z , int height ) const;
public:
	void   setCamera( const Vector& pos, const Vector& target );
	void   setHeight( int x, int z, int height );
	void   setWaterHeight( int x, int z, int height );
	void   setAttribute( int x, int z, int attribute );
	void   setObject( int x, int z, unsigned char object );
	void   save( const char * filename );
	void   load( std::string filename );
private:
#	pragma pack( 1 )
		struct DATA {
			struct Sector {
				unsigned char height;
				unsigned char water_height;//êÖ
				unsigned char attribute;
				unsigned char object;
				unsigned char reserved;
			} sector[ MAP_SIZE * MAP_SIZE ];
			Vector camera_pos;
			Vector camera_target;
		};
#	pragma pack( )
private:
	DATA _data;
};
