//	******************************* //
//
//	- DxBishamon.h -
//
//	******************************* //
//
//	DXライブラリのように関数を直接呼びたい人（或いはクラスがよく分からない人）は、
//	これとDxBMFunc.cppをプロジェクトに入れて下さい。
//

#include "DxBishamon.h"

// 必ず呼ぶ関数
extern	int				DxBM_Init( unsigned int LayerMax = 1, const char *Path = "Effects/" );		// Bishamonを初期化する
extern	int				DxBM_End();															// Bishamonを終了する

// エフェクト登録関数
extern	int				AddBMEffect( unsigned int Layer, const char *in_Name, float x, float y, float z, float in_Scale = 1.0, int in_Life = -1 );
																			// エフェクトを追加する
inline	int				AddBMEffect( const char *in_Name, float x, float y, float z, float in_Scale = 1.0, int in_Life = -1 ) { return AddBMEffect( 0, in_Name, x, y, z, in_Scale, in_Life ); }
extern	int				UpdateBMEffect(double WorldX=0,double WorldY=0);	// エフェクト全てのフレームを進める
extern	int				DrawBMEffect(float eye_x,	float eye_y,	float eye_z, float focus_x,  float focus_y,	float focus_z,unsigned int Layer=0);					// 指定されたレイヤーのエフェクトを描画する

// その他
extern	void			RegistBMEffect(const char *in_Name);				// 使用するエフェクトを登録する
extern	void			SetBMEyeLevel(float in_level);						// エフェクトの傾きを変更する（0=真横から）
extern	void			SetUsePNGTexture(bool Flag);						// エフェクトの傾きを変更する（0=真横から）
extern	unsigned int	GetBMLength(unsigned int Layer);					// エフェクトの総数を返す