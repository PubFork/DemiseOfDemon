#ifndef BM3_SDK_INC_BM3_SDK_INC_BM3_CORE_DRAW_DIRECTX9_BM3_DRAW_MODEL_H
#define BM3_SDK_INC_BM3_SDK_INC_BM3_CORE_DRAW_DIRECTX9_BM3_DRAW_MODEL_H

#include "bm3/system/bm3_Config.h"
#include "../policy/bm3_draw_policy_Matrix1.h"
#include "../policy/bm3_draw_policy_Color1.h"
#include "../policy/bm3_draw_policy_ModelTexture1.h"
#include "../policy/bm3_draw_policy_Common.h"
#include "../policy/bm3_draw_policy_HasNoTexture.h"
#include "../policy/bm3_draw_policy_DefaultRenderState.h"
#include "../../bm3_CoreType.h"
#include "../../bm3_RenderStateManager.h"
#include "../../bm3_ProfileManager.h"
#include "../../bm3_ModelRenderer.h"
#include "../bm3_draw_Common.h"
#include "../../object/bm3_obj_RootNodeBase.h"
#include "../../bm3_VertexInfo.h"
#include "../../bm3_IdeSupport.h"

#include <m3dg/model/m3dg_model.h>
#include <m3dg/m3dg_type.h>

#include <ml/type/ml_matrix44.h>
#include <ml/type/ml_color.h>
#include <ml/utility/ml_simpletimer.h>

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

class DrawInfo;

