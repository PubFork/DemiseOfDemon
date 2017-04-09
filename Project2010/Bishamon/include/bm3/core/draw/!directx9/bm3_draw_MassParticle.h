#ifndef BM3_SDK_INC_BM3_SDK_INC_BM3_CORE_DRAW_DIRECTX9_BM3_DRAW_MASSPARTICLE_H
#define BM3_SDK_INC_BM3_SDK_INC_BM3_CORE_DRAW_DIRECTX9_BM3_DRAW_MASSPARTICLE_H

#include "bm3/system/bm3_Config.h"
#include "../policy/bm3_draw_policy_Common.h"
#include "../policy/bm3_draw_policy_HasTexture.h"
#include "../policy/bm3_draw_policy_DefaultRenderState.h"
#include "../../bm3_CoreType.h"
#include "../../bm3_RenderStateManager.h"
#include "../../bm3_ProfileManager.h"
#include "../../bm3_ResourceManager.h"
#include "../bm3_draw_Common.h"
#include "../../object/bm3_obj_RootNodeBase.h"
#include "../../bm3_VertexInfo.h"
#include "../../bm3_IdeSupport.h"

#include <ml/type/ml_matrix44.h>
#include <ml/type/ml_color.h>
#include <ml/utility/ml_simpletimer.h>
#include "../../policy/Texture/bm3_policy_Texture_Pattern.h"
#include "bm3/core/primitive/bm3_Primitive.h"
#include "m3dg/model/m3dg_model.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

class DrawInfo;

