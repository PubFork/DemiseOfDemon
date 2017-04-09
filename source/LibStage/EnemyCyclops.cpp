#include "EnemyCyclops.h"
#include "Framework.h"
#include "Client.h"
#include "Party.h"
#include "Player.h"
#include "Stage.h"
#include "Effect.h"
#include "Sound.h"
#include "Experiences.h"


const double TIME[ Enemy::MAX_MOTION ] = {
	 90, //MOTION_WAIT,
	 90, //MOTION_WALK,
	100, //MOTION_FALL,
	 90, //MOTION_DAMAGE,
	110, //MOTION_DEAD,
	110, //MOTION_JUMP,
	 90, //MOTION_ATTACK,
};

EnemyCyclops::EnemyCyclops( const Vector& pos )
: Enemy(
	Enemy::CYCLOPS, 
	Framework::getInstance( )->getSettingValue( "ENEMY_CYCLOPS_SPEED" ),
	Framework::getInstance( )->getSettingValue( "ENEMY_CYCLOPS_RANGE" ),
	Framework::getInstance( )->getSettingValue( "ENEMY_CYCLOPS_LIFE" ),
	pos )
, _power( ( int )Framework::getInstance( )->getSettingValue( "ENEMY_CYCLOPS_POWER" ) ) {

}

EnemyCyclops::~EnemyCyclops( ) {
}

double EnemyCyclops::getTime( MOTION motion ) const {
	return TIME[ motion  ];
}

void EnemyCyclops::act( ) {
	searchTarget( );
	moveToTarget( false );
	attackToTarget( );
}

double EnemyCyclops::getHeight( ) const {
	return 4;
}

void EnemyCyclops::onAttack( ) {
	PlayerPtr target = getTarget( );
	if ( !target ) {
		return;
	}

	if ( isPassedTime( 45 ) ) {
		StagePtr stage = Stage::getTask( );
		PartyPtr party = stage->getParty( );
		ClientPtr client = Client::getTask( );
		SoundPtr sound = stage->getSound( );
		sound->play( Sound::SOUND_PLAYER_DAMAGE );
		EffectPtr effect = stage->getEffect( );
		effect->play( Effect::BMVOL2_HIT05, target->getPos( ) + Vector( 0, 1.0, 0 ), 0.3f );
		effect->play( Effect::BMVOL1_1_SHOGEKI, target->getPos( ) + Vector( 0, 1.0, 0 ), 0.1f );	// ŽžŠÔŽw’è‚ ‚è
		Vector ap = getPos( ) + Vector( getDirX( ), 0, getDirZ( ) ).normalize( ) * 3;
		for ( int i = 0; i < PLAYER_NUM; i++ ) {
			PlayerPtr player = party->getPlayer( i );
			if ( !player ) {
				continue;
			}
			Vector vec = player->getPos( ) - ap;
			if ( vec.getLength( ) < 5 ) {
				client->damage( player->getIndex( ), _power );
				client->damage( player->getIndex( ), _power );
			}
		}
	}
}

void EnemyCyclops::onExp( )  {
	StagePtr stage = Stage::getTask( );
	ExperiencesPtr experiences = stage->getExperiences( );
	Vector vec = Vector( getDirX( ), 0.0, getDirZ( ) ).normalize( );
	Vector pos = getPos( ) + vec; 
	FrameworkPtr fw = Framework::getInstance( );
	int exp_num = ( int )fw->getSettingValue( "ENEMY_CYCLOPS_POP_EXP" );
	experiences->create( pos , exp_num );
}