#ifndef BM3_SDK_INC_BM3_CORE_DIRECTX9_bm3_RENDERERINFOPLATFORMDEPENDENCY_H
#define BM3_SDK_INC_BM3_CORE_DIRECTX9_bm3_RENDERERINFOPLATFORMDEPENDENCY_H

#include <windows.h>
#include <d3d9.h>
#if defined(BM3_TARGET_IDE)
#include <string>
#endif

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE
namespace internal{

struct RendererInfoPlatformDependency{

	bool operator==(const RendererInfoPlatformDependency &s){

		return d3d_device_ == s.d3d_device_;
	}

#if defined(BM3_TARGET_IDE)
	std::wstring ToString() const{

		wchar_t	buf[32];
		wsprintf(buf, L"%08X", d3d_device_);

		return buf;
	};
#endif

	LPDIRECT3DDEVICE9	d3d_device_;				// D3Dデバイス
};

}	// namespace internal
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_DIRECTX9_bm3_RENDERERINFOPLATFORMDEPENDENCY_H
