#include "DxLib.h"
#include "DxBishamon.h"
#include <assert.h>


//	************************************************************************************************************************ //
//	- Functions of DxBishamon -
//	************************************************************************************************************************ //

// インスタンス
static CDxBishamon *HIDDEN_DXBM = NULL;

// デバイスロスト
namespace DxBM_DeviceLost {
void LostCallbackFunction( void *Data ) {
	if (HIDDEN_DXBM != NULL) {
		HIDDEN_DXBM->DeviceLost();
	}
}
void RestoreCallbackFunction( void *Data ) {
	if (HIDDEN_DXBM != NULL) {
		HIDDEN_DXBM->DeviceRestore();
	}
}
};

int DxBM_Init( unsigned int LayerMax, const char *Path ) {
	try {
		HIDDEN_DXBM = new CDxBishamon( LayerMax, Path );
		SetGraphicsDeviceLostCallbackFunction(DxBM_DeviceLost::LostCallbackFunction,NULL);
		SetGraphicsDeviceRestoreCallbackFunction(DxBM_DeviceLost::RestoreCallbackFunction,NULL);
		return 0;
	} catch( ... ) {
		return -1;
	}
}

int DxBM_End() {
	try {
		delete HIDDEN_DXBM;
		return 0;
	} catch( ... ) {
		return -1;
	}
}

// その他関数

int AddBMEffect( unsigned int Layer, const char *in_Name, float x, float y, float z, float in_Scale ,int in_Life ) {
	assert( HIDDEN_DXBM != NULL );
	return HIDDEN_DXBM->AddEffect( Layer, in_Name, x, y, z, in_Scale, in_Life );
}

int UpdateBMEffect(double WorldX,double WorldY) {
	assert( HIDDEN_DXBM != NULL );
	return HIDDEN_DXBM->Update(WorldX,WorldY);
}
int DrawBMEffect(
	float eye_x,	float eye_y,	float eye_z,
	float focus_x,  float focus_y,	float focus_z,
	unsigned int Layer) {
	assert( HIDDEN_DXBM != NULL );
	return HIDDEN_DXBM->Draw(eye_x, eye_y, eye_z, focus_x, focus_y, focus_z, Layer);
}

unsigned int GetBMLength(unsigned int Layer) {
	assert( HIDDEN_DXBM != NULL );
	return HIDDEN_DXBM->GetLength(Layer);
}

void SetBMEyeLevel(float in_level) {
	assert( HIDDEN_DXBM != NULL );
	return HIDDEN_DXBM->SetEyeLevel(in_level);
}

void RegistBMEffect(const char *in_Name) {
	assert( HIDDEN_DXBM != NULL );
	return HIDDEN_DXBM->RegistEffect(in_Name);
}

void SetUsePNGTexture(bool Flag) {
	assert( HIDDEN_DXBM != NULL );
	return HIDDEN_DXBM->SetUsePNGTexture(Flag);
}