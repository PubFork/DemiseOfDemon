#include "BulletShot.h"
#include "Framework.h"
#include "Field.h"
#include "Enemy.h"
#include "Stage.h"
#include "Effect.h"

const int RANGE = 3;

double BulletShot::_power;
double BulletShot::_speed;

void BulletShot::initialize( ) {
	FrameworkPtr fw = Framework::getInstance( );
	_power = fw->getSettingValue( "WEAPON_SHOT_POWER" );
	_speed = fw->getSettingValue( "WEAPON_SHOT_SPEED" );
}

BulletShot::BulletShot( const Vector& pos, const Vector& dir )
: Bullet( Bullet::SHOT )
, _pos( pos )
, _dir( dir.normalize( ) ) {
}

Vector BulletShot::getPos( ) const {
	return _pos;
}

Vector BulletShot::getDir( ) const {
	return _dir;
}

BulletShot::~BulletShot( ) {
}


bool BulletShot::update( ) {
	StagePtr stage = Stage::getTask( );
	FieldPtr field = stage->getField( );

	// 移動
	_pos += _dir * _speed;

	{	// 消失チェック
		FrameworkPtr fw = Framework::getInstance( );
		if ( fw->isOutterOfScreen( _pos ) ) {
			return false;
		}
		int x = ( int )_pos.x;
		int z = ( int )_pos.z;
		if ( field->getHeight( x, z ) > ( int )_pos.y ) {
			return false;
		}
		if ( isDoor( x, z ) ) {
			return false;
		}
	}
	
	
	{	// 攻撃判定
		for ( int i = 0; i < RANGE; i++ ) {
			for ( int j = 0; j < RANGE; j++ ) {
				Vector pos = _pos + Vector( i - RANGE / 2, 0, j - RANGE / 2 );
				if ( attackEnemy( pos, _power ) ) {
					EffectPtr effect = stage->getEffect( );
					effect->play( Effect::BMVOL2_HIT04, pos + Vector( 0, 0.3, 0 ), 0.3f );
					return false;
				}
			}
		}
	}

	return true;
}
