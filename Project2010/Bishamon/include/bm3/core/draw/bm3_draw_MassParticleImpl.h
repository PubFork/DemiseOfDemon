#ifndef BM3_SDK_INC_BM3_SDK_INC_BM3_CORE_DRAW_BM3_DRAW_MASSPARTICLEIMPL_H
#define BM3_SDK_INC_BM3_SDK_INC_BM3_CORE_DRAW_BM3_DRAW_MASSPARTICLEIMPL_H

#include "bm3/system/bm3_Config.h"
#include "policy/bm3_draw_policy_Common.h"
#include "policy/bm3_draw_policy_HasTexture.h"
#include "policy/bm3_draw_policy_DefaultRenderState.h"
#include "../bm3_CoreType.h"
#include "../bm3_RenderStateManager.h"
#include "../bm3_ProfileManager.h"
#include "../bm3_ResourceManager.h"
#include "bm3_draw_Common.h"
#include "../object/bm3_obj_RootNodeBase.h"
#include "../bm3_VertexInfo.h"
#include "../bm3_IdeSupport.h"

#include <ml/type/ml_matrix44.h>
#include <ml/type/ml_color.h>
#include <ml/utility/ml_simpletimer.h>
#include "../policy/Texture/bm3_policy_Texture_Pattern.h"
#include "bm3/core/primitive/bm3_Primitive.h"
#include "m3dg/model/m3dg_model.h"
#if defined(BM3_PLATFORM_win_opengl)
#include "bm3/core/!opengl/bm3_InternalTexture.h"
#endif
#if defined(BM3_PLATFORM_psvita)
#include "bm3/core/!psvita/bm3_InternalTexture.h"
#include <vectormath.h>
#endif


namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

class DrawInfo;

