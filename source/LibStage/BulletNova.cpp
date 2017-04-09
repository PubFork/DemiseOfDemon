#include "BulletNova.h"
#include "Framework.h"
#include "Field.h"
#include "Enemy.h"
#include "Effect.h"
#include "Stage.h"

double BulletNova::_power;
double BulletNova::_speed;

void BulletNova::initialize( ) {
	FrameworkPtr fw = Framework::getInstance( );
	_power = fw->getSettingValue( "WEAPON_NOVA_POWER" );
	_speed = fw->getSettingValue( "WEAPON_NOVA_SPEED" );
}

BulletNova::BulletNova( const Vector& pos, int level )
: Bullet( Bullet::NOVA )
, _pos( pos )
, _level( level ) {
	for ( int i = 0; i < DIV; i++ ) {
		_expired[ i ] = true;
	}
	_ratio = 0;
}

Vector BulletNova::getPos( ) const {
	return _pos;
}

double BulletNova::getLength( ) const {
	return ( _level + 1 ) * _ratio + 1;
}


bool BulletNova::isExpired( int num ) {
	return _expired[ num ];
}

BulletNova::~BulletNova( ) {
}

bool BulletNova::update( ) {
	_ratio += _speed;

	StagePtr stage = Stage::getTask( );
	FieldPtr field = stage->getField( );
	Vector pos = getPos( );

	std::array< EnemyPtr, 100 > target;
	int idx = 0;

	int range = ( int )getLength( );
	for ( int i = 0; i < range * 2; i++ ) {
		for ( int j = 0; j < range * 2; j++ ) {
			Vector p(
				( int )pos.x + 0.5 + i - range,
				pos.y,
				( int )pos.z + 0.5 + j - range );
			if ( ( pos - p ).getLength( ) < getLength( ) + 1 ) {
				if ( attackEnemy( p, _power ) ) {
					ObjectPtr object = field->getTarget( ( int )p.x, ( int )p.z );
					EnemyPtr enemy = std::dynamic_pointer_cast< Enemy >( object );
					EffectPtr effect = stage->getEffect( );
					effect->play( Effect::BMvol2_ALL, p + Vector( 0, 0.3, 0 ), 0.3f );
					// ノックバック
					if ( enemy ) {
						Vector dir = ( enemy->getPos( ) - _pos );
						dir.y = 0;
						dir = dir.normalize( );
						enemy->knockback( dir );
					}
				}
			}
		}
	}

	return _ratio < 1.0;
}
