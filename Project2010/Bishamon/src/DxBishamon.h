//	******************************* //
//
//	- DxBishamon.h -	ver 1.00
//
//		Created by EIKI` <http://illucalab.com>
//
//	******************************* //
//
//	BISHAMON�G�t�F�N�g
//
//	CDxBMEffect - �G�t�F�N�g�{�́B����ȓ���͌p�����Ďg�p�\�B
//	CDxBishamonLayer - �G�t�F�N�g�̃��C���[�B�D���Ȑ��̃��C���[��p���ĕʁX�ɕ`��\�B
//	CDxBishamon - �Ǘ��B����̃C���X�^���X�����O�ō��B
//
//	# DxLib 3.06d�ȍ~�̂ݑΉ�
//

#pragma warning(disable:4244)

#include <vector>

class BMManager;
class BMEffect;
struct IDirect3DDevice9;
typedef struct IDirect3DDevice9 *LPDIRECT3DDEVICE9, *PDIRECT3DDEVICE9;

const int SCREEN_XSIZE = 640;					// ��ʂ̉��T�C�Y
const int SCREEN_YSIZE = 480;					// ��ʂ̏c�T�C�Y
const int DXBMEFFECT_MAX = 200;					// �G�t�F�N�g�̍ő�\����

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// �G�t�F�N�g�{��

class CDxBMEffect {
 protected:
	double m_x,m_y;								// �G�t�F�N�g�̖{���̍��W
	float m_Scale;								// �X�P�[��
	int m_Life;									// �����i-1�Ŗ����j
	BMManager *m_Manager;						// �}�l�[�W���Q��
	BMEffect *m_Effect;							// �G�t�F�N�g����

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
// �G�t�F�N�g���C���[

class CDxBishamonLayer {
 private:
	std::vector<CDxBMEffect *> m_BMObject;		// �o�^�����G�t�F�N�g���X�g
	BMManager *m_Manager;						// �}�l�[�W���Q��

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
// �G�t�F�N�g�Ǘ��C���X�^���X

class CDxBishamon {
 private:
	BMManager *m_Manager;						// �}�l�[�W������
	LPDIRECT3DDEVICE9 m_Device;					// �f�o�C�X�Q��

	std::vector<CDxBishamonLayer *> m_Layer;	// ���C���[
	std::vector<BMEffect *> m_EffectList;		// �G�t�F�N�g�̃��X�g

	float m_EyeLevel;							// ���_�̈ʒu

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