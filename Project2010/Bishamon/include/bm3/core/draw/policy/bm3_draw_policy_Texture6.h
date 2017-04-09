#ifndef BM3_SDK_INC_BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_TEXTURE6_H
#define BM3_SDK_INC_BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_TEXTURE6_H

#include "bm3/system/bm3_Config.h"
#include "bm3_draw_policy_Common.h"
#include "../../bm3_DrawInfo.h"
#include "../../bm3_CoreType.h"
#include "../../bm3_Const.h"

#include "bm3_draw_policy_Texture4.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE
namespace draw{
namespace policy{

#define	BM3_DST(INDEX)	(reinterpret_cast<DstTextureCoordinateType *>(int(dst) + stride * (INDEX)))

template<DrawTexture::Type DrawTextureType>
struct Texture6{
	template<typename PrimitiveType, typename ResourceType, typename DstTextureCoordinateType>
	BM3_FORCEINLINE void Push(DstTextureCoordinateType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, TextureLayer::Type lt, int stride){
		ML_PTR_ASSERT(dst);

		Texture4<DrawTextureType>().Push(dst, prim, draw_info, res, lt, draw::policy::V_TYPE(), stride);

		BM3_DST(4)->u_	= BM3_DST(VertexElement::T0)->u_;
		BM3_DST(4)->v_	= BM3_DST(VertexElement::T0)->v_;
		BM3_DST(5)->u_	= BM3_DST(VertexElement::T2)->u_;
		BM3_DST(5)->v_	= BM3_DST(VertexElement::T2)->v_;
	}

	template<typename PrimitiveType, typename ResourceType, typename DstTextureCoordinateType>
	static void StaticPush(DstTextureCoordinateType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, TextureLayer::Type lt, int stride){

		Texture6().Push(dst, prim, draw_info, res, lt, stride);
	}
};

#undef	BM3_DST

}	// namespace policy
}	// namespace draw
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_TEXTURE6_H
