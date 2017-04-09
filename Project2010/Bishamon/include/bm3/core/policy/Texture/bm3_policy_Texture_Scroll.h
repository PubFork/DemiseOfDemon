#ifndef BM3_SDK_INC_BM3_CORE_TEXTURE_BM3_POLICY_TEXTURE_SCROLL_H
#define BM3_SDK_INC_BM3_CORE_TEXTURE_BM3_POLICY_TEXTURE_SCROLL_H

#include "../../bm3_Const.h"
#include "../../bm3_CoreType.h"
#include "../../bm3_CoreUtility.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

namespace policy{

namespace impl{

/// @brief Texture_Scrollクラス
class Texture_Scroll{

public:
	template<typename ResourceType>
	static BM3_FORCEINLINE void	Initialize(const ResourceType &/*res*/, ml::random &/*random*/, TextureCrop &/*value*/, float &h_scroll, float &v_scroll, TextureLayer::Type /*lt*/){

//		if(type == TextureAnimationTypeConst_Scroll)	// 将来的にこのif文はなくなる
		{
			h_scroll = 0.0f;
			v_scroll = 0.0f;
		}

#if defined(BM3_TARGET_IDE)
		// IDEの時、途中でタイプが変わったときに参照されてしまう。
		h_scroll = 0.0f;
		v_scroll = 0.0f;
#endif
	}

	template<typename ResourceType, typename UpdateInfo>
	static BM3_FORCEINLINE void	Update(UpdateInfo &update_info, const ResourceType &res, TextureCrop &/*value*/, float &h_scroll, float &v_scroll, TextureAnimationTypeConst /*type*/, TextureLayer::Type lt){

//		if(type == TextureAnimationTypeConst_Scroll)	// 将来的にこのif文はなくなる
		{
			h_scroll += Texture_Layer_HScrollSpeed(res, lt)*update_info.GetUpdateFrame();
			v_scroll += Texture_Layer_VScrollSpeed(res, lt)*update_info.GetUpdateFrame();
		}
	}
};

}	// namespace impl

class Texture_Scroll{

public:
	template<typename ResourceType>
	BM3_FORCEINLINE void	Initialize(const ResourceType &res, ml::random &random, TextureCrop &value, TextureAnimationTypeConst type, TextureLayer::Type lt){

		impl::Texture_Scroll::Initialize(res, random, value, h_scroll_, v_scroll_, type, lt);
	}

	template<typename ResourceType, typename UpdateInfo>
	BM3_FORCEINLINE void	Update(UpdateInfo &update_info, const ResourceType &res, TextureCrop &value, TextureAnimationTypeConst type, TextureLayer::Type lt){

		impl::Texture_Scroll::Update(update_info, res, value, h_scroll_, v_scroll_, type, lt);
	}

private:
	float	h_scroll_;
	float	v_scroll_;
};

}	// namespace policy
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_TEXTURE_BM3_POLICY_TEXTURE_SCROLL_H