namespace draw{

#define BM3_BLEND_PARENTCOLOR(c, parent_color)	((1.0f - res.Basic_ParentNodeInfluence_ParentColorRate()) * (c) + (res.Basic_ParentNodeInfluence_ParentColorRate()) * (parent_color))
#define BM3_BLEND_PARENTALPHA(c, parent_color)																\
	((res.Basic_ParentNodeInfluence_AlphaParentNodeInfluenceType()==AlphaParentNodeInfluenceTypeConst_On) ?	\
	(res.Basic_ParentNodeInfluence_ParentAlphaRate()==0.0f) ? (c) : (c) * (parent_color) * res.Basic_ParentNodeInfluence_ParentAlphaRate() : 	\
	((1.0f - res.Basic_ParentNodeInfluence_ParentAlphaRate()) * (c) + (res.Basic_ParentNodeInfluence_ParentAlphaRate()) * (parent_color)))

/// @brief MassParticle�N���X
class MassParticle :
	public policy::HasTexture,
	public policy::DefaultRenderState{
		
		
#if defined(BM3_PLATFORM_win_opengl)
		#include "!opengl/bm3_draw_MassParticleDependency.h"
#endif
#if defined(BM3_PLATFORM_psvita)
		#include "!psvita/bm3_draw_MassParticleDependency.h"
#endif
		
	/*
		�p�[�e�B�N���̐��𐧌䂷��N���X
		���[�U�[�ɂ�萶�����𐧌�ł���悤�ɂ��邽�ߍő吔���`�悷�鐔�ƈ�v���Ȃ����Ƃ�����
	*/
	typedef struct ParticleCountManager{
	public:
		ParticleCountManager() :
			min_width_(0),
			min_count_(0),
			max_width_(0),
			max_count_(0),
			current_count_(0),
			min_width_align_(0),
			max_width_align_(0),
			max_count_align_(0)
		{
		}
		/*
			@brief	�������B�p�[�e�B�N���̐����m�肷��
			@param	info	InitializeInfo
			@param	count	�p�[�e�B�N���̐�
			@return	�Ȃ�
		*/
		void Initialize(InitializeInfo &info, int count){
			int	sq			= (int)ml::sqrtf((float)count);
			min_width_		= sq;
			min_count_		= min_width_ * min_width_;
			max_width_		= (int)(min_width_ * info.GenScale());
			max_count_		= max_width_ * max_width_;

			min_width_align_ = ml::align<InternalTextureAlignSize>(min_width_);
			max_width_align_ = ml::align<InternalTextureAlignSize>(max_width_);
			max_count_align_ = max_width_align_ * max_width_align_;
		}

		/*
			@brief	�����������l�����ăp�[�e�B�N���̐����X�V����
			@param	scale	��������
			@return	�Ȃ�
		*/
		void Update(float scale){
			current_count_	= (int)(scale * min_count_);
			current_count_	= ml::clamp(current_count_, 0, max_count_);
		}

		int MinWidth()		const {	return min_width_;	}
		int MinCount()		const {	return min_count_;	}
		int MaxWidth()		const {	return max_width_;	}
		int MaxCount()		const {	return max_count_;	}
		int CurrentCount()	const {	return current_count_;	}
		int MinWidthAlign()	const {	return min_width_align_;	}
		int MaxWidthAlign()	const {	return max_width_align_;	}
		int MaxCountAlign()	const {	return max_count_align_;	}

	private:
		int min_width_;	// �������̊����𐧌䂵�Ȃ��ꍇ�̕�
		int min_count_;
		int max_width_;	// �������̊����𐧌䂵���ꍇ�̕�
		int max_count_;
		int current_count_;	// ���ݕ`�悷�鐔
		int min_width_align_;	// �������̊����𐧌䂵�Ȃ��ꍇ�̕�
		int max_width_align_;	// �������̊����𐧌䂵���ꍇ�̕�
		int max_count_align_;
	} ParticleCountManager;

public:
	MassParticle() : 
		is_initialized(false),
		resource_size_(0){	
	}
	~MassParticle(){}

	// -----------------------------------------------------------------
	/*
		@brief	�p�[�e�B�N���Ɏg�p����e�N�X�`���̃������T�C�Y
		@return	�e�N�X�`���̃������T�C�Y
	*/
	int PrimitiveTextureSize() const {	return InternalTexture::FormatToByte(InternalTextureFormatConst_R16G16B16A16F) * particle_count_.MaxCountAlign();	}

	// -----------------------------------------------------------------
	/*
		@brief	�}�X�p�[�e�B�N���Ŏg�p���郂�f���̃������T�C�Y��Ԃ�
		@param	is_enable	���f���^�C�v���ǂ���
		@param	name		���f����
		@return	�}�X�p�[�e�B�N���Ŏg�p���郂�f���̃������T�C�Y
	*/
	int CalModelType(bool is_enable, const bm3::StringType &name){
		int size = 0;
		if(is_enable){
			
			const void *m3r_image = ResourceManager::instance().M3rImageFromName(name);
			if(m3r_image){
				m3dg::VertexArrayInfo	vai;
				m3dg::model::GetVertexArrayInfo(vai, *reinterpret_cast<const m3dg::fileformat::m3r::Header *>(m3r_image));

				int	width		= ml::align<InternalTextureAlignSize>((int)ml::sqrtf((float)vai.vertex_count_) + 1);
#ifdef BM3_PLATFORM_win_opengl
				size += particle_count_.MaxWidthAlign() * particle_count_.MaxWidthAlign() * InternalTexture::FormatToByte(InternalTextureFormatConst_R16G16B16A16) + 
						width * width * InternalTexture::FormatToByte(InternalTextureFormatConst_R32G32B32A32F) + 
						width * width * InternalTexture::FormatToByte(InternalTextureFormatConst_R32G32B32A32F);
#endif
#ifdef BM3_PLATFORM_psvita
				size += particle_count_.MaxWidthAlign() * particle_count_.MaxWidthAlign() * InternalTexture::FormatToByte(InternalTextureFormatConst_R16G16B16A16F) + 
						width * width * InternalTexture::FormatToByte(InternalTextureFormatConst_R16G16B16A16F) + 
						width * width * InternalTexture::FormatToByte(InternalTextureFormatConst_R16G16B16A16F);
#endif
			}
		}
		
		return size;
	}
		
	// -----------------------------------------------------------------
	/*
		@brief	�}�X�p�[�e�B�N���Ŏg�p���郁�����T�C�Y��Ԃ�
		@return	�}�X�p�[�e�B�N���Ŏg�p���郁�����T�C�Y
	*/
	template<typename ResourceType>
	int CalTotalResourceSize(const ResourceType &res){
		int size =	sizeof(VertexType) * particle_count_.MaxCount() + // vertex buffer
					PrimitiveTextureSize() * (4 + 2 + 1) + // position,velocity * 2 + org + color
					InternalTexture::FormatToByte(InternalTextureFormatConst_R32F) * 256 + 
					InternalTexture::FormatToByte(InternalTextureFormatConst_R8G8B8A8) * 256;

		size += CalModelType(res.GenerateShape_Basic_GenerateShapeType()==GenerateShapeTypeConst_Model, res.GenerateShape_Basic_ModelName());
		return size;
	}

	// -----------------------------------------------------------------
	/*
		@brief	�}�X�p�[�e�B�N���Ŏg�p����e�N�X�`���̏�����
		@param	renderer_info	RendererInfo
		@param	res				���\�[�X�N���X
		@return	�Ȃ�
	*/
	template<typename ResourceType>
	void InitializeParam(RendererInfo &renderer_info, const ResourceType &res){

		int size = CalTotalResourceSize(res);
		if(VertexManager::instance().Alloc(size)){
			resource_size_	= size;
			InitVertices(renderer_info);
			InitParticleMaps(renderer_info, res);
			InitAlphaMap(res);
			InitPatternTextureMap(res);
			
#if defined(BM3_PLATFORM_win_opengl)
			for(int i=0 ;i<2 ; ++i){
				update_surface_[i].Create();
				update_surface_[i].BindTexture(position_texture_[i].Texture(), velocity_texture_[i].Texture());
			}
#endif
		}
	}

	// -----------------------------------------------------------------
	/*
		@brief	���_�o�b�t�@���쐬����
		@param	info	RendererInfo
		@return	�p�[�e�B�N����0����false�B
	*/
	bool InitVertices(RendererInfo &info){
		
		if(particle_count_.MaxCount()){
			vertex_buffer_.Create(info, particle_count_.MaxCount() ,sizeof(VertexType));

			VertexType	*vtx	= reinterpret_cast<VertexType*>(vertex_buffer_.BeginBuffer());
			{
				int i,j;
				float u = 0.0f;
				float v = 0.0f;

				float step = 1.0f/(float)particle_count_.MaxWidthAlign();
				// �s�N�Z���̐^�񒆂��w���悤�ɔ��s�N�Z�������炷
				float	centroid_u	= 0.5f/(float)particle_count_.MaxWidthAlign();
				float	centroid_v	= 0.5f/(float)particle_count_.MaxWidthAlign();

				int num = 0;
				for (i=0;i<particle_count_.MaxWidthAlign();i++){
					v = (float)i * step + centroid_v;

					for (j=0;j<particle_count_.MaxWidthAlign();j++){

						if(particle_count_.MaxCount()<=num)	break;
						num++;

						u = (float)j * step + centroid_u;

						vtx->texture_.u_ = u;
						vtx->texture_.v_ = v;
						vtx++;
					}
				}
			}
			vertex_buffer_.EndBuffer();
#ifdef BM3_PLATFORM_psvita
			uint16_t	*idx	= vertex_buffer_.BeginIndexBuffer();
			for(int i=0 ; i<particle_count_.MaxCount() ; ++i, ++idx){
				*idx = i;
			}
			vertex_buffer_.EndIndexBuffer();
#endif

			return true;
		}
		return false;
	}

	// -----------------------------------------------------------------
	/*
		@brief	�e�N�X�`���̃��������m�ۂ���
		@param	info	RendererInfo
		@param	res		���\�[�X�N���X
		@return	�Ȃ�
	*/
	template<typename ResourceType>
	void InitParticleMaps(RendererInfo &info, const ResourceType &res){

		for(int i=0 ; i<2 ; ++i){
			position_texture_[i].Create(info, particle_count_.MaxWidthAlign(), particle_count_.MaxWidthAlign(), InternalTextureFormatConst_R16G16B16A16F);
			position_texture_[i].Texture().SetAddress(InternalTextureAddressConst_Clamp);
			position_texture_[i].Texture().SetFilter(InternalTextureFilterConst_Point);
			velocity_texture_[i].Create(info, particle_count_.MaxWidthAlign(), particle_count_.MaxWidthAlign(), InternalTextureFormatConst_R16G16B16A16F);
			velocity_texture_[i].Texture().SetAddress(InternalTextureAddressConst_Clamp);
			velocity_texture_[i].Texture().SetFilter(InternalTextureFilterConst_Point);
		}

		org_position_texture_.Create(info, particle_count_.MaxWidthAlign(), particle_count_.MaxWidthAlign(), InternalTextureFormatConst_R16G16B16A16F);
		org_position_texture_.Texture().SetAddress(InternalTextureAddressConst_Clamp);
		org_position_texture_.Texture().SetFilter(InternalTextureFilterConst_Point);
		org_velocity_texture_.Create(info, particle_count_.MaxWidthAlign(), particle_count_.MaxWidthAlign(), InternalTextureFormatConst_R16G16B16A16F);
		org_velocity_texture_.Texture().SetAddress(InternalTextureAddressConst_Clamp);
		org_velocity_texture_.Texture().SetFilter(InternalTextureFilterConst_Point);
		colorsize_texture_.Create(	info, particle_count_.MaxWidthAlign(), particle_count_.MaxWidthAlign(), InternalTextureFormatConst_R16G16B16A16F);
		colorsize_texture_.Texture().SetAddress(InternalTextureAddressConst_Clamp);
		colorsize_texture_.Texture().SetFilter(InternalTextureFilterConst_Point);

		alpha_texture_.Create(info, 256, 1, InternalTextureFormatConst_A8);
		alpha_texture_.SetAddress(InternalTextureAddressConst_Clamp);
		alpha_texture_.SetFilter(InternalTextureFilterConst_Linear);
		pattern_texture_.Create(info, 256, 1, InternalTextureFormatConst_R8G8B8A8);
		pattern_texture_.SetAddress(InternalTextureAddressConst_Clamp);
		pattern_texture_.SetFilter(InternalTextureFilterConst_Point);

		switch(res.GenerateShape_Basic_GenerateShapeType()){
		case GenerateShapeTypeConst_Point:
			break;
		case GenerateShapeTypeConst_Sphere:
		case GenerateShapeTypeConst_Cylinder:
		case GenerateShapeTypeConst_Cube:
			shape_texture_.Create(info, particle_count_.MaxWidthAlign(), particle_count_.MaxWidthAlign(), InternalTextureFormatConst_R16G16B16A16F);
			shape_texture_.Texture().SetAddress(InternalTextureAddressConst_Clamp);
			shape_texture_.Texture().SetFilter(InternalTextureFilterConst_Point);
			break;
		case GenerateShapeTypeConst_Model:
			break;
		default:
			break;
		}
	}

	// -----------------------------------------------------------------
	/*
		@brief	���f�����g�p�����ꍇ�Ɏg�p����e�N�X�`���𐶐�����
		@param	info		RendererInfo
		@param	random		�����_���N���X
		@param	name		���f���̖��O
		@param	scale		���f���̃X�P�[��
		@param	index		�C���f�b�N�X�p�e�N�X�`��
		@param	position	�ʒu�p�e�N�X�`��
		@param	normal		�@���p�e�N�X�`��
		@param	model_param	���f���̃p�����[�^���i�[����ml::vector3d
		@return	���f���̃C���[�W���擾�ł��Ȃ��Ƃ�false
	*/
	bool CreateModelRenderingTexture(RendererInfo &info, ml::random &random, const bm3::StringType &name, const ml::vector3d &scale, InternalTexture &index, InternalTexture &position, InternalTexture &normal, ml::vector3d &model_param){

		const void *m3r_image = ResourceManager::instance().M3rImageFromName(name);
		if(m3r_image){

			m3dg::VertexArrayInfo	vai;
			m3dg::model::GetVertexArrayInfo(vai, *reinterpret_cast<const m3dg::fileformat::m3r::Header *>(m3r_image));

			int	width		= ml::align<InternalTextureAlignSize>((int)ml::sqrtf((float)vai.vertex_count_) + 1);
			model_param.e_.x_	= (float)width;
			model_param.e_.y_	= vai.vertex_count_ * ml::inverse3;
			model_param.e_.z_	= 1.0f/width;
			model_param.e_.w_	= 1.0f/width;

#ifdef BM3_PLATFORM_win_opengl
			index.Create(	info, particle_count_.MaxWidthAlign(), particle_count_.MaxWidthAlign(), InternalTextureFormatConst_R16G16B16A16);
			index.SetAddress(InternalTextureAddressConst_Clamp);
			index.SetFilter(InternalTextureFilterConst_Point);
			position.Create(info, width, width, InternalTextureFormatConst_R32G32B32A32F);
			position.SetAddress(InternalTextureAddressConst_Clamp);
			position.SetFilter(InternalTextureFilterConst_Point);
			normal.Create(	info, width, width, InternalTextureFormatConst_R32G32B32A32F);
			normal.SetAddress(InternalTextureAddressConst_Clamp);
			normal.SetFilter(InternalTextureFilterConst_Point);
#endif
#ifdef BM3_PLATFORM_psvita
			index.Create(	info, particle_count_.MaxWidthAlign(), particle_count_.MaxWidthAlign(), InternalTextureFormatConst_R16G16B16A16F);
			index.SetAddress(InternalTextureAddressConst_Clamp);
			index.SetFilter(InternalTextureFilterConst_Point);
			position.Create(info, width, width, InternalTextureFormatConst_R16G16B16A16F);
			position.SetAddress(InternalTextureAddressConst_Clamp);
			position.SetFilter(InternalTextureFilterConst_Point);
			normal.Create(	info, width, width, InternalTextureFormatConst_R16G16B16A16F);
			normal.SetAddress(InternalTextureAddressConst_Clamp);
			normal.SetFilter(InternalTextureFilterConst_Point);
#endif

			InitModelTexture(vai, random, width, scale, index, position, normal);

			return true;
		}

		return false;
	}

	// -----------------------------------------------------------------
	/*
		@brief	���f���p�e�N�X�`���̏�����
		@param	vai			���f�����_���
		@param	random		�����_���I�u�W�F�N�g
		@param	height		�e�N�X�`���̍���
		@param	scale		���f���̃X�P�[��
		@param	index		�C���f�b�N�X�e�N�X�`��
		@param	position	�ʒu�e�N�X�`��
		@param	normal		�@���e�N�X�`��
		@return	�Ȃ�
	*/
	void InitModelTexture(m3dg::VertexArrayInfo &vai, ml::random &random, int height, const ml::vector3d &scale, InternalTexture &index, InternalTexture &position, InternalTexture &normal){

		// �C���f�b�N�X�e�N�X�`�����쐬
		{
			void*	buffer = index.BeginBuffer();
			{
				int				cnt = index.Width();
				int num = 0;
				unsigned short*	b	= (unsigned short*)buffer;
				for(int h=0 ; h<particle_count_.MaxWidthAlign() ; ++h){
					for(int w=0 ; w<cnt; ++w){

						if(particle_count_.MaxCount()<=num)	break;

						int		tri_count	= (int)(vai.vertex_count_ * ml::inverse3);
						float	measure		= random.f32(vai.all_triangle_measure_);
						const float *i = std::lower_bound(vai.triangle_addition_measure_, vai.triangle_addition_measure_ + tri_count, measure);
						int tri_no = i - vai.triangle_addition_measure_;
						tri_no = ml::clamp(tri_no - 1, 0, tri_count - 1);
						const m3dg::VertexArrayInfo::IndexType *idx = vai.index_array_ + tri_no * 3;
						SetModelIndex(b, idx);
						b += 4;
						num++;
					}
				}
			}
			index.EndBuffer();
		}
		// �ʒu�e�N�X�`���̍쐬
		{
			void*	buffer = position.BeginBuffer();
			{
				const ml::f32*	src	= vai.position_array_;
				int				cnt = position.Width();
#ifdef BM3_PLATFORM_win_opengl
				float*		b	= (float*)buffer;
#endif
#ifdef BM3_PLATFORM_psvita
				uint16_t*	b	= (uint16_t*)buffer;
#endif
				for(int h=0 ; h<height ; ++h){
					for(int w=0 ; w<cnt; ++w){
						int num = w + h*height;
						if(vai.vertex_count_<=num)	break;
						const float *pos = ml::offset_pointer<const ml::f32 *>(src, vai.position_array_stride_ * num);
						SetModelPosition(b, pos, scale);
						b += 4;
					}
				}
			}
			position.EndBuffer();
		}
		// �@���e�N�X�`���̍쐬
		{
			void*	buffer = normal.BeginBuffer();
			{
				const ml::f32*	src	= vai.normal_array_;
				int				cnt = normal.Width();
#ifdef BM3_PLATFORM_win_opengl
				float*	b	= (float*)buffer;
#endif
#ifdef BM3_PLATFORM_psvita
				uint16_t*	b	= (uint16_t*)buffer;
#endif
				for(int h=0 ; h<height ; ++h){
					for(int w=0 ; w<cnt; ++w){
						int num = w + h*height;
						if(vai.vertex_count_<=num)	break;
						const float *nor = ml::offset_pointer<const ml::f32 *>(src, vai.normal_array_stride_ * num);
						SetModelNormal(b, nor);
						b += 4;
					}
				}
			}
			normal.EndBuffer();
		}
	}

	// -----------------------------------------------------------------
	/*
		@brief	�A���t�@�e�N�X�`���̍쐬
		@param	res	�}�X�p�[�e�B�N�����\�[�X�I�u�W�F�N�g
		@return	�e�N�X�`���쐬�̐���
	*/
	template<typename ResourceType>
	bool InitAlphaMap(const ResourceType &res){
		
		void* buffer	= alpha_texture_.BeginBuffer();
		{
			unsigned char* b = (unsigned char*)buffer;
			int cnt = alpha_texture_.Width();
			int i=0;
			switch(res.Blend_Basic_BlendAlphaType()){
				case BlendAlphaTypeConst_Constant:
					{
						unsigned char alpha = (unsigned char)(res.Blend_Basic_Alpha() * 255.0f);
						for(i=0 ; i<cnt ; ++i, ++b){
							*b = alpha;
						}
					}
					break;
				case BlendAlphaTypeConst_Curve:
					for(i=0 ; i<cnt ; ++i, ++b){
						float rate = (float)i/cnt;
						*b = (unsigned char)(res.Blend_Basic_Curve().Value(rate) * 255.0f);
					}
					break;
				default:	ML_ASSERT(0);			break;
			}
		}
		alpha_texture_.EndBuffer();

		return true;
	}

	// -----------------------------------------------------------------
	/*
		@brief	�p�^�[���e�N�X�`���̍쐬
		@param	res	�}�X�p�[�e�B�N�����\�[�X�I�u�W�F�N�g
		@return	�e�N�X�`���쐬�̐���
	*/
	template<typename ResourceType>
	bool InitPatternTextureMap(const ResourceType &res){
		
		void* buffer	= pattern_texture_.BeginBuffer();
	
		unsigned char* b = (unsigned char*)buffer;
		int cnt = pattern_texture_.Width();
		int i=0;
		switch(res.Texture_Layer1_TextureAnimationType()){
		case TextureAnimationTypeConst_Nothing:
				for(i=0 ; i<cnt ; ++i, b += 4){
					b[0]	= (unsigned char)(0.0f * 255.0f);
					b[1]	= (unsigned char)(0.0f * 255.0f);
					b[2]	= (unsigned char)(1.0f * 255.0f);
					b[3]	= (unsigned char)(1.0f * 255.0f);
				}
				break;
		case TextureAnimationTypeConst_Crop:		
			{
				int frame	= 0;
				float step	= 0.0f;
				TextureCrop	dummy;
				ml::random	dummy_rand;
				bm3::policy::impl::Texture_Crop::InitializeLayer1(res, dummy_rand, dummy, frame, step, TextureAnimationTypeConst_Crop);

				UpdateInfo update_info;
				for(i=0 ; i<cnt ; ++i, b += 4){
					bm3::policy::impl::Texture_Crop::UpdateLayer1(update_info, res, dummy, frame, step, TextureAnimationTypeConst_Crop);
					const AnimCrop	*ac			= Texture_Layer_AnimCrop(res.Texture_Layer1_TextureCropAnimationType());
					int				anim_frame	= frame & (ac->count_ - 1);

					b[0]	= (unsigned char)(ac->origin_[anim_frame].u_ * 255.0f);	// x
					b[1]	= (unsigned char)(ac->origin_[anim_frame].v_ * 255.0f);	// y
					b[2]  = (unsigned char)(ac->size_.u_ * 255.0f);	// width
					b[3]  = (unsigned char)(ac->size_.v_ * 255.0f);	// height
				}
			}
			break;
		case TextureAnimationTypeConst_Scroll:		break;
		case TextureAnimationTypeConst_Pattern:
			{
				int frame	= 0;
				float step	= 0.0f;
				TextureCrop	dummy;
				ml::random	dummy_rand;
				bm3::policy::impl::Texture_Pattern::InitializeLayer1(res, dummy_rand, dummy, frame, step, TextureAnimationTypeConst_Pattern);

				UpdateInfo update_info;
				for(i=0 ; i<cnt ; ++i, b += 4){
					bm3::policy::impl::Texture_Pattern::UpdateLayer1(update_info, res, dummy, frame, step, TextureAnimationTypeConst_Pattern);
					const TexturePattern &tp = res.Texture_Layer1_Pattern().Pattern(frame);
					b[0]	= (unsigned char)(tp.crop_.left_ * 255.0f);
					b[1]	= (unsigned char)(tp.crop_.top_ * 255.0f);
					b[2]	= (unsigned char)(tp.crop_.width_ * 255.0f);
					b[3]	= (unsigned char)(tp.crop_.height_ * 255.0f);
				}
			}
			break;
		case TextureAnimationTypeConst_Transform:	break;
		default: break;
		}
		
		pattern_texture_.EndBuffer();
		return true;
	}
		
	// -----------------------------------------------------------------
	/*
		@brief	�������B�p�[�e�B�N���̐��A�g�p���郁�����T�C�Y���v�Z����B
		@param	info	InitializeInfo
		@param	res		�}�X�p�[�e�B�N�����\�[�X�I�u�W�F�N�g
		@return	�Ȃ�
	*/
	template<typename ResourceType>
	void	Initialize(InitializeInfo &info, const ResourceType &res){
		particle_count_.Initialize(info, res.Generate_Constant_GenerateCount());
	}

	// -----------------------------------------------------------------
	/*
		@brief	�������B�g�p����e�N�X�`���̃��������m�ۂ���
		@param	info	InitInfo
		@param	res		�}�X�p�[�e�B�N�����\�[�X�I�u�W�F�N�g
		@return	�Ȃ�
	*/
	template<typename ResourceType>
	void	Initialize(InitInfo &info, const ResourceType &res){
		if(vertex_buffer_.IsInitialized()){
			InitializeParam(info.Renderer(), res);
		}
	}
		
	// -----------------------------------------------------------------
	/*
		@brief	�I�������B�������̉��
		@return	�Ȃ�
	*/
	void	Finalize(){
		if(vertex_buffer_.IsInitialized()){
			vertex_buffer_.Release();
		}


#if defined(BM3_PLATFORM_win_opengl)
		for(int i=0; i<2 ; ++i){
			update_surface_[i].Release();
		}
#endif
		pattern_texture_.Release();
		alpha_texture_.Release();
		colorsize_texture_.Release();

		shape_texture_.Release();		
		model_index_.Release();		
		model_position_.Release();	
		model_normal_.Release();		


		goal_texture_.Release();
		goal_model_index_.Release();		
		goal_model_position_.Release();	
		goal_model_normal_.Release();		

		org_position_texture_.Release();
		org_velocity_texture_.Release();
		for(int i=0; i<2 ; ++i){
			position_texture_[i].Release();
			velocity_texture_[i].Release();
		}

		VertexManager::instance().Free(resource_size_);
	}

	// -----------------------------------------------------------------
	/*
		@brief	������Ԃɖ߂�
		@param	info	ResetInfo
		@return	�Ȃ�
	*/
	void	Reset(ResetInfo &info){
		if(vertex_buffer_.IsInitialized()){
			current_map_index_	= current_map_index_ ? 0 : 1;
#ifdef BM3_PLATFORM_win_opengl
			DrawInfo draw_info;
			draw_info.SetRenderer(info.Renderer());
			org_position_texture_.SetRenderTarget(draw_info);
			position_texture_[current_map_index_].Copy(info.Renderer(), org_position_texture_);
			org_position_texture_.UnsetRenderTarget(draw_info);
			org_velocity_texture_.SetRenderTarget(draw_info);
			velocity_texture_[current_map_index_].Copy(info.Renderer(), org_velocity_texture_);
			org_velocity_texture_.UnsetRenderTarget(draw_info);
#endif
#ifdef BM3_PLATFORM_psvita
			position_texture_[current_map_index_].Copy(info.Renderer(), org_position_texture_);
			velocity_texture_[current_map_index_].Copy(info.Renderer(), org_velocity_texture_);
#endif
		}
	}

	// -----------------------------------------------------------------
	/*
		@brief	�`��O����
		@param	draw_info	DrawInfo
		@param	res			�}�X�p�[�e�B�N�����\�[�X�I�u�W�F�N�g
		@return	�Ȃ�
	*/
	template<typename ResourceType>
	void	Begin(DrawInfo & /*draw_info*/, const ResourceType & /*res*/){
	}

	// -----------------------------------------------------------------
	/*
		@brief	�����ʒu�������_�����O����
		@param	info			RendererInfo
		@param	res				�}�X�p�[�e�B�N�����\�[�X�I�u�W�F�N�g
		@param	shader			MassShader�I�u�W�F�N�g
		@param	target_texture	�����_�����O�����RenderingTexture�I�u�W�F�N�g
		@param	offset			�m�C�Y�I�t�Z�b�g�x�N�g��
		@return	�Ȃ�
	*/
	template<typename ResourceType>
	void SetPosition(RendererInfo &info, const ResourceType &res, MassShader &shader, ml::vector3d &offset){

		{
			ml::vector3d	temp;
			shader.SetVectorParam(info, MassShader::SetupShaderParam_ps_noiseuv_offset,		offset.v_);
			float life_param[4]	= {
				-(float)res.Basic_Basic_ActiveStart(),
				-(float)res.Basic_Basic_ActiveStartRange(),
				(float)res.Generate_Constant_GenerateTime(),
				(float)particle_count_.MaxWidth()//pitch
			};
			shader.SetVectorParam(info, MassShader::SetupShaderParam_ps_w_param,			life_param);
			shader.SetVectorParam(info, MassShader::SetupShaderParam_ps_initial_value,		res.Translate_Value_Initial().v_);
			shader.SetVectorParam(info, MassShader::SetupShaderParam_ps_variance_scalar,	res.Translate_Value_Range().v_);
			temp	= ml::vector3d(2.0f, 2.0f, 2.0f, 2.0f);
			shader.SetVectorParam(info, MassShader::SetupShaderParam_ps_noise_scalar,	temp.v_);
			temp	= ml::vector3d(-1.0f, -1.0f, -1.0f, -1.0f);
			shader.SetVectorParam(info, MassShader::SetupShaderParam_ps_noise_offset,	temp.v_);

			shader.EnableNoiseTexture(info, MassShader::SetupShaderParam_ps_noise_texture);
		}
		vertex_buffer_.Draw(info, PrimitiveConst_Point, particle_count_.MaxCount());

		shader.UnsetPixelTexture(info, MassShader::SetupShaderParam_ps_shape_texture);
	}

	// -----------------------------------------------------------------
	/*
		@brief	�������x�������_�����O����
		@param	info			RendererInfo
		@param	res				�}�X�p�[�e�B�N�����\�[�X�I�u�W�F�N�g
		@param	shader			MassShader�I�u�W�F�N�g
		@param	target_texture	�����_�����O�����RenderingTexture�I�u�W�F�N�g
		@param	offset			�m�C�Y�I�t�Z�b�g�x�N�g��
		@return	�Ȃ�
	*/
	template<typename ResourceType>
	void SetVelocity(RendererInfo &info, const ResourceType &res, MassShader &shader, ml::vector3d &offset){

		{
			shader.SetVectorParam(info, MassShader::SetupShaderParam_ps_noiseuv_offset,		offset.v_);
			float life_param[4]	= {
				(float)res.Basic_Basic_Life(),
				(float)res.Basic_Basic_LifeRange(),
				0.0f,
				0.0f
			};
			shader.SetVectorParam(info, MassShader::SetupShaderParam_ps_w_param,	life_param);

			ml::vector3d	temp;
			temp = res.Translate_Speed_Initial();
			shader.SetVectorParam(info, MassShader::SetupShaderParam_ps_initial_value,	temp.v_);
			temp = res.Translate_Speed_Range();
			shader.SetVectorParam(info, MassShader::SetupShaderParam_ps_variance_scalar, temp.v_);
			temp = ml::vector3d(2.0f, 2.0f, 2.0f, 2.0f);
			shader.SetVectorParam(info, MassShader::SetupShaderParam_ps_noise_scalar,	temp.v_);
			temp = ml::vector3d(-1.0f, -1.0f, -1.0f, -1.0f);
			shader.SetVectorParam(info, MassShader::SetupShaderParam_ps_noise_offset,	temp.v_);

			shader.EnableNoiseTexture(info, MassShader::SetupShaderParam_ps_noise_texture);
		}
		vertex_buffer_.Draw(info, PrimitiveConst_Point, particle_count_.MaxCount());

	}

	// -----------------------------------------------------------------
	/*
		@brief	�����`��������_�����O����
		@param	info			DrawInfo
		@param	res				�}�X�p�[�e�B�N�����\�[�X�I�u�W�F�N�g
		@param	shader			MassShader�I�u�W�F�N�g
		@param	random			ml::random
		@param	target_texture	�����_�����O�����RenderingTexture�I�u�W�F�N�g
		@param	shape_texture	shape texture
		@param	type			�����`��̃^�C�v
		@return	�Ȃ�
	*/
	template<typename ResourceType>
	void SetShapeVelocity(DrawInfo &info, const ResourceType &res, MassShader &shader, ml::random &random, RenderingTexture &target_texture, RenderingTexture &shape_texture, MassShader::SetupShaderType type){

		target_texture.SetRenderTarget(info);
		ml::vector3d	offset(random.f32(0.001f, 1.0f), random.f32(0.001f, 1.0f), 0 ,1);
#ifdef BM3_PLATFORM_win_opengl
		if(res.GenerateShape_Basic_Normal()){
			RenderStateManager::instance().ApplyMassParticle(info.Renderer(), MassShader::ProcType_Setup, type);
			shader.SetPixelTexture(info.Renderer(), MassShader::SetupShaderParam_ps_shape_texture,		shape_texture.Texture());
		}
		else{
			RenderStateManager::instance().ApplyMassParticle(info.Renderer(), MassShader::ProcType_Setup, MassShader::ShaderType_SetupSpeed);
		}
		SetVelocity(info.Renderer(), res, shader, offset);
		org_velocity_texture_.Copy(info.Renderer(), target_texture);
		target_texture.UnsetRenderTarget(info);
#endif
#ifdef BM3_PLATFORM_psvita
		if(res.GenerateShape_Basic_Normal()){
			RenderStateManager::instance().ApplyMassParticle(info.Renderer(), MassShader::ProcType_Setup, type);
			shader.EnablePixelShaderParam(info.Renderer());
			shader.SetPixelTexture(info.Renderer(), MassShader::SetupShaderParam_ps_shape_texture,		shape_texture.Texture());
		}
		else{
			RenderStateManager::instance().ApplyMassParticle(info.Renderer(), MassShader::ProcType_Setup, MassShader::ShaderType_SetupSpeed);
			shader.EnablePixelShaderParam(info.Renderer());
		}
		SetVelocity(info.Renderer(), res, shader, offset);
		target_texture.UnsetRenderTarget(info);
		GpuCopy(info, shader, org_velocity_texture_, target_texture);
#endif
	}

	// -----------------------------------------------------------------
	/*
		@brief	�����`��^�C�v�̈ʒu�������_�����O����
		@param	info			DrawInfo
		@param	res				�}�X�p�[�e�B�N�����\�[�X�I�u�W�F�N�g
		@param	shader			MassShader�I�u�W�F�N�g
		@param	target_texture	�����_�����O�����RenderingTexture�I�u�W�F�N�g
		@param	shape_texture	shape texture
		@return	�Ȃ�
	*/
	template<typename ResourceType>
	void SetShapePosition(DrawInfo &info, const ResourceType &res, MassShader &shader, ml::random &random, RenderingTexture &target_texture, RenderingTexture &shape_texture){

		target_texture.SetRenderTarget(info);
		ml::vector3d	offset(random.f32(0.001f, 1.0f), random.f32(0.001f, 1.0f), 0 ,1);
#ifdef BM3_PLATFORM_win_opengl
		{
			RenderStateManager::instance().ApplyMassParticle(info.Renderer(), MassShader::ProcType_Setup, MassShader::ShaderType_SetupShape);
			shader.SetPixelTexture(info.Renderer(), MassShader::SetupShaderParam_ps_shape_texture,		shape_texture.Texture());
			SetPosition(info.Renderer(), res, shader, offset);
		}
		org_position_texture_.Copy(info.Renderer(), target_texture);
		target_texture.UnsetRenderTarget(info);
#endif
#ifdef BM3_PLATFORM_psvita
		{
			RenderStateManager::instance().ApplyMassParticle(info.Renderer(), MassShader::ProcType_Setup, MassShader::ShaderType_SetupShape);
			shader.EnablePixelShaderParam(info.Renderer());
			shader.SetPixelTexture(info.Renderer(), MassShader::SetupShaderParam_ps_shape_texture,		shape_texture.Texture());
			SetPosition(info.Renderer(), res, shader, offset);
		}
		target_texture.UnsetRenderTarget(info);
		GpuCopy(info, shader, org_position_texture_, target_texture);
#endif
	}

	// -----------------------------------------------------------------
	/*
		@brief	�����`�󃂃f���^�C�v�̑��x�������_�����O����
		@param	info			DrawInfo
		@param	res				�}�X�p�[�e�B�N�����\�[�X�I�u�W�F�N�g
		@param	shader			MassShader�I�u�W�F�N�g
		@param	target_texture	�����_�����O�����RenderingTexture�I�u�W�F�N�g
		@param	model_param		(���A�v���~�e�B�u���A�O�p�`�̑��ʐρi���g�p�j�A���̋t��)�x�N�g��
		@param	model_index		���f���̃C���f�b�N�X�e�N�X�`��
		@param	model_normal	���f���̖@���e�N�X�`��
		@param	offset			�m�C�Y�I�t�Z�b�g�x�N�g��
		@return	�Ȃ�
	*/
	template<typename ResourceType>
	void SetModelVelocity(DrawInfo &info, const ResourceType &res, MassShader &shader, RenderingTexture &target_texture, ml::vector3d &model_param, InternalTexture &model_index, InternalTexture &model_normal, ml::vector3d &offset){

		target_texture.SetRenderTarget(info);
#ifdef BM3_PLATFORM_win_opengl
		if(res.GenerateShape_Basic_Normal()){
			RenderStateManager::instance().ApplyMassParticle(info.Renderer(), MassShader::ProcType_Setup, MassShader::ShaderType_SetupSpeedModel);
			shader.SetVectorParam(info.Renderer(),	MassShader::SetupShaderParam_shape_ps_modelparam,		model_param.v_);
			shader.SetPixelTexture(info.Renderer(), MassShader::SetupShaderParam_shape_ps_modelindex,		model_index);
			shader.SetPixelTexture(info.Renderer(), MassShader::SetupShaderParam_shape_ps_modelnormal,		model_normal);
		}
		else{
			RenderStateManager::instance().ApplyMassParticle(info.Renderer(), MassShader::ProcType_Setup, MassShader::ShaderType_SetupSpeed);
		}
		SetVelocity(info.Renderer(), res, shader, offset);
		org_velocity_texture_.Copy(info.Renderer(), target_texture);
		target_texture.UnsetRenderTarget(info);
#endif
#ifdef BM3_PLATFORM_psvita
		if(res.GenerateShape_Basic_Normal()){
			RenderStateManager::instance().ApplyMassParticle(info.Renderer(), MassShader::ProcType_Setup, MassShader::ShaderType_SetupSpeedModel);
			shader.EnablePixelShaderParam(info.Renderer());
			shader.SetVectorParam(info.Renderer(),	MassShader::SetupShaderParam_shape_ps_modelparam,		model_param.v_);
			shader.SetPixelTexture(info.Renderer(), MassShader::SetupShaderParam_shape_ps_modelindex,		model_index);
			shader.SetPixelTexture(info.Renderer(), MassShader::SetupShaderParam_shape_ps_modelnormal,		model_normal);
		}
		else{
			RenderStateManager::instance().ApplyMassParticle(info.Renderer(), MassShader::ProcType_Setup, MassShader::ShaderType_SetupSpeed);
			shader.EnablePixelShaderParam(info.Renderer());
		}
		SetVelocity(info.Renderer(), res, shader, offset);
		target_texture.UnsetRenderTarget(info);
		GpuCopy(info, shader, org_velocity_texture_, target_texture);
#endif
	}

	// -----------------------------------------------------------------
	/*
		@brief	�����`�󃂃f���^�C�v�̑��x�������_�����O����
		@param	info			DrawInfo
		@param	res				�}�X�p�[�e�B�N�����\�[�X�I�u�W�F�N�g
		@param	shader			MassShader�I�u�W�F�N�g
		@param	target_texture	�����_�����O�����RenderingTexture�I�u�W�F�N�g
		@param	model_param		(���A�v���~�e�B�u���A�O�p�`�̑��ʐρi���g�p�j�A���̋t��)�x�N�g��
		@param	model_index		���f���̃C���f�b�N�X�e�N�X�`��
		@param	model_position	���f���̈ʒu�e�N�X�`��
		@param	offset			�m�C�Y�I�t�Z�b�g�x�N�g��
		@return	�Ȃ�
	*/
	template<typename ResourceType>
	void SetModelPosition(DrawInfo &info, const ResourceType &res, MassShader &shader, RenderingTexture &target_texture,ml::vector3d &model_param, InternalTexture &model_index, InternalTexture &model_position, ml::vector3d &offset){

		target_texture.SetRenderTarget(info);
		RenderStateManager::instance().ApplyMassParticle(info.Renderer(), MassShader::ProcType_Setup, MassShader::ShaderType_SetupShapeModel);
#ifdef BM3_PLATFORM_win_opengl
		shader.SetVectorParam(info.Renderer(),		MassShader::SetupShaderParam_shape_ps_modelparam,	model_param.v_);
		shader.SetPixelTexture(info.Renderer(), MassShader::SetupShaderParam_shape_ps_modelindex,		model_index);
		shader.SetPixelTexture(info.Renderer(), MassShader::SetupShaderParam_shape_ps_modelposition,	model_position);
		SetPosition(info.Renderer(), res, shader, offset);
#endif
#ifdef BM3_PLATFORM_psvita
		shader.EnablePixelShaderParam(info.Renderer());
		shader.SetVectorParam(info.Renderer(),		MassShader::SetupShaderParam_shape_ps_modelparam,	model_param.v_);
		shader.SetPixelTexture(info.Renderer(), MassShader::SetupShaderParam_shape_ps_modelindex,		model_index);
		shader.SetPixelTexture(info.Renderer(), MassShader::SetupShaderParam_shape_ps_modelposition,	model_position);
		SetPosition(info.Renderer(), res, shader, offset);
#endif
	}

#define SET_GENERATESHAPE_RADIUS(TYPE)							\
	temp		= res.GenerateShape_Area_Outside();				\
	temp.e_.w_	= res.GenerateShape_Area_Inside();				\
	shader.SetVectorParam(info.Renderer(), MassShader::TYPE,	temp.v_);

#define SET_GENERATESHAPE_ANGLE(TYPE)																					\
	temp.e_.x_	= ml::minimum(res.GenerateShape_HorizontalArea_StartAngle(), res.GenerateShape_HorizontalArea_EndAngle());	\
	temp.e_.y_	= ml::abs(res.GenerateShape_HorizontalArea_StartAngle() - res.GenerateShape_HorizontalArea_EndAngle());	\
	temp.e_.z_	= ml::minimum(res.GenerateShape_VerticalArea_StartAngle(), res.GenerateShape_VerticalArea_EndAngle());		\
	temp.e_.w_	= ml::abs(res.GenerateShape_VerticalArea_StartAngle() - res.GenerateShape_VerticalArea_EndAngle());		\
	shader.SetVectorParam(info.Renderer(), MassShader::TYPE,	temp.v_);

#define SET_GOAL_RADIUS(TYPE)							\
	temp		= res.Goal_Area_Outside();				\
	temp.e_.w_	= res.Goal_Area_Inside();				\
	shader.SetVectorParam(info.Renderer(), MassShader::TYPE,	temp.v_);

#define SET_GOAL_ANGLE(TYPE)																			\
	temp.e_.x_	= ml::minimum(res.Goal_HorizontalArea_StartAngle(), res.Goal_HorizontalArea_EndAngle());	\
	temp.e_.y_	= ml::abs(res.Goal_HorizontalArea_StartAngle() - res.Goal_HorizontalArea_EndAngle());	\
	temp.e_.z_	= ml::minimum(res.Goal_VerticalArea_StartAngle(), res.Goal_VerticalArea_EndAngle());		\
	temp.e_.w_	= ml::abs(res.Goal_VerticalArea_StartAngle() - res.Goal_VerticalArea_EndAngle());		\
	shader.SetVectorParam(info.Renderer(), MassShader::TYPE,	temp.v_);

	// -----------------------------------------------------------------
	/*
		@brief	����̈ʒu�������_�����O����i�m�[�}���C�Y����Ă���̂Ŗ@���Ƃ��Ă��g����j
		@param	info				DrawInfo
		@param	res					�}�X�p�[�e�B�N�����\�[�X�I�u�W�F�N�g
		@param	shader				MassShader�I�u�W�F�N�g
		@param	target_texture		�����_�����O�����RenderingTexture�I�u�W�F�N�g
		@param	is_generateshape	�����`�󂩂ǂ���
		@return	�Ȃ�
	*/
	template<typename ResourceType>
	void MakeSphereTexture(DrawInfo &info, const ResourceType &res, MassShader &shader, ml::random &random, RenderingTexture &target_texture, bool is_generateshape = true){

		target_texture.SetRenderTarget(info);
		RenderStateManager::instance().ApplyMassParticle(info.Renderer(), MassShader::ProcType_Setup, MassShader::ShaderType_SetupSphere);
#ifdef BM3_PLATFORM_psvita
			shader.EnablePixelShaderParam(info.Renderer());
#endif
		{
			ml::vector3d		temp;
			ml::vector3d	offset(random.f32(0.001f, 1.0f), random.f32(0.001f, 1.0f), 0 ,1);
			shader.SetVectorParam(info.Renderer(),		MassShader::SetupShaderParam_ps_noiseuv_offset,		offset.v_);
			if(is_generateshape){
				SET_GENERATESHAPE_RADIUS(SetupShaderParam_shape_ps_radius);
				SET_GENERATESHAPE_ANGLE(SetupShaderParam_shape_ps_angle);
			}else{
				SET_GOAL_RADIUS(SetupShaderParam_shape_ps_radius);
				SET_GOAL_ANGLE(SetupShaderParam_shape_ps_angle);
			}
		}
		shader.EnableNoiseTexture(info.Renderer(), MassShader::SetupShaderParam_ps_noise_texture);
		vertex_buffer_.Draw(info.Renderer(), PrimitiveConst_Point, particle_count_.MaxCount());
		shader.DisableNoiseTexture(info.Renderer(), MassShader::SetupShaderParam_ps_noise_texture);
		target_texture.UnsetRenderTarget(info);
	}

	// -----------------------------------------------------------------
	/*
		@brief	�~���̈ʒu�������_�����O����i�m�[�}���C�Y����Ă���̂Ŗ@���Ƃ��Ă��g����j
		@param	info				DrawInfo
		@param	res					�}�X�p�[�e�B�N�����\�[�X�I�u�W�F�N�g
		@param	shader				MassShader�I�u�W�F�N�g
		@param	target_texture		�����_�����O�����RenderingTexture�I�u�W�F�N�g
		@param	is_generateshape	�����`�󂩂ǂ���
		@return	�Ȃ�
	*/
	template<typename ResourceType>
	void MakeCylinderTexture(DrawInfo &info, const ResourceType &res, MassShader &shader, ml::random &random, RenderingTexture &target_texture, bool is_generateshape = true){

		target_texture.SetRenderTarget(info);
		RenderStateManager::instance().ApplyMassParticle(info.Renderer(), MassShader::ProcType_Setup, MassShader::ShaderType_SetupCylinder);
#ifdef BM3_PLATFORM_psvita
			shader.EnablePixelShaderParam(info.Renderer());
#endif
		{
			ml::vector3d		temp;
			ml::vector3d	offset(random.f32(0.001f, 1.0f), random.f32(0.001f, 1.0f), 0 ,1);
			shader.SetVectorParam(info.Renderer(),		MassShader::SetupShaderParam_ps_noiseuv_offset,		offset.v_);
			if(is_generateshape){
				SET_GENERATESHAPE_RADIUS(SetupShaderParam_shape_ps_radius);
				SET_GENERATESHAPE_ANGLE(SetupShaderParam_shape_ps_angle);
			}else{
				SET_GOAL_RADIUS(SetupShaderParam_shape_ps_radius);
				SET_GOAL_ANGLE(SetupShaderParam_shape_ps_angle);
			}
		}
		shader.EnableNoiseTexture(info.Renderer(), MassShader::SetupShaderParam_ps_noise_texture);
		vertex_buffer_.Draw(info.Renderer(), PrimitiveConst_Point, particle_count_.MaxCount());
		shader.DisableNoiseTexture(info.Renderer(), MassShader::SetupShaderParam_ps_noise_texture);
		target_texture.UnsetRenderTarget(info);
	}

	// -----------------------------------------------------------------
	/*
		@brief	�����̂̈ʒu�������_�����O����i�m�[�}���C�Y����Ă���̂Ŗ@���Ƃ��Ă��g����j
		@param	info				DrawInfo
		@param	res					�}�X�p�[�e�B�N�����\�[�X�I�u�W�F�N�g
		@param	shader				MassShader�I�u�W�F�N�g
		@param	target_texture		�����_�����O�����RenderingTexture�I�u�W�F�N�g
		@param	is_generateshape	�����`�󂩂ǂ���
		@return	�Ȃ�
	*/
	template<typename ResourceType>
	void MakeCubeTexture(DrawInfo &info, const ResourceType &res, MassShader &shader, ml::random &random, RenderingTexture &target_texture, bool is_generateshape = true){

		target_texture.SetRenderTarget(info);
		RenderStateManager::instance().ApplyMassParticle(info.Renderer(), MassShader::ProcType_Setup, MassShader::ShaderType_SetupCube);
#ifdef BM3_PLATFORM_psvita
			shader.EnablePixelShaderParam(info.Renderer());
#endif
		{
			ml::vector3d		temp;
			ml::vector3d	offset(random.f32(0.001f, 1.0f), random.f32(0.001f, 1.0f), 0 ,1);
			shader.SetVectorParam(info.Renderer(),		MassShader::SetupShaderParam_ps_noiseuv_offset,		offset.v_);
			shader.EnableNoiseTexture(info.Renderer(), MassShader::SetupShaderParam_ps_noise_texture);
			if(is_generateshape){
				SET_GENERATESHAPE_RADIUS(SetupShaderParam_shape_ps_radius);
			}else{
				SET_GOAL_RADIUS(SetupShaderParam_shape_ps_radius);
			}
		}
		vertex_buffer_.Draw(info.Renderer(), PrimitiveConst_Point, particle_count_.MaxCount());
		shader.DisableNoiseTexture(info.Renderer(), MassShader::SetupShaderParam_ps_noise_texture);
		target_texture.UnsetRenderTarget(info);
	}
#undef SET_GOAL_RADIUS
#undef SET_GOAL_ANGLE
#undef SET_GENERATESHAPE_RADIUS
#undef SET_GENERATESHAPE_ANGLE

	// -----------------------------------------------------------------
	/*
		@brief	�ʒu�A���x�̃e�N�X�`���𐶐�����
		@param	info		DrawInfo
		@param	res			�}�X�p�[�e�B�N�����\�[�X�I�u�W�F�N�g
		@param	shader		MassShader�I�u�W�F�N�g
		@param	random		ml::random
		@return	�Ȃ�
	*/
	template<typename ResourceType>
	void SetPointPositionVelocity(DrawInfo &info, const ResourceType &res, MassShader &shader, ml::random &random){

		{
			velocity_texture_[current_map_index_].SetRenderTarget(info);
			RenderStateManager::instance().ApplyMassParticle(info.Renderer(), MassShader::ProcType_Setup, MassShader::ShaderType_SetupSpeed);
#ifdef BM3_PLATFORM_psvita
			shader.EnablePixelShaderParam(info.Renderer());
#endif
			ml::vector3d	offset(random.f32(0.001f, 1.0f), random.f32(0.001f, 1.0f), 0 ,1);
			SetVelocity(info.Renderer(), res, shader, offset);
#ifdef BM3_PLATFORM_win_opengl
			org_velocity_texture_.Copy(info.Renderer(), velocity_texture_[current_map_index_]);
			velocity_texture_[current_map_index_].UnsetRenderTarget(info);
#endif
#ifdef BM3_PLATFORM_psvita
			velocity_texture_[current_map_index_].UnsetRenderTarget(info);
			GpuCopy(info, shader, org_velocity_texture_, velocity_texture_[current_map_index_]);
#endif
		}
		{
			position_texture_[current_map_index_].SetRenderTarget(info);
			RenderStateManager::instance().ApplyMassParticle(info.Renderer(), MassShader::ProcType_Setup, MassShader::ShaderType_Setup);
#ifdef BM3_PLATFORM_psvita
			shader.EnablePixelShaderParam(info.Renderer());
#endif
			ml::vector3d	offset(random.f32(0.001f, 1.0f), random.f32(0.001f, 1.0f), 0 ,1);
			SetPosition(info.Renderer(), res, shader, offset);
#ifdef BM3_PLATFORM_win_opengl
			org_position_texture_.Copy(info.Renderer(), position_texture_[current_map_index_]);
			position_texture_[current_map_index_].UnsetRenderTarget(info);
#endif
#ifdef BM3_PLATFORM_psvita
			position_texture_[current_map_index_].UnsetRenderTarget(info);
			GpuCopy(info, shader, org_position_texture_, position_texture_[current_map_index_]);
#endif
		}
	}

	// -----------------------------------------------------------------
	/*
		@brief	����̈ʒu�A���x�̃e�N�X�`���𐶐�����
		@param	info		DrawInfo
		@param	res			�}�X�p�[�e�B�N�����\�[�X�I�u�W�F�N�g
		@param	shader		MassShader�I�u�W�F�N�g
		@param	random		ml::random
		@return	�Ȃ�
	*/
	template<typename ResourceType>
	void SetSpherePositionVelocity(DrawInfo &info, const ResourceType &res, MassShader &shader, ml::random &random){

		MakeSphereTexture(info, res, shader, random, shape_texture_);
		SetShapeVelocity(info, res, shader, random, velocity_texture_[current_map_index_], shape_texture_, MassShader::ShaderType_SetupSpeedSphere);
		SetShapePosition(info, res, shader, random, position_texture_[current_map_index_], shape_texture_);
	}

	// -----------------------------------------------------------------
	/*
		@brief	�~���̈ʒu�A���x�̃e�N�X�`���𐶐�����
		@param	info		DrawInfo
		@param	res			�}�X�p�[�e�B�N�����\�[�X�I�u�W�F�N�g
		@param	shader		MassShader�I�u�W�F�N�g
		@param	random		ml::random
		@return	�Ȃ�
	*/
	template<typename ResourceType>
	void SetCylinderPositionVelocity(DrawInfo &info, const ResourceType &res, MassShader &shader, ml::random &random){

		MakeCylinderTexture(info, res, shader, random, shape_texture_);
		SetShapeVelocity(info, res, shader, random, velocity_texture_[current_map_index_], shape_texture_, MassShader::ShaderType_SetupSpeedCylinder);
		SetShapePosition(info, res, shader, random, position_texture_[current_map_index_], shape_texture_);
	}

	// -----------------------------------------------------------------
	/*
		@brief	�����̂̈ʒu�A���x�̃e�N�X�`���𐶐�����
		@param	info		DrawInfo
		@param	res			�}�X�p�[�e�B�N�����\�[�X�I�u�W�F�N�g
		@param	shader		MassShader�I�u�W�F�N�g
		@param	random		ml::random
		@return	�Ȃ�
	*/
	template<typename ResourceType>
	void SetCubePositionVelocity(DrawInfo &info, const ResourceType &res, MassShader &shader, ml::random &random){

		MakeCubeTexture(info, res, shader, random, shape_texture_);
		SetShapeVelocity(info, res, shader, random, velocity_texture_[current_map_index_], shape_texture_, MassShader::ShaderType_SetupSpeedCube);
		SetShapePosition(info, res, shader, random, position_texture_[current_map_index_], shape_texture_);
	}

	// -----------------------------------------------------------------
	/*
		@brief	���f���̈ʒu�A���x�̃e�N�X�`���𐶐�����
		@param	info		DrawInfo
		@param	res			�}�X�p�[�e�B�N�����\�[�X�I�u�W�F�N�g
		@param	shader		MassShader�I�u�W�F�N�g
		@param	random		ml::random
		@return	�Ȃ�
	*/
	template<typename ResourceType>
	void SetModelPositionVelocity(DrawInfo &info, const ResourceType &res, MassShader &shader, ml::random &random){

		ml::vector3d		model_param;
		if(CreateModelRenderingTexture(info.Renderer(), random, res.GenerateShape_Basic_ModelName(), res.GenerateShape_Area_Scale(), model_index_, model_position_, model_normal_, model_param)){
			// ���x�ƈʒu�Ń����_���̒l�𓯂��ɂ������̂�time��offset���g���܂킷
			ml::vector3d	offset(random.f32(0.001f, 1.0f), random.f32(0.001f, 1.0f), 0 ,1);
			SetModelVelocity(info, res, shader, velocity_texture_[current_map_index_], model_param, model_index_, model_normal_, offset);
			SetModelPosition(info, res, shader, position_texture_[current_map_index_], model_param, model_index_, model_position_ ,offset);
			// goal �ł��g������R�s�[�͊O�ł��
#ifdef BM3_PLATFORM_win_opengl
			org_position_texture_.Copy(info.Renderer(), position_texture_[current_map_index_]);
			position_texture_[current_map_index_].UnsetRenderTarget(info);
#endif
#ifdef BM3_PLATFORM_psvita
			position_texture_[current_map_index_].UnsetRenderTarget(info);
			GpuCopy(info, shader, org_position_texture_, position_texture_[current_map_index_]);
#endif
		}
		else{
			SetPointPositionVelocity(info, res, shader, random);
		}
	}

	// -----------------------------------------------------------------
	/*
		@brief	�ʒu�A���x�̃e�N�X�`���𐶐�����
		@param	info		DrawInfo
		@param	res			�}�X�p�[�e�B�N�����\�[�X�I�u�W�F�N�g
		@param	shader		MassShader�I�u�W�F�N�g
		@param	random		ml::random
		@return	�Ȃ�
	*/
	template<typename ResourceType>
	void SetupInitPositionVelocity(DrawInfo &info, const ResourceType &res, MassShader &shader, ml::random &random){
		switch(res.GenerateShape_Basic_GenerateShapeType()){
			case GenerateShapeTypeConst_Point:		SetPointPositionVelocity(	info, res, shader, random);	break;
			case GenerateShapeTypeConst_Sphere:		SetSpherePositionVelocity(	info, res, shader, random);	break;
			case GenerateShapeTypeConst_Cylinder:	SetCylinderPositionVelocity(info, res, shader, random);	break;
			case GenerateShapeTypeConst_Cube:		SetCubePositionVelocity(	info, res, shader, random);	break;
			case GenerateShapeTypeConst_Model:		SetModelPositionVelocity(	info, res, shader, random);	break;
			default:								ML_ASSERT(false);										break;
		}
	}

	// -----------------------------------------------------------------
	/*
		@brief	�����J���[�l�A�T�C�Y�e�N�X�`�������
		@param	info		DrawInfo
		@param	res			�}�X�p�[�e�B�N�����\�[�X�I�u�W�F�N�g
		@param	shader		MassShader�I�u�W�F�N�g
		@param	random		ml::random
		@return	�Ȃ�
	*/
	template<typename ResourceType>
	void SetupInitColorSize(DrawInfo &info, const ResourceType &res, MassShader &shader, Viewport & /*vp*/, ml::random &random){
		
		colorsize_texture_.SetRenderTarget(info);
		{
			if(res.Color_Center_RandomChannelSync()){	RenderStateManager::instance().ApplyMassParticle(info.Renderer(), MassShader::ProcType_Setup, MassShader::ShaderType_SetupSyncColor);	}
			else{										RenderStateManager::instance().ApplyMassParticle(info.Renderer(), MassShader::ProcType_Setup, MassShader::ShaderType_SetupColor);		}
#ifdef BM3_PLATFORM_psvita
				shader.EnablePixelShaderParam(info.Renderer());
#endif
			{
				ml::vector3d	temp;
				ml::color_rgba<float>	color;

				ml::vector3d	offset(random.f32(0.001f, 1.0f), random.f32(0.001f, 1.0f), 0 ,1);
				shader.SetVectorParam(info.Renderer(), MassShader::SetupShaderParam_ps_noiseuv_offset,		offset.v_);
				float	size_param[4] = {
					res.Scale_Value_InitialX() * parent_scale_.e_.x_ * 2.0f,	// �V���v���p�[�e�B�N���̃T�C�Y�ɍ��킹��
					res.Scale_Value_RangeX() * 2.0f,
					0.0f,
					0.0f
				};
				shader.SetVectorParam(info.Renderer(), MassShader::SetupShaderParam_ps_w_param,	size_param);

				color	= res.Color_Center_Value();
				shader.SetVectorParam(info.Renderer(), MassShader::SetupShaderParam_ps_initial_value,	color.v_);
				color	= res.Color_Center_Range();
				shader.SetVectorParam(info.Renderer(), MassShader::SetupShaderParam_ps_variance_scalar, color.v_);

				temp = ml::vector3d(2.0f, 2.0f, 2.0f, 2.0f);
				shader.SetVectorParam(info.Renderer(), MassShader::SetupShaderParam_ps_noise_scalar,	temp.v_);
				temp = ml::vector3d(-1.0f, -1.0f, -1.0f, -1.0f);
				shader.SetVectorParam(info.Renderer(), MassShader::SetupShaderParam_ps_noise_offset,	temp.v_);

				shader.EnableNoiseTexture(info.Renderer(), MassShader::SetupShaderParam_ps_noise_texture);
			}

			vertex_buffer_.Draw(info.Renderer(), PrimitiveConst_Point, particle_count_.MaxCount());
			
			shader.DisableNoiseTexture(info.Renderer(), MassShader::SetupShaderParam_ps_noise_texture);
		}
		colorsize_texture_.UnsetRenderTarget(info);
	}

	// -----------------------------------------------------------------
	/*
		@brief	�S�[���p�̃e�N�X�`�������
		@param	info		DrawInfo
		@param	res			�}�X�p�[�e�B�N�����\�[�X�I�u�W�F�N�g
		@param	shader		MassShader�I�u�W�F�N�g
		@param	random		ml::random
		@return	�Ȃ�
	*/
	template<typename ResourceType>
	void SetupInitGoal(DrawInfo &info, const ResourceType &res, MassShader &shader, ml::random &random){

		int size = PrimitiveTextureSize();	// goal_texture
		size += CalModelType(res.Goal_Basic_FieldGoalShapeType()==FieldGoalShapeTypeConst_Model, res.Goal_Basic_ModelName());	// index,position,normal
		if(VertexManager::instance().Alloc(size)){
			resource_size_	+= size;

			goal_texture_.Create(info.Renderer(), particle_count_.MaxWidthAlign(), particle_count_.MaxWidthAlign(), InternalTextureFormatConst_R16G16B16A16F);
			goal_texture_.Texture().SetAddress(InternalTextureAddressConst_Clamp);
			goal_texture_.Texture().SetFilter(InternalTextureFilterConst_Point);
			switch(res.Goal_Basic_FieldGoalShapeType()){
				case FieldGoalShapeTypeConst_Point:																				break;
				case FieldGoalShapeTypeConst_Sphere:	MakeSphereTexture(info, res, shader, random, goal_texture_, false);		break;
				case FieldGoalShapeTypeConst_Cylinder:	MakeCylinderTexture(info, res, shader, random, goal_texture_, false);	break;
				case FieldGoalShapeTypeConst_Cube:		MakeCubeTexture(info, res, shader, random, goal_texture_, false);		break;
				case FieldGoalShapeTypeConst_Model:
				{
					ml::vector3d		model_param;
					if(CreateModelRenderingTexture(info.Renderer(), random, res.Goal_Basic_ModelName(), res.Goal_Area_Scale(), goal_model_index_, goal_model_position_, goal_model_normal_, model_param)){
						ml::vector3d	offset(random.f32(0.001f, 1.0f), random.f32(0.001f, 1.0f), 0 ,1);
						SetModelPosition(info, res, shader, goal_texture_, model_param, goal_model_index_, goal_model_position_ ,offset);
						goal_texture_.UnsetRenderTarget(info);
					}
					else{
						goal_texture_.SetRenderTarget(info);
						ml::vector3d	offset(random.f32(0.001f, 1.0f), random.f32(0.001f, 1.0f), 0 ,1);
						RenderStateManager::instance().ApplyMassParticle(info.Renderer(), MassShader::ProcType_Setup, MassShader::ShaderType_Setup);
#ifdef BM3_PLATFORM_psvita
			shader.EnablePixelShaderParam(info.Renderer());
#endif
						SetPosition(info.Renderer(), res, shader, offset);
						goal_texture_.UnsetRenderTarget(info);
					}
				}
				break;
				default:	ML_ASSERT(false);				break;
			}
		}
	}

	// -----------------------------------------------------------------
	/*!
		@brief	�����l�e�N�X�`���𐶐�����
		@param	info	DrawInfo
		@pram	res		���\�[�X�N���X�̎Q��
		@param	random	�����_���N���X
		@return	�Ȃ�
	*/
	template<typename ResourceType>
	void SetupParticle(DrawInfo &info, const ResourceType &res, ml::random &random){

		MassShader &shader = RenderStateManager::instance().GetMassShader();

		RenderStateController::SetRenderState(info ,false);
		RenderStateController::SetBlendState(info, BlendTypeConst_Non);
#ifdef BM3_PLATFORM_psvita
			RenderStateManager::instance().SetShaderBlendType(BlendTypeConst_Non);
#endif
		RenderStateController::SetPointSpriteState(info, false, 1.0f, 1.0f);

		Viewport			vp;
		FramebufferStore	src_framebuffer;
		vp.Store();
		src_framebuffer.Store(info.Renderer());
		{
			
			Viewport		temp_vp;
			temp_vp.Set(0, 0, particle_count_.MaxWidthAlign(), particle_count_.MaxWidthAlign());
			vertex_buffer_.Bind(info.Renderer());
			{
				SetupInitPositionVelocity(info, res, shader, random);
				SetupInitColorSize(info, res, shader, temp_vp, random);
				if(res.Goal_Basic_FieldGoalShapeType()!=FieldGoalShapeTypeConst_Point){
					SetupInitGoal(info, res, shader, random);
				}
			}
		}
		
		src_framebuffer.Restore(info.Renderer());
		vp.Restore();
	}

	// -----------------------------------------------------------------
	/*!
		@brief	�`�悷��
		@param	info		DrawInfo
		@pram	shader_info	ShaderInfo
		@pram	res			���\�[�X�N���X�̎Q��
		@param	map_index	�g�p����e�N�X�`���̔ԍ�
		@return	�Ȃ�
	*/
	template<typename ResourceType>
	void RenderParticle(DrawInfo &info, ShaderInfo &shader_info, const ResourceType &res, int map_index){

		MassShader &shader = RenderStateManager::instance().GetMassShader();
		internal::IdeSupport::BeginRenderStateApply(info, res);
		{
			// �����_�[�X�e�C�g
			RenderStateController::SetPointSpriteState(info, true, 0.0f, 100.0f);
			RenderStateManager::instance().SetEnable<RenderState::DepthTest>(	info, true		);
			RenderStateManager::instance().SetEnable<RenderState::DepthWrite>(	info, res.Material_DrawControl_DepthWrite()		);
			RenderStateManager::instance().SetDepthTestType(					info, res.Material_DrawControl_DepthTestType()	);
			RenderStateManager::instance().SetEnable<RenderState::Culling>(		info, false);
			RenderStateManager::instance().SetBlendType(						info, res.Blend_Basic_BlendType()				);
			RenderStateManager::instance().SetEnable<RenderState::AlphaTest>(	info, res.Blend_Basic_AlphaTest()				);
			RenderStateManager::instance().SetAlphaTestType(					info, res.Blend_Basic_AlphaTestType(), res.Blend_Basic_AlphaRef());

#ifdef BM3_PLATFORM_psvita
			RenderStateManager::instance().SetShaderBlendType(res.Blend_Basic_BlendType());
#endif

			vertex_buffer_.Bind(info.Renderer());
			// �V�F�[�_�̐ݒ�
			if(res.Goal_Basic_FieldGoalShapeType()!=FieldGoalShapeTypeConst_Point){
				if(is_enabled_bm2_)	RenderStateManager::instance().ApplyMassParticle(info.Renderer(), MassShader::ProcType_Render, MassShader::ShaderType_RenderGoalBM2);
				else				RenderStateManager::instance().ApplyMassParticle(info.Renderer(), MassShader::ProcType_Render, MassShader::ShaderType_RenderGoal);
			}
			else{
				if(is_enabled_bm2_)	RenderStateManager::instance().ApplyMassParticle(info.Renderer(), MassShader::ProcType_Render, MassShader::ShaderType_RenderBM2);
				else				RenderStateManager::instance().ApplyMassParticle(info.Renderer(), MassShader::ProcType_Render, MassShader::ShaderType_Render);
			}

			// constant vertex shader
#ifdef BM3_PLATFORM_psvita
			shader.EnableVertexShaderParam(info.Renderer());
#endif
			{
				shader.SetVertexTexture(info.Renderer(), MassShader::RenderShaderParam_vs_goal_map,	goal_texture_.Texture());
				ml::vector3d	temp(
					res.Goal_Term_Start(), res.Goal_Term_End(), 0.0f, 0.0f
				);
				shader.SetVectorParam(info.Renderer(), MassShader::RenderShaderParam_vs_goal_param,		temp.v_);
				shader.SetVectorParam(info.Renderer(), MassShader::RenderShaderParam_vs_goal_position,	res.Goal_Position_Initial().v_);
			}
			{
				ml::matrix44	wvp;
				if(res.Basic_ParentNodeInfluence_MatrixParentNodeInfluenceType()==MatrixParentNodeInfluenceTypeConst_OnGenerateTime)	wvp.set_unit();
				else if(res.Basic_ParentNodeInfluence_MatrixParentNodeInfluenceType()==MatrixParentNodeInfluenceTypeConst_Off)			wvp.set_unit();
				else																													wvp	= parent_matrix_;

#ifdef BM3_TARGET_IDE
				wvp.mul(RenderStateManager::instance().GetWorldMatrix());
#endif
				wvp.add_translate(shader_info.offset_position_);
				wvp.mul(info.ViewMatrix());
				wvp.mul(info.ProjectionMatrix());
				wvp.transpose();
				shader.SetMatrixParam(info.Renderer(), MassShader::RenderShaderParam_vs_wvp_matrix,		wvp);
			}

			shader.SetVertexTexture(info.Renderer(), MassShader::RenderShaderParam_vs_position_map,	position_texture_[map_index].Texture());
			shader.SetVertexTexture(info.Renderer(), MassShader::RenderShaderParam_vs_velocity_map,	velocity_texture_[map_index].Texture());
			shader.SetVertexTexture(info.Renderer(), MassShader::RenderShaderParam_vs_colorsize_map,colorsize_texture_.Texture());
#ifdef BM3_PLATFORM_psvita
			shader.SetVertexTexture(info.Renderer(), MassShader::RenderShaderParam_vs_alpha_map,	alpha_texture_);
#endif
			
			{
				Viewport	vp;
				vp.Store();
#ifdef BM3_PLATFORM_win_opengl
				ml::vector3d	temp(prim_->scale_, 1.0f, (float)vp.Height(), 0.0f);
#endif
#ifdef BM3_PLATFORM_psvita
				ml::vector3d	temp(prim_->scale_ * 500.0f, 1.0f, (float)vp.Height(), 0.0f);
#endif
				shader.SetVectorParam(info.Renderer(), MassShader::RenderShaderParam_vs_size_param,			temp.v_);
			}
			{
				ml::vector3d	temp;
				ml::color_rgba<float>	color	= shader_info.color_scale_;
				color.e_.r_	= shader_info.color_scale_.e_.r_ * BM3_BLEND_PARENTCOLOR(prim_->color_center_.value_.e_.r_,parent_color_->e_.r_);
				color.e_.g_	= shader_info.color_scale_.e_.g_ * BM3_BLEND_PARENTCOLOR(prim_->color_center_.value_.e_.g_,parent_color_->e_.g_);
				color.e_.b_	= shader_info.color_scale_.e_.b_ * BM3_BLEND_PARENTCOLOR(prim_->color_center_.value_.e_.b_,parent_color_->e_.b_);
				color.e_.a_	= shader_info.color_scale_.e_.a_ * BM3_BLEND_PARENTALPHA(prim_->alpha_                    ,parent_color_->e_.a_);
				shader.SetVectorParam(info.Renderer(), MassShader::RenderShaderParam_vs_color_scale,		color.v_);
				shader.SetVectorParam(info.Renderer(), MassShader::RenderShaderParam_vs_offset_position,	prim_->position_.v_);
			}

#ifdef BM3_PLATFORM_win_opengl
			if(res.Goal_Basic_FieldGoalShapeType()==FieldGoalShapeTypeConst_Point){
				shader.SetVertexTexture(info.Renderer(), MassShader::RenderShaderParam_vs_alpha_map,	alpha_texture_);
			}
			else{
				shader.SetPixelTexture(info.Renderer(), MassShader::RenderShaderParam_ps_alpha_map,	alpha_texture_);
			}
#endif

			// constant pixel shader 
#ifdef BM3_PLATFORM_psvita
			shader.EnablePixelShaderParam(info.Renderer());
#endif
			shader.SetPixelTexture( info.Renderer(), MassShader::RenderShaderParam_ps_pattern_texture,pattern_texture_);
			{
				texture::HandleBase* texture = ResourceManager::instance().TextureHandleFromName(res.Texture_Layer1_PictureName(), info.Renderer());

				InternalTextureAddressConst address;
				InternalTextureFilterConst filter;
				switch(res.Texture_Layer1_TextureAddressType()){
					case TextureAddressTypeConst_Clamp:		address = InternalTextureAddressConst_Clamp;	break;
					case TextureAddressTypeConst_Repeat:	address = InternalTextureAddressConst_Repeat;	break;
					case TextureAddressTypeConst_Mirror:	address = InternalTextureAddressConst_Mirror;	break;
					default:		ML_ASSERT(false);		address = InternalTextureAddressConst_Unknown;	break;
				}
				switch(res.Texture_Layer1_TextureFilterType()){
					case TextureFilterTypeConst_Nearest:	filter = InternalTextureFilterConst_Point;	break;
					case TextureFilterTypeConst_Linear:		filter = InternalTextureFilterConst_Linear;	break;
					default:	ML_ASSERT(false);			filter = InternalTextureFilterConst_Unknown;break;
				}

				if(texture!=NULL){
					static InternalTexture dummy_texture;

					texture->Bind(bm3::TextureLayer::_1, info);
					dummy_texture.SetAddress(address);
					dummy_texture.SetFilter(filter);
					RenderStateManager::instance().SetMassParticleTexture(info.Renderer(), dummy_texture);
				}
				else{
					shader.EnableParticleTexture(info.Renderer(), address, filter);
				}
			}
		}
		internal::IdeSupport::EndRenderStateApply(info, res);

		// �`��
		{
			internal::IdeSupport::BeginDraw(info, res);
			vertex_buffer_.Draw(info.Renderer(), PrimitiveConst_Point, particle_count_.CurrentCount());
			internal::IdeSupport::EndDraw(info, res);
		}

		// ��n��
		{
			shader.UnsetVertexTexture(info.Renderer(), MassShader::RenderShaderParam_vs_position_map);
			shader.UnsetVertexTexture(info.Renderer(), MassShader::RenderShaderParam_vs_velocity_map);
			shader.UnsetVertexTexture(info.Renderer(), MassShader::RenderShaderParam_vs_colorsize_map);
			shader.UnsetPixelTexture(info.Renderer(), MassShader::RenderShaderParam_ps_pattern_texture);
			shader.UnsetVertexTexture(info.Renderer(), MassShader::RenderShaderParam_vs_goal_map);
#ifdef BM3_PLATFORM_win_opengl
			shader.UnsetPixelTexture(info.Renderer(), MassShader::RenderShaderParam_ps_alpha_map);
#endif
			shader.UnsetVertexTexture(info.Renderer(), MassShader::RenderShaderParam_vs_alpha_map);
			shader.DisableParticleTexture(info.Renderer());
		}
	}

	// -----------------------------------------------------------------
	/*!
		@brief	�X�V����B�i�ʒu�Ƒ��x�̃e�N�X�`��������������j
		@param	info		UpdateInfo
		@pram	res			���\�[�X�N���X�̎Q��
		@param	update_rate	�t���[�����[�g
		@return	�Ȃ�
	*/
	template<typename ResourceType>
	void UpdateParticle(DrawInfo &info, const ResourceType &res, float update_rate){

#if defined(BM3_PLATFORM_win_opengl)
		int map_index	= current_map_index_ ? 0 : 1;
		Viewport			vp;
		FramebufferStore	src_framebuffer;
		vp.Store();
		src_framebuffer.Store(info.Renderer());
		update_surface_[map_index].SetRenderTarget();
		{
			Viewport		temp_vp;
			temp_vp.Set(0, 0, particle_count_.MaxWidthAlign(), particle_count_.MaxWidthAlign());

			// �����_�[�X�e�C�g
			RenderStateController::SetRenderState(info, false);
			RenderStateManager::instance().SetBlendType(info, BlendTypeConst_Non);
			RenderStateController::SetPointSpriteState(info, false, 0.0f, 64.0f);

			MassShader &shader = RenderStateManager::instance().GetMassShader();
			vertex_buffer_.Bind(info.Renderer());
			// �V�F�[�_�̊m��
			if(res.Generate_Constant_EndlessRepeat()){
				if(res.Basic_ParentNodeInfluence_MatrixParentNodeInfluenceType()==MatrixParentNodeInfluenceTypeConst_OnGenerateTime)
					RenderStateManager::instance().ApplyMassParticle(info.Renderer(), MassShader::ProcType_Update, MassShader::ShaderType_UpdateParentInfl);
				else
					RenderStateManager::instance().ApplyMassParticle(info.Renderer(), MassShader::ProcType_Update, MassShader::ShaderType_Update);
			}
			else{
				if(res.Basic_ParentNodeInfluence_MatrixParentNodeInfluenceType()==MatrixParentNodeInfluenceTypeConst_OnGenerateTime)
					RenderStateManager::instance().ApplyMassParticle(info.Renderer(), MassShader::ProcType_Update, MassShader::ShaderType_UpdateOneShootParentInfl);
				else
					RenderStateManager::instance().ApplyMassParticle(info.Renderer(), MassShader::ProcType_Update, MassShader::ShaderType_UpdateOneShoot);
			}

			// �V�F�[�_�萔
			{
				if(res.Basic_ParentNodeInfluence_MatrixParentNodeInfluenceType()==MatrixParentNodeInfluenceTypeConst_OnGenerateTime){
					ml::matrix44	w;
#ifdef BM3_TARGET_IDE
					w	= parent_matrix_;
#else
					w.mul(parent_matrix_, RenderStateManager::instance().GetWorldMatrix());
#endif
					w.transpose();
					shader.SetMatrixParam(info.Renderer(), MassShader::UpdateShaderParam_ps_w_matrix,		w);
				}

				shader.SetPixelTexture(info.Renderer(), MassShader::UpdateShaderParam_ps_position_map,		position_texture_[current_map_index_].Texture());
				shader.SetPixelTexture(info.Renderer(), MassShader::UpdateShaderParam_ps_velocity_map,		velocity_texture_[current_map_index_].Texture());
				shader.SetPixelTexture(info.Renderer(), MassShader::UpdateShaderParam_ps_orig_position_map,	org_position_texture_.Texture());
				shader.SetPixelTexture(info.Renderer(), MassShader::UpdateShaderParam_ps_orig_velocity_map,	org_velocity_texture_.Texture());
				shader.EnableNoiseTexture(info.Renderer(), MassShader::UpdateShaderParam_ps_noise_texture);

				{
					ml::vector3d	temp;
					temp.e_.x_	= update_rate;
					shader.SetVectorParam(info.Renderer(), MassShader::UpdateShaderParam_ps_elapsed_time,	temp.v_);

					temp.add(res.Translate_Gravity_Value(), prim_->gravity_);	
					temp.e_.w_	= prim_->velocity_;
					shader.SetVectorParam(info.Renderer(), MassShader::UpdateShaderParam_ps_gravity,		temp.v_);

					temp.e_.x_	= res.Translate_Deceleration_Value() * prim_->deceleration_;;
					shader.SetVectorParam(info.Renderer(), MassShader::UpdateShaderParam_ps_deceleration,	temp.v_);

					temp.e_.x_	= 1.0f;
					temp.e_.y_	= res.Translate_Noise_Frequency() * prim_->noise_frequency_;
					temp.e_.z_	= res.Translate_Noise_StartRate();
					temp.e_.w_	= res.Translate_Noise_Scale() * prim_->noise_scale_;
					shader.SetVectorParam(info.Renderer(), MassShader::UpdateShaderParam_ps_noise_param,	temp.v_);
				}
			}

			// �`��
			{
				vertex_buffer_.Draw(info.Renderer(), PrimitiveConst_Point, particle_count_.CurrentCount());
			}

			// ��n��
			{
				shader.DisableNoiseTexture(info.Renderer(), MassShader::UpdateShaderParam_ps_noise_texture);
				shader.UnsetPixelTexture(info.Renderer(), MassShader::UpdateShaderParam_ps_position_map);
				shader.UnsetPixelTexture(info.Renderer(), MassShader::UpdateShaderParam_ps_velocity_map);
				shader.UnsetPixelTexture(info.Renderer(), MassShader::UpdateShaderParam_ps_orig_position_map);
				shader.UnsetPixelTexture(info.Renderer(), MassShader::UpdateShaderParam_ps_orig_velocity_map);
			}
		}
		update_surface_[map_index].UnsetRenderTarget();
		src_framebuffer.Restore(info.Renderer());
		vp.Restore();
#endif


#if defined(BM3_PLATFORM_psvita)
		int map_index	= current_map_index_ ? 0 : 1;
		Viewport			vp;
		FramebufferStore	src_framebuffer;
		vp.Store();
		src_framebuffer.Store(info.Renderer());
		
		position_texture_[map_index].SetRenderTarget(info);
		{
			Viewport		temp_vp;
			temp_vp.Set(0, 0, particle_count_.MaxWidthAlign(), particle_count_.MaxWidthAlign());

			// �����_�[�X�e�C�g
			RenderStateController::SetRenderState(info, false);
			RenderStateManager::instance().SetBlendType(info, BlendTypeConst_Non);
#ifdef BM3_PLATFORM_psvita
			RenderStateManager::instance().SetShaderBlendType(BlendTypeConst_Non);
#endif
			RenderStateController::SetPointSpriteState(info, false, 0.0f, 64.0f);

			MassShader &shader = RenderStateManager::instance().GetMassShader();
			vertex_buffer_.Bind(info.Renderer());
			// �V�F�[�_�̊m��
			if(res.Generate_Constant_EndlessRepeat()){
				if(res.Basic_ParentNodeInfluence_MatrixParentNodeInfluenceType()==MatrixParentNodeInfluenceTypeConst_OnGenerateTime)
					RenderStateManager::instance().ApplyMassParticle(info.Renderer(), MassShader::ProcType_UpdatePosition, MassShader::ShaderType_UpdatePositionParentInfl);
				else
					RenderStateManager::instance().ApplyMassParticle(info.Renderer(), MassShader::ProcType_UpdatePosition, MassShader::ShaderType_UpdatePosition);
			}
			else{
				if(res.Basic_ParentNodeInfluence_MatrixParentNodeInfluenceType()==MatrixParentNodeInfluenceTypeConst_OnGenerateTime)
					RenderStateManager::instance().ApplyMassParticle(info.Renderer(), MassShader::ProcType_UpdatePosition, MassShader::ShaderType_UpdatePositionOneShootParentInfl);
				else
					RenderStateManager::instance().ApplyMassParticle(info.Renderer(), MassShader::ProcType_UpdatePosition, MassShader::ShaderType_UpdatePositionOneShoot);
			}
#ifdef BM3_PLATFORM_psvita
			shader.EnablePixelShaderParam(info.Renderer());
#endif

			// �V�F�[�_�萔
			{
				if(res.Basic_ParentNodeInfluence_MatrixParentNodeInfluenceType()==MatrixParentNodeInfluenceTypeConst_OnGenerateTime){
					ml::matrix44	w;
					w.mul(parent_matrix_, RenderStateManager::instance().GetWorldMatrix());
					w.transpose();
					shader.SetMatrixParam(info.Renderer(), MassShader::UpdatePositionShaderParam_ps_w_matrix,		w);
				}

				shader.SetPixelTexture(info.Renderer(), MassShader::UpdatePositionShaderParam_ps_position_map,		position_texture_[current_map_index_].Texture());
				shader.SetPixelTexture(info.Renderer(), MassShader::UpdatePositionShaderParam_ps_velocity_map,		velocity_texture_[current_map_index_].Texture());
				shader.SetPixelTexture(info.Renderer(), MassShader::UpdatePositionShaderParam_ps_orig_position_map,	org_position_texture_.Texture());

				{
					ml::vector3d	temp;
					temp.e_.x_	= update_rate;
					shader.SetVectorParam(info.Renderer(), MassShader::UpdatePositionShaderParam_ps_elapsed_time,	temp.v_);
				}
			}

			// �`��
			{
				vertex_buffer_.Draw(info.Renderer(), PrimitiveConst_Point, particle_count_.CurrentCount());
			}

			// ��n��
			{
				shader.UnsetPixelTexture(info.Renderer(), MassShader::UpdatePositionShaderParam_ps_position_map);
				shader.UnsetPixelTexture(info.Renderer(), MassShader::UpdatePositionShaderParam_ps_velocity_map);
				shader.UnsetPixelTexture(info.Renderer(), MassShader::UpdatePositionShaderParam_ps_orig_position_map);
			}
		}
		position_texture_[map_index].UnsetRenderTarget(info);

		velocity_texture_[map_index].SetRenderTarget(info);
		{
			Viewport		temp_vp;
			temp_vp.Set(0, 0, particle_count_.MaxWidthAlign(), particle_count_.MaxWidthAlign());

			// �����_�[�X�e�C�g
			RenderStateController::SetRenderState(info, false);
			RenderStateManager::instance().SetBlendType(info, BlendTypeConst_Non);
			RenderStateController::SetPointSpriteState(info, false, 0.0f, 64.0f);

			MassShader &shader = RenderStateManager::instance().GetMassShader();
			vertex_buffer_.Bind(info.Renderer());
			// �V�F�[�_�̊m��
			if(res.Generate_Constant_EndlessRepeat()){
				if(res.Basic_ParentNodeInfluence_MatrixParentNodeInfluenceType()==MatrixParentNodeInfluenceTypeConst_OnGenerateTime)
					RenderStateManager::instance().ApplyMassParticle(info.Renderer(), MassShader::ProcType_UpdateVelocity, MassShader::ShaderType_UpdateVelocityParentInfl);
				else
					RenderStateManager::instance().ApplyMassParticle(info.Renderer(), MassShader::ProcType_UpdateVelocity, MassShader::ShaderType_UpdateVelocity);
			}
			else{
				if(res.Basic_ParentNodeInfluence_MatrixParentNodeInfluenceType()==MatrixParentNodeInfluenceTypeConst_OnGenerateTime)
					RenderStateManager::instance().ApplyMassParticle(info.Renderer(), MassShader::ProcType_UpdateVelocity, MassShader::ShaderType_UpdateVelocityOneShootParentInfl);
				else
					RenderStateManager::instance().ApplyMassParticle(info.Renderer(), MassShader::ProcType_UpdateVelocity, MassShader::ShaderType_UpdateVelocityOneShoot);
			}
#ifdef BM3_PLATFORM_psvita
			shader.EnablePixelShaderParam(info.Renderer());
#endif

			// �V�F�[�_�萔
			{
				if(res.Basic_ParentNodeInfluence_MatrixParentNodeInfluenceType()==MatrixParentNodeInfluenceTypeConst_OnGenerateTime){
					ml::matrix44	w;
					w.mul(parent_matrix_, RenderStateManager::instance().GetWorldMatrix());
					w.transpose();
					shader.SetMatrixParam(info.Renderer(), MassShader::UpdateVelocityShaderParam_ps_w_matrix,		w);
				}

				shader.SetPixelTexture(info.Renderer(), MassShader::UpdateVelocityShaderParam_ps_position_map,		position_texture_[current_map_index_].Texture());
				shader.SetPixelTexture(info.Renderer(), MassShader::UpdateVelocityShaderParam_ps_velocity_map,		velocity_texture_[current_map_index_].Texture());
				shader.SetPixelTexture(info.Renderer(), MassShader::UpdateVelocityShaderParam_ps_orig_position_map,	org_position_texture_.Texture());
				shader.SetPixelTexture(info.Renderer(), MassShader::UpdateVelocityShaderParam_ps_orig_velocity_map,	org_velocity_texture_.Texture());
				shader.EnableNoiseTexture(info.Renderer(), MassShader::UpdateVelocityShaderParam_ps_noise_texture);

				{
					ml::vector3d	temp;
					temp.e_.x_	= update_rate;
					shader.SetVectorParam(info.Renderer(), MassShader::UpdateVelocityShaderParam_ps_elapsed_time,	temp.v_);

					temp.add(res.Translate_Gravity_Value(), prim_->gravity_);	
					temp.e_.w_	= prim_->velocity_;
					shader.SetVectorParam(info.Renderer(), MassShader::UpdateVelocityShaderParam_ps_gravity,		temp.v_);

					temp.e_.x_	= res.Translate_Deceleration_Value() * prim_->deceleration_;;
					shader.SetVectorParam(info.Renderer(), MassShader::UpdateVelocityShaderParam_ps_deceleration,	temp.v_);

					temp.e_.x_	= 1.0f;
					temp.e_.y_	= res.Translate_Noise_Frequency() * prim_->noise_frequency_;
					temp.e_.z_	= res.Translate_Noise_StartRate();
					temp.e_.w_	= res.Translate_Noise_Scale() * prim_->noise_scale_;
					shader.SetVectorParam(info.Renderer(), MassShader::UpdateVelocityShaderParam_ps_noise_param,	temp.v_);
				}
			}

			// �`��
			{
				vertex_buffer_.Draw(info.Renderer(), PrimitiveConst_Point, particle_count_.CurrentCount());
			}

			// ��n��
			{
				shader.DisableNoiseTexture(info.Renderer(), MassShader::UpdateVelocityShaderParam_ps_noise_texture);
				shader.UnsetPixelTexture(info.Renderer(), MassShader::UpdateVelocityShaderParam_ps_position_map);
				shader.UnsetPixelTexture(info.Renderer(), MassShader::UpdateVelocityShaderParam_ps_velocity_map);
				shader.UnsetPixelTexture(info.Renderer(), MassShader::UpdateVelocityShaderParam_ps_orig_position_map);
				shader.UnsetPixelTexture(info.Renderer(), MassShader::UpdateVelocityShaderParam_ps_orig_velocity_map);
			}
		}
		velocity_texture_[map_index].UnsetRenderTarget(info);
		src_framebuffer.Restore(info.Renderer());
		vp.Restore();
#endif
	}
//#define BM3_DEBUG_MASS

	// -----------------------------------------------------------------
	template<typename ResourceType>
	void	End(DrawInfo &draw_info, ShaderInfo &shader_info, VertexContext & /*ctx*/, int /*child_count*/, int /*push_count*/, const ResourceType &res){

		if(vertex_buffer_.IsInitialized()==false){
			InitializeParam(draw_info.Renderer(), res);
		}

		if(vertex_buffer_.IsInitialized()){

#ifdef BM3_PLATFORM_psvita
			// store user render target
			StoreRenderTarget(draw_info);
#endif

			float	update_rate	= prim_->update_frame_;
			// initialize
			if(is_initialized==false){
				is_initialized	= true;
				ml::random	rand;
				{
					ml::random::seed_info rsi;
					rsi.set1(ml::ui32(this));
					rand.set_seed(rsi);
				}
				current_map_index_	= 0;
				SetupParticle(draw_info, res, rand);
				update_rate	= 0.0f;
			}
			{
				// update
				particle_count_.Update(prim_->generate_scale_);
				int map_index = current_map_index_;
				if(is_updated_){
					UpdateParticle(draw_info, res, update_rate);
					current_map_index_	= current_map_index_ ? 0 : 1;
					is_updated_	= false;
				}

				// return user scene
				RestoreRenderTarget(draw_info);

				// draw
				RenderParticle(draw_info, shader_info, res, map_index);

#ifdef BM3_DEBUG_MASS
				Debug(draw_info);
#endif
			}
			// clean
			vertex_buffer_.Unbind(draw_info.Renderer());
			RenderStateController::UnetPointSpriteState(draw_info);
			RenderStateController::RestoreRenderState(draw_info);
			RenderStateManager::instance().SetWorldMatrix(ml::matrix44());

			// profile
			ProfileManager::instance().Draw_AddVertexProcessCount(particle_count_.CurrentCount());
			ProfileManager::instance().Draw_AddPrimitiveProcessCount(EmitterTypeConst_MassParticle, particle_count_.CurrentCount());
			ProfileManager::instance().Draw_AddVertexArrayDrawCount(1);
		}

	}
#ifdef BM3_DEBUG_MASS
	// -----------------------------------------------------------------
	void Debug(DrawInfo & /*draw_info*/){

		typedef struct _VERTEX{
			float	x,y,z;
			float	s;
			float	r,g,b,a;
		} VERTEX;

		enum{
			TEST_POS,
			TEST_SIZE,
			TEST_COL,
//			TEST_TEX,
			TEST_MAX
		};

		static const char* s_attrib_name[] = {
			"atPosition",
			"atSize",
			"atColor",
		};


		static const char* fragment_shader = "	\
uniform sampler2D BaseTexture;	\
varying vec4 vaColor;	\
void main(){			\
    gl_FragColor =  vaColor * texture2D(BaseTexture, gl_PointCoord);	\
}";

		static const char* vertex_shader = "\
varying vec4 vaColor;				\
attribute vec4 atPosition;			\
attribute float atSize;				\
attribute vec4 atColor;				\
									\
void main(){						\
    gl_Position = vec4(atPosition.xyz, 1.0f);	\
    gl_PointSize =  atSize;					\
    vaColor =  atColor;												\
    vaColor.a +=  0.2f;												\
}";

		static bool	is_init = false;
		static const int vertex_count = 7;

		static GLuint	s_vertex_buffer;
		static void*	s_vertex_pointer_;
		static GLuint	s_fragment_shader;
		static GLuint	s_vertex_shader;
		static GLuint	s_program_object;
		static GLint	s_attrib_pointer[TEST_MAX];


		if(is_init==false){
			is_init	= true;
			glGenBuffers(1, &s_vertex_buffer);
			glBindBuffer(GL_ARRAY_BUFFER, s_vertex_buffer);	// �C���f�b�N�X�o�b�t�@�̂Ƃ���GL_ELEMENT_ARRAY_BUFFER
			glBufferData(GL_ARRAY_BUFFER, sizeof(VERTEX) * vertex_count, NULL, GL_STATIC_DRAW);	// ���_�ݒ肵�ĂȂ��̂��������ŌĂяo���Ă����̂��E�E�E
			{
				void* buffer = malloc(sizeof(VERTEX) * vertex_count);
				VERTEX* v = (VERTEX*)(buffer);
				float	size = 80.0f;
				float	x = -0.8f;
				float	y = -0.8f;
				float	step = 0.3f;
				for(int i=0 ; i<vertex_count ; ++i){
					v->x	= x;	v->y	= y;	v->z	= 0.0f;
					v->s	= size;
					v->r	= 1.0f;	v->g	= 1.0f;	v->b	= 1.0f;	v->a	= 1.0f;
					x += step;
					v++;
				}
				glBindBuffer(GL_ARRAY_BUFFER, s_vertex_buffer);	// �C���f�b�N�X�o�b�t�@�̂Ƃ���GL_ELEMENT_ARRAY_BUFFER
				glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(VERTEX) * vertex_count, buffer);	// ���_�ݒ肵�ĂȂ��̂��������ŌĂяo���Ă����̂��E�E�E
				free(buffer);
			}

			s_vertex_shader	= glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(s_vertex_shader, 1, &vertex_shader, NULL);
			glCompileShader(s_vertex_shader);
			s_fragment_shader	= glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(s_fragment_shader, 1, &fragment_shader, NULL);
			glCompileShader(s_fragment_shader);

			s_program_object	= glCreateProgram();
			glAttachShader(s_program_object, s_fragment_shader);
			glAttachShader(s_program_object, s_vertex_shader);

			for(int i=0 ; i<TEST_MAX ; ++i){
				s_attrib_pointer[i]	= i;
				glBindAttribLocation(s_program_object, s_attrib_pointer[i], s_attrib_name[i]);
			}
			glLinkProgram(s_program_object);
		}

		glBindBuffer(GL_ARRAY_BUFFER, s_vertex_buffer);	// �C���f�b�N�X�o�b�t�@�̂Ƃ���GL_ELEMENT_ARRAY_BUFFER
		glUseProgram(s_program_object);
		{
#if 1
			int offset	= 0;
			glEnableVertexAttribArray(s_attrib_pointer[TEST_POS]);
			glVertexAttribPointer(s_attrib_pointer[TEST_POS], 3, GL_FLOAT, GL_FALSE, sizeof(VERTEX), (const void*)offset);
			offset	+= sizeof(float) * 3;
			glEnableVertexAttribArray(s_attrib_pointer[TEST_SIZE]);
			glVertexAttribPointer(s_attrib_pointer[TEST_SIZE], 1, GL_FLOAT, GL_FALSE, sizeof(VERTEX), (const void*)offset);
			offset	+= sizeof(float) * 1;
			glEnableVertexAttribArray(s_attrib_pointer[TEST_COL]);
			glVertexAttribPointer(s_attrib_pointer[TEST_COL], 4, GL_FLOAT, GL_FALSE, sizeof(VERTEX), (const void*)offset);
			offset	+= sizeof(float) * 4;
#else
			VERTEX* v = (VERTEX*)(s_vertex_pointer_);
			int offset	= 0;
			glEnableVertexAttribArray(s_attrib_pointer[TEST_POS]);
			glVertexAttribPointer(s_attrib_pointer[TEST_POS], 3, GL_FLOAT, GL_FALSE, sizeof(VERTEX), &v->x);
			offset	+= sizeof(float) * 3;
			glEnableVertexAttribArray(s_attrib_pointer[TEST_SIZE]);
			glVertexAttribPointer(s_attrib_pointer[TEST_SIZE], 1, GL_FLOAT, GL_FALSE, sizeof(VERTEX), &v->s);
			offset	+= sizeof(float) * 1;
			glEnableVertexAttribArray(s_attrib_pointer[TEST_COL]);
			glVertexAttribPointer(s_attrib_pointer[TEST_COL], 4, GL_FLOAT, GL_FALSE, sizeof(VERTEX), &v->r);
			offset	+= sizeof(float) * 4;
#endif
		}

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_POINT_SPRITE);
		glEnable(GL_PROGRAM_POINT_SIZE);

