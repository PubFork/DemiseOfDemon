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

/// @brief MassParticleクラス
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
		パーティクルの数を制御するクラス
		ユーザーにより生成数を制御できるようにするため最大数が描画する数と一致しないことがある
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
			@brief	初期化。パーティクルの数を確定する
			@param	info	InitializeInfo
			@param	count	パーティクルの数
			@return	なし
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
			@brief	生成割合を考慮してパーティクルの数を更新する
			@param	scale	生成割合
			@return	なし
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
		int min_width_;	// 生成数の割合を制御しない場合の幅
		int min_count_;
		int max_width_;	// 生成数の割合を制御した場合の幅
		int max_count_;
		int current_count_;	// 現在描画する数
		int min_width_align_;	// 生成数の割合を制御しない場合の幅
		int max_width_align_;	// 生成数の割合を制御した場合の幅
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
		@brief	パーティクルに使用するテクスチャのメモリサイズ
		@return	テクスチャのメモリサイズ
	*/
	int PrimitiveTextureSize() const {	return InternalTexture::FormatToByte(InternalTextureFormatConst_R16G16B16A16F) * particle_count_.MaxCountAlign();	}

	// -----------------------------------------------------------------
	/*
		@brief	マスパーティクルで使用するモデルのメモリサイズを返す
		@param	is_enable	モデルタイプかどうか
		@param	name		モデル名
		@return	マスパーティクルで使用するモデルのメモリサイズ
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
		@brief	マスパーティクルで使用するメモリサイズを返す
		@return	マスパーティクルで使用するメモリサイズ
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
		@brief	マスパーティクルで使用するテクスチャの初期化
		@param	renderer_info	RendererInfo
		@param	res				リソースクラス
		@return	なし
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
		@brief	頂点バッファを作成する
		@param	info	RendererInfo
		@return	パーティクルが0だとfalse。
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
				// ピクセルの真ん中を指すように半ピクセル分ずらす
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
		@brief	テクスチャのメモリを確保する
		@param	info	RendererInfo
		@param	res		リソースクラス
		@return	なし
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
		@brief	モデルを使用した場合に使用するテクスチャを生成する
		@param	info		RendererInfo
		@param	random		ランダムクラス
		@param	name		モデルの名前
		@param	scale		モデルのスケール
		@param	index		インデックス用テクスチャ
		@param	position	位置用テクスチャ
		@param	normal		法線用テクスチャ
		@param	model_param	モデルのパラメータを格納するml::vector3d
		@return	モデルのイメージを取得できないときfalse
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
		@brief	モデル用テクスチャの初期化
		@param	vai			モデル頂点情報
		@param	random		ランダムオブジェクト
		@param	height		テクスチャの高さ
		@param	scale		モデルのスケール
		@param	index		インデックステクスチャ
		@param	position	位置テクスチャ
		@param	normal		法線テクスチャ
		@return	なし
	*/
	void InitModelTexture(m3dg::VertexArrayInfo &vai, ml::random &random, int height, const ml::vector3d &scale, InternalTexture &index, InternalTexture &position, InternalTexture &normal){

		// インデックステクスチャを作成
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
		// 位置テクスチャの作成
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
		// 法線テクスチャの作成
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
		@brief	アルファテクスチャの作成
		@param	res	マスパーティクルリソースオブジェクト
		@return	テクスチャ作成の成否
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
		@brief	パターンテクスチャの作成
		@param	res	マスパーティクルリソースオブジェクト
		@return	テクスチャ作成の成否
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
		@brief	初期化。パーティクルの数、使用するメモリサイズを計算する。
		@param	info	InitializeInfo
		@param	res		マスパーティクルリソースオブジェクト
		@return	なし
	*/
	template<typename ResourceType>
	void	Initialize(InitializeInfo &info, const ResourceType &res){
		particle_count_.Initialize(info, res.Generate_Constant_GenerateCount());
	}

	// -----------------------------------------------------------------
	/*
		@brief	初期化。使用するテクスチャのメモリを確保する
		@param	info	InitInfo
		@param	res		マスパーティクルリソースオブジェクト
		@return	なし
	*/
	template<typename ResourceType>
	void	Initialize(InitInfo &info, const ResourceType &res){
		if(vertex_buffer_.IsInitialized()){
			InitializeParam(info.Renderer(), res);
		}
	}
		
	// -----------------------------------------------------------------
	/*
		@brief	終了処理。メモリの解放
		@return	なし
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
		@brief	初期状態に戻す
		@param	info	ResetInfo
		@return	なし
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
		@brief	描画前処理
		@param	draw_info	DrawInfo
		@param	res			マスパーティクルリソースオブジェクト
		@return	なし
	*/
	template<typename ResourceType>
	void	Begin(DrawInfo & /*draw_info*/, const ResourceType & /*res*/){
	}

	// -----------------------------------------------------------------
	/*
		@brief	初期位置をレンダリングする
		@param	info			RendererInfo
		@param	res				マスパーティクルリソースオブジェクト
		@param	shader			MassShaderオブジェクト
		@param	target_texture	レンダリングされるRenderingTextureオブジェクト
		@param	offset			ノイズオフセットベクトル
		@return	なし
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
		@brief	初期速度をレンダリングする
		@param	info			RendererInfo
		@param	res				マスパーティクルリソースオブジェクト
		@param	shader			MassShaderオブジェクト
		@param	target_texture	レンダリングされるRenderingTextureオブジェクト
		@param	offset			ノイズオフセットベクトル
		@return	なし
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
		@brief	生成形状をレンダリングする
		@param	info			DrawInfo
		@param	res				マスパーティクルリソースオブジェクト
		@param	shader			MassShaderオブジェクト
		@param	random			ml::random
		@param	target_texture	レンダリングされるRenderingTextureオブジェクト
		@param	shape_texture	shape texture
		@param	type			生成形状のタイプ
		@return	なし
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
		@brief	生成形状タイプの位置をレンダリングする
		@param	info			DrawInfo
		@param	res				マスパーティクルリソースオブジェクト
		@param	shader			MassShaderオブジェクト
		@param	target_texture	レンダリングされるRenderingTextureオブジェクト
		@param	shape_texture	shape texture
		@return	なし
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
		@brief	生成形状モデルタイプの速度をレンダリングする
		@param	info			DrawInfo
		@param	res				マスパーティクルリソースオブジェクト
		@param	shader			MassShaderオブジェクト
		@param	target_texture	レンダリングされるRenderingTextureオブジェクト
		@param	model_param		(幅、プリミティブ数、三角形の総面積（未使用）、幅の逆数)ベクトル
		@param	model_index		モデルのインデックステクスチャ
		@param	model_normal	モデルの法線テクスチャ
		@param	offset			ノイズオフセットベクトル
		@return	なし
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
		@brief	生成形状モデルタイプの速度をレンダリングする
		@param	info			DrawInfo
		@param	res				マスパーティクルリソースオブジェクト
		@param	shader			MassShaderオブジェクト
		@param	target_texture	レンダリングされるRenderingTextureオブジェクト
		@param	model_param		(幅、プリミティブ数、三角形の総面積（未使用）、幅の逆数)ベクトル
		@param	model_index		モデルのインデックステクスチャ
		@param	model_position	モデルの位置テクスチャ
		@param	offset			ノイズオフセットベクトル
		@return	なし
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
		@brief	球状の位置をレンダリングする（ノーマライズされているので法線としても使える）
		@param	info				DrawInfo
		@param	res					マスパーティクルリソースオブジェクト
		@param	shader				MassShaderオブジェクト
		@param	target_texture		レンダリングされるRenderingTextureオブジェクト
		@param	is_generateshape	生成形状かどうか
		@return	なし
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
		@brief	円柱の位置をレンダリングする（ノーマライズされているので法線としても使える）
		@param	info				DrawInfo
		@param	res					マスパーティクルリソースオブジェクト
		@param	shader				MassShaderオブジェクト
		@param	target_texture		レンダリングされるRenderingTextureオブジェクト
		@param	is_generateshape	生成形状かどうか
		@return	なし
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
		@brief	立方体の位置をレンダリングする（ノーマライズされているので法線としても使える）
		@param	info				DrawInfo
		@param	res					マスパーティクルリソースオブジェクト
		@param	shader				MassShaderオブジェクト
		@param	target_texture		レンダリングされるRenderingTextureオブジェクト
		@param	is_generateshape	生成形状かどうか
		@return	なし
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
		@brief	位置、速度のテクスチャを生成する
		@param	info		DrawInfo
		@param	res			マスパーティクルリソースオブジェクト
		@param	shader		MassShaderオブジェクト
		@param	random		ml::random
		@return	なし
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
		@brief	球状の位置、速度のテクスチャを生成する
		@param	info		DrawInfo
		@param	res			マスパーティクルリソースオブジェクト
		@param	shader		MassShaderオブジェクト
		@param	random		ml::random
		@return	なし
	*/
	template<typename ResourceType>
	void SetSpherePositionVelocity(DrawInfo &info, const ResourceType &res, MassShader &shader, ml::random &random){

		MakeSphereTexture(info, res, shader, random, shape_texture_);
		SetShapeVelocity(info, res, shader, random, velocity_texture_[current_map_index_], shape_texture_, MassShader::ShaderType_SetupSpeedSphere);
		SetShapePosition(info, res, shader, random, position_texture_[current_map_index_], shape_texture_);
	}

	// -----------------------------------------------------------------
	/*
		@brief	円柱の位置、速度のテクスチャを生成する
		@param	info		DrawInfo
		@param	res			マスパーティクルリソースオブジェクト
		@param	shader		MassShaderオブジェクト
		@param	random		ml::random
		@return	なし
	*/
	template<typename ResourceType>
	void SetCylinderPositionVelocity(DrawInfo &info, const ResourceType &res, MassShader &shader, ml::random &random){

		MakeCylinderTexture(info, res, shader, random, shape_texture_);
		SetShapeVelocity(info, res, shader, random, velocity_texture_[current_map_index_], shape_texture_, MassShader::ShaderType_SetupSpeedCylinder);
		SetShapePosition(info, res, shader, random, position_texture_[current_map_index_], shape_texture_);
	}

	// -----------------------------------------------------------------
	/*
		@brief	立方体の位置、速度のテクスチャを生成する
		@param	info		DrawInfo
		@param	res			マスパーティクルリソースオブジェクト
		@param	shader		MassShaderオブジェクト
		@param	random		ml::random
		@return	なし
	*/
	template<typename ResourceType>
	void SetCubePositionVelocity(DrawInfo &info, const ResourceType &res, MassShader &shader, ml::random &random){

		MakeCubeTexture(info, res, shader, random, shape_texture_);
		SetShapeVelocity(info, res, shader, random, velocity_texture_[current_map_index_], shape_texture_, MassShader::ShaderType_SetupSpeedCube);
		SetShapePosition(info, res, shader, random, position_texture_[current_map_index_], shape_texture_);
	}

	// -----------------------------------------------------------------
	/*
		@brief	モデルの位置、速度のテクスチャを生成する
		@param	info		DrawInfo
		@param	res			マスパーティクルリソースオブジェクト
		@param	shader		MassShaderオブジェクト
		@param	random		ml::random
		@return	なし
	*/
	template<typename ResourceType>
	void SetModelPositionVelocity(DrawInfo &info, const ResourceType &res, MassShader &shader, ml::random &random){

		ml::vector3d		model_param;
		if(CreateModelRenderingTexture(info.Renderer(), random, res.GenerateShape_Basic_ModelName(), res.GenerateShape_Area_Scale(), model_index_, model_position_, model_normal_, model_param)){
			// 速度と位置でランダムの値を同じにしたいのでtimeとoffsetを使いまわす
			ml::vector3d	offset(random.f32(0.001f, 1.0f), random.f32(0.001f, 1.0f), 0 ,1);
			SetModelVelocity(info, res, shader, velocity_texture_[current_map_index_], model_param, model_index_, model_normal_, offset);
			SetModelPosition(info, res, shader, position_texture_[current_map_index_], model_param, model_index_, model_position_ ,offset);
			// goal でも使うからコピーは外でやる
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
		@brief	位置、速度のテクスチャを生成する
		@param	info		DrawInfo
		@param	res			マスパーティクルリソースオブジェクト
		@param	shader		MassShaderオブジェクト
		@param	random		ml::random
		@return	なし
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
		@brief	初期カラー値、サイズテクスチャを作る
		@param	info		DrawInfo
		@param	res			マスパーティクルリソースオブジェクト
		@param	shader		MassShaderオブジェクト
		@param	random		ml::random
		@return	なし
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
					res.Scale_Value_InitialX() * parent_scale_.e_.x_ * 2.0f,	// シンプルパーティクルのサイズに合わせる
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
		@brief	ゴール用のテクスチャを作る
		@param	info		DrawInfo
		@param	res			マスパーティクルリソースオブジェクト
		@param	shader		MassShaderオブジェクト
		@param	random		ml::random
		@return	なし
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
		@brief	初期値テクスチャを生成する
		@param	info	DrawInfo
		@pram	res		リソースクラスの参照
		@param	random	ランダムクラス
		@return	なし
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
		@brief	描画する
		@param	info		DrawInfo
		@pram	shader_info	ShaderInfo
		@pram	res			リソースクラスの参照
		@param	map_index	使用するテクスチャの番号
		@return	なし
	*/
	template<typename ResourceType>
	void RenderParticle(DrawInfo &info, ShaderInfo &shader_info, const ResourceType &res, int map_index){

		MassShader &shader = RenderStateManager::instance().GetMassShader();
		internal::IdeSupport::BeginRenderStateApply(info, res);
		{
			// レンダーステイト
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
			// シェーダの設定
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

		// 描画
		{
			internal::IdeSupport::BeginDraw(info, res);
			vertex_buffer_.Draw(info.Renderer(), PrimitiveConst_Point, particle_count_.CurrentCount());
			internal::IdeSupport::EndDraw(info, res);
		}

		// 後始末
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
		@brief	更新する。（位置と速度のテクスチャを書き換える）
		@param	info		UpdateInfo
		@pram	res			リソースクラスの参照
		@param	update_rate	フレームレート
		@return	なし
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

			// レンダーステイト
			RenderStateController::SetRenderState(info, false);
			RenderStateManager::instance().SetBlendType(info, BlendTypeConst_Non);
			RenderStateController::SetPointSpriteState(info, false, 0.0f, 64.0f);

			MassShader &shader = RenderStateManager::instance().GetMassShader();
			vertex_buffer_.Bind(info.Renderer());
			// シェーダの確定
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

			// シェーダ定数
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

			// 描画
			{
				vertex_buffer_.Draw(info.Renderer(), PrimitiveConst_Point, particle_count_.CurrentCount());
			}

			// 後始末
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

			// レンダーステイト
			RenderStateController::SetRenderState(info, false);
			RenderStateManager::instance().SetBlendType(info, BlendTypeConst_Non);
#ifdef BM3_PLATFORM_psvita
			RenderStateManager::instance().SetShaderBlendType(BlendTypeConst_Non);
#endif
			RenderStateController::SetPointSpriteState(info, false, 0.0f, 64.0f);

			MassShader &shader = RenderStateManager::instance().GetMassShader();
			vertex_buffer_.Bind(info.Renderer());
			// シェーダの確定
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

			// シェーダ定数
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

			// 描画
			{
				vertex_buffer_.Draw(info.Renderer(), PrimitiveConst_Point, particle_count_.CurrentCount());
			}

			// 後始末
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

			// レンダーステイト
			RenderStateController::SetRenderState(info, false);
			RenderStateManager::instance().SetBlendType(info, BlendTypeConst_Non);
			RenderStateController::SetPointSpriteState(info, false, 0.0f, 64.0f);

			MassShader &shader = RenderStateManager::instance().GetMassShader();
			vertex_buffer_.Bind(info.Renderer());
			// シェーダの確定
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

			// シェーダ定数
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

			// 描画
			{
				vertex_buffer_.Draw(info.Renderer(), PrimitiveConst_Point, particle_count_.CurrentCount());
			}

			// 後始末
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
			glBindBuffer(GL_ARRAY_BUFFER, s_vertex_buffer);	// インデックスバッファのときはGL_ELEMENT_ARRAY_BUFFER
			glBufferData(GL_ARRAY_BUFFER, sizeof(VERTEX) * vertex_count, NULL, GL_STATIC_DRAW);	// 頂点設定してないのだがここで呼び出していいのか・・・
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
				glBindBuffer(GL_ARRAY_BUFFER, s_vertex_buffer);	// インデックスバッファのときはGL_ELEMENT_ARRAY_BUFFER
				glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(VERTEX) * vertex_count, buffer);	// 頂点設定してないのだがここで呼び出していいのか・・・
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

		glBindBuffer(GL_ARRAY_BUFFER, s_vertex_buffer);	// インデックスバッファのときはGL_ELEMENT_ARRAY_BUFFER
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
		@brief	更新情報をキャッシュしておく。
		@param	prim				massparticle primitive structer
		@param	draw_info			DrawInfo
		@param	packet_draw_info	未使用
		@param	res					マスパーティクルリソースオブジェクト
		@param	is_updated			更新したかどうか
		@return	なし
	*/
	template<typename PrimitiveType, typename ResourceType>
	void	PushRange(const PrimitiveType *prim, DrawInfo & /*draw_info*/, const PacketDrawInfo &pdi, const ResourceType & /*res*/, bool is_updated){
		
		PushRange(prim, is_updated, false, pdi);
	}

	// -----------------------------------------------------------------
	/*
		@brief	更新情報をキャッシュしておく。BM2互換版
		@param	prim				massparticle primitive structer
		@param	draw_info			DrawInfo
		@param	packet_draw_info	未使用
		@param	res					マスパーティクルリソースオブジェクト
		@param	is_updated			更新したかどうか
		@return	なし
	*/
	template<typename PrimitiveType, typename ResourceType>
	void	PushRangeBm2(const PrimitiveType *prim, DrawInfo & /*draw_info*/, const PacketDrawInfo &pdi, const ResourceType & /*res*/, bool is_updated){
		
		PushRange(prim, is_updated, true, pdi);
	}
		
	// -----------------------------------------------------------------
	/*
		@brief	更新情報をキャッシュしておく。BM2互換版
		@param	prim				massparticle primitive structer
		@param	is_updated			更新したかどうか
		@param	is_enabled_bm2		bm2互換するかどうか
		@return	なし
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
		@brief	頂点作成開始
		@param	root			ルート
		@param	emitter_index	エミッター番号
		@return	なし
	*/
	void BeginMakeVertex(obj::RootNodeBase &/*root*/, int /*emitter_index*/){
	}

	// -----------------------------------------------------------------
	/*
		@brief	頂点作成終了
		@param	root			ルート
		@param	emitter_index	エミッター番号
		@return	なし
	*/
	void EndMakeVertex(obj::RootNodeBase &/*root*/, int /*emitter_index*/){
	}

#ifdef BM3_PLATFORM_psvita
	void PostMakeVertex(obj::RootNodeBase & /*root*/, int /*emitter_index*/, int /*total_active_count*/){
	}
#endif

	// -----------------------------------------------------------------
	/*
		@brief	頂点バッファのサイズを返す
		@param	child_count	パーティクルの数
		@return	頂点バッファのサイズ
	*/
	int VertexBufferSize(int child_count){ return child_count * VertexPerPrimitiveCount * vertex_element_size_; }
	/*
		@brief	頂点バッファのサイズを返す
		@param	child_count	パーティクルの数
		@param	push_count	親から放出された数
		@return	頂点バッファのサイズ
	*/
	int VertexBufferSize(int child_count, int /*push_count*/){ return VertexBufferSize(child_count); }
#ifdef BM3_PLATFORM_psvita
	/*
		@brief	インデックスバッファのサイズを返す
		@param	child_count	パーティクルの数
		@param	push_count	親から放出された数
		@return	頂点バッファのサイズ
	*/
	int IndexBufferSize(int child_count, int /*push_count*/){ return 0; }
	/*
		@brief	インデックスバッファのサイズを返す
		@param	child_count	パーティクルの数
		@param	push_count	親から放出された数
		@return	頂点バッファのサイズ
	*/
	int IndexBufferSizeAlign(int child_count, int /*push_count*/){ return 0; }
#endif
	/*
		@brief	頂点バッファとテクスチャのトータルサイズを返す
		@param	child_count	パーティクルの数
		@param	push_count	親から放出された数
		@return	頂点バッファとテクスチャのトータルサイズ
	*/
	int TotalVertexBufferSize(int child_count, int /*push_count*/){ return VertexBufferSize(child_count) + resource_size_; }

	/*
		未使用
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
		未使用
	*/
	void DestroyResource(){

		Finalize();
	}
	// -----------------------------------------------------------------
	/*
		未使用
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

	RenderingTexture	position_texture_[2];	//< 位置テクスチャ（ダブルバッファのため2枚）
	RenderingTexture	velocity_texture_[2];	//< 速度テクスチャ（ダブルバッファのため2枚）
	RenderingTexture	org_position_texture_;	//< 初期位置テクスチャ（w is current life）
	RenderingTexture	org_velocity_texture_;	//< 初期速度テクスチャ（ w is lifespan）
	RenderingTexture	colorsize_texture_;		//< カラーサイズテクスチャ(r,g,b,size)
	InternalTexture		alpha_texture_;			//< アルファアニメーションテクスチャ
	InternalTexture		pattern_texture_;		//< テクスチャパターンテクスチャ
	RenderingTexture	shape_texture_;			//< 生成形状テクスチャ
	InternalTexture		model_index_;			//< モデルインデックステクスチャ
	InternalTexture		model_position_;		//< モデル位置テクスチャ
	InternalTexture		model_normal_;			//< モデル法線テクスチャ

	RenderingTexture	goal_texture_;			//< ゴールテクスチャ
	InternalTexture		goal_model_index_;		//< ゴールモデルインデックステクスチャ
	InternalTexture		goal_model_position_;	//< ゴールモデル位置テクスチャ
	InternalTexture		goal_model_normal_;		//< ゴールモデル法線テクスチャ
		
#if defined(BM3_PLATFORM_win_opengl)
	InternalSurface		update_surface_[2];		//< アップデートフレームバッファオブジェクト
#endif

	ParticleCountManager	particle_count_;	//< パーティクル管理

	bool	is_initialized;						//< 初期化されたかどうかフラグ
	bool	is_enabled_reset_;					//< リセットするかどうか

	ml::matrix44						parent_matrix_;	//< 親マトリックス
	ml::vector3d						parent_scale_;	//< 親スケール
	ColorType*							parent_color_;
	const prim::massparticle::Primitive	*prim_;			//< マスパーティクルプリミティブのポインタ
	bool								is_updated_;	//< 更新されたかどうかのフラグ
	int									resource_size_;	//< 使用されているリソースのサイズ
	bool								is_enabled_bm2_;	//< bm2互換を取るかどうか
};

#undef BM3_BLEND_PARENTALPHA
#undef BM3_BLEND_PARENTCOLOR
	
}	// namespace draw
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_SDK_INC_BM3_CORE_DRAW_BM3_DRAW_MASSPARTICLEIMPL_H
