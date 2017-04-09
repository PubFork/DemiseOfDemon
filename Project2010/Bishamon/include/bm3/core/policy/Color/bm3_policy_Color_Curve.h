#ifndef BM3_SDK_INC_BM3_CORE_COLOR_BM3_POLICY_COLOR_CURVE_H
#define BM3_SDK_INC_BM3_CORE_COLOR_BM3_POLICY_COLOR_CURVE_H

#include <ml/math/ml_random.h>
#include <ml/math/ml_arithmetic.h>

#include "../../bm3_Const.h"
#include "../../bm3_CoreType.h"
#include "../../bm3_UpdateInfo.h"
#include "../../bm3_CoreTypeSwitch.h"
#include "../../primitive/bm3_Primitive.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

class DrawInfo;

namespace policy{

namespace impl{

/// @brief Color_Curveクラス
class Color_Curve{

public:
	template<typename ResourceType>
	static BM3_FORCEINLINE void	Initialize(const ResourceType & /*res*/, ml::random &/*random*/, ColorType &value, const ColorType &/*initial*/, const ColorType &/*range*/, float blend_value){

//		if(res.Color_Basic_ColorType() == ColorTypeConst_Curve)	// 将来的にこのif文はなくなる
		{
			value.e_.a_ = blend_value;
		}
	}

	template<typename ResourceType, typename UpdateInfo>
	static BM3_FORCEINLINE void	Update(UpdateInfo &/*update_info*/, const ResourceType &/*res*/, ColorType &/*value*/){

		// 何もしない
		;
	}

	template<typename ResourceType>
	static BM3_FORCEINLINE void	Prepare(const ResourceType & /*res*/, ColorType &value, const ColorCurveType &curve, const ColorType &range, bool is_random_channel_sync, const float life_rate, const unsigned int random_seed, float blend_value){

//		if(res.Color_Basic_ColorType() == ColorTypeConst_Curve)	// 将来的にこのif文はなくなる
		{
			ColorType temp =
				is_random_channel_sync ?
					curve.ValueRandomChannelSync(	life_rate, random_seed, range) :
					curve.Value(					life_rate, random_seed, range);

			value.e_.r_ = ml::clamp(temp.e_.r_, 0.0f, 1.0f);
			value.e_.g_ = ml::clamp(temp.e_.g_, 0.0f, 1.0f);
			value.e_.b_ = ml::clamp(temp.e_.b_, 0.0f, 1.0f);
			value.e_.a_ = ml::clamp(blend_value, 0.0f, 1.0f);
		}
	}
};

}	// namespace impl

class Color_Curve{

public:
	template<typename ResourceType>
	void	BM3_FORCEINLINE Initialize(const ResourceType &res, ml::random &random, ColorType &value, const ColorType &initial, const ColorType &range){

		impl::Color_Curve::Initialize(res, random, value, initial, range);
	}

	template<typename ResourceType, typename UpdateInfo>
	void	BM3_FORCEINLINE Update(UpdateInfo &update_info, const ResourceType &res, ColorType &value){

		impl::Color_Curve::Update(update_info, res, value);
	}
};

}	// namespace policy
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_COLOR_BM3_POLICY_COLOR_CURVE_H
