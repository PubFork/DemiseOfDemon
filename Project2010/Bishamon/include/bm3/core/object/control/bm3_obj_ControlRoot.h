#ifndef BM3_SDK_INC_BM3_CORE_OBJECT_CONTROL_BM3_OBJ_CONTROLROOT_H
#define BM3_SDK_INC_BM3_CORE_OBJECT_CONTROL_BM3_OBJ_CONTROLROOT_H

#include "bm3_obj_ControlBase.h"
#include "bm3/core/object/bm3_obj_RootNodeBase.h"
#include "bm3/core/resource/bm3_res_Root.h"
#include "bm3/core/policy/Life/bm3_policy_Life_Infinity.h"
#include "bm3/core/policy/ChildGenerateTiming/bm3_policy_ChildGenerateTiming_Nothing.h"
#include <ml/type/ml_basic.h>
#include <ml/math/ml_random.h>
#include "bm3/core/bm3_VertexInfo.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

class InitializeInfo;
class DrawInfo;

namespace fileformat{
namespace bmb{
struct NodeHeader;
}	// namespace bmb
}	// namespace fileformat

namespace obj{
namespace control{

template<typename ResourceType = res::Root>
class Root : public Base<ResourceType, RootNodeBase, policy::Life_Infinity, policy::ChildGenerateTiming_Nothing>{

public:
	typedef	Root			ThisType_;
	typedef	ResourceType	ResourceType_;

	Root();
	~Root();

#if defined(BM3_TARGET_IDE)
	bool	Initialize(const ResourceType &res);
#endif
	/*!
		@brief	�e�m�[�h�̃������m�ۂƏ��������s��
		@param	res_root	���[�g�m�[�h�̃��\�[�X
		@param	header		bmb�̃w�b�_
		@param	gen_scale	�������̊���
		@return	�������m�ۂɎ��s�����false��Ԃ�
	*/
	bool	Initialize(const ResourceType &res, const fileformat::bmb::Header *header, const float gen_scale);
	bool	InitializePrimitive(InitializeInfo &info);
	/*!
		@brief	�e�m�[�h�ł��łɏ������ł�����̂͂��Ă���
		@param	init_info	InitInfo
		@return	�������̐���
		@detail	����}�X�p�[�e�B�N���̏������������m�ۂ���B
				�����ŏ���������Ȃ��ꍇ�͕`�撼�O�Ƀ��������m�ۂ���B
	*/
	bool	Initialize(InitInfo &init_info);

	void	Finalize();

	/*!
		@brief	�G�t�F�N�g���I���������ǂ�����Ԃ�
		@return	�G�t�F�N�g���I���������ǂ���
	*/
	bool	IsExpired() const;

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

	int		NodeCount();
	void	NodeVertexInfo(int emitter_id, VertexInfo &info, int vertex_buffer_id = 0);

	int		SwitchEffect(const ResourceType &res, const fileformat::bmb::Header *header);

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
	void	Reset(ResetInfo &reset_info);

	void	DestroyResource();
	void	RestoreResource(RestoreInfo &info);

	/*!
		@brief	�G�t�F�N�g�̃��[�g�Ƀ}�g���b�N�X��ݒ肷��B�p���֌W���l�������B
		@param	matrix	�ݒ肷��}�g���b�N�X
		@return	�Ȃ�
		@detail	�X�V����������O�܂łɐݒ肵�Ă����K�v������B
				���[�g�ɐݒ肷��̂Ōp���ݒ肪���f�����B
	*/
	void	SetMatrix(const ml::matrix44 &matrix);
	/*!
		@brief	���ׂẴm�[�h�Ŏg�p���钸�_�o�b�t�@�T�C�Y�̑��a��Ԃ�
		@return ���ׂẴm�[�h�Ŏg�p���钸�_�o�b�t�@�T�C�Y�̑��a
	*/
	int		TotalVertexBufferSize() const ;
	int		MaxVertexBufferSize() const;
	int		MaxPrimCount() const;

	/*!
		@brief	�G�t�F�N�g�ɃJ���[���|�����킹��
		@param	color_scale	�|�����킹��J���[
		@return	�Ȃ�
		@detail	���_�J���[�Ɋ|�����킳���B
				���_�쐬�܂łɐݒ肵�Ă����K�v������B
	*/
	void					SetColorScale(const ColorType &color_scale);
	const ColorType &		ColorScale();

	/*!
		@brief	�G�t�F�N�g�Ɉʒu�𑫂����킹��
		@param	offset_position	�������킹��ʒu
		@return	�Ȃ�
		@detail	�e�ȃ}�g���b�N�X�Ȃǂ̉e���͎󂯂��A�P���ɒ��_���W�ɑ������킹�邾���ő��ɂ͉������Ȃ��B
				���_�쐬�܂łɐݒ肵�Ă����K�v������B
	*/
	void					SetOffsetPosition(const ml::vector3d &offset_position);
	const ml::vector3d &	OffsetPosition();

	void	RegistryCreatePrimitiveCallback(void (*callback)(internal::CreatePrimitiveCallbackInfo &info, void *data), void *data);
	void	RegistryDestoryPrimitiveCallback(void (*callback)(internal::DestoryPrimitiveCallbackInfo &info, void *data), void *data);
	
#ifdef BM3_PLATFORM_playstation3
	template<typename UpdateInfo>
	void	RawSpuUpdate(UpdateInfo &info);
	template<typename UpdateInfo>
	void	RawSpuUpdateField(UpdateInfo &info);
	void	RawSpuMakeVertex(DrawInfo &info);
#endif

#ifdef BM3_PLATFORM_xbox360
	BOOL IsVertexBufferBusy();
#endif

private:
	void	MakeVertexChild(DrawInfo &info);
#if defined(BM3_TARGET_IDE)
	void MakeEmitterInfo(const ResourceType &res);
#endif

};

}	// namespace control
}	// namespace obj
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#include "bm3_obj_ControlRoot_inl.h"

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_OBJECT_CONTROL_BM3_OBJ_CONTROLROOT_H
