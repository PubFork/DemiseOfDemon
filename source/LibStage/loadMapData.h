#pragma once
#include "smart_ptr.h"
#include <vector>
#include <string>
#include "Keyboard.h"

PTR( LoadMapData );

class LoadMapData {
public:
	LoadMapData( );
	virtual ~LoadMapData( );
public:
	bool selectMapData( );
	const char * getMapFileName( );
private:
	void input( );
	void draw( );
private:
	KeyboardPtr _keyboard;
	std::vector< std::string > _fileNameList;
	int _selectNum;
	bool _isSelecting;
	bool _isEscape;
	const char* _selectMapFileName;
};

