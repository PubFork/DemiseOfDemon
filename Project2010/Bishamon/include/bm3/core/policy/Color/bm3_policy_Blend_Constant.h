#ifndef BM3_SDK_INC_BM3_CORE_BLEND_BM3_POLICY_COLOR_CONSTANT_H
#define BM3_SDK_INC_BM3_CORE_BLEND_BM3_POLICY_COLOR_CONSTANT_H

#include "../../bm3_Const.h"
#include "../../bm3_CoreType.h"
#include "../../bm3_CoreTypeSwitch.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

class DrawInfo;

namespace policy{

namespace impl{

/// @brief Blend_ConstantƒNƒ‰ƒX
class Blend_Constant{

public:
	template<typename ResourceType>
	static BM3_FORCEINLINE void	Initialize(const ResourceType &res, ml::random &random, float &value){

//		if(res.Blend_Basic_BlendAlphaType() == BlendAlphaTypeConst_Constant)
		{
			value = random.f32_range(res.Blend_Basic_Alpha(), res.Blend_Basic_Range());
		}
	}

	template<typename ResourceType, typename UpdateInfo>
	static BM3_FORCEINLINE void	Update(UpdateInfo &/*update_info*/, const ResourceType &/*res*/, float &/*value*/){

		// ‰½‚à‚µ‚È‚¢
		;
	}

	template<typename ResourceType>
	static BM3_FORCEINLINE void	Prepare(const ResourceType & /*res*/, float &/*value*/){
	}
};

}	// namespace impl

class Blend_Constant{

public:
	template<typename ResourceType>
	void	BM3_FORCEINLINE Initialize(const ResourceType &res, ml::random &random, float &value){

		impl::Blend_Constant::Initialize(res, random, value);
	}

	template<typename ResourceType, typename UpdateInfo>
	void	BM3_FORCEINLINE Update(UpdateInfo &update_info, const ResourceType &res, float &value){

		impl::Blend_Constant::Update(update_info, res, value);
	}
};

}	// namespace policy
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_BLEND_BM3_POLICY_COLOR_CONSTANT_H
