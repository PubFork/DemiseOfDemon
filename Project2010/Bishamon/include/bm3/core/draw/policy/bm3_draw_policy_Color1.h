#ifndef BM3_SDK_INC_BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_COLOR1_H
#define BM3_SDK_INC_BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_COLOR1_H

#include "bm3/system/bm3_Config.h"
#include "bm3_draw_policy_Common.h"
#include "../../bm3_DrawInfo.h"
#include "../../bm3_CoreType.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE
namespace draw{
namespace policy{


#define BM3_BLEND_PARENTCOLOR(c, parent_color)	((1.0f - res.Basic_ParentNodeInfluence_ParentColorRate()) * (c) + (res.Basic_ParentNodeInfluence_ParentColorRate()) * (parent_color))
#define BM3_BLEND_PARENTALPHA(c, parent_color)																\
	((res.Basic_ParentNodeInfluence_AlphaParentNodeInfluenceType()==AlphaParentNodeInfluenceTypeConst_On) ?	\
	(res.Basic_ParentNodeInfluence_ParentAlphaRate()==0.0f) ? (c) : (c) * (parent_color) * res.Basic_ParentNodeInfluence_ParentAlphaRate() : 	\
	((1.0f - res.Basic_ParentNodeInfluence_ParentAlphaRate()) * (c) + (res.Basic_ParentNodeInfluence_ParentAlphaRate()) * (parent_color)))
	
template<
	DrawColor::Type DrawColorType
>
struct Color1{
	template<typename PrimitiveType, typename ResourceType>
	BM3_FORCEINLINE void Push(ColorType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, const PacketDrawInfo& pdi);
};

template<>
struct Color1<DrawColor::Nothing>{

	template<typename PrimitiveType, typename ResourceType>
	BM3_FORCEINLINE void Push(ColorType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, const PacketDrawInfo& pdi){
		ML_PTR_ASSERT(dst);

		// ‰½‚à‚µ‚È‚¢
		;
	}

	//BM2ŒÝŠ·
	template<typename PrimitiveType, typename ResourceType>
	BM3_FORCEINLINE void PushBm2(ColorType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, const PacketDrawInfo& pdi){
		ML_PTR_ASSERT(dst);

		// ‰½‚à‚µ‚È‚¢
		;
	}
};

template<>
struct Color1<DrawColor::_1>{

	template<typename PrimitiveType, typename ResourceType>
	BM3_FORCEINLINE void Push(ColorType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, const PacketDrawInfo& pdi){
		ML_PTR_ASSERT(dst);

		dst[0].e_.r_ = BM3_BLEND_PARENTCOLOR(prim.color_center_.value_.e_.r_,pdi.parent_color_->e_.r_);
		dst[0].e_.g_ = BM3_BLEND_PARENTCOLOR(prim.color_center_.value_.e_.g_,pdi.parent_color_->e_.g_);
		dst[0].e_.b_ = BM3_BLEND_PARENTCOLOR(prim.color_center_.value_.e_.b_,pdi.parent_color_->e_.b_);
		dst[0].e_.a_ = BM3_BLEND_PARENTALPHA(prim.color_center_.value_.e_.a_,pdi.parent_color_->e_.a_);
	}

	//BM2ŒÝŠ·
	template<typename PrimitiveType, typename ResourceType>
	BM3_FORCEINLINE void PushBm2(ColorType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, const PacketDrawInfo& pdi){
		ML_PTR_ASSERT(dst);

		dst[0].e_.r_ = BM3_BLEND_PARENTCOLOR(prim.color_center_.value_.e_.r_ * prim.color_center_.value_.e_.a_,pdi.parent_color_->e_.r_);
		dst[0].e_.g_ = BM3_BLEND_PARENTCOLOR(prim.color_center_.value_.e_.g_ * prim.color_center_.value_.e_.a_,pdi.parent_color_->e_.g_);
		dst[0].e_.b_ = BM3_BLEND_PARENTCOLOR(prim.color_center_.value_.e_.b_ * prim.color_center_.value_.e_.a_,pdi.parent_color_->e_.b_);
		dst[0].e_.a_ = BM3_BLEND_PARENTALPHA(prim.color_center_.value_.e_.a_                                  ,pdi.parent_color_->e_.a_);

	}
};

template<>
struct Color1<DrawColor::_2>{

	template<typename PrimitiveType, typename ResourceType>
	BM3_FORCEINLINE void Push(ColorType *dst, const PrimitiveType &prim, DrawInfo &/*draw_info*/, const ResourceType &res, const PacketDrawInfo& pdi){
		ML_PTR_ASSERT(dst);

		dst[0].e_.r_ = BM3_BLEND_PARENTCOLOR(prim.color_center_.value_.e_.r_,pdi.parent_color_->e_.r_);
		dst[0].e_.g_ = BM3_BLEND_PARENTCOLOR(prim.color_center_.value_.e_.g_,pdi.parent_color_->e_.g_);
		dst[0].e_.b_ = BM3_BLEND_PARENTCOLOR(prim.color_center_.value_.e_.b_,pdi.parent_color_->e_.b_);
		dst[0].e_.a_ = BM3_BLEND_PARENTALPHA(prim.color_center_.value_.e_.a_,pdi.parent_color_->e_.a_);
	}

	//BM2ŒÝŠ·
	template<typename PrimitiveType, typename ResourceType>
	BM3_FORCEINLINE void PushBm2(ColorType *dst, const PrimitiveType &prim, DrawInfo &/*draw_info*/, const ResourceType &res, const PacketDrawInfo& pdi){
		ML_PTR_ASSERT(dst);

		dst[0].e_.r_ = BM3_BLEND_PARENTCOLOR(prim.color_center_.value_.e_.r_ * prim.color_center_.value_.e_.a_,pdi.parent_color_->e_.r_);
		dst[0].e_.g_ = BM3_BLEND_PARENTCOLOR(prim.color_center_.value_.e_.g_ * prim.color_center_.value_.e_.a_,pdi.parent_color_->e_.g_);
		dst[0].e_.b_ = BM3_BLEND_PARENTCOLOR(prim.color_center_.value_.e_.b_ * prim.color_center_.value_.e_.a_,pdi.parent_color_->e_.b_);
		dst[0].e_.a_ = BM3_BLEND_PARENTALPHA(prim.color_center_.value_.e_.a_                                  ,pdi.parent_color_->e_.a_);
	}
};
	
#undef BM3_BLEND_PARENTALPHA
#undef BM3_BLEND_PARENTCOLOR
	

}	// namespace policy
}	// namespace draw
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_COLOR1_H
