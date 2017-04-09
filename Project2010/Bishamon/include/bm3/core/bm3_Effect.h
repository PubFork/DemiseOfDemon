#ifndef BM3_SDK_INC_BM3_CORE_BM3_EFFECT_H
#define BM3_SDK_INC_BM3_CORE_BM3_EFFECT_H

#include "object/control/bm3_obj_ControlRoot.h"
#include "object/bm3_obj_RootNodeBase.h"
#include "bm3_CoreType.h"
#include "bm3_VertexInfo.h"
#include "bm3_RestoreInfo.h"


namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

typedef	obj::internal::CreatePrimitiveCallbackInfo	CreatePrimitiveCallbackInfo;
typedef	obj::internal::DestoryPrimitiveCallbackInfo	DestoryPrimitiveCallbackInfo;

class DrawInfo;

namespace fileformat{
namespace bmb{
struct NodeHeader;
}	// namespace bmb
}	// namespace fileformat

/// @brief	�G�t�F�N�g�N���X
template<typename ResRootType = res::Root>
class Effect{

public:
	Effect();
	~Effect();

#if defined(BM3_TARGET_IDE)
	bool	Initialize(const ResRootType &res_root);
#endif
	/*!
		@brief	�e�m�[�h�̃������m�ۂƏ��������s��
		@param	res_root	���[�g�m�[�h�̃��\�[�X
		@param	header		bmb�̃w�b�_
		@param	gen_scale	�������̊���
		@return	�������m�ۂɎ��s�����false��Ԃ�
	*/
	bool	Initialize(const ResRootType &res_root, const fileformat::bmb::Header *header, const float gen_scale);
	/*!
		@brief	�e�m�[�h�ł��łɏ������ł�����̂͂��Ă���
		@param	init_info	InitInfo
		@return	�������̐���
		@detail	����}�X�p�[�e�B�N���̏������������m�ۂ���B
				�����ŏ���������Ȃ��ꍇ�͕`�撼�O�Ƀ��������m�ۂ���B
	*/
	bool	Initialize(InitInfo &init_info);

	/*!
		@brief	�G�t�F�N�g���X�V����
		@param	info	UpdateInfo
		@return	�Ȃ�
	*/
	template<typename UpdateInfo>
	void	Update(UpdateInfo &info);
	/*!
		@brief	�G�t�F�N�g�̒��_���쐬����
		@param	info	DrawInfo
		@return	�Ȃ�
		@detail	�w�肳�ꂽ���_�o�b�t�@�ɏ������݂��s��
	*/
	void	MakeVertex(DrawInfo &info);
	/*!
		@brief	�G�t�F�N�g��`�悷��
		@param	info	DrawInfo
		@return	�Ȃ�
	*/
	void	Draw(DrawInfo &info);

	/*!
		@brief	�g�p���钸�_�o�b�t�@�̃T�C�Y��Ԃ�
		@return	�Ȃ�
		@detail	��������p�[�e�B�N���̐��ɂ���Ė��t���[���T�C�Y���ύX������B
				�X�V������������ɌĂяo���K�v������B
	*/
	int		VertexBufferSize() const;
	/*!
		@brief	�G�t�F�N�g�̃��[�g�Ƀ}�g���b�N�X��ݒ肷��B�p���֌W���l�������B
		@param	matrix	�ݒ肷��}�g���b�N�X
		@return	�Ȃ�
		@detail	�X�V����������O�܂łɐݒ肵�Ă����K�v������B
				���[�g�ɐݒ肷��̂Ōp���ݒ肪���f�����B
	*/
	void	SetMatrix(const ml::matrix44 &matrix);
	/*!
		@brief	�G�t�F�N�g�ɃJ���[���|�����킹��
		@param	color_scale	�|�����킹��J���[
		@return	�Ȃ�
		@detail	���_�J���[�Ɋ|�����킳���B
				���_�쐬�܂łɐݒ肵�Ă����K�v������B
	*/
	void	SetColorScale(const ColorType &color_scale);
	/*!
		@brief	�G�t�F�N�g�Ɉʒu�𑫂����킹��
		@param	offset_position	�������킹��ʒu
		@return	�Ȃ�
		@detail	�e�ȃ}�g���b�N�X�Ȃǂ̉e���͎󂯂��A�P���ɒ��_���W�ɑ������킹�邾���ő��ɂ͉������Ȃ��B
				���_�쐬�܂łɐݒ肵�Ă����K�v������B
	*/
	void	SetOffsetPosition(const ml::vector3d &offset_position);

	/*!
		@brief	�G�t�F�N�g���I���������ǂ�����Ԃ�
		@return	�G�t�F�N�g���I���������ǂ���
	*/
	bool	IsExpired() const;
	/*!
		@brief	�G�t�F�N�g�̐����������I�ɂȂ����B
		@return	�Ȃ��B
		@detail	���R�ɏ������悤�Ɍ�������ʂ�����̂ŗ]�C���c���ꍇ�Ɏg�p�ł���B
	*/
	void	RequestRetire();

	/*!
		@brief	�G�t�F�N�g��������Ԃɖ߂��B
		@param	info	ResetInfo
		@return	�Ȃ��B
		@detail	�������̍Ċm�ۂ͍s�킸�A������Ԃɖ߂��B
	*/
	void	Reset(ResetInfo &info);


	void	RegistryCreatePrimitiveCallback(void (*callback)(CreatePrimitiveCallbackInfo &info, void *data), void *data);
	void	RegistryDestoryPrimitiveCallback(void (*callback)(DestoryPrimitiveCallbackInfo &info, void *data), void *data);

	int		NodeCount();
	void	NodeVertexInfo(int emitter_id, VertexInfo &info, int vertex_buffer_id=0);

	int		SwitchEffect(const void *bmb_image);

	// random�����l�������ۂ̍ő咸�_�o�b�t�@�T�C�Y
	int		MaxVertexBufferSize() const;

	
	void	DestroyResource();
	void	RestoreResource(RestoreInfo &info);
	
#ifdef BM3_PLATFORM_playstation3
	template<typename UpdateInfo>
	void	RawSpuUpdate(UpdateInfo &info);
	template<typename UpdateInfo>
	void	RawSpuUpdateField(UpdateInfo &info);
	void	RawSpuMakeVertex(DrawInfo &info);
#endif

#ifdef BM3_PLATFORM_xbox360
	/*!
		@brief	���_�o�b�t�@�̎g�p�󋵂�₢���킹��
		@return	���_���g�p����Ă��邩�ǂ���
		@detail	�ǂꂩ�P�̃}�X�p�[�e�B�N���̒��_�o�b�t�@���g�p����Ă���ꍇ��TRUE
				���ׂẴ}�X�p�[�e�B�N���̒��_�o�b�t�@���g�p����Ă��Ȃ����FALSE��Ԃ�
	*/
	BOOL IsVertexBufferBusy();
#endif

private:
	obj::control::Root<ResRootType>	obj_root_;
};

BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#include "bm3_Effect_inl.h"

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_BM3_EFFECT_H
