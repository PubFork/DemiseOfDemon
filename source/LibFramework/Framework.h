#pragma once

#include "smart_ptr.h"
#include "mathmatics.h"
#include <string>
#include <map>
#include <vector>

PTR( Framework );
PTR( Task );

class Framework {
public:
	static FrameworkPtr getInstance( );
	static void initialize( );
	static void finalize( );
public:
	virtual ~Framework( );
public:
	void run( );
	void addTask( std::string tag, TaskPtr task );
	TaskPtr getTask( std::string type );
	void load( const char filename[ 256 ], void * data, int size );
	bool inputFilename( char filename[ 256 ], bool save );
	Vector getCursor( int mouse_x, int mouse_y );
	void changeServerWindow( );
	void drawString( int x, int y, const char * format, ... ) const;
	void polygon( Vector v1, Vector v2, Vector v3, int r, int g, int b );
	void line( Vector p1, Vector p2, int r, int g, int b );
	int getScreenWidth( ) const;
	int getScreenHeight( ) const;
	bool isOutterOfScreen( const Vector& pos );
	void setCamera( const Vector& eye, const Vector& target );
	double getSettingValue( std::string key );
	int getSettingNum( );
	std::string getSettingTag( int num );
private:
	Framework( );
	void loadSetting( );
private:
	static FrameworkPtr _instance;
	std::map< std::string, TaskPtr > _task_map;
	int _screen_width;
	int _screen_height;
	std::map< std::string, double > _setting;
	std::vector< std::string > _tag;
	int _settingCounter;
};

std::string BTOS( unsigned char b );
