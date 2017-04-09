#ifndef BM3_SDK_INC_BM3_CORE_RESOURCE_BM3_RES_ROOT_H
#define BM3_SDK_INC_BM3_CORE_RESOURCE_BM3_RES_ROOT_H

#include "../../system/bm3_Config.h"
#include "../type/bm3_Curve.h"
#include "../type/bm3_Texture.h"
#include "bm3/core/resource/policy/bm3_res_policy_StaticChildArray.h"
#include "../bm3_Const.h"
#include <ml/type/ml_basic.h>
#include <ml/type/ml_vector3d.h>
#include <ml/type/ml_color.h>

#include "../fileformat/bm3_bmb.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE
namespace res{

/// @brief RootƒNƒ‰ƒX
class Root  : public fileformat::bmb::EffectProject{
};

}	// namespace res
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_RESOURCE_BM3_RES_ROOT_H
