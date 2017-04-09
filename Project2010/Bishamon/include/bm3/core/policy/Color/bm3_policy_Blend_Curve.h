#ifndef BM3_SDK_INC_BM3_CORE_BLEND_BM3_POLICY_COLOR_CURVE_H
#define BM3_SDK_INC_BM3_CORE_BLEND_BM3_POLICY_COLOR_CURVE_H

#include <ml/math/ml_random.h>
#include <ml/math/ml_arithmetic.h>

#include "../../bm3_Const.h"
#include "../../bm3_CoreType.h"
#include "../../bm3_CoreTypeSwitch.h"
#include "../../primitive/bm3_Primitive.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

class UpdateInfo;
class DrawInfo;

namespace policy{

namespace impl{

/// @brief Blend_CurveƒNƒ‰ƒX
class Blend_Curve{

public:
	template<typename ResourceType>
	static BM3_FORCEINLINE void	Initialize(const ResourceType &res, ml::random &random, float &value){

//		if(res.Blend_Basic_BlendAlphaType() == BlendAlphaTypeConst_Curve)
		{
			value = ml::clamp(random.f32_range(res.Blend_Basic_Curve().Value(0.0f), res.Blend_Basic_Range()), 0.0f, 1.0f);
		}
	}

	template<typename ResourceType, typename UpdateInfo>
	static BM3_FORCEINLINE void	Update(UpdateInfo &/*update_info*/, const ResourceType &/*res*/, float &/*value*/){

		// ‰½‚à‚µ‚È‚¢
		;
	}

	template<typename ResourceType>
	static BM3_FORCEINLINE void	Prepare(const ResourceType &res, float &value, const float life_rate, const unsigned int random_seed){

//		if(res.Blend_Basic_BlendAlphaType() == BlendAlphaTypeConst_Curve)
		{
			value = ml::clamp(res.Blend_Basic_Curve().Value(life_rate, random_seed, res.Blend_Basic_Range()), 0.0f, 1.0f);
		}
	}
};

}	// namespace impl

class Blend_Curve{

public:
	template<typename ResourceType>
	void	BM3_FORCEINLINE Initialize(const ResourceType &res, ml::random &random, float &value){

		impl::Blend_Curve::Initialize(res, random, value);
	}

	template<typename ResourceType, typename UpdateInfo>
	void	BM3_FORCEINLINE Update(UpdateInfo &update_info, const ResourceType &res, float &value){

		impl::Blend_Curve::Update(update_info, res, value);
	}
};

}	// namespace policy
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_BLEND_BM3_POLICY_COLOR_CURVE_H
