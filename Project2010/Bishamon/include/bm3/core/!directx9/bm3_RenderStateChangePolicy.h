#ifndef BM3_SDK_INC_BM3_CORE_BM3_DIRECTX9_BM3_RENDERSTATECHANGEPOLICY_H
#define BM3_SDK_INC_BM3_CORE_BM3_DIRECTX9_BM3_RENDERSTATECHANGEPOLICY_H

#include "../../system/bm3_Config.h"
#include <ml/utility/ml_singleton.h>
#include <ml/type/ml_type2type.h>
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "../bm3_DrawInfo.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

namespace internal{

#pragma warning(push)
#pragma warning(disable: 4189)

class RenderStateChangePolicy{
public:
	RenderStateChangePolicy();
	~RenderStateChangePolicy();

	void	PreBeginRender(DrawInfo &draw_info);
	void	PostBeginRender(DrawInfo &/*draw_info*/){;}
	void	PreEndRender(DrawInfo &/*draw_info*/){;}
	void	PostEndRender(DrawInfo &draw_info);

	#define	BM3_RenderStateChangePolicy_DirectSetEnable(TYPE, DX_TYPE, ON_TRUE, ON_FALSE)													\
		void SP_DirectSetEnable(DrawInfo &draw_info, bool i, ml::enum2type<RenderState::Type, TYPE>){										\
			HRESULT result = draw_info.Renderer().d3d_device_->SetRenderState(DX_TYPE, i ? (ON_TRUE) : (ON_FALSE));							\
			ML_ASSERT(result == D3D_OK);																									\
		}
	#define	BM3_RenderStateChangePolicy_DirectSetDepthTestType(TYPE, DX_TYPE)																\
		void SP_DirectSetDepthTestType(DrawInfo &draw_info, ml::enum2type<DepthTestTypeConst, TYPE>){										\
			HRESULT result = draw_info.Renderer().d3d_device_->SetRenderState(D3DRS_ZFUNC, DX_TYPE);										\
			ML_ASSERT(result == D3D_OK);																									\
		}
	#define	BM3_RenderStateChangePolicy_DirectSetBlendType(TYPE, DX_SRC, DX_DST)															\
		void SP_DirectSetBlendType(DrawInfo &draw_info, ml::enum2type<BlendTypeConst, TYPE>){												\
			HRESULT result_a = draw_info.Renderer().d3d_device_->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);								\
			ML_ASSERT(result_a == D3D_OK);																									\
			HRESULT result_s = draw_info.Renderer().d3d_device_->SetRenderState(D3DRS_SRCBLEND, DX_SRC);									\
			ML_ASSERT(result_s == D3D_OK);																									\
			HRESULT result_d = draw_info.Renderer().d3d_device_->SetRenderState(D3DRS_DESTBLEND, DX_DST);									\
			ML_ASSERT(result_d == D3D_OK);																									\
		}
	#define	BM3_RenderStateChangePolicy_DirectSetAlphaTestType(TYPE, DX_TYPE)																\
		void SP_DirectSetAlphaTestType(DrawInfo &draw_info, ml::enum2type<AlphaTestTypeConst, TYPE>, float ref){							\
			HRESULT result_c = draw_info.Renderer().d3d_device_->SetRenderState(D3DRS_ALPHAFUNC, DX_TYPE);									\
			ML_ASSERT(result_c == D3D_OK);																									\
			HRESULT result_r = draw_info.Renderer().d3d_device_->SetRenderState(D3DRS_ALPHAREF, (DWORD)(ref * 255.0f));						\
			ML_ASSERT(result_r == D3D_OK);																									\
		}
	#define	BM3_RenderStateChangePolicy_DirectSetTextureAddressType(TYPE, DX_TYPE)															\
		void SP_DirectSetTextureAddressType(DrawInfo &draw_info, TextureLayer::Type layer, ml::enum2type<TextureAddressTypeConst, TYPE>){	\
			HRESULT result_u = draw_info.Renderer().d3d_device_->SetSamplerState(layer, D3DSAMP_ADDRESSU, DX_TYPE);							\
			ML_ASSERT(result_u == D3D_OK);																									\
			HRESULT result_v = draw_info.Renderer().d3d_device_->SetSamplerState(layer, D3DSAMP_ADDRESSV, DX_TYPE);							\
			ML_ASSERT(result_v == D3D_OK);																									\
		}
	#define	BM3_RenderStateChangePolicy_DirectSetTextureFilterType(TYPE, DX_TYPE)															\
		void SP_DirectSetTextureFilterType(DrawInfo &draw_info, TextureLayer::Type layer, ml::enum2type<TextureFilterTypeConst, TYPE>){		\
			HRESULT result_u = draw_info.Renderer().d3d_device_->SetSamplerState(layer, D3DSAMP_MAGFILTER, DX_TYPE);						\
			ML_ASSERT(result_u == D3D_OK);																									\
			HRESULT result_v = draw_info.Renderer().d3d_device_->SetSamplerState(layer, D3DSAMP_MINFILTER, DX_TYPE);						\
			ML_ASSERT(result_v == D3D_OK);																									\
		}