		int count = 0;
		GLuint	handle	= glGetUniformLocation(s_program_object, "BaseTexture");
		glActiveTexture(GL_TEXTURE0 + handle);
		{
			glBindTexture(GL_TEXTURE_2D, position_texture_[0].Texture().Handle());
			glUniform1i(handle, handle);
			glDrawArrays(PrimitiveConst_Point, count, 1);
		}
		count++;
		{
			glBindTexture(GL_TEXTURE_2D, position_texture_[1].Texture().Handle());
			glUniform1i(handle, handle);
			glDrawArrays(PrimitiveConst_Point, count, 1);
		}
		count++;
		{
			glBindTexture(GL_TEXTURE_2D, velocity_texture_[0].Texture().Handle());
			glUniform1i(handle, handle);
			glDrawArrays(PrimitiveConst_Point, count, 1);
		}
		count++;
		{
			glBindTexture(GL_TEXTURE_2D, velocity_texture_[1].Texture().Handle());
			glUniform1i(handle, handle);
			glDrawArrays(PrimitiveConst_Point, count, 1);
		}
		count++;
		{
			glBindTexture(GL_TEXTURE_2D,colorsize_texture_.Texture().Handle());
			glUniform1i(handle, handle);
			glDrawArrays(PrimitiveConst_Point, count, 1);
		}
		count++;
		{
			glBindTexture(GL_TEXTURE_2D, alpha_texture_.Handle());
			glUniform1i(handle, handle);
			glDrawArrays(PrimitiveConst_Point, count, 1);
		}
		count++;
	}
