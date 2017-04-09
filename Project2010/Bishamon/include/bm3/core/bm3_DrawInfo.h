#ifndef BM3_SDK_INC_BM3_CORE_BM3_DRAWINFO_H
#define BM3_SDK_INC_BM3_CORE_BM3_DRAWINFO_H

#include <ml/type/ml_matrix44.h>
#include <ml/type/ml_vector3d.h>
#include "../system/bm3_Config.h"
#include "bm3_CoreType.h"
#include "bm3_VertexInfo.h"

#if defined(BM3_PLATFORM_win_opengl)
	#include "!opengl/bm3_DrawInfoPlatformDependency.h"
#endif
#if defined(BM3_PLATFORM_win_directx9)
	#include "!directx9/bm3_DrawInfoPlatformDependency.h"
#endif
#if defined(BM3_PLATFORM_win_directx10)
	#include "!directx10/bm3_DrawInfoPlatformDependency.h"
#endif
#if defined(BM3_PLATFORM_playstation3)
	#include "!playstation3/bm3_DrawInfoPlatformDependency.h"
#endif
#if defined(BM3_PLATFORM_xbox360)
	#include "!xbox360/bm3_DrawInfoPlatformDependency.h"
#endif
#if defined(BM3_PLATFORM_psp)
	#include "!psp/bm3_DrawInfoPlatformDependency.h"
#endif
#if defined(BM3_PLATFORM_gamebryodx9)
	#include "!gamebryodx9/bm3_DrawInfoPlatformDependency.h"
#endif
#if defined(BM3_PLATFORM_opengles)
	#include "!opengles/bm3_DrawInfoPlatformDependency.h"
#endif
#if defined(BM3_PLATFORM_nw4c)
	#include "!nw4c/bm3_DrawInfoPlatformDependency.h"
#endif
#if defined(BM3_PLATFORM_psvita)
	#include "!psvita/bm3_DrawInfoPlatformDependency.h"
#endif

#include "bm3_RendererInfo.h"
#include "texture/bm3_tex_RenderTarget.h"
#include "draw/bm3_draw_Common.h"
#include "bm3_DrawInfoType.h"
#include "bm3_VertexContext.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

/*!
	@brief	フォグに関する情報を収める構造体
*/
struct FogInfo{
	/*!
		コンストラクタ。デフォルトの初期化を行う
	*/
	FogInfo() :
		start_(0.0f), end_(1.0f), near_(0.1f), far_(1000.0f)
	{
		color_.set(1.0f, 1.0f, 1.0f, 1.0f);
	}
	
	/*!
		コンストラクタ。メンバ変数を初期化する
	*/
	FogInfo(ml::color_rgba<float> &color, float start, float end, float near_clip, float far_clip) : 
		color_(color), start_(start), end_(end), near_(near_clip), far_(far_clip)
	{
		;
	}

	ml::color_rgba<float> color_;	//!< フォグカラー
	float	start_;		//!< フォグの始まり（0.0-1.0）
	float	end_;		//!< フォグの終わり（0.0-1.0）
	float	near_;		//!< 有効範囲の近い面
	float	far_;		//!< 有効範囲の遠い面
};

/// @brief DrawInfoクラス
class DrawInfo : public internal::DrawInfoPlatformDependency{

public:
	DrawInfo() :
		on_begin_draw_callback_(NULL),
		on_begin_draw_callback_data_(NULL),
		on_setrenderstate_callback_(NULL),
		on_setrenderstate_callback_data_(NULL),
		on_setshader_callback_(NULL),
		on_setshader_callback_data_(NULL),
		on_draw_callback_(NULL),
		on_draw_callback_data_(NULL),
		on_end_draw_callback_(NULL),
		on_end_draw_callback_data_(NULL),
		fog_info_(NULL),
		time_(0.0f)
	{
		;
	}
	
	DrawInfo(const ml::matrix44 &inverse_view_matrix, const ml::matrix44 &view_matrix, const ml::matrix44 &proj_matrix) :
		inverse_view_matrix_(inverse_view_matrix),
		view_matrix_(view_matrix),
		proj_matrix_(proj_matrix),
		depth_texture_(NULL),
		on_begin_draw_callback_(NULL),
		on_begin_draw_callback_data_(NULL),
		on_setrenderstate_callback_(NULL),
		on_setrenderstate_callback_data_(NULL),
		on_setshader_callback_(NULL),
		on_setshader_callback_data_(NULL),
		on_draw_callback_(NULL),
		on_draw_callback_data_(NULL),
		on_end_draw_callback_(NULL),
		on_end_draw_callback_data_(NULL),
		fog_info_(NULL),
		time_(0.0f)
	{

#if defined(BM3_TARGET_IDE)
		is_selection_highlight_	= true;
		is_wireframe_draw_		= false;
		is_depth_less_			= true;
#endif
	}

	virtual	~DrawInfo(){}

	const ml::matrix44	&InverseViewMatrix() const{						return inverse_view_matrix_;	}
	void				SetInverseViewMatrix(const ml::matrix44 &m){	inverse_view_matrix_ = m;		}

	const ml::matrix44	&ViewMatrix() const{							return view_matrix_;			}
	void				SetViewMatrix(const ml::matrix44 &m){			view_matrix_ = m;				}

	const ml::matrix44	&ProjectionMatrix() const{						return proj_matrix_;			}
	void				SetProjectionMatrix(const ml::matrix44 &m){		proj_matrix_ = m;				}

	RendererInfo		&Renderer(){									return renderer_info_;			}
	void				SetRenderer(RendererInfo &r){					renderer_info_ = r;				}

