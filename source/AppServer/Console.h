#pragma once
#include "smart_ptr.h"
#include "Network.h"
#include "Task.h"
#include <string>
#include <vector>
#include <array>


PTR( Console );

class Console : public Task {
public:
	static std::string getTag( ) { return "CONSOLE"; }
	static ConsolePtr getTask( );
public:
	Console();
	virtual ~Console( );
public://Log
	static const int MESSAGE_COUNT = 20;
public:
	virtual void update( );
public://Command
	std::string get( );
public://Log
	void send( const char * format, ... );
	std::string get( int index );
private:
	void drawStatus( );
	void drawStatusFrame( );
	void drawStatusPlayer( int num );
	void drawStatusPlayerDevice( int num );
	void drawStatusState( int num );
	void drawStatusLevelWeapon( int num );
	void drawStatusLevelLife( int num );
	void drawStatusExperience( int num );
	void drawStatusLife( int num );
	void drawConnect( );
	void drawStatusDevice( );
	void drawBgm( );
	void drawStage( );
	void drawStatusDir( int num );
	void drawStatusButton( int num );
	void drawLog( );
	void drawCommand( );
	void drawBox( int x1, int y1, int x2, int y2 ) const;
	void drawLine( int x1, int y1, int x2, int y2 ) const;
	void drawString( int x, int y, const char * format, ... ) const;
private://Command
	void execute( );
	void parser( );
	void updateCommand( );
private:
	std::string _command;
	std::vector< std::string > _word;
	CLIENTDATA _data;
private://Log
	std::array< std::string, MESSAGE_COUNT > _message;
};