#include "BulletArrow.h"
#include "Framework.h"
#include "Field.h"
#include "Enemy.h"
#include "Stage.h"
#include "Effect.h"

const int READY_TIME = 10;
const int FOUND_TIME = 20;
const double SPEED = 0.2;

double BulletArrow::_power;
int BulletArrow::_range;
void BulletArrow::initialize( ) {
	FrameworkPtr fw = Framework::getInstance( );
	_power = fw->getSettingValue( "WEAPON_ARROW_POWER" );
	_range = ( int )fw->getSettingValue( "WEAPON_ARROW_RANGE" );
}

BulletArrow::BulletArrow( const Vector& pos, const Vector& vec )
: Bullet( Bullet::ARROW )
, _center( pos + Vector( 0, 2.0, 0 ) )
, _pos( pos + vec ) {

	_vec = Vector( 0, 1, 0 ).cross( vec ).normalize( ) * SPEED;
	_count = 0;
}

Vector BulletArrow::getPos( ) const {
	return _pos;
}

BulletArrow::~BulletArrow( ) {
}

bool BulletArrow::update( ) {
	StagePtr stage = Stage::getTask( );
	FieldPtr field = stage->getField( );

	EnemyPtr target = _target.lock( );
	if ( target ) {
		// 敵を追尾
		double radius = target->getHeight( ) / 2;
		Vector vec = target->getPos( ) + Vector( 0, radius, 0 ) - _pos;
		_vec += vec.normalize( ) * ( SPEED * 0.4 );
		_vec = _vec.normalize( ) * SPEED;
	} else {
		// 上昇
		Vector dir = _center - _pos;
		Vector v = _vec + dir.normalize( ) * ( SPEED * ( 0.6 - _count * 0.1 / READY_TIME ) );
		_vec = v.normalize( ) * SPEED;
		_count++;
		if ( _count >= READY_TIME ) {
			// 対象検索
			double min = 1000;
			for ( int i = 0; i < _range * 2 + 1; i++ ) {
				for ( int j = 0; j < _range * 2 + 1; j++ ) {
					int x = ( int )_pos.x - _range + i;
					int z = ( int )_pos.z - _range + j;
					ObjectPtr object = field->getTarget( x, z );
					EnemyPtr enemy = std::dynamic_pointer_cast< Enemy >( object );
					if ( enemy ) {
						//1番近いのか？
						double radius = enemy->getHeight( ) / 2;
						Vector vec = enemy->getPos( ) + Vector( 0, radius, 0 ) - _pos;
						double length = vec.getLength2( );
						if ( min > length ) {
							_target = enemy;
							min = length;
						}
					}
				}
			}
			
			if ( _count >= FOUND_TIME ) {
				// ターゲットが見つからないので消滅
				if ( !_target.lock( ) ) {
					return false;
				}
			}
		}
	}

	// 移動
	_pos += _vec;

	// 画面外
	FrameworkPtr fw = Framework::getInstance( );
	if ( fw->isOutterOfScreen( _pos ) ) {
		return false;
	}

	// もし壁にあたったら消滅
	int x = ( int )_pos.x;
	int z = ( int )_pos.z;
	if ( field->getHeight( x, z ) > ( int )_pos.y ) {
		return false;
	}
	if ( isDoor( x, z ) ) {
		return false;
	}

	// 攻撃判定
	if ( attackEnemy( _pos, _power ) ) {
		EffectPtr effect = stage->getEffect( );
		effect->play( Effect::BMVOL2_HIT03, _pos + Vector( 0, 0.3, 0 ), 0.3f );
		return false;
	}

	return true;
}
