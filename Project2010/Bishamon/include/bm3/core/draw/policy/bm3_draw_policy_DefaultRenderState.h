#ifndef BM3_SDK_INC_BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_DEFAULTSETRENDERSTATE_H
#define BM3_SDK_INC_BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_DEFAULTSETRENDERSTATE_H

#include "bm3/system/bm3_Config.h"
#include "bm3_draw_policy_Common.h"
#include "../../bm3_DrawInfo.h"
#include "../../bm3_CoreType.h"
#include "../../texture/bm3_tex_HandleBase.h"
#include "../../bm3_RenderStateManager.h"
#include "../bm3_draw_Common.h"
#include "../../bm3_Const.h"
#include "../../bm3_VertexInfo.h"


namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE
namespace draw{
namespace policy{

class DefaultRenderState{
	
protected:
	
	template<typename ResourceType>
	static void	OnRenderState(DrawInfo &draw_info, OnSetRenderStateInfo &info, void* /*data*/){
		const ResourceType	&res = *reinterpret_cast<const ResourceType*>(info.resource_);
		RenderStateManager::instance().SetBlendType(draw_info,						res.Blend_Basic_BlendType());
		RenderStateManager::instance().SetDepthTestType(draw_info,					res.Material_DrawControl_DepthTestType());
		RenderStateManager::instance().SetEnable<RenderState::Culling>(draw_info,	res.Material_DrawControl_Culling());
		RenderStateManager::instance().SetEnable<RenderState::DepthWrite>(draw_info,res.Material_DrawControl_DepthWrite());
		RenderStateManager::instance().SetEnable<RenderState::AlphaTest>(draw_info,	res.Blend_Basic_AlphaTest());
		RenderStateManager::instance().SetAlphaTestType(draw_info,					res.Blend_Basic_AlphaTestType(), res.Blend_Basic_AlphaRef());
	}

#if defined(BM3_PLATFORM_xbox360)
	template<typename ResourceType>
	BM3_FORCEINLINE void	SetShaderType(DrawInfo &draw_info, const ResourceType &res){
#else
	template<typename ResourceType>
	BM3_FORCEINLINE void	SetShaderType(DrawInfo &draw_info, const ResourceType &res){
#endif

		RenderStateManager::instance().DisableAllFuture();
		RenderStateManager::instance().EnableVertexColor();

#ifndef BM3_PLATFORM_psp
		if(res.Detail_Soft_EnableSoft()){
			RenderStateManager::instance().EnableSoft();
#if defined(BM3_PLATFORM_win_directx9) || defined(BM3_PLATFORM_win_opengl) || defined(BM3_PLATFORM_xbox360) || defined(BM3_PLATFORM_psvita)
			if(draw_info.IsEnabledSoftN()){
				RenderStateManager::instance().EnableSoftN();
			}
#endif
		}

		if(res.Texture_Basic_TextureDrawType() == TextureDrawTypeConst_Normalmapping){
			RenderStateManager::instance().EnableLight();
			RenderStateManager::instance().EnableSpecular();
		}
#endif
		if(draw_info.GetFogInfo() && (res.Material_DrawControl_FogEffect()!=0.0f)){
			RenderStateManager::instance().EnableFog();
		}
	}

	template<typename ResourceType>
	static BM3_FORCEINLINE void	SetShaderNoTextureType(DrawInfo &draw_info, const ResourceType &res){
	
		RenderStateManager::instance().DisableAllFuture();

		RenderStateManager::instance().EnableVertexColor();
		
		// クアッド以外は必要ない
/*
		if(res.Detail_Soft_EnableSoft()){
			RenderStateManager::instance().EnableSoft();
#if defined(BM3_PLATFORM_xbox360) || defined(BM3_PLATFORM_psvita)
			if(draw_info.IsEnabledSoftN()){
				RenderStateManager::instance().EnableSoftN();
			}
#endif
		}
*/		
		if(draw_info.GetFogInfo() && (res.Material_DrawControl_FogEffect()!=0.0f)){
			RenderStateManager::instance().EnableFog();
		}
	}
	
	template<typename ResourceType>
	static BM3_FORCEINLINE void	SetShaderValue(DrawInfo &draw_info, const ShaderInfo &shader_info, const ResourceType &res){
		
		RenderStateManager::instance().SetDefaultColor();
		RenderStateManager::instance().SetDefaultTexture();
		RenderStateManager::instance().SetColorScale(shader_info.color_scale_);
		RenderStateManager::instance().SetOffsetPosition(shader_info.offset_position_);
		if(res.Detail_Soft_EnableSoft()){
			RenderStateManager::instance().SetSoftThreshold(res.Detail_Soft_Threshold());
		}
#ifndef BM3_PLATFORM_psp
		if(res.Texture_Basic_TextureDrawType() == TextureDrawTypeConst_Normalmapping){
			RenderStateManager::instance().SetSpecularColor(res.Texture_Basic_SpecularColor());
			RenderStateManager::instance().SetSpecularPower(res.Texture_Basic_SpecularFactor());

			RenderStateManager::instance().SetAmbientLightColor(ml::ni::color_rgba<float>(1.0f, 1.0f, 1.0f, 1.0f));
			RenderStateManager::instance().SetDiffuseColor(ml::ni::color_rgba<float>(1.0f, 1.0f, 1.0f, 1.0f));
		}
#endif
		if(draw_info.GetFogInfo()){
			const FogInfo *fog = draw_info.GetFogInfo();
			RenderStateManager::instance().SetFogColor(fog->color_);
			RenderStateManager::instance().SetFogArea(ml::vector3d(fog->start_, fog->end_, fog->near_, fog->far_));
			RenderStateManager::instance().SetFogDensity(res.Material_DrawControl_FogEffect());
		}
	}
	
	template<typename ResourceType>
	static BM3_FORCEINLINE void	SetShaderNoTextureValue(DrawInfo &draw_info, const ShaderInfo &shader_info, const ResourceType &res){

		RenderStateManager::instance().SetDefaultColor();
		RenderStateManager::instance().SetDefaultTexture();
		RenderStateManager::instance().SetColorScale(shader_info.color_scale_);
		RenderStateManager::instance().SetOffsetPosition(shader_info.offset_position_);

		// クアッド以外は必要ない
//		if(res.Detail_Soft_EnableSoft()){
//			RenderStateManager::instance().SetSoftThreshold(res.Detail_Soft_Threshold());
//		}

		if(draw_info.GetFogInfo()){
			const FogInfo *fog = draw_info.GetFogInfo();
			RenderStateManager::instance().SetFogColor(fog->color_);
			RenderStateManager::instance().SetFogArea(ml::vector3d(fog->start_, fog->end_, fog->near_, fog->far_));
			RenderStateManager::instance().SetFogDensity(res.Material_DrawControl_FogEffect());
		}

	}	

	template<
		typename ResourceType
	>
	BM3_FORCEINLINE void	OnBeginDraw_SetRenderState(const BeginInfo &info, DrawInfo &draw_info, const ResourceType &res){

		OnBeginDraw_SetRenderType(res);
		RenderStateManager::instance().SetDefaultColor();
		RenderStateManager::instance().SetDefaultTexture();

		RenderStateManager::instance().SetColorScale(info.color_scale_);
		RenderStateManager::instance().SetOffsetPosition(info.offset_position_);
		RenderStateManager::instance().SetBlendType(draw_info, res.Blend_Basic_BlendType());
		RenderStateManager::instance().SetDepthTestType(draw_info, res.Material_DrawControl_DepthTestType());
		RenderStateManager::instance().SetEnable<RenderState::Culling>(draw_info, res.Material_DrawControl_Culling());
		RenderStateManager::instance().SetEnable<RenderState::DepthWrite>(draw_info, res.Material_DrawControl_DepthWrite());
		RenderStateManager::instance().SetEnable<RenderState::AlphaTest>(draw_info, res.Blend_Basic_AlphaTest());
		RenderStateManager::instance().SetAlphaTestType(draw_info, res.Blend_Basic_AlphaTestType(), res.Blend_Basic_AlphaRef());

#ifndef BM3_PLATFORM_psp
		if(res.Detail_Soft_EnableSoft()){
			RenderStateManager::instance().SetSoftThreshold(res.Detail_Soft_Threshold());
			// シェーダの有効性はOnBeginDraw_SetRenderTypeで設定した方がいいが、draw_infoを得られない関係でこちらへ。
#if defined(BM3_PLATFORM_win_directx9) || defined(BM3_PLATFORM_win_opengl) || defined(BM3_PLATFORM_xbox360) || defined(BM3_PLATFORM_psvita)
			if(draw_info.IsEnabledSoftN()){
				RenderStateManager::instance().EnableSoftN();
			}
#endif
		}
		if(res.Texture_Basic_TextureDrawType() == TextureDrawTypeConst_Normalmapping){
			RenderStateManager::instance().SetSpecularColor(res.Texture_Basic_SpecularColor());
			RenderStateManager::instance().SetSpecularPower(res.Texture_Basic_SpecularFactor());

			RenderStateManager::instance().SetAmbientLightColor(ml::ni::color_rgba<float>(1.0f, 1.0f, 1.0f, 1.0f));
			RenderStateManager::instance().SetDiffuseColor(ml::ni::color_rgba<float>(1.0f, 1.0f, 1.0f, 1.0f));
		}
#endif
	}

	template<
		typename ResourceType
	>
	BM3_FORCEINLINE void	OnBeginDraw_SetRenderType(const ResourceType &res){

		RenderStateManager::instance().DisableAllFuture();

		RenderStateManager::instance().EnableVertexColor();

#ifndef BM3_PLATFORM_psp
		if(res.Detail_Soft_EnableSoft()){
			RenderStateManager::instance().EnableSoft();
		}

		if(res.Texture_Basic_TextureDrawType() == TextureDrawTypeConst_Normalmapping){
			RenderStateManager::instance().EnableLight();
			RenderStateManager::instance().EnableSpecular();
		}
#endif
	}

	template<
		typename ResourceType
	>
	BM3_FORCEINLINE void	OnBeginDraw_SetRenderStateNoTextureDrawType(const BeginInfo &info, DrawInfo &draw_info, const ResourceType &res){

		OnBeginDraw_SetRenderTypeNoTextureDrawType(res);

		RenderStateManager::instance().SetColorScale(info.color_scale_);
		RenderStateManager::instance().SetOffsetPosition(info.offset_position_);
		RenderStateManager::instance().SetBlendType(draw_info, res.Blend_Basic_BlendType());
		RenderStateManager::instance().SetDepthTestType(draw_info, res.Material_DrawControl_DepthTestType());
		RenderStateManager::instance().SetEnable<RenderState::Culling>(draw_info, res.Material_DrawControl_Culling());
		RenderStateManager::instance().SetEnable<RenderState::DepthWrite>(draw_info, res.Material_DrawControl_DepthWrite());
		RenderStateManager::instance().SetEnable<RenderState::AlphaTest>(draw_info, res.Blend_Basic_AlphaTest());
		RenderStateManager::instance().SetAlphaTestType(draw_info, res.Blend_Basic_AlphaTestType(), res.Blend_Basic_AlphaRef());

		if(res.Detail_Soft_EnableSoft()){
			RenderStateManager::instance().SetSoftThreshold(res.Detail_Soft_Threshold());
			// シェーダの有効性はOnBeginDraw_SetRenderTypeで設定した方がいいが、draw_infoを得られない関係でこちらへ。
#if defined(BM3_PLATFORM_win_directx9) || defined(BM3_PLATFORM_win_opengl) || defined(BM3_PLATFORM_xbox360) || defined(BM3_PLATFORM_psvita)
			if(draw_info.IsEnabledSoftN()){
				RenderStateManager::instance().EnableSoftN();
			}
#endif
		}
	}

	template<
		typename ResourceType
	>
	BM3_FORCEINLINE void	OnBeginDraw_SetRenderTypeNoTextureDrawType(const ResourceType &res){

		RenderStateManager::instance().DisableAllFuture();
		RenderStateManager::instance().SetDefaultColor();
		RenderStateManager::instance().SetDefaultTexture();

		RenderStateManager::instance().EnableVertexColor();

		if(res.Detail_Soft_EnableSoft()){
			RenderStateManager::instance().EnableSoft();
		}
	}

	BM3_FORCEINLINE void	GetVertexInfo(VertexInfo &info){
		RenderStateManager::instance().GetVertexInfo(info);
	}

};

}	// namespace policy
}	// namespace draw
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_DEFAULTSETRENDERSTATE_H