#endif

	// -----------------------------------------------------------------
	/*
		@brief	�X�V�����L���b�V�����Ă����B
		@param	prim				massparticle primitive structer
		@param	draw_info			DrawInfo
		@param	packet_draw_info	���g�p
		@param	res					�}�X�p�[�e�B�N�����\�[�X�I�u�W�F�N�g
		@param	is_updated			�X�V�������ǂ���
		@return	�Ȃ�
	*/
	template<typename PrimitiveType, typename ResourceType>
	void	PushRange(const PrimitiveType *prim, DrawInfo & /*draw_info*/, const PacketDrawInfo &pdi, const ResourceType & /*res*/, bool is_updated){
		
		PushRange(prim, is_updated, false, pdi);
	}

	// -----------------------------------------------------------------
	/*
		@brief	�X�V�����L���b�V�����Ă����BBM2�݊���
		@param	prim				massparticle primitive structer
		@param	draw_info			DrawInfo
		@param	packet_draw_info	���g�p
		@param	res					�}�X�p�[�e�B�N�����\�[�X�I�u�W�F�N�g
		@param	is_updated			�X�V�������ǂ���
		@return	�Ȃ�
	*/
	template<typename PrimitiveType, typename ResourceType>
	void	PushRangeBm2(const PrimitiveType *prim, DrawInfo & /*draw_info*/, const PacketDrawInfo &pdi, const ResourceType & /*res*/, bool is_updated){
		
		PushRange(prim, is_updated, true, pdi);
	}
		
	// -----------------------------------------------------------------
	/*
		@brief	�X�V�����L���b�V�����Ă����BBM2�݊���
		@param	prim				massparticle primitive structer
		@param	is_updated			�X�V�������ǂ���
		@param	is_enabled_bm2		bm2�݊����邩�ǂ���
		@return	�Ȃ�
	*/
	template<typename PrimitiveType>
	void PushRange(const PrimitiveType *prim, const bool is_updated ,const bool is_enabled_bm2, const PacketDrawInfo &pdi){

		ML_MAKEVERTEX_TIMER;

		parent_matrix_	= prim->GetParentMatrix();
		parent_scale_	= prim->GetParentScale();
		parent_color_	= pdi.parent_color_;
		is_updated_		= is_updated;
		prim_			= prim;
		is_enabled_bm2_	= is_enabled_bm2;
	}

		
	// -----------------------------------------------------------------
	/*
		@brief	���_�쐬�J�n
		@param	root			���[�g
		@param	emitter_index	�G�~�b�^�[�ԍ�
		@return	�Ȃ�
	*/
	void BeginMakeVertex(obj::RootNodeBase &/*root*/, int /*emitter_index*/){
	}

	// -----------------------------------------------------------------
	/*
		@brief	���_�쐬�I��
		@param	root			���[�g
		@param	emitter_index	�G�~�b�^�[�ԍ�
		@return	�Ȃ�
	*/
	void EndMakeVertex(obj::RootNodeBase &/*root*/, int /*emitter_index*/){
	}

