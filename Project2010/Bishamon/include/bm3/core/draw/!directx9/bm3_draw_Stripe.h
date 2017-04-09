#ifndef BM3_SDK_INC_BM3_SDK_INC_BM3_CORE_DRAW_DIRECTX9_BM3_DRAW_STRIPE_H
#define BM3_SDK_INC_BM3_SDK_INC_BM3_CORE_DRAW_DIRECTX9_BM3_DRAW_STRIPE_H

#include "bm3/system/bm3_Config.h"
#include "../policy/bm3_draw_policy_Position2.h"
#include "../policy/bm3_draw_policy_Color2.h"
#include "../policy/bm3_draw_policy_Texture2.h"
#include "../policy/bm3_draw_policy_TangentSpace2.h"
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

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

class DrawInfo;

namespace draw{

class Stripe :
	public policy::HasTexture,
	public policy::DefaultRenderState{

public:
	Stripe(){	;	}
	~Stripe(){	;	}

	template<typename ResourceType>
	void	Initialize(InitializeInfo & /*info*/, const ResourceType &res){

		if(res.Texture_Basic_TextureDrawType() == TextureDrawTypeConst_Normalmapping){
			vertex_element_size_ = 0;
			vertex_element_size_ += sizeof(PositionType);
			vertex_element_size_ += sizeof(ColorType);
			vertex_element_size_ += sizeof(TextureCoordinate);
			vertex_element_size_ += sizeof(TangentSpaceType);
		}else{
			vertex_element_size_ =
				sizeof(VertexType) +
				sizeof(TextureCoordinate) * TextureReferenceCount(res.Texture_Basic_TextureDrawType());	// テクスチャ枚数分
		}
	}

	void	Finalize(){
	}

	template<
		typename ResourceType
	>
	void	Begin(DrawInfo &draw_info, const ResourceType &res){

#ifdef BM3_TARGET_IDE
		OnSetRenderStateInfo	rsi(NULL, reinterpret_cast<const void*>(&res));
#else
		OnSetRenderStateInfo	rsi(&res.node_header_, reinterpret_cast<const void*>(&res));
#endif
		if(draw_info.OnSetRenderState(draw_info, rsi) == false){
			OnRenderState<ResourceType>(draw_info, rsi, NULL);
		}
	}

#pragma warning(push)
#pragma warning(disable: 4189)

	template<
		typename ResourceType
	>
	void	End(DrawInfo &draw_info, ShaderInfo &shader_info, VertexContext &ctx, int child_count, int push_count, const ResourceType &res){

		if(child_count >= 2){
			ProfileManager::instance().Draw_AddVertexProcessCount((child_count + push_count) * VertexPerPrimitiveCount);
			ProfileManager::instance().Draw_AddPrimitiveProcessCount(EmitterTypeConst_Stripe, push_count);
			ProfileManager::instance().Draw_AddVertexArrayDrawCount(1);

//			OnBeginDraw_SetRenderState(info, draw_info, res);
			internal::IdeSupport::BeginRenderStateApply(draw_info, res);
			// ユーザーコールバック
			if(draw_info.OnSetShader(draw_info, shader_info) == false){
				SetShaderType<ResourceType>(draw_info, res);
				SetShaderValue<ResourceType>(draw_info, shader_info, res);
				
				SetTexture(draw_info, res);
				RenderStateManager::instance().Apply();
			}
			internal::IdeSupport::EndRenderStateApply(draw_info, res);
			
			internal::IdeSupport::BeginDraw(draw_info, res);
			{
				ctx.SetStride(vertex_element_size_);
				ctx.SetVertexCount((child_count + push_count) * VertexPerPrimitiveCount);
				ctx.SetPrimType(D3DPT_TRIANGLESTRIP);
				if(draw_info.OnDraw(draw_info, ctx) == false){
					bm3::VertexManager::instance().PrepareDrawPrimitive(draw_info, ctx);
					bm3::VertexManager::instance().DrawPrimitive(draw_info, ctx);
				}
			}
			internal::IdeSupport::EndDraw(draw_info, res);
		}
	}

#pragma warning(pop)

	template<
		DrawScale::Type DrawScaleType,
		DrawRotate::Type DrawRotateType,
		DrawColor::Type DrawColorType,
		typename PrimitiveType,
		typename ResourceType
	>
	void	PushRange(const PrimitiveType *prim, int active_node_count, int *active_node_index_array,  DrawInfo &draw_info, const PacketDrawInfo &packet_draw_info, const ResourceType &res, void* vertex_buffer){

		ML_MAKEVERTEX_TIMER;
		ML_PTR_ASSERT(prim);
		ML_PTR_ASSERT(vertex_buffer);

		int tex_ref_count = TextureReferenceCount(res.Texture_Basic_TextureDrawType());
		typedef void (* TexturePushFunc)(TextureCoordinate *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, TextureLayer::Type lt, float v, int stride);

		TexturePushFunc	tpf[TextureLayer::Max];

		for(int j = 0;j != tex_ref_count;++ j){
			switch(Texture_Layer_TextureAnimationType(res, TextureLayer::Type(j))){
				case TextureAnimationTypeConst_Nothing:		tpf[j] = (TexturePushFunc)&policy::Texture2<DrawTexture::Nothing>::StaticPush;		break;
				case TextureAnimationTypeConst_Crop:		tpf[j] = (TexturePushFunc)&policy::Texture2<DrawTexture::Crop>::StaticPush;			break;
				case TextureAnimationTypeConst_Scroll:		tpf[j] = (TexturePushFunc)&policy::Texture2<DrawTexture::Scroll>::StaticPush;		break;
				case TextureAnimationTypeConst_Pattern:		tpf[j] = (TexturePushFunc)&policy::Texture2<DrawTexture::Pattern>::StaticPush;		break;
				case TextureAnimationTypeConst_Transform:	tpf[j] = (TexturePushFunc)&policy::Texture2<DrawTexture::Transform>::StaticPush;	break;
				default:									ML_ASSERT(false);																	break;
			}
		}

		if(vertex_buffer){
			float div_inv	= ml::inverse(ml::maximum(1.0f, float(packet_draw_info.all_active_child_count_ - 1)));
			float pc		= ml::numeric_cast<float>(packet_draw_info.index_);
			float ratio		= pc * div_inv;

			const PrimitiveType	&p_origin	= prim[active_node_index_array[0]];
			if(res.Texture_Basic_TextureDrawType() == TextureDrawTypeConst_Normalmapping){

				void	*dst_vertex = static_cast<void *>(vertex_buffer);

				for(int i=0 ;i < active_node_count ; ++i, dst_vertex = Next(dst_vertex, VertexPerPrimitiveCount), ratio += div_inv){
					int		index	= active_node_index_array[i];
					const PrimitiveType	&p	= prim[index];

					void *dst_ptr = (void *)dst_vertex;

					{
						PositionType *dst = (PositionType *)dst_ptr;
						policy::Position2<DrawScaleType, DrawRotateType>().Push(dst, p, draw_info, res, p.GetParentMatrix(), p.GetParentScale(), packet_draw_info, vertex_element_size_);
						dst_ptr = dst + 1;
					}

					{
						ColorType *dst = (ColorType *)dst_ptr;
						policy::Color2<DrawColorType>().Push(dst, p, draw_info, res, packet_draw_info, vertex_element_size_);
						dst_ptr = dst + 1;
					}

					{
						TextureCoordinate *dst = (TextureCoordinate *)dst_ptr;
						tpf[0](dst, p_origin, draw_info, res, TextureLayer::Type(0), ratio, vertex_element_size_);
						dst_ptr = dst + 1;
					}

					{
						TangentSpaceType *dst = (TangentSpaceType *)dst_ptr;
						policy::TangentSpace2<DrawRotateType>().Push(dst, p, draw_info, res, p.GetParentMatrix(), vertex_element_size_);
						dst_ptr = dst + 1;
					}
				}

			}else{
				VertexType	*dst_vertex = static_cast<VertexType *>(vertex_buffer);

				for(int i=0 ;i < active_node_count ; ++i, dst_vertex = Next(dst_vertex, VertexPerPrimitiveCount), ratio += div_inv){
					int		index	= active_node_index_array[i];
					const PrimitiveType	&p	= prim[index];

					policy::Position2<DrawScaleType, DrawRotateType>().Push(&dst_vertex->position_,	p, draw_info, res, p.GetParentMatrix(), p.GetParentScale(), packet_draw_info,	vertex_element_size_);
					policy::Color2<DrawColorType>().Push(					&dst_vertex->color_,	p, draw_info, res,													packet_draw_info, vertex_element_size_);

					for(int j = 0;j != tex_ref_count;++ j){
						ML_PTR_ASSERT(tpf[j]);
						tpf[j](&dst_vertex->texture_[j],	p_origin, draw_info, res, TextureLayer::Type(j),	ratio, vertex_element_size_);
					}
				}
			}
		}
	}

	// BM2互換版
	template<
		DrawScale::Type DrawScaleType,
		DrawRotate::Type DrawRotateType,
		DrawColor::Type DrawColorType,
		typename PrimitiveType,
		typename ResourceType
	>
	void	PushRangeBm2(const PrimitiveType *prim, int active_node_count, int *active_node_index_array,  DrawInfo &draw_info, const PacketDrawInfo &packet_draw_info, const ResourceType &res, void* vertex_buffer){

		ML_MAKEVERTEX_TIMER;
		ML_PTR_ASSERT(prim);
		ML_PTR_ASSERT(vertex_buffer);

		int tex_ref_count = TextureReferenceCount(res.Texture_Basic_TextureDrawType());
		typedef void (* TexturePushFunc)(TextureCoordinate *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, TextureLayer::Type lt, float v, int stride);

		TexturePushFunc	tpf[TextureLayer::Max];

		for(int j = 0;j != tex_ref_count;++ j){
			switch(Texture_Layer_TextureAnimationType(res, TextureLayer::Type(j))){
				case TextureAnimationTypeConst_Nothing:		tpf[j] = (TexturePushFunc)&policy::Texture2<DrawTexture::Nothing>::StaticPush;		break;
				case TextureAnimationTypeConst_Crop:		tpf[j] = (TexturePushFunc)&policy::Texture2<DrawTexture::Crop>::StaticPush;			break;
				case TextureAnimationTypeConst_Scroll:		tpf[j] = (TexturePushFunc)&policy::Texture2<DrawTexture::Scroll>::StaticPush;		break;
				case TextureAnimationTypeConst_Pattern:		tpf[j] = (TexturePushFunc)&policy::Texture2<DrawTexture::Pattern>::StaticPush;		break;
				case TextureAnimationTypeConst_Transform:	tpf[j] = (TexturePushFunc)&policy::Texture2<DrawTexture::Transform>::StaticPush;	break;
				default:									ML_ASSERT(false);																	break;
			}
		}

		if(vertex_buffer){
			float div_inv	= ml::inverse(ml::maximum(1.0f, float(packet_draw_info.all_active_child_count_ - 1)));
			float pc		= ml::numeric_cast<float>(packet_draw_info.index_);
			float ratio		= pc * div_inv;

			const PrimitiveType	&p_origin	= prim[active_node_index_array[0]];
			if(res.Texture_Basic_TextureDrawType() == TextureDrawTypeConst_Normalmapping){
				void	*dst_vertex = static_cast<void *>(vertex_buffer);

				for(int i=0 ;i < active_node_count ; ++i, dst_vertex = Next(dst_vertex, VertexPerPrimitiveCount), ratio += div_inv){
					int		index	= active_node_index_array[i];
					const PrimitiveType	&p	= prim[index];

					void *dst_ptr = (void *)dst_vertex;

					{
						PositionType *dst = (PositionType *)dst_ptr;
						policy::Position2<DrawScaleType, DrawRotateType>().PushBm2(dst, p, draw_info, res, p.GetParentMatrix(), p.GetParentScale(), packet_draw_info, vertex_element_size_);
						dst_ptr = dst + 1;
					}

					{
						ColorType *dst = (ColorType *)dst_ptr;
						if(res.Blend_Basic_BlendType() == bm3::BlendTypeConst_Add ||res.Blend_Basic_BlendType() == bm3::BlendTypeConst_Sub )
							policy::Color2<DrawColorType>().PushBm2(dst, p, draw_info, res, packet_draw_info, vertex_element_size_);
						else
							policy::Color2<DrawColorType>().Push(dst, p, draw_info, res, packet_draw_info, vertex_element_size_);
						dst_ptr = dst + 1;
					}

					{
						TextureCoordinate *dst = (TextureCoordinate *)dst_ptr;
						tpf[0](dst, p_origin, draw_info, res, TextureLayer::Type(0), ratio, vertex_element_size_);
						dst_ptr = dst + 1;
					}

					{
						TangentSpaceType *dst = (TangentSpaceType *)dst_ptr;
						policy::TangentSpace2<DrawRotateType>().Push(dst, p, draw_info, res, p.GetParentMatrix(), vertex_element_size_);
						dst_ptr = dst + 1;
					}
				}

			}else{
				VertexType	*dst_vertex = static_cast<VertexType *>(vertex_buffer);

				if(res.Blend_Basic_BlendType() == bm3::BlendTypeConst_Add ||res.Blend_Basic_BlendType() == bm3::BlendTypeConst_Sub ){
					for(int i=0 ;i < active_node_count ; ++i, dst_vertex = Next(dst_vertex, VertexPerPrimitiveCount), ratio += div_inv){
						int		index	= active_node_index_array[i];
						const PrimitiveType	&p	= prim[index];

						policy::Position2<DrawScaleType, DrawRotateType>().PushBm2(&dst_vertex->position_,	p, draw_info, res, p.GetParentMatrix(), p.GetParentScale(), packet_draw_info,	vertex_element_size_);
						policy::Color2<DrawColorType>().PushBm2(					&dst_vertex->color_,	p, draw_info, res,													packet_draw_info, vertex_element_size_);
						for(int j = 0;j != tex_ref_count;++ j){
							ML_PTR_ASSERT(tpf[j]);
							tpf[j](&dst_vertex->texture_[j],	p_origin, draw_info, res, TextureLayer::Type(j),	ratio, vertex_element_size_);
						}
					}
				}
				else{

					for(int i=0 ;i < active_node_count ; ++i, dst_vertex = Next(dst_vertex, VertexPerPrimitiveCount), ratio += div_inv){
						int		index	= active_node_index_array[i];
						const PrimitiveType	&p	= prim[index];

						policy::Position2<DrawScaleType, DrawRotateType>().PushBm2(&dst_vertex->position_,	p, draw_info, res, p.GetParentMatrix(), p.GetParentScale(), packet_draw_info,	vertex_element_size_);
						policy::Color2<DrawColorType>().Push(					&dst_vertex->color_,	p, draw_info, res,													packet_draw_info, vertex_element_size_);
						for(int j = 0;j != tex_ref_count;++ j){
							ML_PTR_ASSERT(tpf[j]);
							tpf[j](&dst_vertex->texture_[j],	p_origin, draw_info, res, TextureLayer::Type(j),	ratio, vertex_element_size_);
						}
					}
				}
			}
		}
	}


	void BeginMakeVertex(obj::RootNodeBase &root, int emitter_index){
		vertex_first_ = root.VertexBuffer(emitter_index, vertex_element_size_);		// 縮退ポリゴン用バッファを確保
	}

	void EndMakeVertex(obj::RootNodeBase &root, int emitter_index){
		void* vertex_buffer = root.VertexBuffer(emitter_index, vertex_element_size_);

		// 縮退ポリゴン作成
		if(vertex_buffer!=NULL){
			VertexType	*vertex = static_cast<VertexType*>(vertex_first_);
			Copy(vertex, Next(vertex));

			vertex = static_cast<VertexType*>(vertex_buffer);
			Copy(vertex, Prior(vertex));
		}
	}

	int VertexBufferSize(int child_count){ return child_count * VertexPerPrimitiveCount * vertex_element_size_; }
	int VertexBufferSize(int child_count, int push_count){ return (child_count + push_count) * VertexPerPrimitiveCount * vertex_element_size_; }
	int TotalVertexBufferSize(int child_count, int push_count){ return (child_count + push_count) * VertexPerPrimitiveCount * vertex_element_size_; }

	template<typename ResourceType>
	void GetVertexInfo(VertexInfo &info, int child_count, int push_count, const ResourceType &res){
		info.SetCount(VertexPerPrimitiveCount*(child_count + push_count));
		info.SetSize(vertex_element_size_);
		info.SetCountPerPrimitive(VertexPerPrimitiveCount);

		{
			OnBeginDraw_SetRenderType(res);
			SetTextureType(res);
			policy::DefaultRenderState::GetVertexInfo(info);
		}
	}

	void DestroyResource(){
	}
	
	template<typename ResourceType>
	void RestoreResource(RestoreInfo & /*info*/, const ResourceType & /*res*/){
	}
		
private:
	enum{
		VertexPerPrimitiveCount	= 2
	};

	typedef	draw::policy::PositionType		PositionType;
	typedef	draw::policy::NormalType		NormalType;
	typedef	draw::policy::ByteColorBgra		ColorType;
	typedef	draw::policy::TangentSpaceType	TangentSpaceType;

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4200)
#endif
	struct VertexType{
		PositionType		position_;
		ColorType			color_;
		TextureCoordinate	texture_[];
	};
#if defined(_MSC_VER)
#pragma warning(pop)
#endif

	void*		vertex_first_;
	int			vertex_element_size_;

	void	*Next(void *v, int count = 1){	return ml::offset_pointer<void *>(v, +vertex_element_size_ * count);	}
	void	*Prior(void *v, int count = 1){	return ml::offset_pointer<void *>(v, -vertex_element_size_ * count);	}
	VertexType	*Next(VertexType *v, int count = 1){	return ml::offset_pointer<VertexType *>(v, +vertex_element_size_ * count);	}
	VertexType	*Prior(VertexType *v, int count = 1){	return ml::offset_pointer<VertexType *>(v, -vertex_element_size_ * count);	}

	void		Copy(VertexType *dst, const VertexType *src){
		std::memcpy(dst, src, vertex_element_size_);
	}
};

}	// namespace draw
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_SDK_INC_BM3_CORE_DRAW_DIRECTX9_BM3_DRAW_STRIPE_H
