#ifndef BM3_SDK_INC_BM3_CORE_DIRECTX9_BM3_RENDERSTATEINITIALIZEINFOPLATFORMDEPENDENCY_H
#define BM3_SDK_INC_BM3_CORE_DIRECTX9_BM3_RENDERSTATEINITIALIZEINFOPLATFORMDEPENDENCY_H

#include "../../system/bm3_Config.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

namespace internal{

struct RenderStateInitializeInfoPlatformDependency{

	RenderStateInitializeInfoPlatformDependency(LPDIRECT3DDEVICE9 d3d_device) :
		d3d_device_(d3d_device){;}

	LPDIRECT3DDEVICE9	d3d_device_;				// D3Dデバイス
};

}	// namespace internal
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_DIRECTX9_BM3_RENDERSTATEINITIALIZEINFOPLATFORMDEPENDENCY_H

