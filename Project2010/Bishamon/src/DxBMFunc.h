//	******************************* //
//
//	- DxBishamon.h -
//
//	******************************* //
//
//	DX���C�u�����̂悤�Ɋ֐��𒼐ڌĂт����l�i�����̓N���X���悭������Ȃ��l�j�́A
//	�����DxBMFunc.cpp���v���W�F�N�g�ɓ���ĉ������B
//

#include "DxBishamon.h"

// �K���ĂԊ֐�
extern	int				DxBM_Init( unsigned int LayerMax = 1, const char *Path = "Effects/" );		// Bishamon������������
extern	int				DxBM_End();															// Bishamon���I������

// �G�t�F�N�g�o�^�֐�
extern	int				AddBMEffect( unsigned int Layer, const char *in_Name, float x, float y, float z, float in_Scale = 1.0, int in_Life = -1 );
																			// �G�t�F�N�g��ǉ�����
inline	int				AddBMEffect( const char *in_Name, float x, float y, float z, float in_Scale = 1.0, int in_Life = -1 ) { return AddBMEffect( 0, in_Name, x, y, z, in_Scale, in_Life ); }
extern	int				UpdateBMEffect(double WorldX=0,double WorldY=0);	// �G�t�F�N�g�S�Ẵt���[����i�߂�
extern	int				DrawBMEffect(float eye_x,	float eye_y,	float eye_z, float focus_x,  float focus_y,	float focus_z,unsigned int Layer=0);					// �w�肳�ꂽ���C���[�̃G�t�F�N�g��`�悷��

// ���̑�
extern	void			RegistBMEffect(const char *in_Name);				// �g�p����G�t�F�N�g��o�^����
extern	void			SetBMEyeLevel(float in_level);						// �G�t�F�N�g�̌X����ύX����i0=�^������j
extern	void			SetUsePNGTexture(bool Flag);						// �G�t�F�N�g�̌X����ύX����i0=�^������j
extern	unsigned int	GetBMLength(unsigned int Layer);					// �G�t�F�N�g�̑�����Ԃ