#ifndef BM3_SDK_INC_BM3_CORE_COLOR_BM3_POLICY_BLEND_RESOURCESWITCH_H
#define BM3_SDK_INC_BM3_CORE_COLOR_BM3_POLICY_BLEND_RESOURCESWITCH_H

#include "../../bm3_Const.h"
#include "../../bm3_CoreType.h"
#include "../../bm3_CoreTypeSwitch.h"
#include "bm3_policy_Blend_Constant.h"
#include "bm3_policy_Blend_Curve.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

namespace policy{

namespace impl{

/// @brief Blend_SwitchResourceƒNƒ‰ƒX
class Blend_SwitchResource{

public:
	template<typename ResourceType>
	static BM3_FORCEINLINE void Switch(const ResourceType &res, const ResourceType &old_res, ml::random &random, float &value, const PrimitiveInitializeInfo & /*pii*/){
		switch(old_res.Blend_Basic_BlendAlphaType()){
			case BlendAlphaTypeConst_Constant:
			case BlendAlphaTypeConst_Curve:
				switch(res.Blend_Basic_BlendAlphaType()){
				case BlendAlphaTypeConst_Constant:	Blend_Constant::Initialize(		res, random, value						);	break;
				case BlendAlphaTypeConst_Curve:		Blend_Curve::Initialize(		res, random, value						);	break;
				case BlendAlphaTypeConst_MAX:
				case BlendAlphaTypeConst_FORCE32:
					break;
				}
				break;
			case BlendAlphaTypeConst_MAX:
			case BlendAlphaTypeConst_FORCE32:
				break;
		}
	}
};

}	// namespace impl


}	// namespace policy
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_COLOR_BM3_POLICY_BLEND_RESOURCESWITCH_H
