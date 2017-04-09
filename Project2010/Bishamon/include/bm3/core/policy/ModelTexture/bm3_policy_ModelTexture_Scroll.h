#ifndef BM3_SDK_INC_BM3_CORE_TEXTURE_BM3_POLICY_ModelTexture_Scroll_H
#define BM3_SDK_INC_BM3_CORE_TEXTURE_BM3_POLICY_ModelTexture_Scroll_H

#include "../../bm3_Const.h"
#include "../../bm3_CoreType.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

namespace policy{

namespace impl{

/// @brief ModelTexture_Scrollクラス
class ModelTexture_Scroll{

public:
	template<typename ResourceType>
	static BM3_FORCEINLINE void	Initialize(const ResourceType &/*res*/, ml::random &/*random*/, float &h_scroll, float &v_scroll){

//		if(type == TextureAnimationTypeConst_Scroll)	// 将来的にこのif文はなくなる
		{
			h_scroll = 0.0f;
			v_scroll = 0.0f;
		}
	}

	template<typename ResourceType, typename UpdateInfo>
	static BM3_FORCEINLINE void	Update(UpdateInfo &/*update_info*/, const ResourceType &res, float &h_scroll, float &v_scroll){

//		if(type == TextureAnimationTypeConst_Scroll)	// 将来的にこのif文はなくなる
		{
			h_scroll += res.Model_TextureLayer1_HScrollSpeed();
			v_scroll += res.Model_TextureLayer1_VScrollSpeed();
		}
	}
};

}	// namespace impl

class ModelTexture_Scroll{

public:
	template<typename ResourceType>
	BM3_FORCEINLINE void	Initialize(const ResourceType &res, ml::random &random){

		impl::ModelTexture_Scroll::Initialize(res, random, h_scroll_, v_scroll_);
	}

	template<typename ResourceType, typename UpdateInfo>
	BM3_FORCEINLINE void	Update(UpdateInfo &update_info, const ResourceType &res){

		impl::ModelTexture_Scroll::Update(update_info, res, h_scroll_, v_scroll_);
	}

private:
	float	h_scroll_;
	float	v_scroll_;
};

}	// namespace policy
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_TEXTURE_BM3_POLICY_ModelTexture_Scroll_H
