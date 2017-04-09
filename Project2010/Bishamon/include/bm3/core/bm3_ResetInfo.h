#ifndef BM3_SDK_INC_BM3_CORE_BM3_RESETINFO_H
#define BM3_SDK_INC_BM3_CORE_BM3_RESETINFO_H

#include "bm3_RendererInfo.h"


namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

/// @brief ResetInfoƒNƒ‰ƒX
class ResetInfo{

public:
	ResetInfo(){}

	RendererInfo		&Renderer(){									return renderer_info_;			}
	void				SetRenderer(RendererInfo &r){					renderer_info_ = r;				}

private:
	RendererInfo	renderer_info_;
};

BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_BM3_RESETINFO_H
