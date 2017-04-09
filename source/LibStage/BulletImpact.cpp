#include "BulletImpact.h"
#include "Framework.h"
#include "Field.h"
#include "Enemy.h"
#include "Stage.h"
#include "Effect.h"

const int TIME = 10;
const double RANGE = 3;

void BulletImpact::initialize( ) {
}

BulletImpact::BulletImpact( const Vector& pos, int level )
: Bullet( Bullet::IMPACT )
, _pos( pos )
, _level( level )
, _count( 0 ) {
}

Vector BulletImpact::getPos( ) const {
	return _pos;
}

int BulletImpact::getAlpha( ) const {
	return 255 - _count * 255 / TIME;
}

double BulletImpact::getLength( ) const {
	return _count * ( RANGE + _level ) / TIME;
}

BulletImpact::~BulletImpact( ) {
}

bool BulletImpact::update( ) {
	_count++;
	if ( _count >= TIME ) {
		return false;
	}

	StagePtr stage = Stage::getTask( );
	FieldPtr field = stage->getField( );
	
	int range = ( int )getLength( );
	for ( int i = 0; i < range * 2 + 1; i++ ) {
		for ( int j = 0; j < range * 2 + 1; j++ ) {
			Vector vec( i - range, 0, j - range );
			if ( vec.getLength2( ) < range * range ) {
				vec += getPos( );
				ObjectPtr object = field->getTarget( ( int )vec.x, ( int )vec.z );
				EnemyPtr enemy = std::dynamic_pointer_cast< Enemy >( object );
				if ( enemy ) {
					Vector dir = enemy->getPos( ) - getPos( );
					enemy->knockback( dir.normalize( ) );
				}
			}
		}
	}

	return true;
}
