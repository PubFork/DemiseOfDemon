#include "Bullet.h"
#include "mathmatics.h"
#include "Framework.h"
#include "Object.h"
#include "Enemy.h"
#include "Field.h"
#include "Door.h"
#include "Stage.h"

Bullet::Bullet( TYPE type )
: _type( type ) {
}


Bullet::~Bullet( ) {
}

Bullet::TYPE Bullet::getType( ) const {
	return _type;
}

bool Bullet::attackEnemy( const Vector& pos, double power ) {
	StagePtr stage = Stage::getTask( );
	FieldPtr field = stage->getField( );
	ObjectPtr object = field->getTarget( ( int )pos.x, ( int )pos.z );
	EnemyPtr enemy = std::dynamic_pointer_cast< Enemy >( object );
	if ( enemy ) {
		double bottom = enemy->getPos( ).y;
		double top = bottom + enemy->getHeight( );
		if ( pos.y > bottom && pos.y < top ) {
			enemy->damage( power );
			return true;
		}
	}
	return false;
}

bool Bullet::isDoor( int x, int z ) {
	StagePtr stage = Stage::getTask( );
	FieldPtr field = stage->getField( );
	ObjectPtr object = field->getTarget( x, z );
	DoorPtr door = std::dynamic_pointer_cast< Door >( object );
	if ( door ) {
		return true;
	}

	return false;
}
