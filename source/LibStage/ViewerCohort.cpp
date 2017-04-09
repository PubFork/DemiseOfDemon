#include "ViewerCohort.h"
#include "Cohort.h"
#include "DxLib.h"
#include "Framework.h"
#include "Stage.h"

const int MAX_NUM[ Enemy::MAX_TYPE ] = {
	100, //GOBLIN
	 30, //REDGOBLIN
	 30, //GHOST
	 30, //ARMOR
	  3, //CYCLOPS
	100, //MINOTAUR
};

ViewerCohort::ViewerCohort( ) {
	init( );
	createModel( );
}

ViewerCohort::~ViewerCohort( ) {
}

void ViewerCohort::draw( ) {
	StagePtr stage = Stage::getTask( );
	CohortPtr cohort = stage->getCohort( );
	FrameworkPtr fw = Framework::getInstance( );

	for ( int i = 0; i < Cohort::MAX_NUM; i++ ) {
		EnemyConstPtr enemy = cohort->getEnemy( i );
		if ( !enemy ) {
			continue;
		}
		if ( fw->isOutterOfScreen( enemy->getPos( ) ) ) {
			continue;
		}
		drawEnemy( enemy );
	}
	
	for ( int i = 0; i < Enemy::MAX_TYPE; i++ ) {
		GROUP& group = _group[ i ];
		group.num = 0;
	}
}

void ViewerCohort::drawEnemy( EnemyConstPtr enemy ) {
	Enemy::MOTION motion = enemy->getMotion( );
	Enemy::TYPE type = enemy->getType( );
	GROUP& group = _group[ type ];
	if ( group.num >= MAX_NUM[ type ] ) {
		return;
	}

	int body_id = group.enemy[ group.num ].id[ motion ].body_id;
	int anim_id = group.enemy[ group.num ].id[ motion ].anim_id;

	if ( body_id < 0 ) {
		return;
	}
	group.num++;

	Vector pos = enemy->getPos( );
	double time = enemy->getTime( );
	double dir_x = enemy->getDirX( );
	double dir_z = enemy->getDirZ( );
	if ( dir_x == 0 ) {
		dir_x = 0.001f;
	}

	// À•W•ÏŠ·
	MATRIX matrix = MGetIdent( );
	matrix = MMult( matrix, MGetRotVec2( VGet( 0, 0, -1 ), VGet( ( float )dir_x, 0, ( float )dir_z ) ) );
	matrix = MMult( matrix, MGetScale( VGet( 0.008f, 0.008f, 0.008f ) ) );
	matrix = MMult( matrix, MGetTranslate( VGet( ( float )pos.x, ( float )pos.y, ( float )pos.z ) ) );
	MV1SetMatrix( body_id, matrix );

	// ƒAƒjƒ[ƒVƒ‡ƒ“ŠÔİ’è
	MV1SetAttachAnimTime( body_id, anim_id, ( float )time );

	// ‚R‚cƒ‚ƒfƒ‹‚Ì•`‰æ
	MV1DrawModel( body_id ) ;
}

void ViewerCohort::create( Enemy::TYPE type, Enemy::MOTION motion, const char * filename ) {
	int form_id = MV1LoadModel( filename );
	if ( form_id < 0 ) {
		return;
	}
	int num = MV1GetMaterialNum( form_id ) ;
	for ( int i = 0; i < num; i++ ) {
		MV1SetMaterialEmiColor( form_id, i, GetColorF( 1.0f, 1.0f, 1.0f, 1.0f ) );
	}

	GROUP& group = _group[ type ];
	for ( int j = 0; j < MAX_NUM[ type ]; j++ ) {
		GROUP::ENEMY::ID& id = group.enemy[ j ].id[ motion ];
		id.body_id = MV1DuplicateModel( form_id );
		id.anim_id = MV1AttachAnim( id.body_id, 0, -1, FALSE ) ;
	}
}

void ViewerCohort::init( ) {
	for ( int i = 0; i < Enemy::MAX_TYPE; i++ ) {
		GROUP& group = _group[ i ];
		for ( int j = 0; j < MAX_NUM[ i ]; j++ ) {
			GROUP::ENEMY enemy;
			for ( int k = 0; k < Enemy::MAX_MOTION; k++ ) {
				enemy.id[ k ].body_id = -1;
			}
			group.enemy.push_back( enemy );
		}
		group.num = 0;
	}
}

