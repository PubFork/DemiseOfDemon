//	******************************* //
//
//	- DxBishamon.h -	ver 1.00
//
//		Created by EIKI` <http://illucalab.com>
//
//	******************************* //
//
//	BISHAMONエフェクト
//
//	CDxBMEffect - エフェクト本体。特殊な動作は継承して使用可能。
//	CDxBishamonLayer - エフェクトのレイヤー。好きな数のレイヤーを用いて別々に描画可能。
//	CDxBishamon - 管理。これのインスタンスを自前で作る。
//
//	# DxLib 3.06d以降のみ対応
//

#pragma warning(disable:4244)

#include <vector>

class BMManager;
class BMEffect;
struct IDirect3DDevice9;
typedef struct IDirect3DDevice9 *LPDIRECT3DDEVICE9, *PDIRECT3DDEVICE9;

const int SCREEN_XSIZE = 640;					// 画面の横サイズ
const int SCREEN_YSIZE = 480;					// 画面の縦サイズ
const int DXBMEFFECT_MAX = 200;					// エフェクトの最大表示数

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// エフェクト本体

class CDxBMEffect {
 protected:
	double m_x,m_y;								// エフェクトの本来の座標
	float m_Scale;								// スケール
	int m_Life;									// 寿命（-1で無効）
	BMManager *m_Manager;						// マネージャ参照
	BMEffect *m_Effect;							// エフェクト実体

 public:
	CDxBMEffect( BMManager *in_Manager, BMEffect *in_Effect, float x, float y, float z, float in_Scale, int in_Life );
	virtual ~CDxBMEffect();

	BMEffect* GetBMEffect() { return m_Effect; };
	virtual int Update(double WorldX,double WorldY);
	virtual int Draw();
	
	void ReleaseResource();
	void RestoreResource(LPDIRECT3DDEVICE9 device);
};

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// エフェクトレイヤー

class CDxBishamonLayer {
 private:
	std::vector<CDxBMEffect *> m_BMObject;		// 登録したエフェクトリスト
	BMManager *m_Manager;						// マネージャ参照

	CDxBishamonLayer(const CDxBishamonLayer&);
	CDxBishamonLayer& operator=(const CDxBishamonLayer&);

 public:
	CDxBishamonLayer(BMManager *in_Manager);
	~CDxBishamonLayer();
	
	int AddEffect( const char *in_Name, float x,float y, float z, float in_Scale, int in_Life );
	int Update(double WorldX,double WorldY);
	int Draw();

	void ReleaseResource();
	void RestoreResource(LPDIRECT3DDEVICE9 device);

	int GetSize() { return m_BMObject.size(); }
};

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// エフェクト管理インスタンス

class CDxBishamon {
 private:
	BMManager *m_Manager;						// マネージャ実体
	LPDIRECT3DDEVICE9 m_Device;					// デバイス参照

	std::vector<CDxBishamonLayer *> m_Layer;	// レイヤー
	std::vector<BMEffect *> m_EffectList;		// エフェクトのリスト

	float m_EyeLevel;							// 視点の位置

	float _screen_width; 
	float _screen_height;

	CDxBishamon(const CDxBishamon&);
	CDxBishamon& operator=(const CDxBishamon&);

 public:
	CDxBishamon( unsigned int LayerMax = 1, const char *Path="Effects/" );
	~CDxBishamon();

	int AddEffect( unsigned int Layer, const char *in_Name, float x, float y, float z, float in_Scale = 1.0, int in_Life = -1 );
	int AddEffect( const char *in_Name, float x, float y, float z, float in_Scale = 1.0, int in_Life = -1 ) { return AddEffect( 0, in_Name, x, y, z, in_Scale, in_Life ); };
	
	int Update(double WorldX = 0,double WorldY = 0);
	int Draw(float eye_x, float eye_y,float eye_z, float focus_x, float focus_y, float focus_z,  unsigned int Layer = 0);

	unsigned int GetLength(unsigned int Layer = 0) { return m_Layer[Layer]->GetSize(); }
	void RegistEffect(const char *in_Name);
	void SetEyeLevel(float in_level = 0) { m_EyeLevel = in_level; }
	void SetUsePNGTexture(bool Flag);

	// Functions for DeviceLost 
	void ReleaseResource();
	void RestoreResource();
	void DeviceLost();
	void DeviceRestore();
};