namespace draw{

class Model :
	public policy::HasNoTexture,
	public policy::DefaultRenderState{

public:
	Model(){	;	}
	~Model(){	;	}

	template<typename ResourceType>
	void	Initialize(InitializeInfo & /*info*/, const ResourceType &/*res*/){
		vertex_element_size_ = sizeof(VertexType);
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

	template<
		typename ResourceType
	>
	void	End(DrawInfo &draw_info, ShaderInfo &shader_info, VertexContext &ctx, int child_count, int /*push_count*/, const ResourceType &res){

		ProfileManager::instance().Draw_AddPrimitiveProcessCount(EmitterTypeConst_Model, child_count);

		RenderStateManager::instance().SetTextureAddressType(draw_info, TextureLayer::_1, res.Model_TextureLayer1_TextureAddressType());
		RenderStateManager::instance().SetTextureFilterType( draw_info, TextureLayer::_1, res.Model_TextureLayer1_TextureFilterType());

		{
			SetTexture(draw_info, res);	// テクスチャのタイプを設定しているだけ
		}
		
		shader_info.fog_influence_	= res.Material_DrawControl_FogEffect();
		shader_info.lighting_		= res.Material_DrawControl_Lighting();
		internal::RenderDevicePolicyBase::DirectionalLight	backup_light;
		if(res.Material_DrawControl_Lighting()==false){
			RenderStateManager::instance().GetDirectionalLight(backup_light);
			RenderStateManager::instance().NonDirectionalLight();
		}
		
		internal::IdeSupport::BeginDraw(draw_info, res);
		{
			VertexType*	vertex_array = static_cast<VertexType*>(ctx.Buffer());
			ml::vector3d	tex_scroll;
			for(int i = 0;i != child_count;++ i){

				tex_scroll.e_.x_	= vertex_array->texture_scroll_.u_;
				tex_scroll.e_.y_	= vertex_array->texture_scroll_.v_;
				tex_scroll.e_.z_	= 0.0f;	tex_scroll.e_.w_	= 1.0f;
				shader_info.world_matrix_	= &vertex_array->matrix_;
				shader_info.texture_scroll_	= &tex_scroll;

				ModelDrawInfo	mdi;
				{
					mdi.frame_			= 0.0f;
					mdi.life_frame_		= vertex_array->life_frame_;
					mdi.diffuse_color_	= vertex_array->color_;
				}

				ModelRenderer::instance().Draw(res.Model_Basic_ModelName(), mdi, draw_info, shader_info);
				vertex_array++;
			}
		}
		internal::IdeSupport::EndDraw(draw_info, res);

		if(res.Material_DrawControl_Lighting()==false){
			RenderStateManager::instance().SetDirectionalLight(backup_light);
		}
		RenderStateManager::instance().SetTexture0Scroll(ml::vector3d());
		RenderStateManager::instance().SetWorldMatrix(ml::matrix44());
	}

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

		VertexType	*dst_vertex = static_cast<VertexType*>(vertex_buffer);

		for(int i=0 ;i < active_node_count ; ++i, dst_vertex += VertexPerPrimitiveCount){
			int		index	= active_node_index_array[i];
			const PrimitiveType	&p	= prim[index];
			
			policy::Matrix1<DrawScaleType, DrawRotateType>().Push(	&dst_vertex->matrix_,			p, draw_info, res , p.GetParentMatrix(), p.GetParentScale(), packet_draw_info);
			policy::Color1<DrawColorType>().Push(					&dst_vertex->color_,			p, draw_info, res, packet_draw_info);
			policy::ModelTexture1<DrawColorType>().Push(			&dst_vertex->texture_scroll_,	p, draw_info, res);
			dst_vertex->life_frame_	= p.LifeFrame();
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

		VertexType	*dst_vertex = static_cast<VertexType*>(vertex_buffer);
		
		if(res.Blend_Basic_BlendType() == bm3::BlendTypeConst_Add ||res.Blend_Basic_BlendType() == bm3::BlendTypeConst_Sub ){
			for(int i=0 ;i < active_node_count ; ++i, dst_vertex += VertexPerPrimitiveCount){
				int		index	= active_node_index_array[i];
				const PrimitiveType	&p	= prim[index];
				
				policy::Matrix1<DrawScaleType, DrawRotateType>().PushBm2(	&dst_vertex->matrix_,			p, draw_info, res , p.GetParentMatrix(), p.GetParentScale(), packet_draw_info);
				policy::Color1<DrawColorType>().PushBm2(					&dst_vertex->color_,			p, draw_info, res, packet_draw_info);
				policy::ModelTexture1<DrawColorType>().Push(			&dst_vertex->texture_scroll_,	p, draw_info, res);
				dst_vertex->life_frame_	= p.LifeFrame();
			}
		}
		else{
			for(int i=0 ;i < active_node_count ; ++i, dst_vertex += VertexPerPrimitiveCount){
				int		index	= active_node_index_array[i];
				const PrimitiveType	&p	= prim[index];
				
				policy::Matrix1<DrawScaleType, DrawRotateType>().PushBm2(	&dst_vertex->matrix_,			p, draw_info, res , p.GetParentMatrix(), p.GetParentScale(), packet_draw_info);
				policy::Color1<DrawColorType>().Push(					&dst_vertex->color_,			p, draw_info, res, packet_draw_info);
				policy::ModelTexture1<DrawColorType>().Push(			&dst_vertex->texture_scroll_,	p, draw_info, res);
				dst_vertex->life_frame_	= p.LifeFrame();
			}
		}
	}

	void BeginMakeVertex(obj::RootNodeBase &/*root*/, int /*emitter_index*/){
	}

	void EndMakeVertex(obj::RootNodeBase &/*root*/, int /*emitter_index*/){
	}

	int VertexBufferSize(int child_count){ return child_count * VertexPerPrimitiveCount * vertex_element_size_; }
	int VertexBufferSize(int child_count, int /*push_count*/){ return VertexBufferSize(child_count); }
	int TotalVertexBufferSize(int child_count, int /*push_count*/){ return VertexBufferSize(child_count); }

	template<typename ResourceType>
	void GetVertexInfo(VertexInfo &info, int child_count, int /*push_count*/, const ResourceType &res){
		info.SetCount(VertexPerPrimitiveCount*child_count);
		info.SetSize(vertex_element_size_);
		info.SetCountPerPrimitive(VertexPerPrimitiveCount);

		{
			OnBeginDraw_SetRenderTypeNoTextureDrawType(res);
			ModelRenderer::instance().SetRenderType(res.Model_Basic_ModelName());
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
		VertexPerPrimitiveCount	= 1
	};

	typedef	ml::ni::matrix44	MatrixType;

	struct VertexType{
		MatrixType			matrix_;
		ColorType			color_;
		TextureCoordinate	texture_scroll_;
		float               life_frame_;
	};

	int			vertex_element_size_;
};

}	// namespace draw
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_SDK_INC_BM3_CORE_DRAW_DIRECTX9_BM3_DRAW_Model_H
