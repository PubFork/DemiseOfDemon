#include "Dxlib.h"
#include "Framework.h"
#include "Task.h"
#include <typeinfo>
#include "Exception.h"

// 唯一のグローバル変数（シングルトン）
FrameworkPtr Framework::_instance;

void Framework::initialize( ) {
	if ( !_instance ) {
		_instance = FrameworkPtr( new Framework );
	}
}

void Framework::finalize( ) {
	if ( _instance ) {
		_instance.reset( );
	}
}

FrameworkPtr Framework::getInstance( ) {
	return _instance;
}

Framework::Framework( ) {
	int sx = GetSystemMetrics( SM_CXSCREEN );
	int sy = GetSystemMetrics( SM_CYSCREEN );

#	if FULLSCREEN
		SetGraphMode( sx, sy, 32 );
		ChangeWindowMode( FALSE );
		_screen_width  = sx;
		_screen_height = sy;
#	else
		int wx = 1280; //sx * 3 / 4;
		int wy = 1024; //sy * 3 / 4;	
		SetGraphMode( wx, wy, 32 );
		SetWindowInitPosition( ( sx - wx ) / 2, ( sy - wy ) / 2 );
		ChangeWindowMode( TRUE );
		_screen_width  = wx;
		_screen_height = wy;
#	endif
	
	SetWindowText( "Attraction WeAreTCA 2016 ver" );
	SetOutApplicationLogValidFlag( FALSE );
	SetDoubleStartValidFlag( TRUE );

	SetUseDirect3D9Ex( FALSE ) ;
	
	//SetUseDirect3DVersion( DX_DIRECT3D_9EX );

	if ( DxLib_Init( ) == -1 ) {
		return;
	}

//	SetUseSetDrawScreenSettingReset(FALSE);
	SetUseLighting( FALSE );
	SetLightEnable( FALSE );
	SetUseZBuffer3D( TRUE );
	SetWriteZBuffer3D( TRUE );
	SetDrawScreen( DX_SCREEN_BACK );
	SetupCamera_Perspective( 8.0f * DX_PI_F / 180.0f );
	SetCameraNearFar( 10.0f, 200.0f );
	SetAlwaysRunFlag( TRUE ) ;
	SetUseBackCulling( TRUE ) ;
    SetTextureAddressModeUV( DX_TEXADDRESS_WRAP, DX_TEXADDRESS_WRAP );
	
	loadSetting( );
}

Framework::~Framework( ) {
	DxLib_End( );
}

void Framework::loadSetting( ) {
	int fh = FileRead_open( "setting.ini", FALSE );
	_settingCounter = 0;
	if ( fh == 0 ) {
		return;
	}
	char buf[ 1024 ];
	char str[ 256 ];
	double value;
	while ( FileRead_eof( fh ) == 0 ) {
		FileRead_gets( buf, 1024, fh );
		sscanf_s( buf, "%s %lf", str, 256, &value );
		_setting[ str ] = value;
		_tag.push_back( str );
		_settingCounter++;
	}

	FileRead_close( fh );
}

double Framework::getSettingValue( std::string key ) {
    if ( _setting.find( key ) == _setting.end( ) ) {
		exception( "setting[ %s ]が定義されていません", key.c_str( ) );
	}
	return _setting[ key ];
}

int Framework::getSettingNum( ) {
	return _settingCounter; 
}

std::string Framework::getSettingTag( int num ) {
	return _tag[ num ];
}

int Framework::getScreenWidth( ) const {
	return _screen_width;
}

int Framework::getScreenHeight( ) const {
	return _screen_height;
}

void Framework::run( ) {

	while ( true ) {
		if ( ProcessMessage( ) == -1 ) {
			break;
		}
		if ( CheckHitKey( KEY_INPUT_ESCAPE ) ) {
			break;
		}

		// タスクを回す
		std::map< std::string, TaskPtr >::iterator it = _task_map.begin( );
		while ( it != _task_map.end( ) ) {
			TaskPtr task = it->second;
			task->update( );
			it++;
		}
	}
}

void Framework::addTask( std::string tag, TaskPtr task ) {
	if ( task ) {
		_task_map[ tag ] = task;
	}
}

TaskPtr Framework::getTask( std::string type ) {
    if ( _task_map.find( type ) == _task_map.end( ) ) {
        return TaskPtr();
    }
	return _task_map[ type ];
}

void Framework::load( const char filename[ 256 ], void * data, int size ) {
    int fh = FileRead_open( filename );
	if ( fh == 0 ) {
		return;
	}
	unsigned int version;
	FileRead_read( &version, sizeof( unsigned int ), fh );
	/*if ( version == MAP_VERSION ) {
		FileRead_read( data, size, fh );
	}*/
    FileRead_close( fh ) ;
}