	BM3_RenderStateChangePolicy_DirectSetEnable(RenderState::Culling,		D3DRS_CULLMODE,			D3DCULL_CW,	D3DCULL_NONE)
	BM3_RenderStateChangePolicy_DirectSetEnable(RenderState::DepthTest,		D3DRS_ZENABLE,			D3DZB_TRUE,	D3DZB_FALSE)
	BM3_RenderStateChangePolicy_DirectSetEnable(RenderState::DepthWrite,	D3DRS_ZWRITEENABLE,		TRUE,		FALSE)
	BM3_RenderStateChangePolicy_DirectSetEnable(RenderState::AlphaTest,		D3DRS_ALPHATESTENABLE,	TRUE,		FALSE)

	BM3_RenderStateChangePolicy_DirectSetDepthTestType(DepthTestTypeConst_Less,			D3DCMP_LESS)
	BM3_RenderStateChangePolicy_DirectSetDepthTestType(DepthTestTypeConst_LessEqual,	D3DCMP_LESSEQUAL)
	BM3_RenderStateChangePolicy_DirectSetDepthTestType(DepthTestTypeConst_Greater,		D3DCMP_GREATER)
	BM3_RenderStateChangePolicy_DirectSetDepthTestType(DepthTestTypeConst_GreaterEqual,	D3DCMP_GREATEREQUAL)
	BM3_RenderStateChangePolicy_DirectSetDepthTestType(DepthTestTypeConst_Equal,		D3DCMP_EQUAL)
	BM3_RenderStateChangePolicy_DirectSetDepthTestType(DepthTestTypeConst_NotEqual,		D3DCMP_NOTEQUAL)
	BM3_RenderStateChangePolicy_DirectSetDepthTestType(DepthTestTypeConst_Never,		D3DCMP_NEVER)
	BM3_RenderStateChangePolicy_DirectSetDepthTestType(DepthTestTypeConst_Always,		D3DCMP_ALWAYS)

	BM3_RenderStateChangePolicy_DirectSetAlphaTestType(AlphaTestTypeConst_Less,			D3DCMP_LESS)
	BM3_RenderStateChangePolicy_DirectSetAlphaTestType(AlphaTestTypeConst_LessEqual,	D3DCMP_LESSEQUAL)
	BM3_RenderStateChangePolicy_DirectSetAlphaTestType(AlphaTestTypeConst_Greater,		D3DCMP_GREATER)
	BM3_RenderStateChangePolicy_DirectSetAlphaTestType(AlphaTestTypeConst_GreaterEqual,	D3DCMP_GREATEREQUAL)
	BM3_RenderStateChangePolicy_DirectSetAlphaTestType(AlphaTestTypeConst_Equal,		D3DCMP_EQUAL)
	BM3_RenderStateChangePolicy_DirectSetAlphaTestType(AlphaTestTypeConst_NotEqual,		D3DCMP_NOTEQUAL)
	BM3_RenderStateChangePolicy_DirectSetAlphaTestType(AlphaTestTypeConst_Never,		D3DCMP_NEVER)
	BM3_RenderStateChangePolicy_DirectSetAlphaTestType(AlphaTestTypeConst_Always,		D3DCMP_ALWAYS)
	
