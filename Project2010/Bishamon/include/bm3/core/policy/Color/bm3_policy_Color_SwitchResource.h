#ifndef BM3_SDK_INC_BM3_CORE_COLOR_BM3_POLICY_COLOR_SWITCHRESOURCE_H
#define BM3_SDK_INC_BM3_CORE_COLOR_BM3_POLICY_COLOR_SWITCHRESOURCE_H

#include "../../bm3_Const.h"
#include "../../bm3_CoreType.h"
#include "../../bm3_CoreTypeSwitch.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

class DrawInfo;

namespace policy{

namespace impl{

/// @brief Color_SwitchResourceƒNƒ‰ƒX
class Color_SwitchResource{

public:
	template<typename ResourceType>
	static BM3_FORCEINLINE void Switch(const ResourceType &res, const ResourceType &old_res, ml::random &random, ColorType &value, const ColorType &initial, const ColorType &range, bool is_random_channel_sync, float blend_value, const PrimitiveInitializeInfo & /*pii*/){
		switch(old_res.Color_Basic_ColorType()){
			case ColorTypeConst_Constant:
			case ColorTypeConst_Curve:
				switch(res.Color_Basic_ColorType()){
				case ColorTypeConst_Constant:	Color_Constant::Initialize(		res, random, value, initial, range, is_random_channel_sync,	blend_value);	break;
				case ColorTypeConst_Curve:		Color_Curve::Initialize(		res, random, value, initial, range,							blend_value);	break;
				case ColorTypeConst_MAX:
				case ColorTypeConst_FORCE32:
					break;
				}
				break;
			case ColorTypeConst_MAX:
			case ColorTypeConst_FORCE32:
				break;
		}
	}	
};

}	// namespace impl

	
}	// namespace policy
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_COLOR_BM3_POLICY_COLOR_SWITCHRESOURCE_H
