#include "BulletSword.h"
#include "Field.h"
#include "Enemy.h"
#include "Framework.h"
#include "Stage.h"
#include "Effect.h"

const double SLASH_RADIUS = PI * 0.8;

double BulletSword::_power;
double BulletSword::_speed;

void BulletSword::initialize( ) {
	FrameworkPtr fw = Framework::getInstance( );
	_power = fw->getSettingValue( "WEAPON_SWORD_POWER" );
	_speed = fw->getSettingValue( "WEAPON_SWORD_SPEED" );
}

BulletSword::BulletSword( const Vector& pos, double dir_x, double dir_z, int level )
: Bullet( Bullet::SWORD )
, _pos( pos )
, _dir( dir_x, 0, dir_z )
, _level( level ) {
	_ratio = 0;
}

Vector BulletSword::getPos( ) const {
	return _pos;
}

Vector BulletSword::getBeginDir( ) const {
	Matrix mat = Matrix::makeTransformRotation( Vector( 0, -1, 0 ), - SLASH_RADIUS / 2 );
	return mat.multiply( _dir );
}

Vector BulletSword::getEndDir( ) const {
	Matrix mat = Matrix::makeTransformRotation( Vector( 0, -1, 0 ), _ratio * SLASH_RADIUS - SLASH_RADIUS / 2 );
	return mat.multiply( _dir );
}

double BulletSword::getLength( ) const {
	return _level + 2;
}

BulletSword::~BulletSword( ) {
}

bool BulletSword::update( ) {

	_ratio += _speed;

	if ( _ratio < 1.0 ) {
		return true;
	}


	StagePtr stage = Stage::getTask( );
	FieldPtr field = stage->getField( );
	Vector pos = getPos( );
	Vector end_dir = getEndDir( );
	Vector begin_dir = getBeginDir( );
	Vector center_dir = ( end_dir + begin_dir ) * 0.5;
	double angle = center_dir.angle( begin_dir );

	int range = ( int )getLength( );
	for ( int i = 0; i < range * 2; i++ ) {
		for ( int j = 0; j < range * 2; j++ ) {
			Vector p(
				( int )pos.x + 0.5 + i - range,
				pos.y,
				( int )pos.z + 0.5 + j - range );
			if ( center_dir.angle( p - pos ) < angle ) {
				if ( ( pos - p ).getLength( ) < getLength( ) ) {
					if( attackEnemy( p, _power ) ){
						EffectPtr effect = stage->getEffect( );
						effect->play( Effect::BMVOL2_HIT06, p + Vector( 0, 0.3, 0 ), 0.3f );
					}
				}
			}
		}
	}

	return false;
}