#ifdef BM3_PLATFORM_psvita
	void PostMakeVertex(obj::RootNodeBase & /*root*/, int /*emitter_index*/, int /*total_active_count*/){
	}
#endif

	// -----------------------------------------------------------------
	/*
		@brief	���_�o�b�t�@�̃T�C�Y��Ԃ�
		@param	child_count	�p�[�e�B�N���̐�
		@return	���_�o�b�t�@�̃T�C�Y
	*/
	int VertexBufferSize(int child_count){ return child_count * VertexPerPrimitiveCount * vertex_element_size_; }
	/*
		@brief	���_�o�b�t�@�̃T�C�Y��Ԃ�
		@param	child_count	�p�[�e�B�N���̐�
		@param	push_count	�e������o���ꂽ��
		@return	���_�o�b�t�@�̃T�C�Y
	*/
	int VertexBufferSize(int child_count, int /*push_count*/){ return VertexBufferSize(child_count); }
#ifdef BM3_PLATFORM_psvita
	/*
		@brief	�C���f�b�N�X�o�b�t�@�̃T�C�Y��Ԃ�
		@param	child_count	�p�[�e�B�N���̐�
		@param	push_count	�e������o���ꂽ��
		@return	���_�o�b�t�@�̃T�C�Y
	*/
	int IndexBufferSize(int child_count, int /*push_count*/){ return 0; }
	/*
		@brief	�C���f�b�N�X�o�b�t�@�̃T�C�Y��Ԃ�
		@param	child_count	�p�[�e�B�N���̐�
		@param	push_count	�e������o���ꂽ��
		@return	���_�o�b�t�@�̃T�C�Y
	*/
	int IndexBufferSizeAlign(int child_count, int /*push_count*/){ return 0; }
