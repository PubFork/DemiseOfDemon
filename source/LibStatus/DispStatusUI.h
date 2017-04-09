#pragma once
#include "smart_ptr.h"

PTR( DispStatusUI );
PTR( ViewerUI );

class DispStatusUI {
public:
	DispStatusUI( int user_id );
	virtual ~DispStatusUI( );
	void update( );
private:
	const int _user_id;
	ViewerUIPtr _ui;
};
