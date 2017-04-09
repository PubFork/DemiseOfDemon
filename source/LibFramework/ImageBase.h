#pragma once

#include <list>
#include "smart_ptr.h"

PTR( ImageBase );

class ImageBase {
public:
	ImageBase( );
	virtual ~ImageBase( );
public:
	int load( const char *file_name );
private:
	std::list< int > _id_list;
};

