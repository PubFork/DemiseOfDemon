#ifndef BM3_SDK_INC_BM3_CORE_DRAW_BM3_BMSLN_HANDLEBASE_H
#define BM3_SDK_INC_BM3_CORE_DRAW_BM3_BMSLN_HANDLEBASE_H

#include "../../system/bm3_Config.h"
#include <ml/type/ml_basic.h>
#include <ml/type/ml_matrix44.h>

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE


namespace bmsln{

/// @brief HandleBaseƒNƒ‰ƒX
class HandleBase{

public:
	HandleBase();
	virtual ~HandleBase();

	virtual const void* Image() = 0;

protected:
};

}	// namespace bmsln
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_DRAW_BM3_BMSLN_HANDLEBASE_H
