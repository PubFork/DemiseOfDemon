#include "DxLib.h"
#include "Framework.h"
#include "Exception.h"

void main( );
void initialize( );
void finalize( );
LRESULT CALLBACK WndProc( HWND WindowHandle, UINT Message, WPARAM wParam, LPARAM lParam );

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ) {
	
	Framework::initialize( );

	try {

		main( );
		Framework::getInstance( )->run( );

	} catch ( Exception e ) {
		::MessageBox( GetMainWindowHandle( ), e.getMessageCPtr( ), NULL, MB_YESNO | MB_TASKMODAL );
	}

	Framework::finalize( );

	return 0;
}
