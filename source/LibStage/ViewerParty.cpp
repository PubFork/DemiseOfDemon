#include "ViewerParty.h"
#include "Party.h"
#include "Player.h"
#include "Character.h"
#include "DxLib.h"
#include "Stage.h"

ViewerParty::ViewerParty( ) {
	init( );
	createModel( );
}


ViewerParty::~ViewerParty( ) {
}

void ViewerParty::init( ) {
	for ( int i = 0; i < PLAYER_NUM; i++ ) {
		for ( int j = 0; j < Player::MAX_MOTION; j++ ) {
			_party.player[ i ].id[ j ].body = -1;
			_party.player[ i ].id[ j ].shadow = -1;
		}
	}
}

void ViewerParty::create( int index, Player::MOTION motion, const char * filename ) {
	{	// body
		int& id = _party.player[ index ].id[ motion ].body;
		id = MV1LoadModel( filename );
		if ( id < 0 ) {
			return;
		}
		int num = MV1GetMaterialNum( id ) ;
		for ( int i = 0; i < num; i++ ) {
			MV1SetMaterialEmiColor( id, i, GetColorF( 1.0f, 1.0f, 1.0f, 1.0f ) );
		}
		int& anim = _party.player[ index ].id[ motion ].body_anim;
		anim = MV1AttachAnim( id, 0, -1, FALSE ) ;
	}
	{	// shadow
		int& id = _party.player[ index ].id[ motion ].shadow;
		id = MV1DuplicateModel( _party.player[ index ].id[ motion ].body );
		MV1SetUseZBuffer( id, FALSE );
		MV1SetWriteZBuffer( id, FALSE );
		int num = MV1GetMaterialNum( id ) ;
		for ( int i = 0; i < num; i++ ) {
			MV1SetMaterialDifColor( id, i, GetColorF( 0, 0, 0, 0.2f ) );
		}
		_party.player[ index ].id[ motion ].shadow_anim = MV1AttachAnim( id, 0, -1, FALSE ) ;
	}
}

void ViewerParty::drawBody( ) const {
	StagePtr stage = Stage::getTask( );
	PartyPtr party = stage->getParty( );

	for ( int i = 0; i < PLAYER_NUM; i++ ) {
		PlayerConstPtr player = party->getPlayer( i );
		if ( !player ) {
			continue;
		}
		drawPlayerBody( i, player );
	}
}

void ViewerParty::drawShadow( ) const {
	StagePtr stage = Stage::getTask( );
	PartyPtr party = stage->getParty( );

	for ( int i = 0; i < PLAYER_NUM; i++ ) {
		PlayerConstPtr player = party->getPlayer( i );
		if ( !player ) {
			continue;
		}

		drawPlayerShadow( i, player );
	}
}

void ViewerParty::drawPlayerBody( int index, PlayerConstPtr player ) const {
	Player::MOTION motion = player->getMotion( );
	int id = _party.player[ index ].id[ motion ].body;
	int anim = _party.player[ index ].id[ motion ].body_anim;
	drawModel( id, anim, player );
}

void ViewerParty::drawPlayerShadow( int index, PlayerConstPtr player ) const {
	Player::MOTION motion = player->getMotion( );
	int id = _party.player[ index ].id[ motion ].shadow;
	int anim = _party.player[ index ].id[ motion ].shadow_anim;
	drawModel( id, anim, player );
}

void ViewerParty::drawModel( int id, int anim, PlayerConstPtr player ) const {
	Vector pos = player->getPos( );
	double time = player->getTime( );
	double dir_x = player->getDirX( );
	double dir_z = player->getDirZ( );
	if ( ( float )dir_x == 0 ) {
		dir_x = 0.001;
	}

	// 座標変換
	MATRIX matrix = MGetIdent( );
	matrix = MMult( matrix, MGetRotVec2( VGet( 0, 0, -1 ), VGet( ( float )dir_x, 0, ( float )dir_z ) ) );
	matrix = MMult( matrix, MGetScale( VGet( 0.008f, 0.008f, 0.008f ) ) );
	matrix = MMult( matrix, MGetTranslate( VGet( ( float )pos.x, ( float )pos.y, ( float )pos.z ) ) );
	MV1SetMatrix( id, matrix );

	// アニメーション時間設定
	MV1SetAttachAnimTime( id, anim, ( float )time );

	// ３Ｄモデルの描画
	MV1DrawModel( id ) ;
}