#endif
	/*
		@brief	���_�o�b�t�@�ƃe�N�X�`���̃g�[�^���T�C�Y��Ԃ�
		@param	child_count	�p�[�e�B�N���̐�
		@param	push_count	�e������o���ꂽ��
		@return	���_�o�b�t�@�ƃe�N�X�`���̃g�[�^���T�C�Y
	*/
	int TotalVertexBufferSize(int child_count, int /*push_count*/){ return VertexBufferSize(child_count) + resource_size_; }

	/*
		���g�p
	*/
	template<typename ResourceType>
	void GetVertexInfo(VertexInfo & /*info*/, int /*child_count*/, int /*push_count*/, const ResourceType & /*res*/){
/*	
		info.SetCount(VertexPerPrimitiveCount*child_count);
		info.SetSize(vertex_element_size_);
		info.SetCountPerPrimitive(VertexPerPrimitiveCount);

		{
			OnBeginDraw_SetRenderTypeNoTextureDrawType(res);
			SetTextureType(res);
			policy::DefaultRenderState::GetVertexInfo(info);
		}
*/
	}
		
	// -----------------------------------------------------------------
	/*
		���g�p
	*/
	void DestroyResource(){

		Finalize();
	}
	// -----------------------------------------------------------------
	/*
		���g�p
	*/
	template<typename ResourceType>
	void RestoreResource(RestoreInfo & /*info*/, const ResourceType & /*res*/){
	}

