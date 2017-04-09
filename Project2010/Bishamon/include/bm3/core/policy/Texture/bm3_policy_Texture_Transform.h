#ifndef BM3_SDK_INC_BM3_CORE_TEXTURE_BM3_POLICY_TEXTURE_TRANSFORM_H
#define BM3_SDK_INC_BM3_CORE_TEXTURE_BM3_POLICY_TEXTURE_TRANSFORM_H

#include "../../bm3_Const.h"
#include "../../bm3_CoreType.h"
#include "../../bm3_CoreUtility.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

namespace policy{

namespace impl{

/// @brief Texture_Transformクラス
class Texture_Transform{

public:
	template<typename ResourceType>
	static BM3_FORCEINLINE void	Initialize(const ResourceType &/*res*/, ml::random &/*random*/, TextureCrop &/*value*/, TextureLayer::Type /*lt*/){

//		if(type == TextureAnimationTypeConst_Transform)	// 将来的にこのif文はなくなる
//		{
//		}
	}

	template<typename ResourceType, typename UpdateInfo>
	static BM3_FORCEINLINE void	Update(UpdateInfo &/*update_info*/, const ResourceType &/*res*/, TextureCrop &/*value*/, TextureAnimationTypeConst /*type*/, TextureLayer::Type /*lt*/){

//		if(type == TextureAnimationTypeConst_Transform)	// 将来的にこのif文はなくなる
//		{
//		}
	}
};

}	// namespace impl

class Texture_Transform{

public:
	template<typename ResourceType>
	BM3_FORCEINLINE void	Initialize(const ResourceType &res, ml::random &random, TextureCrop &value, TextureAnimationTypeConst type, TextureLayer::Type lt){

		impl::Texture_Transform::Initialize(res, random, value, type, lt);
	}

	template<typename ResourceType, typename UpdateInfo>
	BM3_FORCEINLINE void	Update(UpdateInfo &update_info, const ResourceType &res, TextureCrop &value, TextureAnimationTypeConst type, TextureLayer::Type lt){

		impl::Texture_Transform::Update(update_info, res, value, type, lt);
	}
};

}	// namespace policy
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_TEXTURE_BM3_POLICY_TEXTURE_TRANSFORM_H
