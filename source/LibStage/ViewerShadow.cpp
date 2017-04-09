#include "ViewerShadow.h"
#include "DxLib.h"
#include "Party.h"
#include "Field.h"
#include "Player.h"
#include "Cohort.h"
#include "Enemy.h"
#include "Framework.h"
#include "Stage.h"

const double RADIUS = 1.0;

ViewerShadow::ViewerShadow( ) {
	int id = LoadGraph( "resource/image/shadow/shadow.png" );

	alloc( MAX_NUM * 2 );
	setTexture( id );
	_num = 0;
}

ViewerShadow::~ViewerShadow( ) {
}

void ViewerShadow::draw( ) {
	//プレイヤー
	addPlayerShadow( );

	// モンスター
	addEnemyShadow( );

	setPolygonNum( _num * 2 );
	Model::draw( true );
	_num = 0;
}

void ViewerShadow::add( const Vector& pos ) {
	if ( _num >= MAX_NUM ) {
		return;
	}

	VERTEX vertex[ 4 ];
	for ( int i = 0; i < 4; i++ ) {
		vertex[ i ].pos = Vector( i / 2 * RADIUS - RADIUS / 2, 0.01, i % 2 * RADIUS - RADIUS / 2 ) + pos;
		vertex[ i ].u = i % 2;
		vertex[ i ].v = i / 2;
	}

	set( _num * 6 + 0, vertex[ 0 ] ); 
	set( _num * 6 + 1, vertex[ 1 ] ); 
	set( _num * 6 + 2, vertex[ 2 ] ); 
	set( _num * 6 + 3, vertex[ 1 ] ); 
	set( _num * 6 + 4, vertex[ 3 ] ); 
	set( _num * 6 + 5, vertex[ 2 ] ); 

	_num++;
}

void ViewerShadow::addPlayerShadow( ) {
	StagePtr stage = Stage::getTask( );
	FieldPtr field = stage->getField( );
	PartyPtr party = stage->getParty( );

	for ( int i = 0; i < PLAYER_NUM; i++ ) {
		PlayerPtr player = party->getPlayer( i );
		if ( !player ) {
			continue;
		}

		Vector pos = player->getPos( );

		int min_height = 100;
		int height;

		height = field->getHeight( ( int )( pos.x + RADIUS ), ( int )( pos.z ) );
		if ( min_height > height ) {
			min_height = height; 
		}
		height = field->getHeight( ( int )( pos.x - RADIUS ), ( int )( pos.z ) );
		if ( min_height > height ) {
			min_height = height; 
		}
		height = field->getHeight( ( int )( pos.x ), ( int )( pos.z + RADIUS ) );
		if ( min_height > height ) {
			min_height = height; 
		}
		height = field->getHeight( ( int )( pos.x ), ( int )( pos.z - RADIUS ) );
		if ( min_height > height ) {
			min_height = height; 
		}

		pos.y = min_height + 0.01;

		add( pos );
	}
}

void ViewerShadow::addEnemyShadow( ) {
	FrameworkPtr fw = Framework::getInstance( );
	StagePtr stage = Stage::getTask( );
	FieldPtr field = stage->getField( );
	CohortPtr cohort = stage->getCohort( );
	if ( !cohort ) {
		return;
	}

	for ( int i = 0; i < Cohort::MAX_NUM; i++ ) {
		EnemyPtr enemy = cohort->getEnemy( i );
		if ( !enemy ) {
			continue;
		}

		Vector pos = enemy->getPos( );
		if ( fw->isOutterOfScreen( pos ) ) {
			continue;
		}

		int min_height = 100;
		int height;

		height = field->getHeight( ( int )( pos.x + RADIUS ), ( int )( pos.z ) );
		if ( min_height > height ) {
			min_height = height; 
		}
		height = field->getHeight( ( int )( pos.x - RADIUS ), ( int )( pos.z ) );
		if ( min_height > height ) {
			min_height = height; 
		}
		height = field->getHeight( ( int )( pos.x ), ( int )( pos.z + RADIUS ) );
		if ( min_height > height ) {
			min_height = height; 
		}
		height = field->getHeight( ( int )( pos.x ), ( int )( pos.z - RADIUS ) );
		if ( min_height > height ) {
			min_height = height; 
		}

		pos.y = min_height + 0.01;

		add( pos );
	}
}