protected:

private:
	enum{
		VertexPerPrimitiveCount	= 1
	};

	struct VertexType{
		TextureCoordinate	texture_;
	};

	enum{
		vertex_element_size_	= sizeof(VertexType)
	};
		
	VertexBuffer					vertex_buffer_;

	int					current_map_index_;

	RenderingTexture	position_texture_[2];	//< �ʒu�e�N�X�`���i�_�u���o�b�t�@�̂���2���j
	RenderingTexture	velocity_texture_[2];	//< ���x�e�N�X�`���i�_�u���o�b�t�@�̂���2���j
	RenderingTexture	org_position_texture_;	//< �����ʒu�e�N�X�`���iw is current life�j
	RenderingTexture	org_velocity_texture_;	//< �������x�e�N�X�`���i w is lifespan�j
	RenderingTexture	colorsize_texture_;		//< �J���[�T�C�Y�e�N�X�`��(r,g,b,size)
	InternalTexture		alpha_texture_;			//< �A���t�@�A�j���[�V�����e�N�X�`��
	InternalTexture		pattern_texture_;		//< �e�N�X�`���p�^�[���e�N�X�`��
	RenderingTexture	shape_texture_;			//< �����`��e�N�X�`��
	InternalTexture		model_index_;			//< ���f���C���f�b�N�X�e�N�X�`��
	InternalTexture		model_position_;		//< ���f���ʒu�e�N�X�`��
	InternalTexture		model_normal_;			//< ���f���@���e�N�X�`��

	RenderingTexture	goal_texture_;			//< �S�[���e�N�X�`��
	InternalTexture		goal_model_index_;		//< �S�[�����f���C���f�b�N�X�e�N�X�`��
	InternalTexture		goal_model_position_;	//< �S�[�����f���ʒu�e�N�X�`��
	InternalTexture		goal_model_normal_;		//< �S�[�����f���@���e�N�X�`��
		
