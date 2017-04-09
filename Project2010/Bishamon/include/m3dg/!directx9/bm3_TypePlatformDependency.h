#ifndef M3DG_M3DG_DIRECTX9_BM3_TYPEPLATFORMDEPENDENCY_H_H
#define M3DG_M3DG_DIRECTX9_BM3_TYPEPLATFORMDEPENDENCY_H_H

#include <windows.h>
#include <d3d9.h>
#include <bm3/system/bm3_PlatformNamespace.h>
#include <bm3/core/bm3_RenderStateManager.h>
#include <bm3/core/bm3_ProfileManager.h>

namespace m3dg{
namespace internal{

struct ModelInitializeInfoPlatformDependency{
	LPDIRECT3DDEVICE9					d3d_device_;
	bm3::RenderStateManagerImpl			*render_state_manager_;
};

struct ModelDrawInfoPlatformDependency{
	LPDIRECT3DDEVICE9					d3d_device_;
	bm3::RenderStateManagerImpl			*render_state_manager_;
	bm3::internal::ProfileManagerImpl	*profile_manager_;
};

}	// namespace internal
}	// namespace m3dg

#endif	// #ifndef M3DG_M3DG_DIRECTX9_BM3_TYPEPLATFORMDEPENDENCY_H_H