Vector Framework::getCursor( int mouse_x, int mouse_y ) {
	VECTOR sp = ConvScreenPosToWorldPos( VGet( ( float )mouse_x, ( float )mouse_y, 0.0f ) ) ;
    VECTOR ep = ConvScreenPosToWorldPos( VGet( ( float )mouse_x, ( float )mouse_y, 1.0f ) ) ;
	Vector s( sp.x, sp.y, sp.z );
	Vector e( ep.x, ep.y, ep.z );
	double ratio = abs(s.y) / abs(( e - s ).y);
	return ( e - s ) * ratio + s;
}

void Framework::changeServerWindow( ) {
	int sx = GetSystemMetrics( SM_CXSCREEN );
	int sy = GetSystemMetrics( SM_CYSCREEN );
	_screen_width = 1024;
	_screen_height= 768;
	SetGraphMode( _screen_width, _screen_height, 32 );
	SetWindowInitPosition( ( sx - _screen_width ) / 2, ( sy - _screen_height ) / 2 );
	SetDrawScreen( DX_SCREEN_BACK );
}

void Framework::drawString( int x, int y, const char * format, ... ) const {
	va_list args;
	va_start( args, format );
	char str[ 255 ];
	vsprintf_s( str, 255, format, args );
	DrawFormatString( x, y, GetColor( 255, 255, 255 ), str );
}


void Framework::polygon( Vector v1, Vector v2, Vector v3, int r, int g, int b ) {
	COLOR_U8 color = GetColorU8( r, g, b, 255 );
	VERTEX3D vertex[ 3 ];
    vertex[ 0 ].pos  = VGet( ( float )v1.x, ( float )v1.y, ( float )v1.z ) ;
    vertex[ 0 ].norm = VGet(   0.0f,   0.0f, -1.0f ) ;
    vertex[ 0 ].dif  = color ;
    vertex[ 0 ].spc  = GetColorU8(   0,  0,  0,  0 ) ;
    vertex[ 0 ].u    = 0.0f ;
    vertex[ 0 ].v    = 0.0f ;
    vertex[ 0 ].su   = 0.0f ;
    vertex[ 0 ].sv   = 0.0f ;
    vertex[ 1 ].pos  = VGet( ( float )v2.x, ( float )v2.y, ( float )v2.z ) ;
    vertex[ 1 ].norm = VGet(   0.0f,   0.0f, -1.0f ) ;
    vertex[ 1 ].dif  = color ;
    vertex[ 1 ].spc  = GetColorU8(   0,  0,  0,  0 ) ;
    vertex[ 1 ].u    = 0.0f ;
    vertex[ 1 ].v    = 0.0f ;
    vertex[ 1 ].su   = 0.0f ;
    vertex[ 1 ].sv   = 0.0f ;
    vertex[ 2 ].pos  = VGet( ( float )v3.x, ( float )v3.y, ( float )v3.z ) ;
    vertex[ 2 ].norm = VGet(   0.0f,   0.0f, -1.0f ) ;
    vertex[ 2 ].dif  = color ;
    vertex[ 2 ].spc  = GetColorU8(   0,  0,  0,  0 ) ;
    vertex[ 2 ].u    = 0.0f ;
    vertex[ 2 ].v    = 0.0f ;
    vertex[ 2 ].su   = 0.0f ;
    vertex[ 2 ].sv   = 0.0f ;
	DrawPolygon3D( vertex, 1, DX_NONE_GRAPH , FALSE );
}

void Framework::line( Vector p1, Vector p2, int r, int g, int b ) {
	DrawLine3D( 
		VGet( ( float )p1.x, ( float )p1.y, ( float )p1.z ),
		VGet( ( float )p2.x, ( float )p2.y, ( float )p2.z ),
		GetColor( r, g, b ) );
}

bool Framework::isOutterOfScreen( const Vector& pos ) {
	VECTOR screen_pos = ConvWorldPosToScreenPos( VGet( ( float )pos.x, ( float )pos.y, ( float )pos.z ) );
	if ( screen_pos.z <= 0 || screen_pos.z >= 1.0 ) {
		return true;
	}
	const double BUFF = 50.0;
	if ( screen_pos.x < -BUFF || screen_pos.x > getScreenWidth( ) + BUFF ) {
		return true;
	}
	if ( screen_pos.y < -BUFF || screen_pos.y > getScreenHeight( ) + BUFF ) {
		return true;
	}
	return false;
}

void Framework::setCamera( const Vector& eye, const Vector& target ) {
	SetCameraPositionAndTarget_UpVecY(
		VGet( ( float )eye.x, ( float )eye.y, ( float )eye.z ),
		VGet( ( float )target.x, ( float )target.y, ( float )target.z ) );
}

std::string BTOS( unsigned char b ) {
	std::string str;
	for ( int i = 0; i < 8; i++ ) {
		char s[ 2 ] = { ( ( b & ( 1 << i ) ) != 0 ) + '0', '\0' };
		std::string buf = s;
		str = buf + str;
	}
	return str;
}
