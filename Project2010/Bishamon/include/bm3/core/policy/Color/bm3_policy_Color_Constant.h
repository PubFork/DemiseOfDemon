#ifndef BM3_SDK_INC_BM3_CORE_COLOR_BM3_POLICY_COLOR_CONSTANT_H
#define BM3_SDK_INC_BM3_CORE_COLOR_BM3_POLICY_COLOR_CONSTANT_H

#include "../../bm3_Const.h"
#include "../../bm3_CoreType.h"
#include "../../bm3_CoreTypeSwitch.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

class DrawInfo;

namespace policy{

namespace impl{

/// @brief Color_Constantクラス
class Color_Constant{

public:
	template<typename ResourceType>
	static BM3_FORCEINLINE void	Initialize(const ResourceType & /*res*/, ml::random &random, ColorType &value, const ColorType &initial, const ColorType &range, bool is_random_channel_sync, float blend_value){

//		if(res.Color_Basic_ColorType() == ColorTypeConst_Constant)	// 将来的にこのif文はなくなる
		{
			if(is_random_channel_sync){
				float r = random.f32_range(0.0f, 1.0f);

				value.e_.r_ = initial.e_.r_ + range.e_.r_ * r;
				value.e_.g_ = initial.e_.g_ + range.e_.g_ * r;
				value.e_.b_ = initial.e_.b_ + range.e_.b_ * r;
			}else{
				value.e_.r_ = random.f32_range(initial.e_.r_, range.e_.r_);
				value.e_.g_ = random.f32_range(initial.e_.g_, range.e_.g_);
				value.e_.b_ = random.f32_range(initial.e_.b_, range.e_.b_);
			}
			value.e_.a_ = blend_value;
			value.clamp(0.0f, 1.0f);
		}
	}

	template<typename ResourceType, typename UpdateInfo>
	static BM3_FORCEINLINE void	Update(UpdateInfo &/*update_info*/, const ResourceType &/*res*/, ColorType &/*value*/){

		// 何もしない
		;
	}

	template<typename ResourceType>
	static BM3_FORCEINLINE void	Prepare(const ResourceType & /*res*/, ColorType &value, float blend_value){

//		if(res.Color_Basic_ColorType() == ColorTypeConst_Constant)	// 将来的にこのif文はなくなる
		{
			value.e_.a_ = ml::clamp(blend_value, 0.0f, 1.0f);
		}
	}
};

}	// namespace impl

class Color_Constant{

public:
	template<typename ResourceType>
	void	BM3_FORCEINLINE Initialize(const ResourceType &res, ml::random &random, ColorType &value, const ColorType &initial, const ColorType &range){

		impl::Color_Constant::Initialize(res, random, value, initial, range);
	}

	template<typename ResourceType, typename UpdateInfo>
	void	BM3_FORCEINLINE Update(UpdateInfo &update_info, const ResourceType &res, ColorType &value){

		impl::Color_Constant::Update(update_info, res, value);
	}
};

}	// namespace policy
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_COLOR_CONSTANT_H
