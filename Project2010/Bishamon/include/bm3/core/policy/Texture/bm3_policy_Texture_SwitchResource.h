#ifndef BM3_SDK_INC_BM3_CORE_TEXTURE_BM3_POLICY_TEXTURE_SWITCHRESOURCE_H
#define BM3_SDK_INC_BM3_CORE_TEXTURE_BM3_POLICY_TEXTURE_SWITCHRESOURCE_H

#include "../../bm3_Const.h"
#include "../../bm3_CoreType.h"
#include "../../bm3_CoreUtility.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

namespace policy{

namespace impl{

/// @brief Texture_SwitchResourceƒNƒ‰ƒX
class Texture_SwitchResource{

public:
	template<typename ResourceType>
	static BM3_FORCEINLINE void	Switch(const ResourceType &res, const ResourceType & /*old_res*/, ml::random &random, TextureCrop &value, int &frame, float &step, float h_scroll, float v_scroll, TextureAnimationTypeConst type, TextureAnimationTypeConst /*old_type*/, TextureLayer::Type lt){
//		switch(old_type){
//			case TextureAnimationTypeConst_Nothing:		
				switch(type){
				case TextureAnimationTypeConst_Nothing:		Texture_Nothing::Initialize(	res, random, value,							lt);	break;
				case TextureAnimationTypeConst_Crop:		Texture_Crop::Initialize(		res, random, value,	frame,		step, 		lt);	break;
				case TextureAnimationTypeConst_Scroll:		Texture_Scroll::Initialize(		res, random, value,	h_scroll,	v_scroll,	lt);	break;
				case TextureAnimationTypeConst_Pattern:		Texture_Pattern::Initialize(	res, random, value,	frame,		step, 		lt);	break;
				case TextureAnimationTypeConst_Transform:	Texture_Transform::Initialize(	res, random, value,							lt);	break;
				case TextureAnimationTypeConst_MAX:
				case TextureAnimationTypeConst_FORCE32:
					break;
				}
/*				break;
			case TextureAnimationTypeConst_Crop:		
				switch(type){
				case TextureAnimationTypeConst_Nothing:		Texture_Nothing::Initialize(	res, random, value,							type, lt);	break;
				case TextureAnimationTypeConst_Crop:		Texture_Crop::Initialize(		res, random, value,	frame,		step, 		type, lt);	break;
				case TextureAnimationTypeConst_Scroll:		Texture_Scroll::Initialize(		res, random, value,	h_scroll,	v_scroll,	type, lt);	break;
				case TextureAnimationTypeConst_Pattern:		Texture_Pattern::Initialize(	res, random, value,	frame,		step, 		type, lt);	break;
				case TextureAnimationTypeConst_Transform:	Texture_Transform::Initialize(	res, random, value,							type, lt);	break;
				}
				break;
			case TextureAnimationTypeConst_Scroll:		
				switch(type){
				case TextureAnimationTypeConst_Nothing:		Texture_Nothing::Initialize(	res, random, value,							type, lt);	break;
				case TextureAnimationTypeConst_Crop:		Texture_Crop::Initialize(		res, random, value,	frame,		step, 		type, lt);	break;
				case TextureAnimationTypeConst_Scroll:		Texture_Scroll::Initialize(		res, random, value,	h_scroll,	v_scroll,	type, lt);	break;
				case TextureAnimationTypeConst_Pattern:		Texture_Pattern::Initialize(	res, random, value,	frame,		step, 		type, lt);	break;
				case TextureAnimationTypeConst_Transform:	Texture_Transform::Initialize(	res, random, value,							type, lt);	break;
				}
				break;
			case TextureAnimationTypeConst_Pattern:		
				switch(type){
				case TextureAnimationTypeConst_Nothing:		Texture_Nothing::Initialize(	res, random, value,							type, lt);	break;
				case TextureAnimationTypeConst_Crop:		Texture_Crop::Initialize(		res, random, value,	frame,		step, 		type, lt);	break;
				case TextureAnimationTypeConst_Scroll:		Texture_Scroll::Initialize(		res, random, value,	h_scroll,	v_scroll,	type, lt);	break;
				case TextureAnimationTypeConst_Pattern:		Texture_Pattern::Initialize(	res, random, value,	frame,		step, 		type, lt);	break;
				case TextureAnimationTypeConst_Transform:	Texture_Transform::Initialize(	res, random, value,							type, lt);	break;
				}
				break;
			case TextureAnimationTypeConst_Transform:	
				switch(type){
				case TextureAnimationTypeConst_Nothing:		Texture_Nothing::Initialize(	res, random, value,							type, lt);	break;
				case TextureAnimationTypeConst_Crop:		Texture_Crop::Initialize(		res, random, value,	frame,		step, 		type, lt);	break;
				case TextureAnimationTypeConst_Scroll:		Texture_Scroll::Initialize(		res, random, value,	h_scroll,	v_scroll,	type, lt);	break;
				case TextureAnimationTypeConst_Pattern:		Texture_Pattern::Initialize(	res, random, value,	frame,		step, 		type, lt);	break;
				case TextureAnimationTypeConst_Transform:	Texture_Transform::Initialize(	res, random, value,							type, lt);	break;
				}
				break;
*/
//		}
	}
	
};

}	// namespace impl


}	// namespace policy
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_TEXTURE_BM3_POLICY_TEXTURE_SWITCHRESOURCE_H