void ViewerCohort::createModel( ) {
	create( Enemy::GOBLIN, Enemy::MOTION_WAIT,   "resource/character/enemy/goblin/enemy0_wait.mv1" );
	create( Enemy::GOBLIN, Enemy::MOTION_WALK,   "resource/character/enemy/goblin/enemy0_walk.mv1" );
	create( Enemy::GOBLIN, Enemy::MOTION_DAMAGE, "resource/character/enemy/goblin/enemy0_damage.mv1" );
	create( Enemy::GOBLIN, Enemy::MOTION_ATTACK, "resource/character/enemy/goblin/enemy0_attack.mv1");
	create( Enemy::GOBLIN, Enemy::MOTION_JUMP,   "resource/character/enemy/goblin/enemy0_jump.mv1");
	create( Enemy::GOBLIN, Enemy::MOTION_DEAD,   "resource/character/enemy/goblin/enemy0_dead.mv1");
	create( Enemy::GOBLIN, Enemy::MOTION_FALL,   "resource/character/enemy/goblin/enemy0_fall.mv1");
	
	create( Enemy::REDGOBLIN, Enemy::MOTION_WAIT,   "resource/character/enemy/red_goblin/enemy1_wait.mv1" );
	create( Enemy::REDGOBLIN, Enemy::MOTION_WALK,   "resource/character/enemy/red_goblin/enemy1_walk.mv1" );
	create( Enemy::REDGOBLIN, Enemy::MOTION_DAMAGE, "resource/character/enemy/red_goblin/enemy1_damage.mv1" );
	create( Enemy::REDGOBLIN, Enemy::MOTION_ATTACK, "resource/character/enemy/red_goblin/enemy1_attack.mv1");
	create( Enemy::REDGOBLIN, Enemy::MOTION_JUMP,   "resource/character/enemy/red_goblin/enemy1_jump.mv1");
	create( Enemy::REDGOBLIN, Enemy::MOTION_DEAD,   "resource/character/enemy/red_goblin/enemy1_dead.mv1");
	create( Enemy::REDGOBLIN, Enemy::MOTION_FALL,   "resource/character/enemy/red_goblin/enemy1_fall.mv1");

	create( Enemy::GHOST, Enemy::MOTION_WAIT,   "resource/character/enemy/ghost/enemy_ghost_wait.mv1" );
	create( Enemy::GHOST, Enemy::MOTION_WALK,   "resource/character/enemy/ghost/enemy_ghost_walk.mv1" );
	create( Enemy::GHOST, Enemy::MOTION_DAMAGE, "resource/character/enemy/ghost/enemy_ghost_damage.mv1" );
	create( Enemy::GHOST, Enemy::MOTION_ATTACK, "resource/character/enemy/ghost/enemy_ghost_attack.mv1");
	create( Enemy::GHOST, Enemy::MOTION_JUMP,   "resource/character/enemy/ghost/enemy_ghost_walk.mv1");
	create( Enemy::GHOST, Enemy::MOTION_DEAD,   "resource/character/enemy/ghost/enemy_ghost_dead.mv1");
	create( Enemy::GHOST, Enemy::MOTION_FALL,   "resource/character/enemy/ghost/enemy_ghost_walk.mv1");

	create( Enemy::ARMOR, Enemy::MOTION_WAIT,   "resource/character/enemy/armor/enemy_armor_wait.mv1" );
	create( Enemy::ARMOR, Enemy::MOTION_WALK,   "resource/character/enemy/armor/enemy_armor_walk.mv1" );
	create( Enemy::ARMOR, Enemy::MOTION_DAMAGE, "resource/character/enemy/armor/enemy_armor_damage.mv1" );
	create( Enemy::ARMOR, Enemy::MOTION_ATTACK, "resource/character/enemy/armor/enemy_armor_attack.mv1");
	create( Enemy::ARMOR, Enemy::MOTION_JUMP,   "resource/character/enemy/armor/enemy_armor_walk.mv1");
	create( Enemy::ARMOR, Enemy::MOTION_DEAD,   "resource/character/enemy/armor/enemy_armor_dead.mv1");
	create( Enemy::ARMOR, Enemy::MOTION_FALL,   "resource/character/enemy/armor/enemy_armor_walk.mv1");
	
	create( Enemy::CYCLOPS, Enemy::MOTION_WAIT,   "resource/character/enemy/cyclops/cyclops_wait.mv1" );
	create( Enemy::CYCLOPS, Enemy::MOTION_WALK,   "resource/character/enemy/cyclops/cyclops_walk.mv1" );
	create( Enemy::CYCLOPS, Enemy::MOTION_DAMAGE, "resource/character/enemy/cyclops/cyclops_damage.mv1" );
	create( Enemy::CYCLOPS, Enemy::MOTION_ATTACK, "resource/character/enemy/cyclops/cyclops_attack.mv1");
	create( Enemy::CYCLOPS, Enemy::MOTION_JUMP,   "resource/character/enemy/cyclops/cyclops_jump.mv1");
	create( Enemy::CYCLOPS, Enemy::MOTION_DEAD,   "resource/character/enemy/cyclops/cyclops_dead.mv1");
	create( Enemy::CYCLOPS, Enemy::MOTION_FALL,   "resource/character/enemy/cyclops/cyclops_fall.mv1");

	create( Enemy::MINOTAUR, Enemy::MOTION_WAIT,   "resource/character/enemy/minotaur/enemy_minotaur_wait.mv1" );
	create( Enemy::MINOTAUR, Enemy::MOTION_WALK,   "resource/character/enemy/minotaur/enemy_minotaur_walk.mv1" );
	create( Enemy::MINOTAUR, Enemy::MOTION_DAMAGE, "resource/character/enemy/minotaur/enemy_minotaur_damage.mv1" );
	create( Enemy::MINOTAUR, Enemy::MOTION_ATTACK, "resource/character/enemy/minotaur/enemy_minotaur_smash.mv1");		//–¼Ì¼‹\
	create( Enemy::MINOTAUR, Enemy::MOTION_JUMP,   "resource/character/enemy/minotaur/enemy_minotaur_cleave.mv1");	//–¼Ì¼‹\
	create( Enemy::MINOTAUR, Enemy::MOTION_DEAD,   "resource/character/enemy/minotaur/enemy_minotaur_dead.mv1");		
	create( Enemy::MINOTAUR, Enemy::MOTION_FALL,   "resource/character/enemy/minotaur/enemy_minotaur_statue.mv1");		//–¼Ì¼‹\
	//DASH‚Í‚È‚¢
}
