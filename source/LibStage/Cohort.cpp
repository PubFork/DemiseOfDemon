#include "Cohort.h"
#include "Framework.h"
#include "Enemy.h"
#include "EnemyGoblin.h"
#include "EnemyRedGoblin.h"
#include "EnemyGhost.h"
#include "EnemyArmor.h"
#include "EnemyCyclops.h"
#include "Map.h"
#include "Field.h"
#include "Stage.h"

Cohort::Cohort( ) {
}

Cohort::~Cohort( ) {
}

void Cohort::init( ) {
	FrameworkPtr fw = Framework::getInstance( );

	for ( int i = 0; i < MAX_NUM; i++ ) {
		_enemy[ i ].reset( );
	}
	_alive_cyclops = true;
	_alive_minotaur = true;

	// map‚©‚ç¶¬
	StagePtr stage = Stage::getTask( );
	FieldPtr field = stage->getField( );
	int index = 0;
	for ( int i = 0; i < MAP_SIZE; i++ ) {
		for ( int j = 0; j < MAP_SIZE; j++ ) {
			unsigned char object = field->getObject( i, j );
			EnemyPtr enemy;
			Vector pos( i + 0.5, field->getHeight( i, j ) + 0.5, j + 0.5 );
			switch ( object ){ 
			case OBJECT_ENEMY_GOBLIN:
				{
					Vector p = field->getValidPos( ( int )pos.x, ( int )pos.z, false, ( int )pos.y );
					enemy = EnemyGoblinPtr(new EnemyGoblin( p ));
					if ( index < MAX_NUM ) {
						_enemy[ index ] = enemy;
						index++;
						field->setTarget((int)p.x, (int)p.z, enemy);
					}
				}
				break;
			case OBJECT_ENEMY_REDGOBLIN:
				{
					Vector p = field->getValidPos( (int)pos.x, (int)pos.z );
					enemy = EnemyRedGoblinPtr( new EnemyRedGoblin( p ) );
					if ( index < MAX_NUM ) {
						_enemy[ index ] = enemy;
						index++;
						field->setTarget( ( int )p.x, ( int )p.z, enemy );
					}
				}
				break;
			case OBJECT_ENEMY_GHOST:
				{
					Vector p = field->getValidPos( (int)pos.x, (int)pos.z );
					enemy = EnemyGhostPtr( new EnemyGhost( p ) );
					if ( index < MAX_NUM ) {
						_enemy[ index ] = enemy;
						index++;
						field->setTarget( ( int )p.x, ( int )p.z, enemy );
					}
				}
				break;
			case OBJECT_ENEMY_ARMOR:
				{
					Vector p = field->getValidPos( ( int )pos.x, ( int )pos.z, false, ( int )pos.y );
					enemy = EnemyArmorPtr( new EnemyArmor( p ) );
					if ( index < MAX_NUM ) {
						_enemy[ index ] = enemy;
						index++;
						field->setTarget( ( int )p.x, ( int )p.z, enemy);
					}
				}
				break;
			case OBJECT_ENEMY_CYCLOPS:
				{
					Vector p = field->getValidPos( ( int )pos.x, ( int )pos.z );
					enemy = EnemyCyclopsPtr( new EnemyCyclops( p ) );
					if ( index < MAX_NUM ) {
						_enemy[ index ] = enemy;
						index++;
						field->setTarget( ( int )p.x, ( int )p.z, enemy );
					}
				}
				break;
			}
		}
	}
}

void Cohort::update( ) {
	FrameworkPtr fw = Framework::getInstance( );
	for ( int i = 0; i < MAX_NUM; i++ ) {
		EnemyPtr enemy = _enemy[ i ];
		if ( !enemy ) {
			continue;
		}
		if ( !enemy->isExpired( ) ) {
			if ( enemy->getType( ) == Enemy::CYCLOPS ) {
				_alive_cyclops = false;
			}
			if ( enemy->getType( ) == Enemy::MINOTAUR ) {
				_alive_minotaur = false;
			}
			_enemy[ i ].reset( );
			continue;
		}
		if ( fw->isOutterOfScreen( enemy->getPos( ) ) ) {
			continue;
		}
		enemy->act( );
		enemy->update( );
	}
}

void Cohort::add( EnemyPtr enemy ) {
	for ( int i = 0; i < MAX_NUM; i++ ) {
		EnemyPtr check = _enemy[ i ];
		if ( !check ) {
			_enemy[ i ] = enemy;
			enemy->update( );
			break;
		}
	}
}

EnemyConstPtr Cohort::getEnemy( int index ) const {
	return _enemy[ index ];
}

EnemyPtr Cohort::getEnemy( int index ) {
	return _enemy[ index ];
}

bool Cohort::isAliveCyclops( ) const {
	return _alive_cyclops;
}
bool Cohort::isAliveMinotaur( ) const {
	return _alive_minotaur;
}
