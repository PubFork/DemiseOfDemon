#ifndef BM3_SDK_INC_BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_COLOR6_H
#define BM3_SDK_INC_BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_COLOR6_H

#include "bm3/system/bm3_Config.h"
#include "bm3_draw_policy_Common.h"
#include "../../bm3_DrawInfo.h"
#include "../../bm3_CoreType.h"
#include <ml/utility/ml_assert.h>
#include <ml/utility/ml_cast.h>
#include <ml/type/ml_traits.h>

#include "bm3_draw_policy_Color4.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE
namespace draw{
namespace policy{

#define	BM3_DST(INDEX)	(reinterpret_cast<DstColorType *>(int(dst) + stride * (INDEX)))

template<
	DrawColor::Type DrawColorType
>
struct Color6{
	template<typename PrimitiveType, typename ResourceType, typename DstColorType>
	BM3_FORCEINLINE void Push(DstColorType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, const PacketDrawInfo& pdi, int stride){

		ML_PTR_ASSERT(dst);
	//	ML_STATIC_ASSERT((ml::is_same<typename DstColorType::ChannelType, ml::ui8>::value));			// ç°ÇÃÇ∆Ç±ÇÎ

		Color4<DrawColorType>().Push(dst, prim, draw_info, res, pdi, stride);

		BM3_DST(4)->e_.r_	= BM3_DST(VertexElement::PC0)->e_.r_;
		BM3_DST(4)->e_.g_	= BM3_DST(VertexElement::PC0)->e_.g_;
		BM3_DST(4)->e_.b_	= BM3_DST(VertexElement::PC0)->e_.b_;
		BM3_DST(4)->e_.a_	= BM3_DST(VertexElement::PC0)->e_.a_;
		BM3_DST(5)->e_.r_	= BM3_DST(VertexElement::PC2)->e_.r_;
		BM3_DST(5)->e_.g_	= BM3_DST(VertexElement::PC2)->e_.g_;
		BM3_DST(5)->e_.b_	= BM3_DST(VertexElement::PC2)->e_.b_;
		BM3_DST(5)->e_.a_	= BM3_DST(VertexElement::PC2)->e_.a_;

	}

	template<typename PrimitiveType, typename ResourceType, typename DstColorType>
	BM3_FORCEINLINE void PushBm2(DstColorType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, const PacketDrawInfo& pdi, int stride){

		ML_PTR_ASSERT(dst);
	//	ML_STATIC_ASSERT((ml::is_same<typename DstColorType::ChannelType, ml::ui8>::value));			// ç°ÇÃÇ∆Ç±ÇÎ

		Color4<DrawColorType>().PushBm2(dst, prim, draw_info, res, pdi, stride);

		BM3_DST(4)->e_.r_	= BM3_DST(VertexElement::PC0)->e_.r_;
		BM3_DST(4)->e_.g_	= BM3_DST(VertexElement::PC0)->e_.g_;
		BM3_DST(4)->e_.b_	= BM3_DST(VertexElement::PC0)->e_.b_;
		BM3_DST(4)->e_.a_	= BM3_DST(VertexElement::PC0)->e_.a_;
		BM3_DST(5)->e_.r_	= BM3_DST(VertexElement::PC2)->e_.r_;
		BM3_DST(5)->e_.g_	= BM3_DST(VertexElement::PC2)->e_.g_;
		BM3_DST(5)->e_.b_	= BM3_DST(VertexElement::PC2)->e_.b_;
		BM3_DST(5)->e_.a_	= BM3_DST(VertexElement::PC2)->e_.a_;

	}

};

#undef	BM3_DST

}	// namespace policy
}	// namespace draw
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_COLOR6_H