namespace draw{

#define BM3_BLEND_PARENTCOLOR(c, parent_color)	((1.0f - res.Basic_ParentNodeInfluence_ParentColorRate()) * (c) + (res.Basic_ParentNodeInfluence_ParentColorRate()) * (parent_color))
#define BM3_BLEND_PARENTALPHA(c, parent_color)																\
	((res.Basic_ParentNodeInfluence_AlphaParentNodeInfluenceType()==AlphaParentNodeInfluenceTypeConst_On) ?	\
	(res.Basic_ParentNodeInfluence_ParentAlphaRate()==0.0f) ? (c) : (c) * (parent_color) * res.Basic_ParentNodeInfluence_ParentAlphaRate() : 	\
	((1.0f - res.Basic_ParentNodeInfluence_ParentAlphaRate()) * (c) + (res.Basic_ParentNodeInfluence_ParentAlphaRate()) * (parent_color)))
	
class MassParticle :
	public policy::HasTexture,
	public policy::DefaultRenderState{

	typedef struct RenderingTexture{
		LPDIRECT3DTEXTURE9	texture_;
		LPDIRECT3DSURFACE9	surface_;

		RenderingTexture() : texture_(NULL), surface_(NULL){}
		~RenderingTexture(){
			Release();
		}
		void Release(){
			if(surface_){
				surface_->Release();
				surface_	= NULL;
			}
			if(texture_){
				texture_->Release();
				texture_	= NULL;
			}
		}

	} RenderingTexture;

	typedef struct ParticleCountManager{
	public:
		ParticleCountManager() :
			min_width_(0),
			min_count_(0),
			max_width_(0),
			max_count_(0),
			current_count_(0)
		{
		}

		void Initialize(InitializeInfo &info, int count){
			int	sq					= (int)ml::sqrtf((float)count);
			min_width_		= sq;
			min_count_		= min_width_ * min_width_;
			max_width_		= (int)(min_width_ * info.GenScale());
			max_count_		= max_width_ * max_width_;
		}

		void Update(float scale){
			current_count_	= (int)(scale * min_count_);
			current_count_	= ml::clamp(current_count_, 0, max_count_);
		}

		int MinWidth()		const {	return min_width_;	}
		int MinCount()		const {	return min_count_;	}
		int MaxWidth()		const {	return max_width_;	}
		int MaxCount()		const {	return max_count_;	}
		int CurrentCount()	const {	return current_count_;	}

	private:
		int min_width_;
		int min_count_;
		int max_width_;
		int max_count_;
		int current_count_;
	} ParticleCountManager;

public:
	MassParticle() : 
		vtx_buf_(NULL),
		is_initialized(false),
		alpha_texture_(NULL),
		pattern_texture_(NULL),
		resource_size_(0),
		max_resource_size_(0) {	
	}
	~MassParticle(){
	}

	int PrimitiveTextureSize() const {	return FormatToByte(D3DFMT_A16B16G16R16F) * particle_count_.MaxCount();	}

	int CalModelType(bool is_enable, const bm3::StringType &name){
		int size = 0;
		if(is_enable){
			
			const void *m3r_image = ResourceManager::instance().M3rImageFromName(name);
			if(m3r_image){
				m3dg::VertexArrayInfo	vai;
				m3dg::model::GetVertexArrayInfo(vai, *reinterpret_cast<const m3dg::fileformat::m3r::Header *>(m3r_image));

				int	width		= (int)ml::sqrtf((float)vai.vertex_count_) + 1;
				size += particle_count_.MaxWidth() * particle_count_.MaxWidth() * FormatToByte(D3DFMT_A16B16G16R16) + 
//						(unsigned int)(vai.vertex_count_ * ml::inverse3) * FormatToByte(D3DFMT_LIN_R32F) + 
						width * width * FormatToByte(D3DFMT_A32B32G32R32F) + 
						width * width * FormatToByte(D3DFMT_A32B32G32R32F);
			}
		}
		return size;
	}
		
	template<typename ResourceType>
	int CalTotalResourceSize(const ResourceType &res){
		int size =	sizeof(VertexType) * particle_count_.MaxCount() + // vertex buffer
					PrimitiveTextureSize() * (4 + 2 + 1 + 1) + // position,velocity * 2 + org + color + shape
					FormatToByte(D3DFMT_A8) * 256 + 
					FormatToByte(D3DFMT_A8R8G8B8) * 256;
		
		size += CalModelType(res.GenerateShape_Basic_GenerateShapeType()==GenerateShapeTypeConst_Model, res.GenerateShape_Basic_ModelName());
		
		return size;
	}

	template<typename ResourceType>
	void InitializeParam(RendererInfo &renderer_info, const ResourceType &res){

		int size = CalTotalResourceSize(res);
		if(VertexManager::instance().Alloc(size)){
			resource_size_	= size;
			InitVertices(renderer_info);
			InitParticleMaps(renderer_info, res);
			InitAlphaMap(res);
			InitPatternTextureMap(res);
		}
	}

	bool InitVertices(RendererInfo &info)
	{
		// 頂点バッファ作成
		if(particle_count_.MaxCount()){
			{
				HRESULT r = info.d3d_device_->CreateVertexBuffer(particle_count_.MaxCount() * sizeof(VertexType), 0, 0, D3DPOOL_MANAGED, &vtx_buf_, NULL);
				r;assert(r==S_OK);
			}

			{
				VertexType	*vtx;
				vtx_buf_->Lock(0, particle_count_.MaxCount()*sizeof(VertexType), (void**)&vtx , 0);
				{
					int i,j;
					float u = 0.0f;
					float v = 0.0f;
					float offset_u = 0.5f/particle_count_.MaxWidth();
					float offset_v = 0.5f/particle_count_.MaxWidth();

					float step = 1.0f/(float)particle_count_.MaxWidth();
					for (i=0;i<particle_count_.MaxWidth();i++){
						v = (float)i * step + offset_v;

						for (j=0;j<particle_count_.MaxWidth();j++){
							u = (float)j * step + offset_u;

							vtx->texture_.u_ = u;
							vtx->texture_.v_ = v;
							vtx++;
						} // End for
					} // End for
				}
				vtx_buf_->Unlock();
			}

			return true;
		}
		return false;
	}

	template<typename ResourceType>
	void InitParticleMaps(RendererInfo &info, const ResourceType & /*res*/){

		for(int i=0 ; i<2 ; ++i){
			CreateRenderingTexture(info, position_texture_[i]);
			CreateRenderingTexture(info, velocity_texture_[i]);
		}

		CreateRenderingTexture(info, org_position_texture_);
		CreateRenderingTexture(info, org_velocity_texture_);
		CreateRenderingTexture(info, colorsize_texture_);

		info.d3d_device_->CreateTexture(256, 1, 1, 0, D3DFMT_A8, D3DPOOL_MANAGED, &alpha_texture_, NULL);
		info.d3d_device_->CreateTexture(256, 1, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &pattern_texture_, NULL);
	}
		
	int FormatToByte(D3DFORMAT format) const{
		switch(format){
		case D3DFMT_A8:					return 1;
		case D3DFMT_R32F:				return 4;
		case D3DFMT_A8R8G8B8:			return 4;
		case D3DFMT_A16B16G16R16:		return 8;
		case D3DFMT_A16B16G16R16F:		return 8;
		case D3DFMT_A32B32G32R32F:		return 16;
		default:	ML_ASSERT(false);	return 0;
		}
	}

	void CreateRenderingTexture(RendererInfo &info, RenderingTexture &texture){
		info.d3d_device_->CreateTexture(particle_count_.MaxWidth(), particle_count_.MaxWidth(), 1,
			D3DUSAGE_RENDERTARGET, D3DFMT_A16B16G16R16F, D3DPOOL_DEFAULT, &texture.texture_, NULL);
		texture.texture_->GetSurfaceLevel(0, &texture.surface_);
	}

	bool CreateModelRenderingTexture(RendererInfo &info, ml::random &random, const bm3::StringType &name, const ml::vector3d &scale, RenderingTexture &index, RenderingTexture &position, RenderingTexture &normal, ml::vector3d &model_param){

		const void *m3r_image = ResourceManager::instance().M3rImageFromName(name);
		if(m3r_image){

			m3dg::VertexArrayInfo	vai;
			m3dg::model::GetVertexArrayInfo(vai, *reinterpret_cast<const m3dg::fileformat::m3r::Header *>(m3r_image));

			int	width		= (int)ml::sqrtf((float)vai.vertex_count_) + 1;
			model_param.e_.x_	= (float)width;
			model_param.e_.y_	= vai.vertex_count_ * ml::inverse3;
			model_param.e_.z_	= vai.all_triangle_measure_;
			model_param.e_.w_	= 1.0f/width;

//			info.d3d_device_->CreateTexture((unsigned int)(vai.vertex_count_ * ml::inverse3), 1, 1, 0, D3DFMT_R32F, D3DPOOL_MANAGED, &measure.texture_, NULL);

			info.d3d_device_->CreateTexture(particle_count_.MaxWidth(), particle_count_.MaxWidth(), 1, 0, D3DFMT_A16B16G16R16, D3DPOOL_MANAGED, &index.texture_, NULL);
			info.d3d_device_->CreateTexture(width, width, 1, 0, D3DFMT_A32B32G32R32F, D3DPOOL_MANAGED, &position.texture_, NULL);
			info.d3d_device_->CreateTexture(width, width, 1, 0, D3DFMT_A32B32G32R32F, D3DPOOL_MANAGED, &normal.texture_, NULL);

			InitModelTexture(vai, random, width, scale, index, position, normal);

			return true;
		}

		return false;
	}

	void InitModelTexture(m3dg::VertexArrayInfo &vai, ml::random &random, int height, const ml::vector3d &scale, RenderingTexture &index, RenderingTexture &position, RenderingTexture &normal){

		{
			D3DLOCKED_RECT	rect;
			index.texture_->LockRect(0, &rect, NULL, 0);
			{
				unsigned short*	bit	= (unsigned short*)rect.pBits;
				int				cnt = rect.Pitch/FormatToByte(D3DFMT_A16B16G16R16);

				for(int h=0 ; h<particle_count_.MaxWidth() ; ++h){
					for(int w=0 ; w<cnt; ++w){

						int		tri_count	= (int)(vai.vertex_count_ * ml::inverse3);
						float	measure		= random.f32(vai.all_triangle_measure_);
						const float *i = std::lower_bound(vai.triangle_addition_measure_, vai.triangle_addition_measure_ + tri_count, measure);
						int tri_no = i - vai.triangle_addition_measure_;
						tri_no = ml::clamp(tri_no - 1, 0, tri_count - 1);

						const m3dg::VertexArrayInfo::IndexType *index = vai.index_array_ + tri_no * 3;
						*bit	= index[0];	bit++;
						*bit	= index[1];	bit++;
						*bit	= index[2];	bit++;
						*bit	= 0;		bit++;
					}
				}
			}
			index.texture_->UnlockRect(0);
		}
		{
			D3DLOCKED_RECT	rect;
			position.texture_->LockRect(0, &rect, NULL, 0);
			{
				const ml::f32*	src	= vai.position_array_;
				float*			bit	= (float*)rect.pBits;
				int				cnt = rect.Pitch/FormatToByte(D3DFMT_A32B32G32R32F);
				for(int h=0 ; h<height ; ++h){
					for(int w=0 ; w<cnt; ++w){
						int num = w + h*height;
						if(vai.vertex_count_<=num)	break;
						const float *pos = ml::offset_pointer<const ml::f32 *>(src, vai.position_array_stride_ * num);
						*bit = pos[0] * scale.e_.x_;	bit++;
						*bit = pos[1] * scale.e_.y_;	bit++;
						*bit = pos[2] * scale.e_.z_;	bit++;
						*bit = 1.0f;					bit++;
					}
				}
			}
			position.texture_->UnlockRect(0);
		}
		{
			D3DLOCKED_RECT	rect;
			normal.texture_->LockRect(0, &rect, NULL, 0);
			{
				const ml::f32*	src	= vai.normal_array_;
				float*			bit	= (float*)rect.pBits;
				int				cnt = rect.Pitch/FormatToByte(D3DFMT_A32B32G32R32F);
				for(int h=0 ; h<height ; ++h){
					for(int w=0 ; w<cnt; ++w){
						int num = w + h*height;
						if(vai.vertex_count_<=num)	break;
						const float *nor = ml::offset_pointer<const ml::f32 *>(src, vai.normal_array_stride_ * num);
						*bit = nor[0];	bit++;
						*bit = nor[1];	bit++;
						*bit = nor[2];	bit++;
						*bit = 1.0f;	bit++;
					}
				}
			}
			normal.texture_->UnlockRect(0);
		}
	}

	template<typename ResourceType>
	bool InitAlphaMap(const ResourceType &res){
		
		D3DLOCKED_RECT	rect;
		alpha_texture_->LockRect(0, &rect, NULL, 0);
		{
			unsigned char* bit = (unsigned char*)rect.pBits;
			int cnt = rect.Pitch/FormatToByte(D3DFMT_A8);
			int i=0;
			switch(res.Blend_Basic_BlendAlphaType()){
				case BlendAlphaTypeConst_Constant:
					{
						unsigned char alpha = (unsigned char)(res.Blend_Basic_Alpha() * 255.0f);
						for(i=0 ; i<cnt ; ++i, ++bit){	*bit = alpha;	}
					}
					break;
				case BlendAlphaTypeConst_Curve:
					for(i=0 ; i<cnt ; ++i, ++bit){
						float rate = (float)i/cnt;
						*bit = (unsigned char)(res.Blend_Basic_Curve().Value(rate) * 255.0f);
					}
					break;
				default:	ML_ASSERT(0);			break;
			}
		}
		alpha_texture_->UnlockRect(0);

		return true;
	}

	template<typename ResourceType>
	bool InitPatternTextureMap(const ResourceType &res){
		
		D3DLOCKED_RECT	rect;
		pattern_texture_->LockRect(0, &rect, NULL, 0);

		unsigned char* bit = (unsigned char*)rect.pBits;
		int cnt = rect.Pitch/FormatToByte(D3DFMT_A8R8G8B8);
		int i=0;
		switch(res.Texture_Layer1_TextureAnimationType()){
		case TextureAnimationTypeConst_Nothing:
				for(i=0 ; i<cnt ; ++i, bit += 4){
					bit[2]	= (unsigned char)(0.0f * 255.0f);
					bit[1]	= (unsigned char)(0.0f * 255.0f);
					bit[3]	= (unsigned char)(1.0f * 255.0f);
					bit[0]	= (unsigned char)(1.0f * 255.0f);
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
				for(i=0 ; i<cnt ; ++i, bit += 4){
					bm3::policy::impl::Texture_Crop::UpdateLayer1(update_info, res, dummy, frame, step, TextureAnimationTypeConst_Crop);
					const AnimCrop	*ac			= Texture_Layer_AnimCrop(res.Texture_Layer1_TextureCropAnimationType());
					int				anim_frame	= frame & (ac->count_ - 1);

					bit[2]	= (unsigned char)(ac->origin_[anim_frame].u_ * 255.0f);
					bit[1]	= (unsigned char)(ac->origin_[anim_frame].v_ * 255.0f);
					bit[3]  = (unsigned char)(ac->size_.u_ * 255.0f);
					bit[0]  = (unsigned char)(ac->size_.v_ * 255.0f);
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
				for(i=0 ; i<cnt ; ++i, bit += 4){
					bm3::policy::impl::Texture_Pattern::UpdateLayer1(update_info, res, dummy, frame, step, TextureAnimationTypeConst_Pattern);
					const TexturePattern &tp = res.Texture_Layer1_Pattern().Pattern(frame);
					bit[2]	= (unsigned char)(tp.crop_.left_ * 255.0f);
					bit[1]	= (unsigned char)(tp.crop_.top_ * 255.0f);
					bit[3]	= (unsigned char)(tp.crop_.width_ * 255.0f);
					bit[0]	= (unsigned char)(tp.crop_.height_ * 255.0f);
				}
			}
			break;
		case TextureAnimationTypeConst_Transform:	break;
		default: break;
		}
		
		pattern_texture_->UnlockRect(0);
		return true;
	}
		
	template<typename ResourceType>
	void	Initialize(InitializeInfo &info, const ResourceType &res){
		particle_count_.Initialize(info, res.Generate_Constant_GenerateCount());

		max_resource_size_ = CalTotalResourceSize(res);
		if(res.Goal_Basic_FieldGoalShapeType()!=FieldGoalShapeTypeConst_Point){
			max_resource_size_ += PrimitiveTextureSize();
			max_resource_size_ += CalModelType(res.Goal_Basic_FieldGoalShapeType()==FieldGoalShapeTypeConst_Model, res.Goal_Basic_ModelName());	// index,position,normal
		}
	}

	template<typename ResourceType>
	void	Initialize(InitInfo &info, const ResourceType &res){
		if(vtx_buf_==NULL){
			InitializeParam(info.Renderer(), res);
		}
	}
		
	void	Finalize(){
		if(vtx_buf_){
			vtx_buf_->Release();
			vtx_buf_	= NULL;
		}

		if(pattern_texture_)	pattern_texture_->Release();
		if(alpha_texture_)		alpha_texture_->Release();
		pattern_texture_	= NULL;
		alpha_texture_		= NULL;
		colorsize_texture_.Release();

		shape_texture_.Release();
		model_index_.Release();
		model_measure_.Release();
		model_position_.Release();
		model_normal_.Release();

		goal_texture_.Release();
		goal_model_index_.Release();
		goal_model_measure_.Release();
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
		
	void Reset(ResetInfo &reset_info){
		if(is_initialized){
			Blt(reset_info.Renderer().d3d_device_, org_position_texture_, position_texture_[0]);
			Blt(reset_info.Renderer().d3d_device_, org_velocity_texture_, velocity_texture_[0]);
			current_map_index_	= 0;
		}
	}
		
	template<
		typename ResourceType
	>
	void	Begin(DrawInfo & /*draw_info*/, const ResourceType & /*res*/){
	}

inline DWORD FtoDW( float f ) { return *((DWORD*)&f); };

	void SetRenderState(DrawInfo &draw_info, bool is_zenabled){

		RenderStateManager::instance().SetEnable<RenderState::DepthTest>(	draw_info, is_zenabled);
		RenderStateManager::instance().SetDepthTestType(					draw_info, DepthTestTypeConst_LessEqual);
		RenderStateManager::instance().SetEnable<RenderState::Culling>(		draw_info, D3DCULL_NONE);
		RenderStateManager::instance().SetEnable<RenderState::DepthWrite>(	draw_info, false);

#ifdef BM3_TARGET_IDE
		RenderStateManager::instance().SetEnable<RenderState::AlphaTest>(	draw_info, false);
#endif
}

#define SETRENDERSTATE(TYPE, STATE)							\
	device->GetRenderState(TYPE, &dw);						\
	if(dw!=(unsigned int)STATE)	device->SetRenderState(TYPE, STATE);

	void SetPointSpriteState(LPDIRECT3DDEVICE9 device, bool is_scale, float min, float max){

		DWORD	dw;
		SETRENDERSTATE(D3DRS_POINTSPRITEENABLE	,true);
		SETRENDERSTATE(D3DRS_POINTSCALEENABLE	,is_scale);

		//D3DFVF_PSIZEをハードウェアがサポートしない場合
	//	device->SetRenderState( D3DRS_POINTSIZE			, FtoDW( fPsize ) );//ポイントサイズのセット
		//ポイントの最小サイズ

		SETRENDERSTATE(D3DRS_POINTSIZE_MIN		,FtoDW( min ));
		SETRENDERSTATE(D3DRS_POINTSIZE_MAX		,FtoDW( max ));

		//ポイントスケールの係数
		SETRENDERSTATE(D3DRS_POINTSCALE_A		,FtoDW( 0.00 ));
		SETRENDERSTATE(D3DRS_POINTSCALE_B		,FtoDW( 0.00 ));
		SETRENDERSTATE(D3DRS_POINTSCALE_C		,FtoDW( 1.00 ));
	}

	void RestoreRenderState(DrawInfo &draw_info){
		RenderStateManager::instance().SetEnable<RenderState::DepthTest>(	draw_info, true);
	}

	void Blt(LPDIRECT3DDEVICE9 device, RenderingTexture &src, RenderingTexture &dst){
		RECT srcRect;
		{
			D3DSURFACE_DESC	src_desc;
			src.surface_->GetDesc(&src_desc);
			srcRect.left	= 0;
			srcRect.right	= src_desc.Width;
			srcRect.top		= 0;
			srcRect.bottom	= src_desc.Height;
		}

		RECT dstRect;
		{
			D3DSURFACE_DESC	dst_desc;
			dst.surface_->GetDesc(&dst_desc);
			dstRect.left	= 0;
			dstRect.right	= dst_desc.Width;
			dstRect.top		= 0;
			dstRect.bottom	= dst_desc.Height;
		}

		HRESULT r = device->StretchRect(src.surface_, &srcRect, dst.surface_, &dstRect, D3DTEXF_LINEAR);
		r;assert(r==S_OK);
	}

	inline void SetRenderingTarget(RendererInfo &info, RenderingTexture &texture){
		info.d3d_device_->SetRenderTarget(0, texture.surface_);
		info.d3d_device_->SetRenderTarget(1, NULL);
		info.d3d_device_->SetDepthStencilSurface(NULL);
	}
		
	inline void SetTexture(RendererInfo &info, MassShader &shader, int index, LPDIRECT3DTEXTURE9 texture, D3DTEXTUREADDRESS address, D3DTEXTUREFILTERTYPE filter){
		shader.SetTexture(info, 			index, texture);
		shader.SetTextureAddressType(info,	index, address);
		shader.SetTextureFilterType(info,	index, filter);
	}

	inline void SetTextureAddressFilter(RendererInfo &info, MassShader &shader, int index, TextureAddressTypeConst address, TextureFilterTypeConst filter){

		switch(address){
			case TextureAddressTypeConst_Clamp:		shader.SetTextureAddressType(info,	index, D3DTADDRESS_CLAMP);	break;
			case TextureAddressTypeConst_Repeat:	shader.SetTextureAddressType(info,	index, D3DTADDRESS_WRAP);	break;
			case TextureAddressTypeConst_Mirror:	shader.SetTextureAddressType(info,	index, D3DTADDRESS_MIRROR);	break;
			default:	ML_ASSERT(false);	break;						
		}
		switch(filter){
			case TextureFilterTypeConst_Nearest:	shader.SetTextureFilterType(info,	index, D3DTEXF_POINT);		break;
			case TextureFilterTypeConst_Linear:		shader.SetTextureFilterType(info,	index, D3DTEXF_LINEAR);		break;
			default:	ML_ASSERT(false);	break;						
		}
	}

	template<typename ResourceType>
	void SetPosition(RendererInfo &info, const ResourceType &res, MassShader &shader, RenderingTexture &target_texture, float &time, float &offset){

		SetRenderingTarget(info, target_texture);
		{
			ml::vector3d	temp;
			shader.SetVectorParam(info, MassShader::SetupShaderParam_ps_time,				&time);
			shader.SetVectorParam(info, MassShader::SetupShaderParam_ps_noiseuv_offset,		&offset);
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
		info.d3d_device_->DrawPrimitive(D3DPT_POINTLIST, 0, particle_count_.MaxCount());

		shader.SetTexture(info.d3d_device_, MassShader::SetupShaderParam_ps_shape_texture,		NULL);
	}

	template<typename ResourceType>
	void SetVelocity(RendererInfo &info, const ResourceType &res, MassShader &shader, RenderingTexture &target_texture, float &time, float &offset){

		SetRenderingTarget(info, target_texture);
		{
			shader.SetVectorParam(info, MassShader::SetupShaderParam_ps_time,				&time);
			shader.SetVectorParam(info, MassShader::SetupShaderParam_ps_noiseuv_offset,		&offset);
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

		info.d3d_device_->DrawPrimitive(D3DPT_POINTLIST, 0, particle_count_.MaxCount());
	}

	template<typename ResourceType>
	void SetShapeVelocity(RendererInfo &info, const ResourceType &res, MassShader &shader, ml::random &random, RenderingTexture &target_texture, MassShader::SetupShaderType type){
		float time		= random.f32(0.001f, 1.0f);
		float offset	= random.f32(0.001f, 1.0f);

		if(res.GenerateShape_Basic_Normal()){
			RenderStateManager::instance().ApplyMassParticle(MassShader::ProcType_Setup, type);
			SetTexture(info, shader, MassShader::SetupShaderParam_ps_shape_texture,		target_texture.texture_, D3DTADDRESS_CLAMP, D3DTEXF_POINT);
		}
		else{
			RenderStateManager::instance().ApplyMassParticle(MassShader::ProcType_Setup, MassShader::ShaderType_SetupSpeed);
		}
		SetVelocity(info, res, shader, velocity_texture_[current_map_index_], time, offset);
	}

	template<typename ResourceType>
	void SetShapePosition(RendererInfo &info, const ResourceType &res, MassShader &shader, ml::random &random, RenderingTexture &target_texture){
		float time		= random.f32(0.001f, 1.0f);
		float offset	= random.f32(0.001f, 1.0f);
		{
			RenderStateManager::instance().ApplyMassParticle(MassShader::ProcType_Setup, MassShader::ShaderType_SetupShape);
			SetTexture(info, shader, MassShader::SetupShaderParam_ps_shape_texture,		target_texture.texture_, D3DTADDRESS_CLAMP, D3DTEXF_POINT);
			SetPosition(info, res, shader, position_texture_[current_map_index_], time, offset);
		}
	}

	template<typename ResourceType>
	void SetModelVelocity(RendererInfo &info, const ResourceType &res, MassShader &shader, RenderingTexture &target_texture, ml::vector3d &model_param, RenderingTexture &model_index, RenderingTexture &model_normal, float time, float offset){

		if(res.GenerateShape_Basic_Normal()){
			RenderStateManager::instance().ApplyMassParticle(MassShader::ProcType_Setup, MassShader::ShaderType_SetupSpeedModel);
			shader.SetVectorParam(info,		MassShader::SetupShaderParam_shape_ps_modelparam,				model_param.v_);
			SetTexture(info, shader, MassShader::SetupShaderParam_shape_ps_modelindex,		model_index.texture_,	D3DTADDRESS_CLAMP, D3DTEXF_POINT);
			SetTexture(info, shader, MassShader::SetupShaderParam_shape_ps_modelnormal,		model_normal.texture_,	D3DTADDRESS_CLAMP, D3DTEXF_POINT);
		}
		else{
			RenderStateManager::instance().ApplyMassParticle(MassShader::ProcType_Setup, MassShader::ShaderType_SetupSpeed);
		}
		SetVelocity(info, res, shader, target_texture, time, offset);
	}

	template<typename ResourceType>
	void SetModelPosition(RendererInfo &info, const ResourceType &res, MassShader &shader, RenderingTexture &target_texture,ml::vector3d &model_param, RenderingTexture &model_index, RenderingTexture &model_position, float time, float offset){

		RenderStateManager::instance().ApplyMassParticle(MassShader::ProcType_Setup, MassShader::ShaderType_SetupShapeModel);
		shader.SetVectorParam(info,		MassShader::SetupShaderParam_shape_ps_modelparam,				model_param.v_);
		SetTexture(info, shader, MassShader::SetupShaderParam_shape_ps_modelindex,		model_index.texture_,	D3DTADDRESS_CLAMP, D3DTEXF_POINT);
		SetTexture(info, shader, MassShader::SetupShaderParam_shape_ps_modelposition,	model_position.texture_,D3DTADDRESS_CLAMP, D3DTEXF_POINT);
		SetPosition(info, res, shader, target_texture, time, offset);
	}

#define SET_GENERATESHAPE_RADIUS(TYPE)							\
	temp		= res.GenerateShape_Area_Outside();				\
	temp.e_.w_	= res.GenerateShape_Area_Inside();				\
	shader.SetVectorParam(info, MassShader::TYPE,	temp.v_);

#define SET_GENERATESHAPE_ANGLE(TYPE)																					\
	temp.e_.x_	= ml::minimum(res.GenerateShape_HorizontalArea_StartAngle(), res.GenerateShape_HorizontalArea_EndAngle());	\
	temp.e_.y_	= ml::abs(res.GenerateShape_HorizontalArea_StartAngle() - res.GenerateShape_HorizontalArea_EndAngle());	\
	temp.e_.z_	= ml::minimum(res.GenerateShape_VerticalArea_StartAngle(), res.GenerateShape_VerticalArea_EndAngle());		\
	temp.e_.w_	= ml::abs(res.GenerateShape_VerticalArea_StartAngle() - res.GenerateShape_VerticalArea_EndAngle());		\
	shader.SetVectorParam(info, MassShader::TYPE,	temp.v_);

#define SET_GOAL_RADIUS(TYPE)							\
	temp		= res.Goal_Area_Outside();				\
	temp.e_.w_	= res.Goal_Area_Inside();				\
	shader.SetVectorParam(info, MassShader::TYPE,	temp.v_);

#define SET_GOAL_ANGLE(TYPE)																			\
	temp.e_.x_	= ml::minimum(res.Goal_HorizontalArea_StartAngle(), res.Goal_HorizontalArea_EndAngle());	\
	temp.e_.y_	= ml::abs(res.Goal_HorizontalArea_StartAngle() - res.Goal_HorizontalArea_EndAngle());	\
	temp.e_.z_	= ml::minimum(res.Goal_VerticalArea_StartAngle(), res.Goal_VerticalArea_EndAngle());		\
	temp.e_.w_	= ml::abs(res.Goal_VerticalArea_StartAngle() - res.Goal_VerticalArea_EndAngle());		\
	shader.SetVectorParam(info, MassShader::TYPE,	temp.v_);

	template<typename ResourceType>
	void MakeSphereTexture(RendererInfo &info, const ResourceType &res, MassShader &shader, ml::random &random, RenderingTexture &target_texture, bool is_generateshape = true){

		RenderStateManager::instance().ApplyMassParticle(MassShader::ProcType_Setup, MassShader::ShaderType_SetupSphere);
		SetRenderingTarget(info, target_texture);
		{
			ml::vector3d		temp;
			float time		= random.f32(0.001f, 1.0f);
			float offset	= random.f32(0.001f, 1.0f);
			shader.SetVectorParam(info,		MassShader::SetupShaderParam_ps_time,				&time);
			shader.SetVectorParam(info,		MassShader::SetupShaderParam_ps_noiseuv_offset,		&offset);
			shader.EnableNoiseTexture(info, MassShader::SetupShaderParam_shape_ps_noise_texture);
			if(is_generateshape){
				SET_GENERATESHAPE_RADIUS(SetupShaderParam_shape_ps_radius);
				SET_GENERATESHAPE_ANGLE(SetupShaderParam_shape_ps_angle);
			}else{
				SET_GOAL_RADIUS(SetupShaderParam_shape_ps_radius);
				SET_GOAL_ANGLE(SetupShaderParam_shape_ps_angle);
			}
		}
		info.d3d_device_->DrawPrimitive(D3DPT_POINTLIST, 0, particle_count_.MaxCount());
		shader.DisableNoiseTexture(info, MassShader::SetupShaderParam_shape_ps_noise_texture);
	}

	template<typename ResourceType>
	void MakeCylinderTexture(RendererInfo &info, const ResourceType &res, MassShader &shader, ml::random &random, RenderingTexture &target_texture, bool is_generateshape = true){

		RenderStateManager::instance().ApplyMassParticle(MassShader::ProcType_Setup, MassShader::ShaderType_SetupCylinder);
		SetRenderingTarget(info, target_texture);
		{
			ml::vector3d		temp;
			float time		= random.f32(0.001f, 1.0f);
			float offset	= random.f32(0.001f, 1.0f);
			shader.SetVectorParam(info,		MassShader::SetupShaderParam_ps_time,				&time);
			shader.SetVectorParam(info,		MassShader::SetupShaderParam_ps_noiseuv_offset,		&offset);
			shader.EnableNoiseTexture(info, MassShader::SetupShaderParam_shape_ps_noise_texture);
			if(is_generateshape){
				SET_GENERATESHAPE_RADIUS(SetupShaderParam_shape_ps_radius);
				SET_GENERATESHAPE_ANGLE(SetupShaderParam_shape_ps_angle);
			}else{
				SET_GOAL_RADIUS(SetupShaderParam_shape_ps_radius);
				SET_GOAL_ANGLE(SetupShaderParam_shape_ps_angle);
			}
		}
		info.d3d_device_->DrawPrimitive(D3DPT_POINTLIST, 0, particle_count_.MaxCount());
		shader.DisableNoiseTexture(info, MassShader::SetupShaderParam_shape_ps_noise_texture);
	}

	template<typename ResourceType>
	void MakeCubeTexture(RendererInfo &info, const ResourceType &res, MassShader &shader, ml::random &random, RenderingTexture &target_texture, bool is_generateshape = true){

		RenderStateManager::instance().ApplyMassParticle(MassShader::ProcType_Setup, MassShader::ShaderType_SetupCube);
		SetRenderingTarget(info, target_texture);
		{
			ml::vector3d		temp;
			float time		= random.f32(0.001f, 1.0f);
			float offset	= random.f32(0.001f, 1.0f);
			shader.SetVectorParam(info,		MassShader::SetupShaderParam_ps_time,				&time);
			shader.SetVectorParam(info,		MassShader::SetupShaderParam_ps_noiseuv_offset,		&offset);
			shader.EnableNoiseTexture(info, MassShader::SetupShaderParam_shape_ps_noise_texture);
			if(is_generateshape){
				SET_GENERATESHAPE_RADIUS(SetupShaderParam_shape_ps_radius);
			}else{
				SET_GOAL_RADIUS(SetupShaderParam_shape_ps_radius);
			}
		}
		info.d3d_device_->DrawPrimitive(D3DPT_POINTLIST, 0, particle_count_.MaxCount());
		shader.DisableNoiseTexture(info, MassShader::SetupShaderParam_shape_ps_noise_texture);
	}
#undef SET_GOAL_RADIUS
#undef SET_GOAL_ANGLE
#undef SET_GENERATESHAPE_RADIUS
#undef SET_GENERATESHAPE_ANGLE

	template<typename ResourceType>
	void SetPointPositionVelocity(RendererInfo &info, const ResourceType &res, MassShader &shader, ml::random &random){

		float time		= random.f32(0.001f, 1.0f);
		float offset	= random.f32(0.001f, 1.0f);
		RenderStateManager::instance().ApplyMassParticle(MassShader::ProcType_Setup, MassShader::ShaderType_SetupSpeed);
		SetVelocity(info, res, shader, velocity_texture_[current_map_index_], time, offset);
		time	= random.f32(0.001f, 1.0f);
		offset	= random.f32(0.001f, 1.0f);
		RenderStateManager::instance().ApplyMassParticle(MassShader::ProcType_Setup, MassShader::ShaderType_Setup);
		SetPosition(info, res, shader, position_texture_[current_map_index_], time, offset);
	}

	template<typename ResourceType>
	void SetSpherePositionVelocity(RendererInfo &info, const ResourceType &res, MassShader &shader, ml::random &random){

		CreateRenderingTexture(info, shape_texture_);

		MakeSphereTexture(info, res, shader, random, shape_texture_);
		SetShapeVelocity(info, res, shader, random, shape_texture_, MassShader::ShaderType_SetupSpeedSphere);
		SetShapePosition(info, res, shader, random, shape_texture_);
	}

	template<typename ResourceType>
	void SetCylinderPositionVelocity(RendererInfo &info, const ResourceType &res, MassShader &shader, ml::random &random){

		CreateRenderingTexture(info, shape_texture_);

		MakeCylinderTexture(info, res, shader, random, shape_texture_);
		SetShapeVelocity(info, res, shader, random, shape_texture_, MassShader::ShaderType_SetupSpeedCylinder);
		SetShapePosition(info, res, shader, random, shape_texture_);
	}

	template<typename ResourceType>
	void SetCubePositionVelocity(RendererInfo &info, const ResourceType &res, MassShader &shader, ml::random &random){

		CreateRenderingTexture(info, shape_texture_);

		MakeCubeTexture(info, res, shader, random, shape_texture_);
		SetShapeVelocity(info, res, shader, random, shape_texture_, MassShader::ShaderType_SetupSpeedCube);
		SetShapePosition(info, res, shader, random, shape_texture_);
	}

	template<typename ResourceType>
	void SetModelPositionVelocity(RendererInfo &info, const ResourceType &res, MassShader &shader, ml::random &random){

		ml::vector3d		model_param;
		if(CreateModelRenderingTexture(info, random, res.GenerateShape_Basic_ModelName(), res.GenerateShape_Area_Scale(), model_index_, model_position_, model_normal_, model_param)){
			// 速度と位置でランダムの値を同じにしたいのでtimeとoffsetを使いまわす
			float time		= random.f32(0.001f, 1.0f);
			float offset	= random.f32(0.001f, 1.0f);
			SetModelVelocity(info, res, shader, velocity_texture_[current_map_index_], model_param, model_index_, model_normal_, time, offset);
			SetModelPosition(info, res, shader, position_texture_[current_map_index_], model_param, model_index_, model_position_ ,time, offset);
		}
		else{
			SetPointPositionVelocity(info, res, shader, random);
		}
	}

	template<typename ResourceType>
	void SetupInitPositionVelocity(RendererInfo &info, const ResourceType &res, MassShader &shader, ml::random &random){
		switch(res.GenerateShape_Basic_GenerateShapeType()){
			case GenerateShapeTypeConst_Point:		SetPointPositionVelocity(	info, res, shader, random);	break;
			case GenerateShapeTypeConst_Sphere:		SetSpherePositionVelocity(	info, res, shader, random);	break;
			case GenerateShapeTypeConst_Cylinder:	SetCylinderPositionVelocity(info, res, shader, random);	break;
			case GenerateShapeTypeConst_Cube:		SetCubePositionVelocity(	info, res, shader, random);	break;
			case GenerateShapeTypeConst_Model:		SetModelPositionVelocity(	info, res, shader, random);	break;
			default:								ML_ASSERT(false);										break;
		}
	}

	template<typename ResourceType>
	void SetupInitColorSize(RendererInfo &info, const ResourceType &res, MassShader &shader, D3DVIEWPORT9 &vp, ml::random &random){
		
		if(res.Color_Center_RandomChannelSync()){	RenderStateManager::instance().ApplyMassParticle(MassShader::ProcType_Setup, MassShader::ShaderType_SetupSyncColor);	}
		else{										RenderStateManager::instance().ApplyMassParticle(MassShader::ProcType_Setup, MassShader::ShaderType_SetupColor);		}
		
		SetRenderingTarget(info, colorsize_texture_);
		{
			ml::vector3d	temp;
			ml::color_rgba<float>	color;

			float time		= random.f32(0.001f, 1.0f);
			float offset	= random.f32(0.001f, 1.0f);
			shader.SetVectorParam(info, MassShader::SetupShaderParam_ps_time,				&time);
			shader.SetVectorParam(info, MassShader::SetupShaderParam_ps_noiseuv_offset,		&offset);
			float	size_param[4] = {
				res.Scale_Value_InitialX() * parent_scale_.e_.x_ * float(vp.Height) * 2.0f,	// シンプルパーティクルのサイズに合わせる
				res.Scale_Value_RangeX() * float(vp.Height) * 2.0f,
				0.0f,
				0.0f
			};
			shader.SetVectorParam(info, MassShader::SetupShaderParam_ps_w_param,	size_param);

			color	= res.Color_Center_Value();
			shader.SetVectorParam(info, MassShader::SetupShaderParam_ps_initial_value,	color.v_);

			color	= res.Color_Center_Range();
			shader.SetVectorParam(info, MassShader::SetupShaderParam_ps_variance_scalar, color.v_);
			temp = ml::vector3d(2.0f, 2.0f, 2.0f, 2.0f);
			shader.SetVectorParam(info, MassShader::SetupShaderParam_ps_noise_scalar,	temp.v_);
			temp = ml::vector3d(-1.0f, -1.0f, -1.0f, -1.0f);
			shader.SetVectorParam(info, MassShader::SetupShaderParam_ps_noise_offset,	temp.v_);

			shader.EnableNoiseTexture(info, MassShader::SetupShaderParam_ps_noise_texture);
		}

		info.d3d_device_->DrawPrimitive(D3DPT_POINTLIST, 0, particle_count_.MaxCount());
		
		shader.DisableNoiseTexture(info, MassShader::SetupShaderParam_ps_noise_texture);
	}

	template<typename ResourceType>
	void SetupInitGoal(RendererInfo &info, const ResourceType &res, MassShader &shader, ml::random &random){

		int size = PrimitiveTextureSize();	// goal_texture
		size += CalModelType(res.Goal_Basic_FieldGoalShapeType()==FieldGoalShapeTypeConst_Model, res.Goal_Basic_ModelName());	// index,position,normal
		if(VertexManager::instance().Alloc(size)){
			resource_size_	+= size;

			CreateRenderingTexture(info, goal_texture_);
			switch(res.Goal_Basic_FieldGoalShapeType()){
				case FieldGoalShapeTypeConst_Point:																				break;
				case FieldGoalShapeTypeConst_Sphere:	MakeSphereTexture(info, res, shader, random, goal_texture_, false);		break;
				case FieldGoalShapeTypeConst_Cylinder:	MakeCylinderTexture(info, res, shader, random, goal_texture_, false);	break;
				case FieldGoalShapeTypeConst_Cube:		MakeCubeTexture(info, res, shader, random, goal_texture_, false);		break;
				case FieldGoalShapeTypeConst_Model:
				{
					ml::vector3d		model_param;
					if(CreateModelRenderingTexture(info, random, res.Goal_Basic_ModelName(), res.Goal_Area_Scale(), goal_model_index_, goal_model_position_, goal_model_normal_, model_param)){
						float time		= random.f32(0.001f, 1.0f);
						float offset	= random.f32(0.001f, 1.0f);
						SetModelPosition(info, res, shader, goal_texture_, model_param, goal_model_index_, goal_model_position_ ,time, offset);
					}
					else{
						float time		= random.f32(0.001f, 1.0f);
						float offset	= random.f32(0.001f, 1.0f);
						RenderStateManager::instance().ApplyMassParticle(MassShader::ProcType_Setup, MassShader::ShaderType_Setup);
						SetPosition(info, res, shader, goal_texture_, time, offset);
					}
				}
				break;
				default:	ML_ASSERT(false);				break;
			}
		}
	}

	template<typename ResourceType>
	void SetupParticle(DrawInfo &info, const ResourceType &res, ml::random &random){
		LPDIRECT3DDEVICE9 device	= info.Renderer().d3d_device_;
		MassShader &shader = RenderStateManager::instance().GetMassShader();
		current_map_index_	= 0;

		device->SetStreamSource(0, vtx_buf_, 0, sizeof(VertexType));

		SetRenderState(info ,false);
		RenderStateManager::instance().SetBlendType(info, BlendTypeConst_Non);
		SetPointSpriteState(device, false, 1.0f, 1.0f);

		D3DVIEWPORT9		vp;
		LPDIRECT3DSURFACE9	back_buf;
		LPDIRECT3DSURFACE9	back_buf_z;
		device->GetViewport(&vp);
		device->GetRenderTarget(0, &back_buf);
		device->GetDepthStencilSurface(&back_buf_z);
		{
			D3DVIEWPORT9		temp_vp;
			temp_vp.X		= 0;
			temp_vp.Y		= 0;
			temp_vp.Width	= particle_count_.MaxWidth();
			temp_vp.Height	= particle_count_.MaxWidth();
			temp_vp.MinZ	= 0.0f;
			temp_vp.MaxZ	= 1.0f;
			device->SetViewport(&temp_vp);
		}

		{
			SetupInitPositionVelocity(info.Renderer(), res, shader, random);
			SetupInitColorSize(info.Renderer(), res, shader, vp, random);
			if(res.Goal_Basic_FieldGoalShapeType()!=FieldGoalShapeTypeConst_Point){
				SetupInitGoal(info.Renderer(), res, shader, random);
			}
		}
		
		device->SetRenderTarget(0, back_buf);
		device->SetDepthStencilSurface(back_buf_z);
		device->SetViewport(&vp);
		back_buf->Release();
		back_buf_z->Release();

		Blt(device, position_texture_[0], org_position_texture_);
		Blt(device, velocity_texture_[0], org_velocity_texture_);
	}

	template<typename ResourceType>
	void RenderParticle(DrawInfo &info, ShaderInfo &shader_info, const ResourceType &res, int map_index){

		MassShader &shader = RenderStateManager::instance().GetMassShader();

		info.Renderer().d3d_device_->SetStreamSource(0, vtx_buf_, 0, sizeof(VertexType));

		internal::IdeSupport::BeginRenderStateApply(info, res);
		{
			SetPointSpriteState(info.Renderer().d3d_device_, true, 0.0f, 100.0f);
			RenderStateManager::instance().SetEnable<RenderState::DepthTest>(	info, true		);
			RenderStateManager::instance().SetEnable<RenderState::DepthWrite>(	info, res.Material_DrawControl_DepthWrite()		);
			RenderStateManager::instance().SetDepthTestType(					info, res.Material_DrawControl_DepthTestType()	);
			RenderStateManager::instance().SetEnable<RenderState::Culling>(		info, D3DCULL_NONE);
			RenderStateManager::instance().SetBlendType(						info, res.Blend_Basic_BlendType()				);
			RenderStateManager::instance().SetEnable<RenderState::AlphaTest>(	info, res.Blend_Basic_AlphaTest()				);
			RenderStateManager::instance().SetAlphaTestType(					info, res.Blend_Basic_AlphaTestType(), res.Blend_Basic_AlphaRef());

			if(res.Goal_Basic_FieldGoalShapeType()!=FieldGoalShapeTypeConst_Point){
				if(is_enabled_bm2_)	RenderStateManager::instance().ApplyMassParticle(MassShader::ProcType_Render, MassShader::ShaderType_RenderGoalBM2);
				else				RenderStateManager::instance().ApplyMassParticle(MassShader::ProcType_Render, MassShader::ShaderType_RenderGoal);

				SetTexture(info.Renderer(), shader, MassShader::RenderShaderParam_ps_alpha_map,	alpha_texture_, 		D3DTADDRESS_CLAMP, D3DTEXF_LINEAR);
				SetTexture(info.Renderer(), shader, MassShader::RenderShaderParam_vs_goal_map,	goal_texture_.texture_,	D3DTADDRESS_CLAMP, D3DTEXF_POINT);
				ml::vector3d	temp(
					res.Goal_Term_Start(), res.Goal_Term_End(), 0.0f, 0.0f
				);
				shader.SetVectorParam(info.Renderer(), MassShader::RenderShaderParam_vs_goal_param,		temp.v_);
				shader.SetVectorParam(info.Renderer(), MassShader::RenderShaderParam_vs_goal_position,	res.Goal_Position_Initial().v_);
			}
			else{
				if(is_enabled_bm2_)	RenderStateManager::instance().ApplyMassParticle(MassShader::ProcType_Render, MassShader::ShaderType_RenderBM2);
				else				RenderStateManager::instance().ApplyMassParticle(MassShader::ProcType_Render, MassShader::ShaderType_Render);
				SetTexture(info.Renderer(), shader, MassShader::RenderShaderParam_vs_alpha_map,	alpha_texture_, D3DTADDRESS_CLAMP, D3DTEXF_LINEAR);
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

			SetTexture(info.Renderer(), shader, MassShader::RenderShaderParam_vs_position_map,	position_texture_[map_index].texture_, D3DTADDRESS_CLAMP, D3DTEXF_POINT);
			SetTexture(info.Renderer(), shader, MassShader::RenderShaderParam_vs_velocity_map,	velocity_texture_[map_index].texture_, D3DTADDRESS_CLAMP, D3DTEXF_POINT);
			SetTexture(info.Renderer(), shader, MassShader::RenderShaderParam_vs_colorsize_map,	colorsize_texture_.texture_,			D3DTADDRESS_CLAMP, D3DTEXF_POINT);
			SetTexture(info.Renderer(), shader, MassShader::RenderShaderParam_ps_pattern_texture,pattern_texture_,						D3DTADDRESS_CLAMP, D3DTEXF_POINT);
			
			{
				texture::HandleBase* texture = ResourceManager::instance().TextureHandleFromName(res.Texture_Layer1_PictureName(), info.Renderer());
				if(texture!=NULL){
					texture->Bind(bm3::TextureLayer::_1, info);
					RenderStateManager::instance().SetMassParticleTexture();
				}
				else{
					shader.EnableParticleTexture(info.Renderer());
				}
				SetTextureAddressFilter(
					info.Renderer(),
					shader,
					MassShader::RenderShaderParam_ps_particle_texture,
					res.Texture_Layer1_TextureAddressType(),
					res.Texture_Layer1_TextureFilterType()
				);
			}
			{
				ml::vector3d	temp(prim_->scale_, 1.0f, 0.0f, 0.0f);
				shader.SetVectorParam(info.Renderer(), MassShader::RenderShaderParam_vs_size_param,			temp.v_);//これを消すとなぜか実行時エラーになる
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
		}
		internal::IdeSupport::EndRenderStateApply(info, res);

		internal::IdeSupport::BeginDraw(info, res);
		{
			info.Renderer().d3d_device_->DrawPrimitive(D3DPT_POINTLIST, 0, particle_count_.CurrentCount());
		}
		internal::IdeSupport::EndDraw(info, res);

		shader.SetTexture(info.Renderer(), MassShader::RenderShaderParam_vs_position_map,		NULL);
		shader.SetTexture(info.Renderer(), MassShader::RenderShaderParam_vs_velocity_map,		NULL);
		shader.SetTexture(info.Renderer(), MassShader::RenderShaderParam_vs_colorsize_map,		NULL);
		shader.SetTexture(info.Renderer(), MassShader::RenderShaderParam_ps_pattern_texture,	NULL);
		if(res.Goal_Basic_FieldGoalShapeType()!=FieldGoalShapeTypeConst_Point){
			shader.SetTexture(info.Renderer(), MassShader::RenderShaderParam_vs_goal_map,			NULL);
			shader.SetTexture(info.Renderer(), MassShader::RenderShaderParam_ps_alpha_map,			NULL);
		}
		else{
			shader.SetTexture(info.Renderer(), MassShader::RenderShaderParam_vs_alpha_map,			NULL);
		}
		shader.DisableParticleTexture(info.Renderer());
		info.Renderer().d3d_device_->SetStreamSource(0, NULL, 0, sizeof(VertexType));
	}

	template<typename ResourceType>
	void UpdateParticle(DrawInfo &info, const ResourceType &res, float update_frame){
		MassShader &shader = RenderStateManager::instance().GetMassShader();

		D3DVIEWPORT9		vp;
		LPDIRECT3DSURFACE9	backup0	= NULL;
		LPDIRECT3DSURFACE9	backup1	= NULL;
		LPDIRECT3DSURFACE9	back_buf_z;
		info.Renderer().d3d_device_->GetViewport(&vp);
		info.Renderer().d3d_device_->GetRenderTarget(0, &backup0);
		info.Renderer().d3d_device_->GetRenderTarget(1, &backup1);
		info.Renderer().d3d_device_->GetDepthStencilSurface(&back_buf_z);
		{
			int map_index	= current_map_index_ ? 0 : 1;
			info.Renderer().d3d_device_->SetRenderTarget(0, position_texture_[map_index].surface_);
			info.Renderer().d3d_device_->SetRenderTarget(1, velocity_texture_[map_index].surface_);
			info.Renderer().d3d_device_->SetDepthStencilSurface(NULL);

			D3DVIEWPORT9		temp_vp;
			temp_vp.X		= 0;
			temp_vp.Y		= 0;
			temp_vp.Width	= particle_count_.MaxWidth();
			temp_vp.Height	= particle_count_.MaxWidth();
			temp_vp.MinZ	= 0.0f;
			temp_vp.MaxZ	= 1.0f;
			info.Renderer().d3d_device_->SetViewport(&temp_vp);

			// シェーダの確定
			if(res.Generate_Constant_EndlessRepeat()){
				if(res.Basic_ParentNodeInfluence_MatrixParentNodeInfluenceType()==MatrixParentNodeInfluenceTypeConst_OnGenerateTime)
					RenderStateManager::instance().ApplyMassParticle(MassShader::ProcType_Update, MassShader::ShaderType_UpdateParentInfl);
				else
					RenderStateManager::instance().ApplyMassParticle(MassShader::ProcType_Update, MassShader::ShaderType_Update);
			}
			else{
				if(res.Basic_ParentNodeInfluence_MatrixParentNodeInfluenceType()==MatrixParentNodeInfluenceTypeConst_OnGenerateTime)
					RenderStateManager::instance().ApplyMassParticle(MassShader::ProcType_Update, MassShader::ShaderType_UpdateOneShootParentInfl);
				else
					RenderStateManager::instance().ApplyMassParticle(MassShader::ProcType_Update, MassShader::ShaderType_UpdateOneShoot);
			}

			info.Renderer().d3d_device_->SetStreamSource(0, vtx_buf_, 0, sizeof(VertexType));

			SetRenderState(info, false);
			RenderStateManager::instance().SetBlendType(info, BlendTypeConst_Non);
			SetPointSpriteState(info.Renderer().d3d_device_, false, 0.0f, 64.0f);

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

				SetTexture(info.Renderer(), shader, MassShader::UpdateShaderParam_ps_position_map,		position_texture_[current_map_index_].texture_, D3DTADDRESS_CLAMP, D3DTEXF_POINT);
				SetTexture(info.Renderer(), shader, MassShader::UpdateShaderParam_ps_velocity_map,		velocity_texture_[current_map_index_].texture_, D3DTADDRESS_CLAMP, D3DTEXF_POINT);
				SetTexture(info.Renderer(), shader, MassShader::UpdateShaderParam_ps_orig_position_map,	org_position_texture_.texture_,					D3DTADDRESS_CLAMP, D3DTEXF_POINT);
				SetTexture(info.Renderer(), shader, MassShader::UpdateShaderParam_ps_orig_velocity_map,	org_velocity_texture_.texture_, 				D3DTADDRESS_CLAMP, D3DTEXF_POINT);
				shader.EnableNoiseTexture(info.Renderer(), MassShader::UpdateShaderParam_ps_noise_texture);

				{
					shader.SetVectorParam(info.Renderer(), MassShader::UpdateShaderParam_ps_elapsed_time,		&update_frame);

					ml::vector3d	temp;
					temp.add(res.Translate_Gravity_Value(), prim_->gravity_);
					temp.e_.w_	= prim_->velocity_;
					shader.SetVectorParam(info.Renderer(), MassShader::UpdateShaderParam_ps_gravity,		temp.v_);
					float	deceleration	= res.Translate_Deceleration_Value() * prim_->deceleration_;
					shader.SetVectorParam(info.Renderer(), MassShader::UpdateShaderParam_ps_deceleration,	&deceleration);
					float	noise_param[4] = {	
						1.0f,
						res.Translate_Noise_Frequency() * prim_->noise_frequency_,
						res.Translate_Noise_StartRate(),
						res.Translate_Noise_Scale() * prim_->noise_scale_
					};
					shader.SetVectorParam(info.Renderer(), MassShader::UpdateShaderParam_ps_noise_param,	noise_param);
				}

				info.Renderer().d3d_device_->DrawPrimitive(D3DPT_POINTLIST, 0, particle_count_.CurrentCount());

				shader.DisableNoiseTexture(info.Renderer(), MassShader::UpdateShaderParam_ps_noise_texture);
				shader.SetTexture(info.Renderer(), MassShader::UpdateShaderParam_ps_position_map,		NULL);
				shader.SetTexture(info.Renderer(), MassShader::UpdateShaderParam_ps_velocity_map,		NULL);
				shader.SetTexture(info.Renderer(), MassShader::UpdateShaderParam_ps_orig_position_map,	NULL);
				shader.SetTexture(info.Renderer(), MassShader::UpdateShaderParam_ps_orig_velocity_map,	NULL);
			}
		}
		info.Renderer().d3d_device_->SetRenderTarget(0, backup0);
		info.Renderer().d3d_device_->SetRenderTarget(1, backup1);
		info.Renderer().d3d_device_->SetDepthStencilSurface(back_buf_z);
		info.Renderer().d3d_device_->SetViewport(&vp);
		if(backup0)	backup0->Release();
		if(backup1)	backup1->Release();
		back_buf_z->Release();
	}

	template<typename ResourceType>
	void	End(DrawInfo &draw_info, ShaderInfo &shader_info, VertexContext & /*ctx*/, int /*child_count*/, int /*push_count*/, const ResourceType &res){

		if(vtx_buf_==NULL){
			InitializeParam(draw_info.Renderer(), res);
		}
		if(vtx_buf_){

			float	update_frame	= prim_->update_frame_;	// 現状固定（デルタタイムに対応した場合はここを変更）

			// 初期設定レンダリング
			if(is_initialized==false){
				is_initialized	= true;
				ml::random	rand;
				{
					ml::random::seed_info rsi;
					rsi.set1(ml::ui32(this));
					rand.set_seed(rsi);
				}
				SetupParticle(draw_info, res, rand);
				update_frame	= 0.0f;
			}


			{
				// 更新レンダリング
				particle_count_.Update(prim_->generate_scale_);
				int map_index = current_map_index_;
				if(is_updated_){
					UpdateParticle(draw_info, res, update_frame);
					current_map_index_	= current_map_index_ ? 0 : 1;
					is_updated_	= false;
				}

				//　描画レンダリング
				RenderParticle(draw_info, shader_info, res, map_index);

				RestoreRenderState(draw_info);
			}
			RenderStateManager::instance().SetWorldMatrix(ml::matrix44());

			ProfileManager::instance().Draw_AddVertexProcessCount(particle_count_.CurrentCount());
			ProfileManager::instance().Draw_AddPrimitiveProcessCount(EmitterTypeConst_MassParticle, particle_count_.CurrentCount());
			ProfileManager::instance().Draw_AddVertexArrayDrawCount(1);
		}
	}

	template<
		typename PrimitiveType,
		typename ResourceType
	>
	void	PushRange(const PrimitiveType *prim, DrawInfo & /*draw_info*/, const PacketDrawInfo &pdi, const ResourceType & /*res*/, bool is_updated){
		
		PushRange(prim, is_updated, false, pdi);
	}

	// BM2互換版
	template<
		typename PrimitiveType,
		typename ResourceType
	>
	void	PushRangeBm2(const PrimitiveType *prim, DrawInfo & /*draw_info*/, const PacketDrawInfo &pdi, const ResourceType & /*res*/, bool is_updated){
		
		PushRange(prim, is_updated, true, pdi);
	}

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


	void BeginMakeVertex(obj::RootNodeBase &/*root*/, int /*emitter_index*/){
	}

	void EndMakeVertex(obj::RootNodeBase &/*root*/, int /*emitter_index*/){
	}

	int VertexBufferSize(int child_count){ return child_count * VertexPerPrimitiveCount * vertex_element_size_; }
	int VertexBufferSize(int child_count, int /*push_count*/){ return VertexBufferSize(child_count); }
	int TotalVertexBufferSize(int child_count, int /*push_count*/){ return VertexBufferSize(child_count) + max_resource_size_; }
//	int TotalVertexBufferSize(int child_count, int /*push_count*/){ return VertexBufferSize(child_count) + resource_size_; }

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
		
	void DestroyResource(){

		Finalize();
	}

	template<typename ResourceType>
	void RestoreResource(RestoreInfo &info, const ResourceType & res){
		if(vtx_buf_!=NULL){
			InitializeParam(info.Renderer(), res);
		}
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
	LPDIRECT3DVERTEXBUFFER9			vtx_buf_;

	int					current_map_index_;
	RenderingTexture	org_position_texture_;// w is current life
	RenderingTexture	org_velocity_texture_;// w is lifespan

	RenderingTexture	position_texture_[2];
	RenderingTexture	velocity_texture_[2];

	RenderingTexture	shape_texture_;
	RenderingTexture	model_index_;
	RenderingTexture	model_measure_;
	RenderingTexture	model_position_;
	RenderingTexture	model_normal_;

	RenderingTexture	colorsize_texture_;
	LPDIRECT3DTEXTURE9	alpha_texture_;
	LPDIRECT3DTEXTURE9	pattern_texture_;

	RenderingTexture	goal_texture_;
	RenderingTexture	goal_model_index_;
	RenderingTexture	goal_model_measure_;
	RenderingTexture	goal_model_position_;
	RenderingTexture	goal_model_normal_;

	ParticleCountManager	particle_count_;

	bool	is_initialized;

	ml::matrix44						parent_matrix_;
	ml::vector3d						parent_scale_;
	ColorType*							parent_color_;
	const prim::massparticle::Primitive	*prim_;
	bool								is_updated_;
	int									resource_size_;
	int									max_resource_size_;
	bool								is_enabled_bm2_;
};

#undef BM3_BLEND_PARENTALPHA
#undef BM3_BLEND_PARENTCOLOR
	
}	// namespace draw
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_SDK_INC_BM3_CORE_DRAW_DIRECTX9_BM3_DRAW_MASSPARTICLE_H
