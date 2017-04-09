#include "BulletLay.h"
#include "Framework.h"
#include "Field.h"
#include "Enemy.h"
#include "Stage.h"
#include "Effect.h"

const double SPEED = 0.05;
const double RADIUS = 0.1;
const double LENGTH = 30.0;

double BulletLay::_power;

void BulletLay::initialize( ) {
	FrameworkPtr fw = Framework::getInstance( );
	_power = fw->getSettingValue( "WEAPON_LAY_POWER" );
}

BulletLay::BulletLay( const Vector& pos, const Vector& dir, int level )
: Bullet( Bullet::LAY )
, _pos( pos )
, _dir( dir.normalize( ) )
, _level( level ) {
	_expired = true;
	_radius = RADIUS * ( level + 1 );
	
	StagePtr stage = Stage::getTask( );
	FieldPtr field = stage->getField( );
	_length = 0;

	FrameworkPtr fw = Framework::getInstance( );
	while ( _length < LENGTH ) {
		double length = _length + 1.0;
		Vector p = pos + _dir * length;
		// チェックする
		int x = ( int )p.x;
		int z = ( int )p.z;
		if ( field->getHeight( x, z ) > ( int )p.y ) {
			break;
		}
		if ( isDoor( x, z ) ) {
			break;
		}
		if ( fw->isOutterOfScreen( p ) ) {
			break;
		}
		_length = length;

		{	// 攻撃判定
			if( attackEnemy( p, _power ) ) {
				EffectPtr effect = stage->getEffect( );
				effect->play( Effect::BMVOL2_HIT01, p + Vector( 0, 0.3, 0 ), 0.1f );
			}
			Vector vec( 0, _radius, 0 );
			Matrix mat = Matrix::makeTransformRotation( _dir, PI2 / ( _level + 1 ) );
			for ( int i = 0; i < _level + 1; i++ ) {
				vec = mat.multiply( vec );
				Vector pos = p + vec;
				attackEnemy( pos, _power );
			}
		}
	}
}

Vector BulletLay::getPos( ) const {
	return _pos;
}

Vector BulletLay::getDir( ) const {
	return _dir;
}

double BulletLay::getLength( ) const {
	return _length;
}

double BulletLay::getRadius( ) const {
	return _radius;
}

BulletLay::~BulletLay( ) {
}

bool BulletLay::update( ) {
	if ( _show ) {
		_show = false;
		return true;
	}

	return false;
}