	texture::RenderTarget	*DepthTexture(){							return depth_texture_;			}
	void					SetDepthTexture(texture::RenderTarget *t){	depth_texture_ = t;				}

	void				SetFacingPosition(const ml::vector3d &position){	facing_position_ = position;}
	const ml::vector3d&	FacingPosition() const {							return facing_position_;	}

	virtual void OnPrimitiveDrawBegin(OnPrimitiveDrawInfo &/*info*/){	;	}	// 削除予定
	virtual void OnPrimitiveDrawEnd(OnPrimitiveDrawInfo &/*info*/){		;	}	// 削除予定

#if defined(BM3_TARGET_IDE)
	const ml::matrix44	&IdeEffectMatrix() const{						return ide_effect_matrix_;			}
	void				SetIdeEffectMatrix(const ml::matrix44 &m){		ide_effect_matrix_ = m;				}

	bool				IsSelectionHighlight() const{					return is_selection_highlight_;		}
	void				SetIsSelectionHighlight(bool i){				is_selection_highlight_ = i;		}

	bool				IsWireframeDraw() const{						return is_wireframe_draw_;			}
	void				SetIsWireframeDraw(bool i){						is_wireframe_draw_ = i;				}

	bool				IsDepthLess() const {							return is_depth_less_;				}
	void				SetIsDepthLess(bool i) {						is_depth_less_	= i;				}
#endif

	void RegistryOnBeginDraw(bool (*callback)(DrawInfo &draw_info, OnDrawInfo &info, void* data),void* data){
		on_begin_draw_callback_				= callback;
		on_begin_draw_callback_data_		= data;
	}
	void RegistryOnSetRenderState(bool (*callback)(DrawInfo &draw_info, OnSetRenderStateInfo &info, void* data),void* data){
		on_setrenderstate_callback_			= callback;
		on_setrenderstate_callback_data_	= data;
	}
	void RegistryOnSetShader(bool (*callback)(DrawInfo &draw_info, ShaderInfo &info, void* data),void* data){
		on_setshader_callback_				= callback;
		on_setshader_callback_data_			= data;
	}
	void RegistryOnDraw(bool (*callback)(DrawInfo &draw_info, VertexContext &ctx, void* data),void* data){
		on_draw_callback_					= callback;
		on_draw_callback_data_				= data;
	}
	void RegistryOnEndDraw(bool (*callback)(DrawInfo &draw_info, OnDrawInfo &ctx, void* data),void* data){
		on_end_draw_callback_				= callback;
		on_end_draw_callback_data_			= data;
	}
	
	bool OnBeginDraw(DrawInfo &draw_info, OnDrawInfo &info){
		if(on_begin_draw_callback_){
			return on_begin_draw_callback_(draw_info, info, on_begin_draw_callback_data_);
		}
		return false;
	}
	bool OnSetRenderState(DrawInfo &draw_info, OnSetRenderStateInfo &info){
		if(on_setrenderstate_callback_){
			return on_setrenderstate_callback_(draw_info, info, on_setrenderstate_callback_data_);
		}
		return false;
	}
	bool OnSetShader(DrawInfo &draw_info, ShaderInfo &info){
		if(on_setshader_callback_){
			return on_setshader_callback_(draw_info, info, on_setshader_callback_data_);
		}
		return false;
	}
	bool OnDraw(DrawInfo &draw_info, VertexContext &ctx){
		if(on_draw_callback_){
			return on_draw_callback_(draw_info, ctx, on_draw_callback_data_);
		}
		return false;
	}
	bool OnEndDraw(DrawInfo &draw_info, OnDrawInfo &info){
		if(on_end_draw_callback_){
			return on_end_draw_callback_(draw_info, info, on_end_draw_callback_data_);
		}
		return false;
	}
	
	/*!
		FogInfo構造体をセットする。フォグが有効でないときはNULLをセットする
	*/
	void SetFogInfo(const FogInfo *fog_info){	fog_info_	= fog_info;	}
	const FogInfo* GetFogInfo()	const {			return fog_info_;		}
	float GetTime() { return time_; }
	void SetTime(float time) { time_ = time; }
	
	
protected:
	ml::matrix44	inverse_view_matrix_;
	ml::matrix44	view_matrix_;
	ml::matrix44	proj_matrix_;
	RendererInfo	renderer_info_;
	ml::vector3d	facing_position_;

	texture::RenderTarget	*depth_texture_;

	bool (*on_begin_draw_callback_)(DrawInfo &draw_info, OnDrawInfo &info, void* data);
	void *on_begin_draw_callback_data_;
	bool (*on_setrenderstate_callback_)(DrawInfo &draw_info, OnSetRenderStateInfo &info, void* data);
	void *on_setrenderstate_callback_data_;
	bool (*on_setshader_callback_)(DrawInfo &draw_info, ShaderInfo &info, void* data);
	void *on_setshader_callback_data_;
	bool (*on_draw_callback_)(DrawInfo &draw_info, VertexContext &ctx, void* data);
	void *on_draw_callback_data_;
	bool (*on_end_draw_callback_)(DrawInfo &draw_info, OnDrawInfo &info, void* data);
	void *on_end_draw_callback_data_;
	
	const FogInfo		*fog_info_;
	float time_;
	
#if defined(BM3_TARGET_IDE)
	ml::matrix44	ide_effect_matrix_;
	bool			is_selection_highlight_;
	bool			is_wireframe_draw_;
	bool			is_depth_less_;
#endif
};

BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_BM3_DRAWINFO_H
