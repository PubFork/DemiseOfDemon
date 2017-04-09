#include "Map.h"
#include "DxLib.h"
#include "Exception.h"

static const int MAP_VERSION = 0;

Map::Map( std::string filename ) {
	Map( );
	load( filename );
}

Map::Map( ) {
	for ( int i = 0; i < MAP_SIZE * MAP_SIZE; i++ ) {
		_data.sector[ i ].height = 11;
		_data.sector[ i ].water_height = 0;//水
		_data.sector[ i ].attribute = ATTRIBUTE_STONE;
		_data.sector[ i ].object = OBJECT_NONE;
		_data.sector[ i ].reserved = 0;
	}
	_data.camera_target = Vector( MAP_SIZE / 2, 0, MAP_SIZE / 2 );
	_data.camera_pos = Vector( -MAP_SIZE / 2, 0, -MAP_SIZE / 2 );
	_data.camera_pos.y = ( _data.camera_target - _data.camera_pos ).getLength( ) * 0.3;
}

Map::~Map( ) {
}

int Map::getHeight( int x, int z ) const {
	if ( x < 0 || x >= MAP_SIZE ||
		 z < 0 || z >= MAP_SIZE ) return 0;
	return ( int )_data.sector[ x + z * MAP_SIZE ].height;
}

int Map::getWaterHeight( int x, int z ) const {
	if ( x < 0 || x >= MAP_SIZE ||
		 z < 0 || z >= MAP_SIZE ) return 0;
	return ( int )_data.sector[ x + z * MAP_SIZE ].water_height;
}

int Map::getAttribute( int x, int z ) const {
	if ( x < 0 || x >= MAP_SIZE ||
		 z < 0 || z >= MAP_SIZE ) return 0;

	return ( int )_data.sector[ x + z * MAP_SIZE ].attribute;
}

unsigned char Map::getObject( int x, int z ) const {
	if ( x < 0 || x >= MAP_SIZE ||
		 z < 0 || z >= MAP_SIZE ) return 0;

	return _data.sector[ x + z * MAP_SIZE ].object;
}

bool Map::isGround( int x, int z, int height ) const {
	return
		getHeight( x, z ) <= height &&
		getAttribute( x, z ) == ATTRIBUTE_STONE;
}

void Map::setHeight( int x, int z, int height ) {
	if ( x < 0 || x >= MAP_SIZE ||
		 z < 0 || z >= MAP_SIZE ) return;
	_data.sector[ x + z * MAP_SIZE ].height = ( unsigned char )height;
}
//水
void Map::setWaterHeight( int x, int z, int height ) {
	if ( x < 0 || x >= MAP_SIZE ||
		 z < 0 || z >= MAP_SIZE ) return;
	if ( _data.sector[ x + z * MAP_SIZE ].height >= height ) {
		height = 0;
	}
	_data.sector[ x + z * MAP_SIZE ].water_height = ( unsigned char )height;
}

void Map::setAttribute( int x, int z, int attribute ) {
	if ( x < 0 || x >= MAP_SIZE ||
		 z < 0 || z >= MAP_SIZE ) return;
	_data.sector[ x + z * MAP_SIZE ].attribute = ( unsigned char )attribute;
}

void Map::save( const char * filename ) {
	char filePass[ 256 ] = "resource/map/";
	strcat_s( filePass, 256, filename );
	if ( strstr( filePass, ".map" ) == NULL ) {
		strcat_s( filePass, 256, ".map" );
	}
	FILE *file;
	if ( fopen_s( &file, filePass, "wb" ) == 0 ) {
		fwrite( &MAP_VERSION, sizeof( unsigned int ), 1, file );
		fwrite( &_data, sizeof( DATA ), 1, file );
		fclose( file );
	}
}

void Map::load( std::string filename ) {
	std::string filepass = "resource/map/";
	filepass += filename;
	int fh = FileRead_open( filepass.c_str( ) );
	if ( fh == 0 ) {
		exception( "FILR:%sが読み込めません\n※作業ディレクトリ要確認", filepass.c_str( ) );
	}
	unsigned int version;
	FileRead_read( &version, sizeof( unsigned int ), fh );
	if ( version == MAP_VERSION ) {
		FileRead_read( ( void * )&_data, sizeof( DATA ), fh );
	}
    FileRead_close( fh ) ;
}

void Map::setCamera( const Vector& pos, const Vector& target ) {
	_data.camera_pos = pos;
	_data.camera_target = target;
}

Vector Map::getCameraPos( ) const {
	return _data.camera_pos;
}

Vector Map::getCameraTarget( ) const {
	return _data.camera_target;
}

void Map::setObject( int x, int z, unsigned char object ) {
	if ( x < 0 || x >= MAP_SIZE ||
		 z < 0 || z >= MAP_SIZE ) return;
	_data.sector[ x + z * MAP_SIZE ].object = object;
}