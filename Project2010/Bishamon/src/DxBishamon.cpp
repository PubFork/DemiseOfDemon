#include "DxLib.h"
#include "DxBishamon.h"
#include "BMManager.h"
#include <locale.h>

//	************************************************************************************************************************ //
//	- CDxBishamon -
//	************************************************************************************************************************ //

CDxBishamon::CDxBishamon( unsigned int LayerMax, const char *Path )  {
	m_Device = (LPDIRECT3DDEVICE9)GetUseDirect3DDevice9();		// デバイスの設定
	m_Manager = new BMManager(m_Device,Path);					// マネージャ作成、データフォルダの設定

	if (!LayerMax) LayerMax = 1;								// レイヤーは最低でも一つ
	for(unsigned int i=0;i<LayerMax;++i) {
		m_Layer.push_back( new CDxBishamonLayer(m_Manager) );	// レイヤーを挿入
	}
	setlocale(LC_ALL,"japanese");

	//画面サイズの取得
	//FrameworkPtr fw = Framework::getInstance( );
	//_screen_width = fw->getScreenWidth( );
	//_screen_height = fw->getScreenHeight( );
	RECT rect; 
	GetWindowCRect( &rect );
	_screen_width  = rect.right - rect.left;
	_screen_height = rect.bottom - rect.top;

	m_EyeLevel = 0.0f;											// 初期視点位置
}

CDxBishamon::~CDxBishamon() {
	for(unsigned int i=0;i<m_Layer.size();++i) {
		delete m_Layer[i];
	}
	for(unsigned int i=0;i<m_EffectList.size();++i) {
		m_Manager->ReleaseEffect(m_EffectList[i]);
	}

	m_Manager->Release();
}

int CDxBishamon::AddEffect( unsigned int Layer, const char *in_Name, float x, float y, float z , float in_Scale, int in_Life ) {
	if (Layer >= m_Layer.size()) return -1;
	m_Layer[Layer]->AddEffect( in_Name, x, y, z, in_Scale, in_Life );
	return 0;
}

int CDxBishamon::Update( double WorldX, double WorldY ) {
	m_Manager->ResetCurrentPointerFlag( );
	for ( unsigned int i = 0; i < m_Layer.size( ); ++i ) {
		m_Layer[ i ]->Update( WorldX, WorldY );
	}
	return 0;
}

/* エフェクトの描画 */
int CDxBishamon::Draw(
	float eye_x,	float eye_y,	float eye_z,
	float focus_x,  float focus_y,	float focus_z,
	unsigned int Layer) {
	if (Layer >= m_Layer.size()) return -1;

	RenderVertex();		// 強制描画
	// fvfを保持
	DWORD fvf = 0;
	m_Device->GetFVF(&fvf);

	// テクスチャを保持
	IDirect3DBaseTexture9 *texture = NULL;
	m_Device->GetTexture(0, &texture);

	// ビュー行列の設定
	//ml::matrix44 view;
	//ml::vector3d v3eye(0.0f,  m_EyeLevel, -1280, 0);
	//ml::vector3d v3at( 0.0f, -m_EyeLevel, +1280, 0);
	//ml::vector3d v3up(0.0f,1.0f,0.0f,0.0f);
	//view.lookat_r(v3eye,v3at,v3up);

	ml::matrix44 view;
	ml::vector3d v3eye(	eye_x, eye_y, eye_z, 0		);
	ml::vector3d v3at( focus_x,   ( float )focus_y,	( float )focus_z,  0	);
	ml::vector3d v3up(	0.0f,	1.0f,	0.0f,	0.0f	);
	view.lookat_l(	v3eye,	v3at,	v3up	);

	m_Manager->SetView( view );

	/* DxLib側のカメラ設定
	SetupCamera_Perspective( 8.0f * DX_PI_F / 180.0f );
	SetCameraNearFar( 10.0f, 200.0f );
	*/

	// 射影
	ml::matrix44 proj;
	proj.perspectivefov_l( ml::to_radian( 8.0f), _screen_width / _screen_height, 10.0f, 200.0f );
	m_Manager->SetProjection(proj);
//
//#ifndef USER_PERSPECTIVE
//	// 正射影
//	proj.orthographic(-SCREEN_XSIZE*0.5f, SCREEN_XSIZE*0.5f, -SCREEN_YSIZE*0.5f, SCREEN_YSIZE*0.5f, -10000, 10000);
//#else
//	// 透視投影
//	proj.perspectivefov_r(ml::to_radian(35.f), SCREEN_XSIZE/SCREEN_YSIZE, 1.0f, 10000.0f);
//#endif
//	m_Manager->SetProjection(proj);

	// view/projectionを設定した後でBeginして下さい
	m_Manager->Begin();
	m_Layer[Layer]->Draw();		// 描画
	m_Manager->End();
	
	// テクスチャとfvfを元に戻す
	if(texture) {
		m_Device->SetTexture(0, texture);
		texture->Release();
	}
	m_Device->SetFVF(fvf);
	return 0;
}