#if defined(BM3_PLATFORM_win_opengl)
	InternalSurface		update_surface_[2];		//< �A�b�v�f�[�g�t���[���o�b�t�@�I�u�W�F�N�g
#endif

	ParticleCountManager	particle_count_;	//< �p�[�e�B�N���Ǘ�

	bool	is_initialized;						//< ���������ꂽ���ǂ����t���O
	bool	is_enabled_reset_;					//< ���Z�b�g���邩�ǂ���

	ml::matrix44						parent_matrix_;	//< �e�}�g���b�N�X
	ml::vector3d						parent_scale_;	//< �e�X�P�[��
	ColorType*							parent_color_;
	const prim::massparticle::Primitive	*prim_;			//< �}�X�p�[�e�B�N���v���~�e�B�u�̃|�C���^
	bool								is_updated_;	//< �X�V���ꂽ���ǂ����̃t���O
	int									resource_size_;	//< �g�p����Ă��郊�\�[�X�̃T�C�Y
	bool								is_enabled_bm2_;	//< bm2�݊�����邩�ǂ���
};

#undef BM3_BLEND_PARENTALPHA
#undef BM3_BLEND_PARENTCOLOR
	
}	// namespace draw
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_SDK_INC_BM3_CORE_DRAW_BM3_DRAW_MASSPARTICLEIMPL_H
