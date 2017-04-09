#ifndef BM3_SDK_INC_BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_MODELTEXTURE1_H
#define BM3_SDK_INC_BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_MODELTEXTURE1_H

#include "bm3/system/bm3_Config.h"
#include "bm3_draw_policy_Common.h"
#include "../../bm3_DrawInfo.h"
#include "../../bm3_CoreType.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE
namespace draw{
namespace policy{

template<
	DrawColor::Type DrawColorType
>
struct ModelTexture1{
	template<typename PrimitiveType, typename ResourceType>
	BM3_FORCEINLINE void Push(TextureCoordinate *dst, const PrimitiveType &prim, DrawInfo &/*draw_info*/, const ResourceType &/*res*/){

		dst->u_ = prim.texture_[0].h_scroll_;
		dst->v_ = prim.texture_[0].v_scroll_;
	}
};

}	// namespace policy
}	// namespace draw
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_MODELTEXTURE1_H