void CDxBishamon::RegistEffect(const char *in_Name) {
	m_EffectList.push_back( m_Manager->CreateEffect(in_Name) );
}

void CDxBishamon::SetUsePNGTexture(bool Flag) {
	m_Manager->SetUsePNGTexture(Flag);
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Functions for DeviceLost 
void CDxBishamon::ReleaseResource() {
	// 全てのエフェクトのリソースを解放
	for(unsigned int i=0;i<m_Layer.size();++i) m_Layer[i]->ReleaseResource();
}
void CDxBishamon::RestoreResource() {
	// 全てのエフェクトのリソースを解放
	for(unsigned int i=0;i<m_Layer.size();++i) m_Layer[i]->RestoreResource(m_Device);
}
void CDxBishamon::DeviceLost() {
	m_Manager->DeviceLost();
	ReleaseResource();
}
void CDxBishamon::DeviceRestore() {
	m_Manager->ResetDevice(m_Device);
	RestoreResource();
}

//	************************************************************************************************************************ //
//	- CDxBishamonLayer -
//	************************************************************************************************************************ //

CDxBishamonLayer::CDxBishamonLayer(BMManager *in_Manager):m_Manager(in_Manager) {
	m_BMObject.reserve( DXBMEFFECT_MAX );
}

CDxBishamonLayer::~CDxBishamonLayer() {
	for(unsigned int i=0;i<m_BMObject.size();++i) {
		delete m_BMObject[i];
	}
}

int CDxBishamonLayer::AddEffect( const char *in_Name, float x, float y, float z , float in_Scale, int in_Life ) {
	if (GetSize()>=DXBMEFFECT_MAX) return 0;
	WCHAR wStr[260];
	size_t wLen = 0;
	mbstowcs_s(&wLen, wStr, 260, in_Name, _TRUNCATE);
	m_BMObject.push_back( new CDxBMEffect( m_Manager, m_Manager->CreateEffect( wStr ), x, y, z, in_Scale, in_Life ) );
	return 0;
}

int CDxBishamonLayer::Update( double WorldX, double WorldY ) {
	std::vector< CDxBMEffect * >::iterator it = m_BMObject.begin( );
	while ( it != m_BMObject.end( ) ) {
		if ( ( *it )->Update( WorldX, WorldY ) ) {
			delete ( *it );
			it = m_BMObject.erase( it );
		} else {
			++it;
		}
	}
	return 0;
}

int CDxBishamonLayer::Draw() {
	for(unsigned int i=0;i<m_BMObject.size();++i) {
		m_BMObject[i]->Draw();
	}
	return 0;
}

void CDxBishamonLayer::ReleaseResource() {
	for(unsigned int i=0;i<m_BMObject.size();++i) m_BMObject[i]->ReleaseResource();
}
void CDxBishamonLayer::RestoreResource(LPDIRECT3DDEVICE9 device) {
	for(unsigned int i=0;i<m_BMObject.size();++i) m_BMObject[i]->RestoreResource(device);
}

//	************************************************************************************************************************ //
//	- CDxBMEffect -
//	************************************************************************************************************************ //

CDxBMEffect::CDxBMEffect(BMManager *in_Manager,BMEffect *in_Effect, float x, float y, float z, float in_Scale,int in_Life):
						  m_Manager(in_Manager),m_Effect(in_Effect),m_Scale(in_Scale),m_Life(in_Life)
{
	/*// 画面中心を原点とする座標変換
	m_x = SCREEN_XSIZE/2.0 - in_x;
	m_y = SCREEN_YSIZE/2.0 - in_y;

	ml::matrix44 m;
	m.set_scale( m_Scale, m_Scale, m_Scale );
	m.mul_translate((float)m_x,(float)(m_y), 0);
	m_Effect->SetMatrix(m);
	*/

	ml::matrix44 mat;
	mat.set_scale( m_Scale, m_Scale, m_Scale );		// スケール
	mat.mul_translate( x, y, z );	// 座標

	m_Effect->SetMatrix( mat );

}

CDxBMEffect::~CDxBMEffect() {
	m_Manager->ReleaseEffect(m_Effect);
}

int CDxBMEffect::Update( double WorldX, double WorldY ) {
	if ( WorldX != 0 && WorldY != 0 ) {
		ml::matrix44 m;
		m.set_scale( m_Scale, m_Scale, m_Scale );
		m.mul_translate( ( float )( m_x - WorldX ), ( float )( m_y - WorldY ), 0 );
		m_Effect->SetMatrix( m );
	}
	if (m_Life>=0) {
		if (--m_Life==0) return 1;	// 寿命
	}
	m_Effect->Update();
	return m_Effect->IsExpired();	// 終了時は1が返る
}

int CDxBMEffect::Draw() {
	m_Manager->DrawEffect(m_Effect);
	return 0;
}

void CDxBMEffect::ReleaseResource() {
	m_Effect->ReleaseResource();
}
void CDxBMEffect::RestoreResource(LPDIRECT3DDEVICE9 device) {
	m_Effect->RestoreResource(device);
}