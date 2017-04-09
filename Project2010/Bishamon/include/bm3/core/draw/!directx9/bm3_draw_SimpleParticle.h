#ifndef BM3_SDK_INC_BM3_SDK_INC_BM3_CORE_DRAW_DIRECTX9_BM3_DRAW_SIMPLEPARTICLE_H
#define BM3_SDK_INC_BM3_SDK_INC_BM3_CORE_DRAW_DIRECTX9_BM3_DRAW_SIMPLEPARTICLE_H

#include "bm3/system/bm3_Config.h"
#include "../policy/bm3_draw_policy_Position6.h"
#include "../policy/bm3_draw_policy_Color6.h"
#include "../policy/bm3_draw_policy_Texture6.h"
#include "../policy/bm3_draw_policy_Color1.h"
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

class SimpleParticle :
	public policy::HasTexture,
	public policy::DefaultRenderState{

public:
	SimpleParticle(){	;	}
	~SimpleParticle(){	;	}

	template<typename ResourceType>
	void	Initialize(InitializeInfo & /*info*/, const ResourceType &/*res*/){
		;
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

		if(child_count > 0){
			ProfileManager::instance().Draw_AddVertexProcessCount(child_count);
			ProfileManager::instance().Draw_AddPrimitiveProcessCount(EmitterTypeConst_SimpleParticle, child_count);
			ProfileManager::instance().Draw_AddVertexArrayDrawCount(1);

//			OnBeginDraw_SetRenderStateNoTextureDrawType(info, draw_info, res);

			internal::IdeSupport::BeginRenderStateApply(draw_info, res);
			// ユーザーコールバック
			if(draw_info.OnSetShader(draw_info, shader_info) == false){

				SetShaderNoTextureType<ResourceType>(draw_info, res);
				SetShaderNoTextureValue<ResourceType>(draw_info, shader_info, res);
					
				SetTexture(draw_info, res);

#if defined(BM3_TARGET_IDE)
				if(draw_info.IsWireframeDraw()){
					RenderStateManager::instance().ApplySimpleParticleSize1();
				}else{
					RenderStateManager::instance().ApplySimpleParticle();
				}
#else
				RenderStateManager::instance().ApplySimpleParticle();
#endif
			}
			internal::IdeSupport::EndRenderStateApply(draw_info, res);

			internal::IdeSupport::BeginDraw(draw_info, res);
			{
				HRESULT r0 = draw_info.Renderer().d3d_device_->SetRenderState(D3DRS_POINTSPRITEENABLE, TRUE);
				r0;	ML_ASSERT(r0 == D3D_OK);

				{
					ctx.SetStride(vertex_element_size_);
					ctx.SetVertexCount(child_count);
					ctx.SetPrimType(D3DPT_POINTLIST);
					if(draw_info.OnDraw(draw_info, ctx) == false){
						bm3::VertexManager::instance().PrepareDrawPrimitive(draw_info, ctx);
						bm3::VertexManager::instance().DrawPrimitive(draw_info, ctx);
					}
				}

				HRESULT r1 = draw_info.Renderer().d3d_device_->SetRenderState(D3DRS_POINTSPRITEENABLE, FALSE);
				r1;	ML_ASSERT(r1 == D3D_OK);
			}
			internal::IdeSupport::EndDraw(draw_info, res);
		}

	}

	template<
		DrawScale::Type DrawScaleType,
		DrawRotate::Type DrawRotateType,
		DrawColor::Type DrawColorType,
		typename PrimitiveType,
		typename ResourceType
	>
	void	PushRange(const PrimitiveType *prim, int active_node_count, int *active_node_index_array,  DrawInfo & /*draw_info*/, const PacketDrawInfo &/*packet_draw_info*/, const ResourceType & /*res*/, void* vertex_buffer){

		ML_MAKEVERTEX_TIMER;
		ML_PTR_ASSERT(prim);
		ML_PTR_ASSERT(vertex_buffer);

		VertexType	*dst_vertex = static_cast<VertexType *>(vertex_buffer);

		for(int i=0 ;i < active_node_count ; ++i, dst_vertex = Next(dst_vertex, VertexPerPrimitiveCount)){
			int		index	= active_node_index_array[i];
			const PrimitiveType	&p	= prim[index];

			ml::ni::vector3d pos;
			policy::MakeCenterPosition(pos, p, p.GetParentMatrix());

			dst_vertex->translate_.x_			= pos.e_.x_;
			dst_vertex->translate_.y_			= pos.e_.y_;
			dst_vertex->translate_.z_			= pos.e_.z_;
			dst_vertex->size_					= p.scale_.ValueX() * p.GetParentScale().e_.x_;;

			dst_vertex->color_.e_.r_ = ml::numeric_cast<ml::ui8>(p.color_center_.value_.e_.r_ * 255.0f);
			dst_vertex->color_.e_.g_ = ml::numeric_cast<ml::ui8>(p.color_center_.value_.e_.g_ * 255.0f);
			dst_vertex->color_.e_.b_ = ml::numeric_cast<ml::ui8>(p.color_center_.value_.e_.b_ * 255.0f);
			dst_vertex->color_.e_.a_ = ml::numeric_cast<ml::ui8>(p.color_center_.value_.e_.a_ * 255.0f);
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
	void	PushRangeBm2(const PrimitiveType *prim, int active_node_count, int *active_node_index_array,  DrawInfo & /*draw_info*/, const PacketDrawInfo &/*packet_draw_info*/, const ResourceType & /*res*/, void* vertex_buffer){

		ML_MAKEVERTEX_TIMER;
		ML_PTR_ASSERT(prim);
		ML_PTR_ASSERT(vertex_buffer);

		VertexType	*dst_vertex = static_cast<VertexType *>(vertex_buffer);

		for(int i=0 ;i < active_node_count ; ++i, dst_vertex = Next(dst_vertex, VertexPerPrimitiveCount)){
			int		index	= active_node_index_array[i];
			const PrimitiveType	&p	= prim[index];

			ml::ni::vector3d pos;
			policy::MakeCenterPosition(pos, p, p.GetParentMatrix());

			dst_vertex->translate_.x_			= pos.e_.x_;
			dst_vertex->translate_.y_			= pos.e_.y_;
			dst_vertex->translate_.z_			= pos.e_.z_;
			dst_vertex->size_					= p.scale_.ValueX() * p.GetParentScale().e_.x_;;

			float alpha	= p.color_center_.value_.e_.a_ * 255.0f;
			dst_vertex->color_.e_.r_ = ml::numeric_cast<ml::ui8>(p.color_center_.value_.e_.r_ * alpha);
			dst_vertex->color_.e_.g_ = ml::numeric_cast<ml::ui8>(p.color_center_.value_.e_.g_ * alpha);
			dst_vertex->color_.e_.b_ = ml::numeric_cast<ml::ui8>(p.color_center_.value_.e_.b_ * alpha);
			dst_vertex->color_.e_.a_ = ml::numeric_cast<ml::ui8>(alpha);
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
			SetTextureType(res);
			policy::DefaultRenderState::GetVertexInfo(info);
		}
	}

	void DestroyResource(){
	}
	
	template<typename ResourceType>
	void RestoreResource(RestoreInfo & /*info*/, const ResourceType & /*res*/){
	}
		
protected:
	template<
		typename ResourceType
	>
	void	EndCore(DrawInfo &draw_info, EmitterTypeConst emitter_type, void* vertex_buffer, int child_count, ResourceType &res){
/*
		if(child_count > 0){
			ProfileManager::instance().Draw_AddVertexProcessCount(child_count);
			ProfileManager::instance().Draw_AddPrimitiveProcessCount(emitter_type, child_count);
			ProfileManager::instance().Draw_AddVertexArrayDrawCount(1);

			internal::IdeSupport::BeginDraw(draw_info, res);

			HRESULT r0 = draw_info.Renderer().d3d_device_->SetRenderState(D3DRS_POINTSPRITEENABLE, TRUE);
			r0;	ML_ASSERT(r0 == D3D_OK);

#if defined(BM3_TARGET_IDE)
			HRESULT result_d = draw_info.Renderer().d3d_device_->DrawPrimitiveUP(D3DPT_POINTLIST, child_count, vertex_buffer, vertex_element_size_);
			result_d;	ML_ASSERT(result_d == D3D_OK);
#else
			bm3::VertexManager::instance().PrepareDrawPrimitive(draw_info, vertex_buffer, vertex_element_size_);
			HRESULT result_d = draw_info.Renderer().d3d_device_->DrawPrimitive(D3DPT_POINTLIST, 0, child_count);
			result_d;	ML_ASSERT(result_d == D3D_OK);
#endif

			HRESULT r1 = draw_info.Renderer().d3d_device_->SetRenderState(D3DRS_POINTSPRITEENABLE, FALSE);
			r1;	ML_ASSERT(r1 == D3D_OK);
			
			{
				RenderingPrimitiveInfo	prim_info;
				prim_info.SetStride(vertex_element_size_);
				prim_info.SetVertexCount(child_count * VertexPerPrimitiveCount);
				prim_info.SetPrimType(GL_QUADS);
				bm3::VertexManager::instance().DrawPrimitive(draw_info, ctx, prim_info);
			}

			internal::IdeSupport::EndDraw(draw_info, res);
			
			
		}
*/	}

private:
	enum{
		VertexPerPrimitiveCount	= 1
	};

	typedef	draw::policy::PositionType		PositionType;
	typedef	draw::policy::ByteColorBgra		ColorType;
	typedef	float							PointSizeType;

	struct VertexType{
#if 0
		PositionType		translate_;
		PositionType		field_info_position_;
		PointSizeType		size_;
		ml::ni::matrix44	parent_matrix_;

		ColorType			color_;
#else
		PositionType		translate_;
		PointSizeType		size_;
		ColorType			color_;
#endif
	};

	enum{
		vertex_element_size_	= sizeof(VertexType)
	};

	void	*Next(void *v, int count = 1){	return ml::offset_pointer<void *>(v, +vertex_element_size_ * count);	}
	void	*Prior(void *v, int count = 1){	return ml::offset_pointer<void *>(v, -vertex_element_size_ * count);	}
	VertexType	*Next(VertexType *v, int count = 1){	return ml::offset_pointer<VertexType *>(v, +vertex_element_size_ * count);	}
	VertexType	*Prior(VertexType *v, int count = 1){	return ml::offset_pointer<VertexType *>(v, -vertex_element_size_ * count);	}
};

}	// namespace draw
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_SDK_INC_BM3_CORE_DRAW_DIRECTX9_BM3_DRAW_SIMPLEPARTICLE_H
