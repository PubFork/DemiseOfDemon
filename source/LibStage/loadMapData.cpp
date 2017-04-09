#include "LoadMapData.h"
#include "DxLib.h"
#include <string>
#include <vector>
#include "Keyboard.h"

const int DISPLAY_FILE_MAX = 10;

LoadMapData::LoadMapData( ) :
_selectMapFileName( "NULL" ) {
	_keyboard = Keyboard::getTask( );
	_selectNum = 0;
	_isSelecting = TRUE;
	_isEscape = FALSE;

	HANDLE hSearch;
	WIN32_FIND_DATA fd;
	hSearch = FindFirstFile( _T("resource/map/*.map*"), &fd );
	if ( hSearch == INVALID_HANDLE_VALUE ) {
		_fileNameList.push_back( "NONE" );
	} else {
		_fileNameList.push_back( fd.cFileName );
		while( TRUE ) {
			if ( !FindNextFile( hSearch, &fd ) ) {
				break;
			}
			_fileNameList.push_back( fd.cFileName );
		}
	}
	FindClose( hSearch );
}

LoadMapData::~LoadMapData( ) {
}


bool LoadMapData::selectMapData( ) {
	while ( _isSelecting && !_isEscape ) {
		input( );
		draw( );
		/*UPDATE*/
		_keyboard->update( );
		ScreenFlip( );
		ClearDrawScreen( );
	}
	_selectMapFileName = _fileNameList[ _selectNum ].c_str( );
	if( _isEscape ) {
		return false;
	}
	return true;
}

const char* LoadMapData::getMapFileName( ) {
	return _selectMapFileName;
}

void LoadMapData::input( ) {
	//input
	if ( _keyboard->isPushKey( "ENTER" ) ) {
		_isSelecting = false;
		return;
	}
	if ( _keyboard->isPushKey( "ESC" ) ) {
		_isEscape = true;
		return;
	}

	const int size = ( int )_fileNameList.size( );
	if ( _keyboard->isPushKey( "ARROW_UP" ) ) {
		_selectNum = ( _selectNum + size - 1 ) % size;
	}
	if ( _keyboard->isPushKey( "ARROW_DOWN" ) ) {
		_selectNum = ( _selectNum + 1 ) % size;
	}
	if ( _keyboard->isPushKey( "ARROW_LEFT" ) ) {
		_selectNum = ( _selectNum + size - DISPLAY_FILE_MAX ) % size;
	}
	if ( _keyboard->isPushKey( "ARROW_RIGHT" ) ) {
		_selectNum = ( _selectNum +  DISPLAY_FILE_MAX ) % size;
	}
}

void LoadMapData::draw( ) {
	//Draw
	DrawString( 0, 0, "ÉtÉ@ÉCÉãì«çû", 0xffff );


	int page_num = _selectNum / DISPLAY_FILE_MAX;
	int page_max = ( _fileNameList.size( ) - 1 ) / DISPLAY_FILE_MAX + 1;

	const int FILE_FIRST_NUMBER = page_num * DISPLAY_FILE_MAX;
	const int CHARACTER_LENGTH = 400;
	const int CHARACTER_HEIGHT = 20;
	for ( int i = 0; i < DISPLAY_FILE_MAX; i++ ) {
		const int STRING_POS_X = 250;
		const int STRING_POS_Y = 150 + ( i * ( CHARACTER_HEIGHT + 20 ) );
		int num = i + FILE_FIRST_NUMBER;
		if ( num >= ( int )_fileNameList.size( ) ) {
			break;
		}
		if ( _selectNum == num ) {
			DrawBox( STRING_POS_X - 20, STRING_POS_Y - 20, STRING_POS_X + CHARACTER_LENGTH, STRING_POS_Y + CHARACTER_HEIGHT , GetColor( 255, 255, 255 ), TRUE );
			DrawFormatString( STRING_POS_X , STRING_POS_Y, GetColor( 0, 0, 0 ), "%s", _fileNameList[ num ].c_str( ) );
		} else {
			DrawBox( STRING_POS_X - 20, STRING_POS_Y - 20, STRING_POS_X + CHARACTER_LENGTH, STRING_POS_Y + CHARACTER_HEIGHT , GetColor( 255, 255, 255 ), FALSE );
			DrawFormatString( STRING_POS_X , STRING_POS_Y, GetColor( 255, 255, 255 ), "%s",_fileNameList[ num ].c_str( ) );
		}
	}
	
	const int PAGE_POS_X = 400;
	const int PAGE_POS_Y = 600;
	DrawFormatString( PAGE_POS_X, PAGE_POS_Y, GetColor( 255, 255, 255 ), "%d / %d", page_num + 1, page_max );
}