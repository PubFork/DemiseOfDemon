#ifndef BM3_SDK_INC_BM3_CORE_TEXTURE_BM3_POLICY_TEXTURE_CROP_H
#define BM3_SDK_INC_BM3_CORE_TEXTURE_BM3_POLICY_TEXTURE_CROP_H

#include "../../bm3_Const.h"
#include "../../bm3_CoreType.h"
#include "../../bm3_CoreUtility.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

namespace policy{

namespace impl{


/// @brief Texture_Crop�N���X�B�e�N�X�`���N���b�v�̏������s��
class Texture_Crop{

public:
	/*!
		�N���b�v�؂�ւ��p�̃J�E���^�Ƃh�c������������
		@param	res		�G�~�b�^�[�̃��\�[�X�̎Q��
		@param	random	ml::random�N���X
		@param	crop	�g�p���Ȃ�
		@param	crop_id	�N���b�v�̂h�c
		@param	counter	�N���b�v�؂�ւ��̃J�E���^
		@param	lt		���C���^�C�v
		@return	�Ȃ�
	*/
	template<typename ResourceType>
	static BM3_FORCEINLINE void	Initialize(const ResourceType &res, ml::random &random, TextureCrop &/*crop*/, int &crop_id, float &counter, TextureLayer::Type lt){

		if(Texture_Layer_CropRandom(res, lt)){
#ifdef BM3_TARGET_IDE
			Initialize(crop_id, counter, Texture_Layer_AnimationSpeed(res, lt), random, Texture_Layer_TextureCropAnimationCount(res, lt));
#else
			Initialize(crop_id, counter, Texture_Layer_AnimationSpeed(res, lt), random, Texture_Layer_TextureCropAnimationCount(res, lt));
#endif
		}
		else{
			Initialize(crop_id, counter, Texture_Layer_AnimationSpeed(res, lt));
		}
	}

	/*!
		���C���[�P�̃N���b�v�؂�ւ��p�̃J�E���^�Ƃh�c������������
		@param	res		�G�~�b�^�[�̃��\�[�X�̎Q��
		@param	random	ml::random�N���X
		@param	crop	�g�p���Ȃ�
		@param	crop_id	�N���b�v�̂h�c
		@param	counter	�N���b�v�؂�ւ��̃J�E���^
		@param	type	�A�j���[�V�����̃^�C�v
		@return	�Ȃ�
	*/
	template<typename ResourceType>
	static BM3_FORCEINLINE void	InitializeLayer1(const ResourceType &res, ml::random &/*random*/, TextureCrop &/*crop*/, int &crop_id, float &counter, TextureAnimationTypeConst type){

		if(type == TextureAnimationTypeConst_Crop)	// �����I�ɂ���if���͂Ȃ��Ȃ�
		{
			Initialize(crop_id, counter, res.Texture_Layer1_AnimationSpeed());
		}
	}
	
	/*!
		�N���b�v�؂�ւ��p�̃J�E���g���X�V����
		@param	update_info	UpdateInfo�N���X
		@param	res			�G�~�b�^�[�̃��\�[�X�̎Q��
		@param	crop		�g�p���Ȃ�
		@param	crop_id		�N���b�v�ԍ�
		@param	counter		�؂�ւ��J�E���^
		@param	type		�A�j���[�V�����̃^�C�v
		@param	lt			���C���^�C�v
		@return	�Ȃ�
	*/
	template<typename ResourceType, typename UpdateInfo>
	static BM3_FORCEINLINE void	Update(UpdateInfo &update_info, const ResourceType &res, TextureCrop &/*crop*/, int &crop_id, float &counter, ml::random &random, TextureAnimationTypeConst /*type*/, TextureLayer::Type lt){

		if(Texture_Layer_CropRandom(res, lt)){
#ifdef BM3_TARGET_IDE
			Update(update_info, crop_id, counter, Texture_Layer_AnimationSpeed(res, lt), random, Texture_Layer_TextureCropAnimationCount(res, lt));
#else
			Update(update_info, crop_id, counter, Texture_Layer_AnimationSpeed(res, lt), random, Texture_Layer_TextureCropAnimationCount(res, lt));
#endif
		}
		else
		{
			Update(update_info, crop_id, counter, Texture_Layer_AnimationSpeed(res, lt));
		}
	}
	
