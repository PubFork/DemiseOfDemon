#include "ImageBase.h"
#include "DxLib.h"

ImageBase::ImageBase( ) {
}

ImageBase::~ImageBase( ) {
	std::list< int >::iterator it = _id_list.begin( );
	while ( it != _id_list.end( ) ) {
		int id = *it;
		DeleteGraph( id );
		it++;
	}
}

int ImageBase::load( const char *file_name ) {
	int id = LoadGraph( file_name );

	if ( id != -1 ) {
		_id_list.push_back( id );
	}

	return id;
}
