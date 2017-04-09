#include "DispStatusUI.h"
#include "ViewerUI.h"

DispStatusUI::DispStatusUI( int user_id )
: _user_id( user_id ){
	_ui = ViewerUIPtr( new ViewerUI );
}

DispStatusUI::~DispStatusUI( ) {

}

void DispStatusUI::update( ) {
	_ui->draw( _user_id );
}