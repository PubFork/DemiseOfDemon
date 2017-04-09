#include "PlayerMonk.h"
#include "Network.h"
#include "mathmatics.h"
#include "Stage.h"
#include "Field.h"
#include "Enemy.h"
#include "DxLib.h"
#include "Model.h"
#include "Weapon.h"
#include "BulletImpact.h"
#include "Effect.h"

const double ATTACK_TIME = 74;
const double BEGIN_TIME = 0;
const double END_TIME = 74;

const double DIRECT_POWER = 300;
const double DIRECT_POWER_ADD = 100;
const double IMPACT_POWER = 10;

const int IMPACT_NUM = 100;

PlayerMonk::PlayerMonk( Vector pos )
: Player( PLAYER_MONK, pos ){
	int id = LoadGraph( "resource/image/weapon/impact.png" );
	_model = ModelPtr( new Model );
	_model->alloc( IMPACT_NUM * 2 );
	_model->setTexture( id );
}


PlayerMonk::~PlayerMonk( ) {

}

void PlayerMonk::attack( ) {
	StagePtr stage = Stage::getTask( );
	FieldPtr field = stage->getField( );

	// 攻撃
	if ( isPassedTime( 30 ) ) {
		knockback_dir = Vector( getDirX( ), 0, getDirZ( ) ).normalize( );
		Vector pos = getPos( ) + knockback_dir * 2.0;
		int x = ( int )pos.x;
		int z = ( int )pos.z;
		for ( int i = 0; i < 3; i++ ) {
			for ( int j = 0; j < 3; j++ ) {
				ObjectPtr object = field->getTarget( x + i - 1, z + j - 1 );
				EnemyPtr enemy = std::dynamic_pointer_cast< Enemy >( object );
				if ( enemy ) {
					double enemy_y = enemy->getPos( ).y;
					double player_y = getPos( ).y;
					if ( enemy_y >= player_y && enemy_y < player_y + enemy->getHeight( ) ) {
						enemy->damage( DIRECT_POWER + DIRECT_POWER_ADD * _weapon_level );
						_targets.push_back( enemy );
					}
				}
			}
		}
		if ( !_targets.empty( ) ) {
			EffectPtr effect = stage->getEffect( );
			effect->play( Effect::BMVOL1_1_FIRE, pos + Vector( 0, 1.3, 0 ), 0.2f );
		}
	}

	// ノックバック中止
	if ( isPassedTime( 55 ) ) {
		_targets.clear( );
	}

	// ノックバック
	std::list< EnemyPtr >::iterator it = _targets.begin( );
	while ( it != _targets.end( ) ) {
		(*it)->knockback( knockback_dir );
		it++;
	}

	// 衝撃波
	if ( isPassedTime( 35 ) ) {
		if ( !_targets.empty( ) ) {
			WeaponPtr weapon = stage->getWeapon( );
			BulletPtr bullet = BulletImpactPtr( new BulletImpact( getPos( ) + knockback_dir * 2.0, _weapon_level ) );
			weapon->add( bullet );
		}
	}	
}

double PlayerMonk::getAttackTimeBegin( ) {
	return BEGIN_TIME;
}

double PlayerMonk::getAttackTimeEnd( ) {
	return END_TIME;
}

double PlayerMonk::getAttackTotalTime( ) {
	return ATTACK_TIME;
}