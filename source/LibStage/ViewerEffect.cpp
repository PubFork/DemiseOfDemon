#include "ViewerEffect.h"
#include "Effect.h"
#include "DxBMFunc.h"
#include "Camera.h"
#include "DxLib.h"
#include "Stage.h"
#include "Effect.h"

static const char * EFFECT_NAME[ Effect::MAX_EFFECT ] = {
	"bmvol1_1_crystal",
	"bmvol1_1_cure",
	"bmvol1_1_fire",
	"bmvol1_1_ice",
	"bmvol1_1_jewel",
	"bmvol1_1_meteo",
	"bmvol1_1_paper",
	"bmvol1_1_rock",
	"bmvol1_1_shogeki",
	"bmvol1_1_thunder",
	"bmvol1_1_tubo",
	"bmvol1_2_digicore",
	"bmvol1_2_earthnet_01",
	"bmvol1_2_earthnet_02",
	"bmvol1_2_firetree_01",
	"bmvol1_2_firetree_02",
	"bmvol1_2_firetree_03",
	"bmvol1_2_kusa",
	"bmvol1_2_lightning02",
	"bmvol1_2_lightning03",
	"bmvol1_2_lightning04",
	"bmvol1_2_mahojin_01",
	"bmvol1_2_mahojin_02",
	"bmvol1_2_mahojin_03",
	"bmvol1_2_pipe_01",
	"bmvol1_2_pipe_02",
	"bmvol1_2_pipe_03",
	"bmvol1_2_pipe_04",
	"bmvol1_2_water_01",
	"bmvol1_2_water_02",
	"bmvol1_2_watercolorfountain",
	"bmvol1_2_waterrock",
	"bmvol1_3_cubedance",
	"bmvol1_3_eye",
	"bmvol1_3_fireworks",
	"bmvol1_3_icecircle",
	"bmvol1_3_recovery",
	"bmvol1_3_revival",
	"bmvol1_3_slash",
	"bmvol1_3_stardance",
	"BMvol2_all",
	"BMvol2_hit01",
	"BMvol2_hit02",
	"BMvol2_hit03",
	"BMvol2_hit04",
	"BMvol2_hit05",
	"BMvol2_hit06",
	"BMvol2_hit07",
	"BMvol2_hit08",
	"BMvol2_hit09",
	"BMvol2_hit10",
	"BMvol3_chant",
	"BMvol3_dark",
	"BMvol3_fire",
	"BMvol3_ice",
	"BMvol3_light",
	"BMvol3_magic",
	"BMvol3_poison",
	"BMvol3_rock",
	"BMvol3_warp",
	"BMvol3_water",
	"BMvol3_wind",
	"bomb_manga_ks01",
	"e_hitenemy",
	"energywave_ss01",
	"gem_always_ks01",
	"hit09_bomb_ks02",
	"model_coin_ks02",
	"reference_red_rose_ks02",
	"rose_always_ks02",
	"smoke_ks01",
	"testblue",
};

ViewerEffect::ViewerEffect( ) {
	_init = true;

	for ( int i = 0; i < Effect::MAX_EFFECT; i++ ) {
		Effect::EFFECT tag = ( Effect::EFFECT )i;
		_loadfile[ tag ] = false;
	}
}

ViewerEffect::~ViewerEffect( ) {
	if ( !_init ) {
		DxBM_End( );
	}
}

void ViewerEffect::draw( ) {
	if ( _init ) {
		_init = false;
		initEffects( );
	}

	addEffects( );
	updateEffects( );
}

void ViewerEffect::initEffects( ) {
	StagePtr stage = Stage::getTask( );
	EffectPtr effect = stage->getEffect( );
	DxBM_Init( effect->getMaxNum( ), "resource/effect/" );
	SetUsePNGTexture( false );
}

void ViewerEffect::addEffects( ) {
	StagePtr stage = Stage::getTask( );
	EffectPtr effect = stage->getEffect( );
	int num = effect->getNum( );
	for ( int i = 0; i < num; i++ ) {
		Effect::DATA data = effect->getData( i );
		load( data.tag );
		float x = ( float )data.pos.x;
		float y = ( float )data.pos.y;
		float z = ( float )data.pos.z;
		AddBMEffect( EFFECT_NAME[ data.tag ], x, y, z, data.scale, data.time );
	}
	effect->clear( );
}

void ViewerEffect::updateEffects( ) {
	StagePtr stage = Stage::getTask( );
	CameraPtr camera = stage->getCamera( );

	Vector eye = camera->getEye( );
	float eye_x = eye.x;
	float eye_y = eye.y;
	float eye_z = eye.z;
	Vector focus = camera->getFocus( );
	float focus_x = focus.x;
	float focus_y = focus.y;
	float focus_z = focus.z;
	UpdateBMEffect( );
	DrawBMEffect( eye_x, eye_y, eye_z, focus_x, focus_y, focus_z );
	RefreshDxLibDirect3DSetting( );
}

void ViewerEffect::load( int index ) {
	Effect::EFFECT tag = ( Effect::EFFECT )index;
	if ( _loadfile[ tag ] ) {
		return;
	}
	_loadfile[ tag ] = true;
	RegistBMEffect( EFFECT_NAME[ tag ] );
}