	BM3_RenderStateChangePolicy_DirectSetBlendType(BlendTypeConst_Non,			D3DBLEND_ONE,			D3DBLEND_ZERO			)
	BM3_RenderStateChangePolicy_DirectSetBlendType(BlendTypeConst_Semitrans,	D3DBLEND_SRCALPHA,		D3DBLEND_INVSRCALPHA	)
	BM3_RenderStateChangePolicy_DirectSetBlendType(BlendTypeConst_SemiAdd,		D3DBLEND_SRCALPHA,		D3DBLEND_ONE			)
	BM3_RenderStateChangePolicy_DirectSetBlendType(BlendTypeConst_SemiSub,		D3DBLEND_ZERO,			D3DBLEND_INVSRCALPHA	)
	BM3_RenderStateChangePolicy_DirectSetBlendType(BlendTypeConst_Add,			D3DBLEND_ONE,			D3DBLEND_ONE			)
	BM3_RenderStateChangePolicy_DirectSetBlendType(BlendTypeConst_Sub,			D3DBLEND_ZERO,			D3DBLEND_INVSRCCOLOR	)
	BM3_RenderStateChangePolicy_DirectSetBlendType(BlendTypeConst_Reverse,		D3DBLEND_INVDESTCOLOR,	D3DBLEND_INVSRCCOLOR	)

	BM3_RenderStateChangePolicy_DirectSetTextureAddressType(TextureAddressTypeConst_Clamp,	D3DTADDRESS_CLAMP)
	BM3_RenderStateChangePolicy_DirectSetTextureAddressType(TextureAddressTypeConst_Repeat,	D3DTADDRESS_WRAP)
	BM3_RenderStateChangePolicy_DirectSetTextureAddressType(TextureAddressTypeConst_Mirror,	D3DTADDRESS_MIRROR)

	BM3_RenderStateChangePolicy_DirectSetTextureFilterType(TextureFilterTypeConst_Nearest,	D3DTEXF_POINT)
	BM3_RenderStateChangePolicy_DirectSetTextureFilterType(TextureFilterTypeConst_Linear,	D3DTEXF_LINEAR)

	#undef	BM3_RenderStateChangePolicy_DirectSetEnable
	#undef	BM3_RenderStateChangePolicy_DirectSetDepthTestType
	#undef	BM3_RenderStateChangePolicy_DirectSetBlendType
	#undef	BM3_RenderStateChangePolicy_DirectSetAlphaTestType
	#undef	BM3_RenderStateChangePolicy_DirectSetTextureAddressType
	#undef	BM3_RenderStateChangePolicy_DirectSetTextureFilterType

protected:
	void	InitializeChangePolicy(RenderStateInitializeInfo &info);
	void	FinalizeChangePolicy();

private:
	DWORD value_D3DRS_ZFUNC;
	DWORD value_D3DRS_ALPHABLENDENABLE;
	DWORD value_D3DRS_SRCBLEND;
	DWORD value_D3DRS_DESTBLEND;
	DWORD value_D3DRS_CULLMODE;
	DWORD value_D3DRS_ZENABLE;
	DWORD value_D3DRS_ZWRITEENABLE;
	DWORD value_D3DRS_ALPHATESTENABLE;
	DWORD value_D3DRS_ALPHAFUNC;
	DWORD value_D3DRS_ALPHAREF;
};

#pragma warning(pop)

}	// namespace internal
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_BM3_DIRECTX9_BM3_RENDERSTATECHANGEPOLICY_H
