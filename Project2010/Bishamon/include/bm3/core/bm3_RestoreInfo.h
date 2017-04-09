#ifndef BM3_SDK_INC_BM3_CORE_BM3_RESTOREINFO_H
#define BM3_SDK_INC_BM3_CORE_BM3_RESTOREINFO_H

#if defined(BM3_PLATFORM_win_opengl)
	#include "!opengl/bm3_RestoreInfoPlatformDependency.h"
#endif
#if defined(BM3_PLATFORM_win_directx9)
	#include "!directx9/bm3_RestoreInfoPlatformDependency.h"
#endif
#if defined(BM3_PLATFORM_win_directx10)
	#include "!directx10/bm3_RestoreInfoPlatformDependency.h"
#endif
#if defined(BM3_PLATFORM_playstation3)
	#include "bm3/core/!playstation3/bm3_RestoreInfoPlatformDependency.h"
#endif
#if defined(BM3_PLATFORM_xbox360)
	#include "!xbox360/bm3_RestoreInfoPlatformDependency.h"
#endif
#if defined(BM3_PLATFORM_psp)
	#include "!psp/bm3_RestoreInfoPlatformDependency.h"
#endif
#if defined(BM3_PLATFORM_gamebryodx9)
	#include "!gamebryodx9/bm3_RestoreInfoPlatformDependency.h"
#endif
#if defined(BM3_PLATFORM_opengles)
	#include "!opengles/bm3_RestoreInfoPlatformDependency.h"
#endif
#if defined(BM3_PLATFORM_nw4c)
	#include "!nw4c/bm3_RestoreInfoPlatformDependency.h"
#endif
#if defined(BM3_PLATFORM_psvita)
	#include "!psvita/bm3_RestoreInfoPlatformDependency.h"
#endif


namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

/// @brief RestoreInfoƒNƒ‰ƒX
class RestoreInfo : public internal::RestoreInfoPlatformDependency{
public:
	RendererInfo		&Renderer(){									return renderer_info_;			}
	void				SetRenderer(RendererInfo &r){					renderer_info_ = r;				}
private:
	RendererInfo	renderer_info_;
	
};

BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3


#endif	// #ifndef BM3_SDK_INC_BM3_CORE_BM3_RESTOREINFO_H
