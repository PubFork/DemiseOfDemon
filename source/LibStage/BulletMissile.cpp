#include "BulletMissile.h"
#include "Framework.h"
#include "Field.h"
#include "Player.h"
#include "Client.h"
#include "Stage.h"
#include "Effect.h"
#include "Sound.h"

int BulletMissile::_power;
double BulletMissile::_speed;

void BulletMissile::initialize( ) {
	FrameworkPtr fw = Framework::getInstance( );
	_power = ( int )fw->getSettingValue( "WEAPON_MISSILE_POWER" );
	_speed = fw->getSettingValue( "WEAPON_MISSILE_SPEED" );
}

BulletMissile::BulletMissile( const Vector& pos, const Vector& dir )
: Bullet( Bullet::MISSILE )
, _pos( pos )
, _dir( dir.normalize( ) ) {
}

Vector BulletMissile::getPos( ) const {
	return _pos;
}

BulletMissile::~BulletMissile( ) {
}


bool BulletMissile::update( ) {
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
		int x = ( int )_pos.x;
		int z = ( int )_pos.z;
				
		ObjectPtr object = field->getTarget( x, z );
		PlayerPtr player = std::dynamic_pointer_cast< Player >( object );
		if ( player ) {
			double y = player->getPos( ).y;
			if ( y < _pos.y && y + 2 > _pos.y ) {
				int index = player->getIndex( );
				ClientPtr client = Client::getTask( );
				client->damage( index, _power );
				SoundPtr sound = stage->getSound( );
				sound->play( Sound::SOUND_PLAYER_DAMAGE );
				EffectPtr effect = stage->getEffect( );
				effect->play( Effect::BMVOL2_HIT07, player->getPos( ) + Vector( 0, 1.3, 0 ), 0.1f );
				return false;
				
			}
		}
	}

	return true;
}
