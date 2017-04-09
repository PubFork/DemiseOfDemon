#include "Field.h"
#include "Map.h"
#include "Framework.h"
#include "Viewer.h"
#include "Object.h"

Field::Field( MapPtr map ) {
	_map = map;
}

Field::~Field( ) {
}

void Field::init( ) {
	for ( int i = 0; i < MAP_SIZE * MAP_SIZE; i++ ) {
		_target_board[ i ].reset( );
	}
}

void Field::update( ) {
}

unsigned char Field::getObject( int x, int z ) const {
	return _map->getObject( x, z );
}

int Field::getAttribute( int x, int z ) const {
	return _map->getAttribute( x, z );
}

void Field::translate( Vector& pos, double& vec_x, double& vec_z, bool ban_on_glass ) {
	translateVertical( pos, vec_z, ban_on_glass );
	translateHorizontal( pos, vec_x, ban_on_glass );
}

bool Field::knockback( Vector& pos, double vec_x, double vec_z ) {
	Vector add( vec_x, 0, vec_z );
	add = add.normalize( );
	
	Vector p = pos + add;
	int x = ( int )p.x;
	int z = ( int )p.z;
	int y = ( int )p.y;
	if ( p.y < 0 ) {
		y -= 1;
	}

	if ( isExpired( x, z ) ) {
		return false;
	}

	if ( _map->getAttribute( x, z ) == ATTRIBUTE_GRASS ) {
		return false;
	}

	int height = _map->getHeight( x, z );
	if ( p.y >= height ) {
		pos.x = x + 0.5;
		pos.z = z + 0.5;
	}

	return true;
}

void Field::translateHorizontal( Vector& pos, double& vec_x, bool ban_on_glass ) const {
	Vector p = pos;
	if ( vec_x < 0 ) {
		p.x += -0.51;
	} else {
		p.x += 0.51;
	}

	if ( !isEntering( p, ban_on_glass ) ) {
		pos.x = ( int )( pos.x ) + 0.5;
		vec_x = 0;
	} else {
		pos.x += vec_x;
	}
}

void Field::translateVertical( Vector& pos, double& vec_z, bool ban_on_glass ) const {
	Vector p = pos;
	if ( vec_z < 0 ) {
		p.z += -0.51;
	} else {
		p.z += 0.51;
	}

	if ( !isEntering( p, ban_on_glass ) ) {
		pos.z = ( int )( pos.z ) + 0.5;
		vec_z = 0;
	} else {
		pos.z += vec_z;
	}
}

bool Field::isEntering( const Vector& pos, bool ban_on_glass ) const {
	int x = ( int )pos.x;
	int z = ( int )pos.z;

	int height = _map->getHeight( x, z );
	if ( height < 10 ) {
		return false;
	}

	if ( isExpired( x, z ) ) {
		return false;
	}

	if ( ban_on_glass ) {
		if ( getAttribute( x, z ) == ATTRIBUTE_GRASS ) {
			return false;
		}
	}

	int y = ( int )pos.y;
	if ( pos.y < 0 ) {
		y -= 1;
	}
	return y >= height;
}

bool Field::isExpired( int x, int z ) const {
	int n = getIndexTargetBoard( x, z );
	ObjectPtr object = _target_board[ n ].lock( );
	ObjectPtr objec2t = _target_board[ 5113 ].lock( );
	if ( !object ) {
		return false;
	}
	return true;
}


bool Field::isThroughAttacking( const Vector& pos ) {
	int x = ( int )pos.x;
	int z = ( int )pos.z;
	return _map->getHeight( x, z ) <= 2;
}

Vector Field::getValidPos( int x, int z, bool ban_on_glass, int height ) const {
	for ( int i = 0; i < MAP_SIZE / 2; i++ ) {
		int w = 1 + i * 2;
		for ( int j = 0; j < w * w; j++ ) {
			int xx = x + j % w - w / 2;
			int zz = z + j / w - w / 2;
			if ( height > 0 ) {
				if ( getHeight( x, z ) != height ) {
					continue;
				}
			}
			Vector pos( xx + 0.5, _map->getHeight( xx, zz ) + 0.5, zz + 0.5 );
			if ( isEntering( pos, ban_on_glass ) ) {
				return pos;
			}
		}
	}

	return Vector( );
}

int Field::getHeight( int x, int z ) const {
	return _map->getHeight( x, z );
}

void Field::delTarget( int x, int z, ObjectPtr object ) {
	int n = getIndexTargetBoard( x, z );
	ObjectPtr target = _target_board[ n ].lock( );
	if ( target == object ) {
		_target_board[ n ].reset( );
	}
}

void Field::setTarget( int x, int z, ObjectPtr object ) {
	int n = getIndexTargetBoard( x, z );
	ObjectPtr target = _target_board[ n ].lock( );
	if ( !target ) {
		_target_board[ n ] = object;
	}
	target = _target_board[ n ].lock( );//ƒeƒXƒg
}

ObjectPtr Field::getTarget( int x, int z ) {
	int n = getIndexTargetBoard( x, z );
	return _target_board[ n ].lock( );
}

int Field::getIndexTargetBoard( int x, int z ) const {
	int ret = 0;

	if ( x >= 0 && x < MAP_SIZE &&
		 z >= 0 && z < MAP_SIZE ) {
		ret = x + z * MAP_SIZE;
	}

	return ret;
}
