#ifndef BM3_SDK_INC_BM3_CORE_BM3_IDESUPPORT_H
#define BM3_SDK_INC_BM3_CORE_BM3_IDESUPPORT_H

#include "../system/bm3_Config.h"
#include "bm3_DrawInfo.h"
#include "bm3_RenderStateManager.h"
#include <ml/utility/ml_noncopyable.h>

#if defined(BM3_TARGET_IDE)
	#include <windows.h>
	#include <d3d9.h>
	#include <gl/GL.H>
#endif

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

namespace internal{

class IdeSupport : ml::noncopyable{

//	方針
//		メソッドはすべてスタティック。状態は持たない
//		IDE時以外はオーバーヘッドなし

public:
#if defined(BM3_TARGET_IDE)
	template<typename ResourceType>
	BM3_FORCEINLINE static void	BeginDraw(DrawInfo &draw_info, const ResourceType &res){
		IMPL_BeginDraw(draw_info, res);
	}
#else
	template<typename ResourceType>
	BM3_FORCEINLINE static void	BeginDraw(DrawInfo &/*draw_info*/, const ResourceType &/*res*/){
	}
#endif

#if defined(BM3_TARGET_IDE)
	template<typename ResourceType>
	BM3_FORCEINLINE static void	EndDraw(DrawInfo &draw_info, const ResourceType &res){
		IMPL_EndDraw(draw_info, res);
	}
#else
	template<typename ResourceType>
	BM3_FORCEINLINE static void	EndDraw(DrawInfo &/*draw_info*/, const ResourceType &/*res*/){
	}
#endif

#if defined(BM3_TARGET_IDE)
	template<typename ResourceType>
	BM3_FORCEINLINE static void	BeginRenderStateApply(DrawInfo &draw_info, const ResourceType &res){
		IMPL_BeginRenderStateApply(draw_info, res);
	}
#else
	template<typename ResourceType>
	BM3_FORCEINLINE static void	BeginRenderStateApply(DrawInfo &/*draw_info*/, const ResourceType &/*res*/){
	}
#endif

#if defined(BM3_TARGET_IDE)
	template<typename ResourceType>
	BM3_FORCEINLINE static void	EndRenderStateApply(DrawInfo &draw_info, const ResourceType &res){
		IMPL_EndRenderStateApply(draw_info, res);
	}
#else
	template<typename ResourceType>
	BM3_FORCEINLINE static void	EndRenderStateApply(DrawInfo &/*draw_info*/, const ResourceType &/*res*/){
	}
#endif

private:
#if defined(BM3_TARGET_IDE)
	template<typename ResourceType>
	static void	IMPL_BeginDraw(DrawInfo &draw_info, const ResourceType &res){

		if(draw_info.IsWireframeDraw()){
			#if defined(BM3_PLATFORM_win_directx9)
			draw_info.Renderer().d3d_device_->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
			#endif

			#if defined(BM3_PLATFORM_win_opengl)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			#endif
		}

		IMPL_BeginRenderStateApply(draw_info, res);
	}

	template<typename ResourceType>
	static void	IMPL_EndDraw(DrawInfo &draw_info, const ResourceType &res){

		IMPL_EndRenderStateApply(draw_info, res);

		if(draw_info.IsWireframeDraw()){
			#if defined(BM3_PLATFORM_win_directx9)
			draw_info.Renderer().d3d_device_->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
			#endif

			#if defined(BM3_PLATFORM_win_opengl)
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			#endif
		}
	}

	template<typename ResourceType>
	static void	IMPL_BeginRenderStateApply(DrawInfo &draw_info, const ResourceType &res){

		if(draw_info.IsSelectionHighlight()){
			ColorType	cs;
			{
				if(res.IsSelected()){
					cs.e_.r_ = 1.0f;
					cs.e_.g_ = 1.0f;
					cs.e_.b_ = 1.0f;
					cs.e_.a_ = 1.0f;
				}else{
					cs.e_.r_ = 0.15f;
					cs.e_.g_ = 0.15f;
					cs.e_.b_ = 0.15f;
					cs.e_.a_ = 1.0f;
				}

				RenderStateManager::instance().SetColorScale(cs);
			}
		}

		if(draw_info.IsWireframeDraw()){
			RenderStateManager::instance().FunctionMask_DisableTexture();
		}
	}

	template<typename ResourceType>
	static void	IMPL_EndRenderStateApply(DrawInfo &draw_info, const ResourceType &res){

		if(draw_info.IsSelectionHighlight()){
			ColorType	cs;
			{
				cs.e_.r_ = 1.0f;
				cs.e_.g_ = 1.0f;
				cs.e_.b_ = 1.0f;
				cs.e_.a_ = 1.0f;
			}
			RenderStateManager::instance().SetColorScale(cs);
		}

		if(draw_info.IsWireframeDraw()){
			RenderStateManager::instance().FunctionMask_EnableTexture();
		}
	}
#endif

};

}	// namespace internal

BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_BM3_IDESUPPORT_H