void ViewerParty::createModel( ) {
	//プレイヤーモデル
	
	create( 0, Player::MOTION_DAMAGE, "resource/character/player/knight/player_knight_damage.mv1" );
	create( 0, Player::MOTION_DEAD  , "resource/character/player/knight/player_knight_dead.mv1");
	create( 0, Player::MOTION_JUMP  , "resource/character/player/knight/player_knight_jump.mv1");
	create( 0, Player::MOTION_ATTACK, "resource/character/player/knight/player_knight_attack.mv1" );
	create( 0, Player::MOTION_USE   , "resource/character/player/knight/player_knight_use.mv1");
	create( 0, Player::MOTION_WAIT  , "resource/character/player/knight/player_knight_wait.mv1" );
	create( 0, Player::MOTION_WALK  , "resource/character/player/knight/player_knight_walk.mv1" );
	
	create( 1, Player::MOTION_DAMAGE, "resource/character/player/warrior/player_warrior_damage.mv1");
	create( 1, Player::MOTION_DEAD  , "resource/character/player/warrior/player_warrior_dead.mv1");
	create( 1, Player::MOTION_JUMP  , "resource/character/player/warrior/player_warrior_jump.mv1");
	create( 1, Player::MOTION_ATTACK, "resource/character/player/warrior/player_warrior_attack.mv1");
	create( 1, Player::MOTION_USE   , "resource/character/player/warrior/player_warrior_use.mv1");
	create( 1, Player::MOTION_WAIT  , "resource/character/player/warrior/player_warrior_wait.mv1" );
	create( 1, Player::MOTION_WALK  , "resource/character/player/warrior/player_warrior_walk.mv1" );

	create( 2, Player::MOTION_DAMAGE, "resource/character/player/mage/player_mage_damage.mv1" );
	create( 2, Player::MOTION_DEAD  , "resource/character/player/mage/player_mage_dead.mv1" );
	create( 2, Player::MOTION_JUMP  , "resource/character/player/mage/player_mage_jump.mv1" );
	create( 2, Player::MOTION_ATTACK, "resource/character/player/mage/player_mage_attack.mv1" );
	create( 2, Player::MOTION_USE   , "resource/character/player/mage/player_mage_use.mv1" );
	create( 2, Player::MOTION_WAIT  , "resource/character/player/mage/player_mage_wait.mv1" );
	create( 2, Player::MOTION_WALK  , "resource/character/player/mage/player_mage_walk.mv1" );
	
	create( 3, Player::MOTION_DAMAGE, "resource/character/player/monk/player_monk_damage.mv1" );
	create( 3, Player::MOTION_DEAD  , "resource/character/player/monk/player_monk_dead.mv1" );
	create( 3, Player::MOTION_JUMP  , "resource/character/player/monk/player_monk_jump.mv1" );
	create( 3, Player::MOTION_ATTACK, "resource/character/player/monk/player_monk_attack.mv1" );
	create( 3, Player::MOTION_USE   , "resource/character/player/monk/player_monk_use.mv1" );
	create( 3, Player::MOTION_WALK  , "resource/character/player/monk/player_monk_walk.mv1" );
	create( 3, Player::MOTION_WAIT  , "resource/character/player/monk/player_monk_wait.mv1" );

	create( 4, Player::MOTION_DAMAGE, "resource/character/player/shooter/player_shooter_damege.mv1" );
	create( 4, Player::MOTION_DEAD  , "resource/character/player/shooter/player_shooter_dead.mv1" );
	create( 4, Player::MOTION_JUMP  , "resource/character/player/shooter/player_shooter_jump.mv1" );
	create( 4, Player::MOTION_ATTACK, "resource/character/player/shooter/player_shooter_attack.mv1" );
	create( 4, Player::MOTION_USE   , "resource/character/player/shooter/player_shooter_use.mv1" );
	create( 4, Player::MOTION_WAIT  , "resource/character/player/shooter/player_shooter_wait.mv1" );
	create( 4, Player::MOTION_WALK  , "resource/character/player/shooter/player_shooter_walk.mv1" );

	create( 5, Player::MOTION_DAMAGE, "resource/character/player/summoner/player_summoner_damege.mv1" );
	create( 5, Player::MOTION_DEAD  , "resource/character/player/summoner/player_summoner_dead.mv1" );
	create( 5, Player::MOTION_JUMP  , "resource/character/player/summoner/player_summoner_jump.mv1" );
	create( 5, Player::MOTION_ATTACK, "resource/character/player/summoner/player_summoner_attack.mv1" );
	create( 5, Player::MOTION_USE   , "resource/character/player/summoner/player_summoner_use.mv1" );
	create( 5, Player::MOTION_WAIT  , "resource/character/player/summoner/player_summoner_wait.mv1" );
	create( 5, Player::MOTION_WALK  , "resource/character/player/summoner/player_summoner_walk.mv1" );
}