	/*!
		���C���[�P�̃N���b�v�؂�ւ��p�̃J�E���g���X�V����
		@param	update_info	UpdateInfo�N���X
		@param	res			�G�~�b�^�[�̃��\�[�X�̎Q��
		@param	crop		�g�p���Ȃ�
		@param	crop_id		�N���b�v�ԍ�
		@param	counter		�؂�ւ��J�E���^
		@param	type		�A�j���[�V�����̃^�C�v
		@return	�Ȃ�
	*/
	template<typename ResourceType, typename UpdateInfo>
	static BM3_FORCEINLINE void	UpdateLayer1(UpdateInfo &update_info, const ResourceType &res, TextureCrop &/*crop*/, int &crop_id, float &counter, TextureAnimationTypeConst type){

		if(type == TextureAnimationTypeConst_Crop)	// �����I�ɂ���if���͂Ȃ��Ȃ�
		{
			Update(update_info, crop_id, counter, res.Texture_Layer1_AnimationSpeed());
		}
	}

private:
	/*!
		�N���b�v�؂�ւ��p�̃J�E���^�Ƃh�c������������
		@param	crop_id		�N���b�v�̂h�c
		@param	counter		�N���b�v�؂�ւ��̃J�E���^
		@param	sw_frame	�؂�ւ��t���[����
		@return	�Ȃ�
	*/
	static BM3_FORCEINLINE void	Initialize(int &crop_id, float &counter, int sw_frame){
		crop_id	= 0;
		counter	= (float)sw_frame;
	}

	/*!
		�N���b�v�؂�ւ��p�̃J�E���^�Ƃh�c������������B�N���b�v�h�c�̓����_���ɐ؂�ւ���B
		@param	crop_id			�N���b�v�̂h�c
		@param	counter			�N���b�v�؂�ւ��̃J�E���^
		@param	sw_frame		�؂�ւ��t���[����
		@param	random			ml::random
		@param	max_crop_count	�ő�N���b�v��
		@return	�Ȃ�
	*/
	static BM3_FORCEINLINE void	Initialize(int &crop_id, float &counter, int sw_frame, ml::random &random, int max_crop_count){
		crop_id	= random.i32(max_crop_count);
		counter	= (float)sw_frame;
	}
	
	/*!
		�N���b�v�؂�ւ��p�̃J�E���g���X�V����
		@param	update_info	UpdateInfo�N���X
		@param	crop_id		�N���b�v�ԍ�
		@param	counter		�؂�ւ��J�E���^
		@param	sw_frame	�؂�ւ��t���[����
		@return	�Ȃ�
	*/
	template<typename UpdateInfo>
	static BM3_FORCEINLINE void	Update(UpdateInfo &update_info, int &crop_id, float &counter, int sw_frame){

		ML_ASSERT(0<=counter);

		//-- counter;
		counter -= update_info.GetUpdateFrame();
		while(counter <= 0){
			counter = sw_frame + counter;
			++ crop_id;
		}
	}

	/*!
		�N���b�v�؂�ւ��p�̃J�E���g���X�V����B�N���b�v�h�c�̓����_���ɐ؂�ւ���B
		@param	update_info	UpdateInfo�N���X
		@param	crop_id		�N���b�v�ԍ�
		@param	counter		�؂�ւ��J�E���^
		@param	sw_frame	�؂�ւ��t���[����
		@param	random			ml::random
		@param	max_crop_count	�ő�N���b�v��
		@return	�Ȃ�
	*/
	template<typename UpdateInfo>
	static BM3_FORCEINLINE void	Update(UpdateInfo &update_info, int &crop_id, float &counter, int sw_frame, ml::random &random, int max_crop_count){

		ML_ASSERT(0<=counter);

		//-- counter;
		counter -= update_info.GetUpdateFrame();

		while(counter <= 0){
			counter = sw_frame + counter;
			crop_id	= random.i32(max_crop_count);
		}

	}
	
};

}	// namespace impl


}	// namespace policy
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_TEXTURE_BM3_POLICY_TEXTURE_CROP_H
