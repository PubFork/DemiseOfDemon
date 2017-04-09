#ifndef BM3_SDK_INC_BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_COLOR4_H
#define BM3_SDK_INC_BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_COLOR4_H

#include "bm3/system/bm3_Config.h"
#include "bm3_draw_policy_Common.h"
#include "../../bm3_DrawInfo.h"
#include "../../bm3_CoreType.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE
namespace draw{
namespace policy{

#define	BM3_DST(INDEX)	(reinterpret_cast<DstType *>(int(dst) + stride * (INDEX)))
#define BM3_BLEND_PARENTCOLOR(c, parent_color)	((1.0f - res.Basic_ParentNodeInfluence_ParentColorRate()) * (c) + (res.Basic_ParentNodeInfluence_ParentColorRate()) * (parent_color))
#define BM3_BLEND_PARENTALPHA(c, parent_color)																\
	((res.Basic_ParentNodeInfluence_AlphaParentNodeInfluenceType()==AlphaParentNodeInfluenceTypeConst_On) ?	\
	(res.Basic_ParentNodeInfluence_ParentAlphaRate()==0.0f) ? (c) : (c) * (parent_color) * res.Basic_ParentNodeInfluence_ParentAlphaRate() : 	\
	((1.0f - res.Basic_ParentNodeInfluence_ParentAlphaRate()) * (c) + (res.Basic_ParentNodeInfluence_ParentAlphaRate()) * (parent_color)))

template<
	DrawColor::Type DrawColorType
>
struct Color4{
	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void Push(DstType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, const PacketDrawInfo& pdi, int stride = sizeof(DstType));
};

template<>
struct Color4<DrawColor::Nothing>{

	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void Push(DstType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, const PacketDrawInfo& pdi, int stride = sizeof(DstType)){
		ML_PTR_ASSERT(dst);

		// �������Ȃ�
		;
	}

	//BM2�݊�
	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void PushBm2(DstType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, const PacketDrawInfo& pdi, int stride = sizeof(DstType)){
		ML_PTR_ASSERT(dst);

		// �������Ȃ�
		;
	}

};

#if defined(BM3_PLATFORM_xbox360)
#endif

template<>
struct Color4<DrawColor::_1>{

	template<typename PrimitiveType, typename ResourceType>
	BM3_FORCEINLINE void Push(ColorType *dst, const PrimitiveType &prim, DrawInfo &/*draw_info*/, const ResourceType &res, const PacketDrawInfo& pdi, int stride = sizeof(ColorType)){
		ML_PTR_ASSERT(dst);

#if defined(BM3_PLATFORM_psp)
		__asm__ (
			".set			push\n"					// save assembler option
			".set			noreorder\n"			// suppress reordering
			"viim.s			s200, 255\n"			// 255 -> 255.0f	
			"lv.q			c000, %4\n"				
			"vscl.q			c100, c000,   s200\n"	
			"sv.q			c100, %0\n"				
			"sv.q			c100, %1\n"				
			"sv.q			c100, %2\n"				
			"sv.q			c100, %3\n"				
			".set			pop\n"					// restore assembler option
			: "=m"(dst[0]), "=m"(dst[1]), "=m"(dst[2]), "=m"(dst[3])
			: "m"(*reinterpret_cast<const ml::detail::color_rgba<float>*>(&prim.color_center_.value_))
		);
		
		dst[0].e_.r_	= BM3_BLEND_PARENTCOLOR(dst[0].e_.r_	,pdi.parent_color_->e_.r_);
		dst[0].e_.g_	= BM3_BLEND_PARENTCOLOR(dst[0].e_.g_	,pdi.parent_color_->e_.g_);
		dst[0].e_.b_	= BM3_BLEND_PARENTCOLOR(dst[0].e_.b_	,pdi.parent_color_->e_.b_);
		dst[0].e_.a_	= BM3_BLEND_PARENTALPHA(dst[0].e_.a_	,pdi.parent_color_->e_.a_);
		dst[1].e_.r_	= BM3_BLEND_PARENTCOLOR(dst[1].e_.r_	,pdi.parent_color_->e_.r_);
		dst[1].e_.g_	= BM3_BLEND_PARENTCOLOR(dst[1].e_.g_	,pdi.parent_color_->e_.g_);
		dst[1].e_.b_	= BM3_BLEND_PARENTCOLOR(dst[1].e_.b_	,pdi.parent_color_->e_.b_);
		dst[1].e_.a_	= BM3_BLEND_PARENTALPHA(dst[1].e_.a_	,pdi.parent_color_->e_.a_);
		dst[2].e_.r_	= BM3_BLEND_PARENTCOLOR(dst[2].e_.r_	,pdi.parent_color_->e_.r_);
		dst[2].e_.g_	= BM3_BLEND_PARENTCOLOR(dst[2].e_.g_	,pdi.parent_color_->e_.g_);
		dst[2].e_.b_	= BM3_BLEND_PARENTCOLOR(dst[2].e_.b_	,pdi.parent_color_->e_.b_);
		dst[2].e_.a_	= BM3_BLEND_PARENTALPHA(dst[2].e_.a_	,pdi.parent_color_->e_.a_);
		dst[3].e_.r_	= BM3_BLEND_PARENTCOLOR(dst[3].e_.r_	,pdi.parent_color_->e_.r_);
		dst[3].e_.g_	= BM3_BLEND_PARENTCOLOR(dst[3].e_.g_	,pdi.parent_color_->e_.g_);
		dst[3].e_.b_	= BM3_BLEND_PARENTCOLOR(dst[3].e_.b_	,pdi.parent_color_->e_.b_);
		dst[3].e_.a_	= BM3_BLEND_PARENTALPHA(dst[3].e_.a_	,pdi.parent_color_->e_.a_);
#else
		typedef	ColorType	DstType;

		BM3_DST(VertexElement::PC0)->e_.r_ = BM3_BLEND_PARENTCOLOR(prim.color_center_.value_.e_.r_	,pdi.parent_color_->e_.r_);
		BM3_DST(VertexElement::PC0)->e_.g_ = BM3_BLEND_PARENTCOLOR(prim.color_center_.value_.e_.g_	,pdi.parent_color_->e_.g_);
		BM3_DST(VertexElement::PC0)->e_.b_ = BM3_BLEND_PARENTCOLOR(prim.color_center_.value_.e_.b_	,pdi.parent_color_->e_.b_);
		BM3_DST(VertexElement::PC0)->e_.a_ = BM3_BLEND_PARENTALPHA(prim.color_center_.value_.e_.a_	,pdi.parent_color_->e_.a_);
		BM3_DST(VertexElement::PC1)->e_.r_ = BM3_BLEND_PARENTCOLOR(prim.color_center_.value_.e_.r_	,pdi.parent_color_->e_.r_);
		BM3_DST(VertexElement::PC1)->e_.g_ = BM3_BLEND_PARENTCOLOR(prim.color_center_.value_.e_.g_	,pdi.parent_color_->e_.g_);
		BM3_DST(VertexElement::PC1)->e_.b_ = BM3_BLEND_PARENTCOLOR(prim.color_center_.value_.e_.b_	,pdi.parent_color_->e_.b_);
		BM3_DST(VertexElement::PC1)->e_.a_ = BM3_BLEND_PARENTALPHA(prim.color_center_.value_.e_.a_	,pdi.parent_color_->e_.a_);
		BM3_DST(VertexElement::PC2)->e_.r_ = BM3_BLEND_PARENTCOLOR(prim.color_center_.value_.e_.r_	,pdi.parent_color_->e_.r_);
		BM3_DST(VertexElement::PC2)->e_.g_ = BM3_BLEND_PARENTCOLOR(prim.color_center_.value_.e_.g_	,pdi.parent_color_->e_.g_);
		BM3_DST(VertexElement::PC2)->e_.b_ = BM3_BLEND_PARENTCOLOR(prim.color_center_.value_.e_.b_	,pdi.parent_color_->e_.b_);
		BM3_DST(VertexElement::PC2)->e_.a_ = BM3_BLEND_PARENTALPHA(prim.color_center_.value_.e_.a_	,pdi.parent_color_->e_.a_);
		BM3_DST(VertexElement::PC3)->e_.r_ = BM3_BLEND_PARENTCOLOR(prim.color_center_.value_.e_.r_	,pdi.parent_color_->e_.r_);
		BM3_DST(VertexElement::PC3)->e_.g_ = BM3_BLEND_PARENTCOLOR(prim.color_center_.value_.e_.g_	,pdi.parent_color_->e_.g_);
		BM3_DST(VertexElement::PC3)->e_.b_ = BM3_BLEND_PARENTCOLOR(prim.color_center_.value_.e_.b_	,pdi.parent_color_->e_.b_);
		BM3_DST(VertexElement::PC3)->e_.a_ = BM3_BLEND_PARENTALPHA(prim.color_center_.value_.e_.a_	,pdi.parent_color_->e_.a_);
#endif
	}

	//BM2�݊�
	template<typename PrimitiveType, typename ResourceType>
	BM3_FORCEINLINE void PushBm2(ColorType *dst, const PrimitiveType &prim, DrawInfo &/*draw_info*/, const ResourceType &res, const PacketDrawInfo& pdi, int stride = sizeof(ColorType)){
		ML_PTR_ASSERT(dst);

#if defined(BM3_PLATFORM_psp)
		__asm__ (
			".set			push\n"					// save assembler option
			".set			noreorder\n"			// suppress reordering
			"viim.s			s200, 255\n"			// 255 -> 255.0f	
			"lv.q			c000, %4\n"				
			"vscl.q			c100, c000,   s200\n"	
			"sv.q			c100, %0\n"				
			"sv.q			c100, %1\n"				
			"sv.q			c100, %2\n"				
			"sv.q			c100, %3\n"				
			".set			pop\n"					// restore assembler option
			: "=m"(dst[0]), "=m"(dst[1]), "=m"(dst[2]), "=m"(dst[3])
			: "m"(*reinterpret_cast<const ml::detail::color_rgba<float>*>(&prim.color_center_.value_))
		);
		dst[0].e_.r_	= BM3_BLEND_PARENTCOLOR(dst[0].e_.r_	,pdi.parent_color_->e_.r_);
		dst[0].e_.g_	= BM3_BLEND_PARENTCOLOR(dst[0].e_.g_	,pdi.parent_color_->e_.g_);
		dst[0].e_.b_	= BM3_BLEND_PARENTCOLOR(dst[0].e_.b_	,pdi.parent_color_->e_.b_);
		dst[0].e_.a_	= BM3_BLEND_PARENTALPHA(dst[0].e_.a_	,pdi.parent_color_->e_.a_);
		dst[1].e_.r_	= BM3_BLEND_PARENTCOLOR(dst[1].e_.r_	,pdi.parent_color_->e_.r_);
		dst[1].e_.g_	= BM3_BLEND_PARENTCOLOR(dst[1].e_.g_	,pdi.parent_color_->e_.g_);
		dst[1].e_.b_	= BM3_BLEND_PARENTCOLOR(dst[1].e_.b_	,pdi.parent_color_->e_.b_);
		dst[1].e_.a_	= BM3_BLEND_PARENTALPHA(dst[1].e_.a_	,pdi.parent_color_->e_.a_);
		dst[2].e_.r_	= BM3_BLEND_PARENTCOLOR(dst[2].e_.r_	,pdi.parent_color_->e_.r_);
		dst[2].e_.g_	= BM3_BLEND_PARENTCOLOR(dst[2].e_.g_	,pdi.parent_color_->e_.g_);
		dst[2].e_.b_	= BM3_BLEND_PARENTCOLOR(dst[2].e_.b_	,pdi.parent_color_->e_.b_);
		dst[2].e_.a_	= BM3_BLEND_PARENTALPHA(dst[2].e_.a_	,pdi.parent_color_->e_.a_);
		dst[3].e_.r_	= BM3_BLEND_PARENTCOLOR(dst[3].e_.r_	,pdi.parent_color_->e_.r_);
		dst[3].e_.g_	= BM3_BLEND_PARENTCOLOR(dst[3].e_.g_	,pdi.parent_color_->e_.g_);
		dst[3].e_.b_	= BM3_BLEND_PARENTCOLOR(dst[3].e_.b_	,pdi.parent_color_->e_.b_);
		dst[3].e_.a_	= BM3_BLEND_PARENTALPHA(dst[3].e_.a_	,pdi.parent_color_->e_.a_);
#else
		typedef	ColorType	DstType;

		ColorType	c;
		c.e_.r_	= BM3_BLEND_PARENTCOLOR(prim.color_center_.value_.e_.r_ * prim.color_center_.value_.e_.a_	,pdi.parent_color_->e_.r_);
		c.e_.g_	= BM3_BLEND_PARENTCOLOR(prim.color_center_.value_.e_.g_ * prim.color_center_.value_.e_.a_	,pdi.parent_color_->e_.g_);
		c.e_.b_	= BM3_BLEND_PARENTCOLOR(prim.color_center_.value_.e_.b_ * prim.color_center_.value_.e_.a_	,pdi.parent_color_->e_.b_);
		c.e_.a_	= BM3_BLEND_PARENTALPHA(prim.color_center_.value_.e_.a_										,pdi.parent_color_->e_.a_);
		*BM3_DST(VertexElement::PC0) = 
		*BM3_DST(VertexElement::PC1) = 
		*BM3_DST(VertexElement::PC2) = 
		*BM3_DST(VertexElement::PC3) = c;

#endif
	}

	template<typename PrimitiveType, typename ResourceType>
	BM3_FORCEINLINE void Push(ColorType *dst, const PrimitiveType &prim, DrawInfo &/*draw_info*/, const ResourceType &res, const ColorType &color_scale, const PacketDrawInfo& pdi, int stride = sizeof(ColorType)){
		ML_PTR_ASSERT(dst);
		typedef	ColorType	DstType;

		ColorType	c;
		c.e_.r_	= BM3_BLEND_PARENTCOLOR(prim.color_center_.value_.e_.r_	,pdi.parent_color_->e_.r_) * color_scale.e_.r_;
		c.e_.g_	= BM3_BLEND_PARENTCOLOR(prim.color_center_.value_.e_.g_	,pdi.parent_color_->e_.g_) * color_scale.e_.g_;
		c.e_.b_	= BM3_BLEND_PARENTCOLOR(prim.color_center_.value_.e_.b_	,pdi.parent_color_->e_.b_) * color_scale.e_.b_;
		c.e_.a_	= BM3_BLEND_PARENTALPHA(prim.color_center_.value_.e_.a_	,pdi.parent_color_->e_.a_) * color_scale.e_.a_;
		*BM3_DST(VertexElement::PC0) = 
		*BM3_DST(VertexElement::PC1) = 
		*BM3_DST(VertexElement::PC2) = 
		*BM3_DST(VertexElement::PC3) = c;
	}

	//BM2�݊�
	template<typename PrimitiveType, typename ResourceType>
	BM3_FORCEINLINE void PushBm2(ColorType *dst, const PrimitiveType &prim, DrawInfo &/*draw_info*/, const ResourceType &res, const ColorType &color_scale, const PacketDrawInfo& pdi, int stride = sizeof(ColorType)){
		ML_PTR_ASSERT(dst);
		typedef	ColorType	DstType;

		ColorType	c;
		c.e_.a_	= BM3_BLEND_PARENTALPHA(prim.color_center_.value_.e_.a_ 									,pdi.parent_color_->e_.a_)	* color_scale.e_.a_;
		c.e_.r_	= BM3_BLEND_PARENTCOLOR(prim.color_center_.value_.e_.r_ * prim.color_center_.value_.e_.a_	,pdi.parent_color_->e_.r_)	* color_scale.e_.r_;
		c.e_.g_	= BM3_BLEND_PARENTCOLOR(prim.color_center_.value_.e_.g_ * prim.color_center_.value_.e_.a_	,pdi.parent_color_->e_.g_)	* color_scale.e_.g_;
		c.e_.b_	= BM3_BLEND_PARENTCOLOR(prim.color_center_.value_.e_.b_ * prim.color_center_.value_.e_.a_	,pdi.parent_color_->e_.b_)	* color_scale.e_.b_;

		*BM3_DST(VertexElement::PC0) = 
		*BM3_DST(VertexElement::PC1) = 
		*BM3_DST(VertexElement::PC2) = 
		*BM3_DST(VertexElement::PC3) = c;
	}

	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void Push(DstType *dst, const PrimitiveType &prim, DrawInfo &/*draw_info*/, const ResourceType &res, const PacketDrawInfo& pdi, int stride = sizeof(DstType)){
		ML_PTR_ASSERT(dst);

#if defined(BM3_PLATFORM_xbox360)
		struct Local{
			static inline void PackVector4ToD3DCOLOR( __vector4 input, DWORD *pDest0, DWORD *pDest1, DWORD *pDest2, DWORD *pDest3){
				static const __vector4 vPackMul8888 = {(255.0 / (1 << 22)), (255.0 / (1 << 22)), (255.0 / (1 << 22)), (255.0 / (1 << 22))};
				static const __vector4 vPackAdd = { 3.0, 3.0, 3.0, 3.0 };

				// Assume the four floats in 'input' are in the range
				// 0.0 to 1.0. We need to get them into the range
				// 3.0 + x*(2^-22) where x is a number from 0 to 255
				__vector4 result = __vmaddfp( input, vPackMul8888, vPackAdd );
				result = __vpkd3d( result, result, VPACK_D3DCOLOR, VPACK_32, 0 );

				// Now we need to store the 32-bit result, using __stvewx. We
				// have to splat the result from element 3 to all four words so
				// that the correct value will be stored regardless of alignment.
				result = __vspltw( result, 3 );

				// Store the result. pDest must be DWORD aligned.
				__stvewx( result, pDest0, 0 );
				__stvewx( result, pDest1, 0 );
				__stvewx( result, pDest2, 0 );
				__stvewx( result, pDest3, 0 );
			}
		};

		__vector4 color_center		= __loadunalignedvector(prim.color_center_.value_.v_);

		Local::PackVector4ToD3DCOLOR(color_center, (DWORD *)BM3_DST(0), (DWORD *)BM3_DST(1), (DWORD *)BM3_DST(2), (DWORD *)BM3_DST(3));
#else
		ml::ui8	temp[]	= {
			ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTCOLOR(prim.color_center_.value_.e_.r_	,pdi.parent_color_->e_.r_) * 255.0f),
			ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTCOLOR(prim.color_center_.value_.e_.g_	,pdi.parent_color_->e_.g_) * 255.0f),
			ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTCOLOR(prim.color_center_.value_.e_.b_	,pdi.parent_color_->e_.b_) * 255.0f),
			ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTALPHA(prim.color_center_.value_.e_.a_	,pdi.parent_color_->e_.a_) * 255.0f)
		};

		BM3_DST(VertexElement::PC0)->e_.r_ = temp[0];
		BM3_DST(VertexElement::PC0)->e_.g_ = temp[1];
		BM3_DST(VertexElement::PC0)->e_.b_ = temp[2];
		BM3_DST(VertexElement::PC0)->e_.a_ = temp[3];
		BM3_DST(VertexElement::PC1)->e_.r_ = temp[0];
		BM3_DST(VertexElement::PC1)->e_.g_ = temp[1];
		BM3_DST(VertexElement::PC1)->e_.b_ = temp[2];
		BM3_DST(VertexElement::PC1)->e_.a_ = temp[3];
		BM3_DST(VertexElement::PC2)->e_.r_ = temp[0];
		BM3_DST(VertexElement::PC2)->e_.g_ = temp[1];
		BM3_DST(VertexElement::PC2)->e_.b_ = temp[2];
		BM3_DST(VertexElement::PC2)->e_.a_ = temp[3];
		BM3_DST(VertexElement::PC3)->e_.r_ = temp[0];
		BM3_DST(VertexElement::PC3)->e_.g_ = temp[1];
		BM3_DST(VertexElement::PC3)->e_.b_ = temp[2];
		BM3_DST(VertexElement::PC3)->e_.a_ = temp[3];
#endif
	}

	//BM2�݊�
	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void PushBm2(DstType *dst, const PrimitiveType &prim, DrawInfo &/*draw_info*/, const ResourceType &res, const PacketDrawInfo& pdi, int stride = sizeof(DstType)){
		ML_PTR_ASSERT(dst);

#if defined(BM3_PLATFORM_xbox360)
		struct Local{
			static inline void PackVector4ToD3DCOLOR( __vector4 input, DWORD *pDest0, DWORD *pDest1, DWORD *pDest2, DWORD *pDest3){
				static const __vector4 vPackMul8888 = {(255.0 / (1 << 22)), (255.0 / (1 << 22)), (255.0 / (1 << 22)), (255.0 / (1 << 22))};
				static const __vector4 vPackAdd = { 3.0, 3.0, 3.0, 3.0 };

				// Assume the four floats in 'input' are in the range
				// 0.0 to 1.0. We need to get them into the range
				// 3.0 + x*(2^-22) where x is a number from 0 to 255
				__vector4 result = __vmaddfp( input, vPackMul8888, vPackAdd );
				result = __vpkd3d( result, result, VPACK_D3DCOLOR, VPACK_32, 0 );

				// Now we need to store the 32-bit result, using __stvewx. We
				// have to splat the result from element 3 to all four words so
				// that the correct value will be stored regardless of alignment.
				result = __vspltw( result, 3 );

				// Store the result. pDest must be DWORD aligned.
				__stvewx( result, pDest0, 0 );
				__stvewx( result, pDest1, 0 );
				__stvewx( result, pDest2, 0 );
				__stvewx( result, pDest3, 0 );
			}
		};

		__vector4 color_center		= __loadunalignedvector(prim.color_center_.value_.v_);

		Local::PackVector4ToD3DCOLOR(color_center, (DWORD *)BM3_DST(0), (DWORD *)BM3_DST(1), (DWORD *)BM3_DST(2), (DWORD *)BM3_DST(3));
#else
		ml::ui8	temp[]	= {
			ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTCOLOR(prim.color_center_.value_.e_.r_ * prim.color_center_.value_.e_.a_	,pdi.parent_color_->e_.r_) * 255.0f),
			ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTCOLOR(prim.color_center_.value_.e_.g_ * prim.color_center_.value_.e_.a_	,pdi.parent_color_->e_.g_) * 255.0f),
			ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTCOLOR(prim.color_center_.value_.e_.b_ * prim.color_center_.value_.e_.a_	,pdi.parent_color_->e_.b_) * 255.0f),
			ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTALPHA(prim.color_center_.value_.e_.a_                                  	,pdi.parent_color_->e_.a_) * 255.0f)
		};

		BM3_DST(VertexElement::PC0)->e_.r_ = temp[0];
		BM3_DST(VertexElement::PC0)->e_.g_ = temp[1];
		BM3_DST(VertexElement::PC0)->e_.b_ = temp[2];
		BM3_DST(VertexElement::PC0)->e_.a_ = temp[3];
		BM3_DST(VertexElement::PC1)->e_.r_ = temp[0];
		BM3_DST(VertexElement::PC1)->e_.g_ = temp[1];
		BM3_DST(VertexElement::PC1)->e_.b_ = temp[2];
		BM3_DST(VertexElement::PC1)->e_.a_ = temp[3];
		BM3_DST(VertexElement::PC2)->e_.r_ = temp[0];
		BM3_DST(VertexElement::PC2)->e_.g_ = temp[1];
		BM3_DST(VertexElement::PC2)->e_.b_ = temp[2];
		BM3_DST(VertexElement::PC2)->e_.a_ = temp[3];
		BM3_DST(VertexElement::PC3)->e_.r_ = temp[0];
		BM3_DST(VertexElement::PC3)->e_.g_ = temp[1];
		BM3_DST(VertexElement::PC3)->e_.b_ = temp[2];
		BM3_DST(VertexElement::PC3)->e_.a_ = temp[3];
#endif
	}

	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void Push(DstType *dst, const PrimitiveType &prim, DrawInfo &/*draw_info*/, const ResourceType &res, const ColorType &color_scale, const PacketDrawInfo& pdi, int stride = sizeof(DstType)){
		ML_PTR_ASSERT(dst);

		ml::ui8	temp[]	= {
			ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTCOLOR(prim.color_center_.value_.e_.r_ ,pdi.parent_color_->e_.r_) * color_scale.e_.r_),
			ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTCOLOR(prim.color_center_.value_.e_.g_ ,pdi.parent_color_->e_.g_) * color_scale.e_.g_),
			ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTCOLOR(prim.color_center_.value_.e_.b_ ,pdi.parent_color_->e_.b_) * color_scale.e_.b_),
			ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTALPHA(prim.color_center_.value_.e_.a_ ,pdi.parent_color_->e_.a_) * color_scale.e_.a_)
		};
		
		BM3_DST(VertexElement::PC0)->e_.r_ = temp[0];
		BM3_DST(VertexElement::PC0)->e_.g_ = temp[1];
		BM3_DST(VertexElement::PC0)->e_.b_ = temp[2];
		BM3_DST(VertexElement::PC0)->e_.a_ = temp[3];
		BM3_DST(VertexElement::PC1)->e_.r_ = temp[0];
		BM3_DST(VertexElement::PC1)->e_.g_ = temp[1];
		BM3_DST(VertexElement::PC1)->e_.b_ = temp[2];
		BM3_DST(VertexElement::PC1)->e_.a_ = temp[3];
		BM3_DST(VertexElement::PC2)->e_.r_ = temp[0];
		BM3_DST(VertexElement::PC2)->e_.g_ = temp[1];
		BM3_DST(VertexElement::PC2)->e_.b_ = temp[2];
		BM3_DST(VertexElement::PC2)->e_.a_ = temp[3];
		BM3_DST(VertexElement::PC3)->e_.r_ = temp[0];
		BM3_DST(VertexElement::PC3)->e_.g_ = temp[1];
		BM3_DST(VertexElement::PC3)->e_.b_ = temp[2];
		BM3_DST(VertexElement::PC3)->e_.a_ = temp[3];
	}

	//BM2�݊�
	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void PushBm2(DstType *dst, const PrimitiveType &prim, DrawInfo &/*draw_info*/, const ResourceType &res, const ColorType &color_scale, const PacketDrawInfo& pdi, int stride = sizeof(DstType)){
		ML_PTR_ASSERT(dst);

		ml::ui8	temp[]	= {
			ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTCOLOR(prim.color_center_.value_.e_.r_	,pdi.parent_color_->e_.r_) * color_scale.e_.r_),
			ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTCOLOR(prim.color_center_.value_.e_.g_	,pdi.parent_color_->e_.g_) * color_scale.e_.g_),
			ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTCOLOR(prim.color_center_.value_.e_.b_	,pdi.parent_color_->e_.b_) * color_scale.e_.b_),
			ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTALPHA(prim.color_center_.value_.e_.a_	,pdi.parent_color_->e_.a_) * color_scale.e_.a_)
		};
		
		BM3_DST(VertexElement::PC0)->e_.r_ = temp[0] * temp[3];
		BM3_DST(VertexElement::PC0)->e_.g_ = temp[1] * temp[3];
		BM3_DST(VertexElement::PC0)->e_.b_ = temp[2] * temp[3];
		BM3_DST(VertexElement::PC0)->e_.a_ = temp[3];
		BM3_DST(VertexElement::PC1)->e_.r_ = temp[0] * temp[3];
		BM3_DST(VertexElement::PC1)->e_.g_ = temp[1] * temp[3];
		BM3_DST(VertexElement::PC1)->e_.b_ = temp[2] * temp[3];
		BM3_DST(VertexElement::PC1)->e_.a_ = temp[3];
		BM3_DST(VertexElement::PC2)->e_.r_ = temp[0] * temp[3];
		BM3_DST(VertexElement::PC2)->e_.g_ = temp[1] * temp[3];
		BM3_DST(VertexElement::PC2)->e_.b_ = temp[2] * temp[3];
		BM3_DST(VertexElement::PC2)->e_.a_ = temp[3];
		BM3_DST(VertexElement::PC3)->e_.r_ = temp[0] * temp[3];
		BM3_DST(VertexElement::PC3)->e_.g_ = temp[1] * temp[3];
		BM3_DST(VertexElement::PC3)->e_.b_ = temp[2] * temp[3];
		BM3_DST(VertexElement::PC3)->e_.a_ = temp[3];
	}
};

template<>
struct Color4<DrawColor::_4>{

	template<typename PrimitiveType, typename ResourceType>
	BM3_FORCEINLINE void Push(ColorType *dst, const PrimitiveType &prim, DrawInfo &/*draw_info*/, const ResourceType &res, const PacketDrawInfo& pdi, int stride = sizeof(ColorType)){
		ML_PTR_ASSERT(dst);

		typedef	ColorType	DstType;

		BM3_DST(VertexElement::PC0)->e_.r_ = BM3_BLEND_PARENTCOLOR(prim.color_left_top_.value_.e_.r_		,pdi.parent_color_->e_.r_);
		BM3_DST(VertexElement::PC0)->e_.g_ = BM3_BLEND_PARENTCOLOR(prim.color_left_top_.value_.e_.g_		,pdi.parent_color_->e_.g_);
		BM3_DST(VertexElement::PC0)->e_.b_ = BM3_BLEND_PARENTCOLOR(prim.color_left_top_.value_.e_.b_		,pdi.parent_color_->e_.b_);
		BM3_DST(VertexElement::PC0)->e_.a_ = BM3_BLEND_PARENTALPHA(prim.color_left_top_.value_.e_.a_		,pdi.parent_color_->e_.a_);
		BM3_DST(VertexElement::PC1)->e_.r_ = BM3_BLEND_PARENTCOLOR(prim.color_left_bottom_.value_.e_.r_		,pdi.parent_color_->e_.r_);
		BM3_DST(VertexElement::PC1)->e_.g_ = BM3_BLEND_PARENTCOLOR(prim.color_left_bottom_.value_.e_.g_		,pdi.parent_color_->e_.g_);
		BM3_DST(VertexElement::PC1)->e_.b_ = BM3_BLEND_PARENTCOLOR(prim.color_left_bottom_.value_.e_.b_		,pdi.parent_color_->e_.b_);
		BM3_DST(VertexElement::PC1)->e_.a_ = BM3_BLEND_PARENTALPHA(prim.color_left_bottom_.value_.e_.a_		,pdi.parent_color_->e_.a_);
		BM3_DST(VertexElement::PC2)->e_.r_ = BM3_BLEND_PARENTCOLOR(prim.color_right_bottom_.value_.e_.r_	,pdi.parent_color_->e_.r_);
		BM3_DST(VertexElement::PC2)->e_.g_ = BM3_BLEND_PARENTCOLOR(prim.color_right_bottom_.value_.e_.g_	,pdi.parent_color_->e_.g_);
		BM3_DST(VertexElement::PC2)->e_.b_ = BM3_BLEND_PARENTCOLOR(prim.color_right_bottom_.value_.e_.b_	,pdi.parent_color_->e_.b_);
		BM3_DST(VertexElement::PC2)->e_.a_ = BM3_BLEND_PARENTALPHA(prim.color_right_bottom_.value_.e_.a_	,pdi.parent_color_->e_.a_);
		BM3_DST(VertexElement::PC3)->e_.r_ = BM3_BLEND_PARENTCOLOR(prim.color_right_top_.value_.e_.r_		,pdi.parent_color_->e_.r_);
		BM3_DST(VertexElement::PC3)->e_.g_ = BM3_BLEND_PARENTCOLOR(prim.color_right_top_.value_.e_.g_		,pdi.parent_color_->e_.g_);
		BM3_DST(VertexElement::PC3)->e_.b_ = BM3_BLEND_PARENTCOLOR(prim.color_right_top_.value_.e_.b_		,pdi.parent_color_->e_.b_);
		BM3_DST(VertexElement::PC3)->e_.a_ = BM3_BLEND_PARENTALPHA(prim.color_right_top_.value_.e_.a_		,pdi.parent_color_->e_.a_);
	}

	//BM2�݊�
	template<typename PrimitiveType, typename ResourceType>
	BM3_FORCEINLINE void PushBm2(ColorType *dst, const PrimitiveType &prim, DrawInfo &/*draw_info*/, const ResourceType &res, const PacketDrawInfo& pdi, int stride = sizeof(ColorType)){
		ML_PTR_ASSERT(dst);

		typedef	ColorType	DstType;


		ColorType	c;

		c.e_.r_	= BM3_BLEND_PARENTCOLOR(prim.color_left_top_.value_.e_.r_ * prim.color_left_top_.value_.e_.a_,pdi.parent_color_->e_.r_);
		c.e_.g_	= BM3_BLEND_PARENTCOLOR(prim.color_left_top_.value_.e_.g_ * prim.color_left_top_.value_.e_.a_,pdi.parent_color_->e_.g_);
		c.e_.b_	= BM3_BLEND_PARENTCOLOR(prim.color_left_top_.value_.e_.b_ * prim.color_left_top_.value_.e_.a_,pdi.parent_color_->e_.b_);
		c.e_.a_	= BM3_BLEND_PARENTALPHA(prim.color_left_top_.value_.e_.a_                                    ,pdi.parent_color_->e_.a_);
		*BM3_DST(VertexElement::PC0) = c;

		c.e_.r_	= BM3_BLEND_PARENTCOLOR(prim.color_left_bottom_.value_.e_.r_ * prim.color_left_bottom_.value_.e_.a_,pdi.parent_color_->e_.r_);
		c.e_.g_	= BM3_BLEND_PARENTCOLOR(prim.color_left_bottom_.value_.e_.g_ * prim.color_left_bottom_.value_.e_.a_,pdi.parent_color_->e_.g_);
		c.e_.b_	= BM3_BLEND_PARENTCOLOR(prim.color_left_bottom_.value_.e_.b_ * prim.color_left_bottom_.value_.e_.a_,pdi.parent_color_->e_.b_);
		c.e_.a_	= BM3_BLEND_PARENTALPHA(prim.color_left_bottom_.value_.e_.a_                                       ,pdi.parent_color_->e_.a_);
		*BM3_DST(VertexElement::PC1) = c;

		c.e_.r_	= BM3_BLEND_PARENTCOLOR(prim.color_right_bottom_.value_.e_.r_ * prim.color_right_bottom_.value_.e_.a_,pdi.parent_color_->e_.r_);
		c.e_.g_	= BM3_BLEND_PARENTCOLOR(prim.color_right_bottom_.value_.e_.g_ * prim.color_right_bottom_.value_.e_.a_,pdi.parent_color_->e_.g_);
		c.e_.b_	= BM3_BLEND_PARENTCOLOR(prim.color_right_bottom_.value_.e_.b_ * prim.color_right_bottom_.value_.e_.a_,pdi.parent_color_->e_.b_);
		c.e_.a_	= BM3_BLEND_PARENTALPHA(prim.color_right_bottom_.value_.e_.a_                                        ,pdi.parent_color_->e_.a_);
		*BM3_DST(VertexElement::PC2) = c;

		c.e_.r_	= BM3_BLEND_PARENTCOLOR(prim.color_right_top_.value_.e_.r_ * prim.color_right_top_.value_.e_.a_,pdi.parent_color_->e_.r_);
		c.e_.g_	= BM3_BLEND_PARENTCOLOR(prim.color_right_top_.value_.e_.g_ * prim.color_right_top_.value_.e_.a_,pdi.parent_color_->e_.g_);
		c.e_.b_	= BM3_BLEND_PARENTCOLOR(prim.color_right_top_.value_.e_.b_ * prim.color_right_top_.value_.e_.a_,pdi.parent_color_->e_.b_);
		c.e_.a_	= BM3_BLEND_PARENTALPHA(prim.color_right_top_.value_.e_.a_                                     ,pdi.parent_color_->e_.a_);
		*BM3_DST(VertexElement::PC3) = c;

	}

	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void Push(DstType *dst, const PrimitiveType &prim, DrawInfo &/*draw_info*/, const ResourceType &res, const PacketDrawInfo& pdi, int stride = sizeof(DstType)){
		ML_PTR_ASSERT(dst);

#if defined(BM3_PLATFORM_xbox360)
		struct Local{
			static inline void PackVector4ToD3DCOLOR( __vector4 input, DWORD *pDest){
				static const __vector4 vPackMul8888 = {(255.0 / (1 << 22)), (255.0 / (1 << 22)), (255.0 / (1 << 22)), (255.0 / (1 << 22))};
				static const __vector4 vPackAdd = { 3.0, 3.0, 3.0, 3.0 };

				// Assume the four floats in 'input' are in the range
				// 0.0 to 1.0. We need to get them into the range
				// 3.0 + x*(2^-22) where x is a number from 0 to 255
				__vector4 result = __vmaddfp( input, vPackMul8888, vPackAdd );
				result = __vpkd3d( result, result, VPACK_D3DCOLOR, VPACK_32, 0 );

				// Now we need to store the 32-bit result, using __stvewx. We
				// have to splat the result from element 3 to all four words so
				// that the correct value will be stored regardless of alignment.
				result = __vspltw( result, 3 );

				// Store the result. pDest must be DWORD aligned.
				__stvewx( result, pDest, 0 );
			}
		};

		__vector4 left_top		= __loadunalignedvector(prim.color_left_top_.value_.v_);
		__vector4 left_bottom	= __loadunalignedvector(prim.color_left_bottom_.value_.v_);
		__vector4 right_bottom	= __loadunalignedvector(prim.color_right_bottom_.value_.v_);
		__vector4 right_top		= __loadunalignedvector(prim.color_right_top_.value_.v_);

		Local::PackVector4ToD3DCOLOR(left_top,     (DWORD *)BM3_DST(VertexElement::PC0));
		Local::PackVector4ToD3DCOLOR(left_bottom,  (DWORD *)BM3_DST(VertexElement::PC1));
		Local::PackVector4ToD3DCOLOR(right_bottom, (DWORD *)BM3_DST(VertexElement::PC2));
		Local::PackVector4ToD3DCOLOR(right_top,    (DWORD *)BM3_DST(VertexElement::PC3));
#else
		
		BM3_DST(VertexElement::PC0)->e_.r_ = ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTCOLOR(prim.color_left_top_.value_.e_.r_,		pdi.parent_color_->e_.r_)	* 255.0f);
		BM3_DST(VertexElement::PC0)->e_.g_ = ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTCOLOR(prim.color_left_top_.value_.e_.g_, 	pdi.parent_color_->e_.g_)	* 255.0f);
		BM3_DST(VertexElement::PC0)->e_.b_ = ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTCOLOR(prim.color_left_top_.value_.e_.b_, 	pdi.parent_color_->e_.b_)	* 255.0f);
		BM3_DST(VertexElement::PC0)->e_.a_ = ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTALPHA(prim.color_left_top_.value_.e_.a_, 	pdi.parent_color_->e_.a_)	* 255.0f);
		BM3_DST(VertexElement::PC1)->e_.r_ = ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTCOLOR(prim.color_left_bottom_.value_.e_.r_,	pdi.parent_color_->e_.r_)	* 255.0f);
		BM3_DST(VertexElement::PC1)->e_.g_ = ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTCOLOR(prim.color_left_bottom_.value_.e_.g_,	pdi.parent_color_->e_.g_)	* 255.0f);
		BM3_DST(VertexElement::PC1)->e_.b_ = ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTCOLOR(prim.color_left_bottom_.value_.e_.b_,	pdi.parent_color_->e_.b_)	* 255.0f);
		BM3_DST(VertexElement::PC1)->e_.a_ = ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTALPHA(prim.color_left_bottom_.value_.e_.a_,	pdi.parent_color_->e_.a_)	* 255.0f);
		BM3_DST(VertexElement::PC2)->e_.r_ = ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTCOLOR(prim.color_right_bottom_.value_.e_.r_,	pdi.parent_color_->e_.r_)	* 255.0f);
		BM3_DST(VertexElement::PC2)->e_.g_ = ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTCOLOR(prim.color_right_bottom_.value_.e_.g_,	pdi.parent_color_->e_.g_)	* 255.0f);
		BM3_DST(VertexElement::PC2)->e_.b_ = ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTCOLOR(prim.color_right_bottom_.value_.e_.b_,	pdi.parent_color_->e_.b_)	* 255.0f);
		BM3_DST(VertexElement::PC2)->e_.a_ = ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTALPHA(prim.color_right_bottom_.value_.e_.a_,	pdi.parent_color_->e_.a_)	* 255.0f);
		BM3_DST(VertexElement::PC3)->e_.r_ = ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTCOLOR(prim.color_right_top_.value_.e_.r_,	pdi.parent_color_->e_.r_)	* 255.0f);
		BM3_DST(VertexElement::PC3)->e_.g_ = ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTCOLOR(prim.color_right_top_.value_.e_.g_,	pdi.parent_color_->e_.g_)	* 255.0f);
		BM3_DST(VertexElement::PC3)->e_.b_ = ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTCOLOR(prim.color_right_top_.value_.e_.b_,	pdi.parent_color_->e_.b_)	* 255.0f);
		BM3_DST(VertexElement::PC3)->e_.a_ = ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTALPHA(prim.color_right_top_.value_.e_.a_,	pdi.parent_color_->e_.a_)	* 255.0f);
#endif
	}

	//BM2�݊�
	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void PushBm2(DstType *dst, const PrimitiveType &prim, DrawInfo &/*draw_info*/, const ResourceType &res, const PacketDrawInfo& pdi, int stride = sizeof(DstType)){
		ML_PTR_ASSERT(dst);

#if defined(BM3_PLATFORM_xbox360)
		struct Local{
			static inline void PackVector4ToD3DCOLOR( __vector4 input, DWORD *pDest){
				static const __vector4 vPackMul8888 = {(255.0 / (1 << 22)), (255.0 / (1 << 22)), (255.0 / (1 << 22)), (255.0 / (1 << 22))};
				static const __vector4 vPackAdd = { 3.0, 3.0, 3.0, 3.0 };

				// Assume the four floats in 'input' are in the range
				// 0.0 to 1.0. We need to get them into the range
				// 3.0 + x*(2^-22) where x is a number from 0 to 255
				__vector4 result = __vmaddfp( input, vPackMul8888, vPackAdd );
				result = __vpkd3d( result, result, VPACK_D3DCOLOR, VPACK_32, 0 );

				// Now we need to store the 32-bit result, using __stvewx. We
				// have to splat the result from element 3 to all four words so
				// that the correct value will be stored regardless of alignment.
				result = __vspltw( result, 3 );

				// Store the result. pDest must be DWORD aligned.
				__stvewx( result, pDest, 0 );
			}
		};

		__vector4 left_top		= __loadunalignedvector(prim.color_left_top_.value_.v_);
		__vector4 left_bottom	= __loadunalignedvector(prim.color_left_bottom_.value_.v_);
		__vector4 right_bottom	= __loadunalignedvector(prim.color_right_bottom_.value_.v_);
		__vector4 right_top		= __loadunalignedvector(prim.color_right_top_.value_.v_);

		Local::PackVector4ToD3DCOLOR(left_top,     (DWORD *)BM3_DST(VertexElement::PC0));
		Local::PackVector4ToD3DCOLOR(left_bottom,  (DWORD *)BM3_DST(VertexElement::PC1));
		Local::PackVector4ToD3DCOLOR(right_bottom, (DWORD *)BM3_DST(VertexElement::PC2));
		Local::PackVector4ToD3DCOLOR(right_top,    (DWORD *)BM3_DST(VertexElement::PC3));
#else
		BM3_DST(VertexElement::PC0)->e_.r_ = ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTCOLOR(prim.color_left_top_.value_.e_.r_		* prim.color_left_top_.value_.e_.a_ 	,pdi.parent_color_->e_.r_)	* 255.0f);
		BM3_DST(VertexElement::PC0)->e_.g_ = ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTCOLOR(prim.color_left_top_.value_.e_.g_		* prim.color_left_top_.value_.e_.a_ 	,pdi.parent_color_->e_.g_)	* 255.0f);
		BM3_DST(VertexElement::PC0)->e_.b_ = ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTCOLOR(prim.color_left_top_.value_.e_.b_		* prim.color_left_top_.value_.e_.a_ 	,pdi.parent_color_->e_.b_)	* 255.0f);
		BM3_DST(VertexElement::PC0)->e_.a_ = ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTALPHA(prim.color_left_top_.value_.e_.a_												,pdi.parent_color_->e_.a_)	* 255.0f);
		BM3_DST(VertexElement::PC1)->e_.r_ = ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTCOLOR(prim.color_left_bottom_.value_.e_.r_	* prim.color_left_bottom_.value_.e_.a_	,pdi.parent_color_->e_.r_)	* 255.0f);
		BM3_DST(VertexElement::PC1)->e_.g_ = ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTCOLOR(prim.color_left_bottom_.value_.e_.g_	* prim.color_left_bottom_.value_.e_.a_	,pdi.parent_color_->e_.g_)	* 255.0f);
		BM3_DST(VertexElement::PC1)->e_.b_ = ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTCOLOR(prim.color_left_bottom_.value_.e_.b_	* prim.color_left_bottom_.value_.e_.a_	,pdi.parent_color_->e_.b_)	* 255.0f);
		BM3_DST(VertexElement::PC1)->e_.a_ = ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTALPHA(prim.color_left_bottom_.value_.e_.a_											,pdi.parent_color_->e_.a_)	* 255.0f);
		BM3_DST(VertexElement::PC2)->e_.r_ = ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTCOLOR(prim.color_right_bottom_.value_.e_.r_	* prim.color_right_bottom_.value_.e_.a_	,pdi.parent_color_->e_.r_)	* 255.0f);
		BM3_DST(VertexElement::PC2)->e_.g_ = ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTCOLOR(prim.color_right_bottom_.value_.e_.g_	* prim.color_right_bottom_.value_.e_.a_	,pdi.parent_color_->e_.g_)	* 255.0f);
		BM3_DST(VertexElement::PC2)->e_.b_ = ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTCOLOR(prim.color_right_bottom_.value_.e_.b_	* prim.color_right_bottom_.value_.e_.a_	,pdi.parent_color_->e_.b_)	* 255.0f);
		BM3_DST(VertexElement::PC2)->e_.a_ = ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTALPHA(prim.color_right_bottom_.value_.e_.a_											,pdi.parent_color_->e_.a_)	* 255.0f);
		BM3_DST(VertexElement::PC3)->e_.r_ = ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTCOLOR(prim.color_right_top_.value_.e_.r_		* prim.color_right_top_.value_.e_.a_	,pdi.parent_color_->e_.r_)	* 255.0f);
		BM3_DST(VertexElement::PC3)->e_.g_ = ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTCOLOR(prim.color_right_top_.value_.e_.g_		* prim.color_right_top_.value_.e_.a_	,pdi.parent_color_->e_.g_)	* 255.0f);
		BM3_DST(VertexElement::PC3)->e_.b_ = ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTCOLOR(prim.color_right_top_.value_.e_.b_		* prim.color_right_top_.value_.e_.a_	,pdi.parent_color_->e_.b_)	* 255.0f);
		BM3_DST(VertexElement::PC3)->e_.a_ = ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTALPHA(prim.color_right_top_.value_.e_.a_												,pdi.parent_color_->e_.a_)	* 255.0f);
#endif
	}

	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void Push(DstType *dst, const PrimitiveType &prim, DrawInfo &/*draw_info*/, const ResourceType &res, const ColorType &color_scale, const PacketDrawInfo& pdi, int stride = sizeof(DstType)){
		ML_PTR_ASSERT(dst);

		BM3_DST(VertexElement::PC0)->e_.r_ = ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTCOLOR(prim.color_left_top_.value_.e_.r_		,pdi.parent_color_->e_.r_)	* color_scale.e_.r_);
		BM3_DST(VertexElement::PC0)->e_.g_ = ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTCOLOR(prim.color_left_top_.value_.e_.g_		,pdi.parent_color_->e_.g_)	* color_scale.e_.g_);
		BM3_DST(VertexElement::PC0)->e_.b_ = ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTCOLOR(prim.color_left_top_.value_.e_.b_		,pdi.parent_color_->e_.b_)	* color_scale.e_.b_);
		BM3_DST(VertexElement::PC0)->e_.a_ = ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTALPHA(prim.color_left_top_.value_.e_.a_		,pdi.parent_color_->e_.a_)	* color_scale.e_.a_);
		BM3_DST(VertexElement::PC1)->e_.r_ = ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTCOLOR(prim.color_left_bottom_.value_.e_.r_	,pdi.parent_color_->e_.r_)	* color_scale.e_.r_);
		BM3_DST(VertexElement::PC1)->e_.g_ = ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTCOLOR(prim.color_left_bottom_.value_.e_.g_	,pdi.parent_color_->e_.g_)	* color_scale.e_.g_);
		BM3_DST(VertexElement::PC1)->e_.b_ = ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTCOLOR(prim.color_left_bottom_.value_.e_.b_	,pdi.parent_color_->e_.b_)	* color_scale.e_.b_);
		BM3_DST(VertexElement::PC1)->e_.a_ = ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTALPHA(prim.color_left_bottom_.value_.e_.a_	,pdi.parent_color_->e_.a_)	* color_scale.e_.a_);
		BM3_DST(VertexElement::PC2)->e_.r_ = ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTCOLOR(prim.color_right_bottom_.value_.e_.r_	,pdi.parent_color_->e_.r_)	* color_scale.e_.r_);
		BM3_DST(VertexElement::PC2)->e_.g_ = ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTCOLOR(prim.color_right_bottom_.value_.e_.g_	,pdi.parent_color_->e_.g_)	* color_scale.e_.g_);
		BM3_DST(VertexElement::PC2)->e_.b_ = ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTCOLOR(prim.color_right_bottom_.value_.e_.b_	,pdi.parent_color_->e_.b_)	* color_scale.e_.b_);
		BM3_DST(VertexElement::PC2)->e_.a_ = ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTALPHA(prim.color_right_bottom_.value_.e_.a_	,pdi.parent_color_->e_.a_)	* color_scale.e_.a_);
		BM3_DST(VertexElement::PC3)->e_.r_ = ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTCOLOR(prim.color_right_top_.value_.e_.r_		,pdi.parent_color_->e_.r_)	* color_scale.e_.r_);
		BM3_DST(VertexElement::PC3)->e_.g_ = ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTCOLOR(prim.color_right_top_.value_.e_.g_		,pdi.parent_color_->e_.g_)	* color_scale.e_.g_);
		BM3_DST(VertexElement::PC3)->e_.b_ = ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTCOLOR(prim.color_right_top_.value_.e_.b_		,pdi.parent_color_->e_.b_)	* color_scale.e_.b_);
		BM3_DST(VertexElement::PC3)->e_.a_ = ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTALPHA(prim.color_right_top_.value_.e_.a_		,pdi.parent_color_->e_.a_)	* color_scale.e_.a_);
	}

	//BM2�݊�
	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void PushBm2(DstType *dst, const PrimitiveType &prim, DrawInfo &/*draw_info*/, const ResourceType &res, const ColorType &color_scale, const PacketDrawInfo& pdi, int stride = sizeof(DstType)){
		ML_PTR_ASSERT(dst);

		BM3_DST(VertexElement::PC0)->e_.a_ = ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTALPHA(prim.color_left_top_.value_.e_.a_												,pdi.parent_color_->e_.a_)	* color_scale.e_.a_);
		BM3_DST(VertexElement::PC0)->e_.r_ = ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTCOLOR(prim.color_left_top_.value_.e_.r_		* BM3_DST(VertexElement::PC0)->e_.a_	,pdi.parent_color_->e_.r_)	* color_scale.e_.r_);
		BM3_DST(VertexElement::PC0)->e_.g_ = ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTCOLOR(prim.color_left_top_.value_.e_.g_		* BM3_DST(VertexElement::PC0)->e_.a_	,pdi.parent_color_->e_.g_)	* color_scale.e_.g_);
		BM3_DST(VertexElement::PC0)->e_.b_ = ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTCOLOR(prim.color_left_top_.value_.e_.b_		* BM3_DST(VertexElement::PC0)->e_.a_	,pdi.parent_color_->e_.b_)	* color_scale.e_.b_);
		BM3_DST(VertexElement::PC1)->e_.a_ = ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTALPHA(prim.color_left_bottom_.value_.e_.a_											,pdi.parent_color_->e_.a_)	* color_scale.e_.a_);
		BM3_DST(VertexElement::PC1)->e_.r_ = ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTCOLOR(prim.color_left_bottom_.value_.e_.r_	* BM3_DST(VertexElement::PC1)->e_.a_	,pdi.parent_color_->e_.r_)	* color_scale.e_.r_);
		BM3_DST(VertexElement::PC1)->e_.g_ = ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTCOLOR(prim.color_left_bottom_.value_.e_.g_	* BM3_DST(VertexElement::PC1)->e_.a_	,pdi.parent_color_->e_.g_)	* color_scale.e_.g_);
		BM3_DST(VertexElement::PC1)->e_.b_ = ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTCOLOR(prim.color_left_bottom_.value_.e_.b_	* BM3_DST(VertexElement::PC1)->e_.a_	,pdi.parent_color_->e_.b_)	* color_scale.e_.b_);
		BM3_DST(VertexElement::PC2)->e_.a_ = ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTALPHA(prim.color_right_bottom_.value_.e_.a_											,pdi.parent_color_->e_.a_)	* color_scale.e_.a_);
		BM3_DST(VertexElement::PC2)->e_.r_ = ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTCOLOR(prim.color_right_bottom_.value_.e_.r_	* BM3_DST(VertexElement::PC2)->e_.a_	,pdi.parent_color_->e_.r_)	* color_scale.e_.r_);
		BM3_DST(VertexElement::PC2)->e_.g_ = ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTCOLOR(prim.color_right_bottom_.value_.e_.g_	* BM3_DST(VertexElement::PC2)->e_.a_	,pdi.parent_color_->e_.g_)	* color_scale.e_.g_);
		BM3_DST(VertexElement::PC2)->e_.b_ = ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTCOLOR(prim.color_right_bottom_.value_.e_.b_	* BM3_DST(VertexElement::PC2)->e_.a_	,pdi.parent_color_->e_.b_)	* color_scale.e_.b_);
		BM3_DST(VertexElement::PC3)->e_.a_ = ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTALPHA(prim.color_right_top_.value_.e_.a_												,pdi.parent_color_->e_.a_)	* color_scale.e_.a_);
		BM3_DST(VertexElement::PC3)->e_.r_ = ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTCOLOR(prim.color_right_top_.value_.e_.r_		* BM3_DST(VertexElement::PC3)->e_.a_	,pdi.parent_color_->e_.r_)	* color_scale.e_.r_);
		BM3_DST(VertexElement::PC3)->e_.g_ = ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTCOLOR(prim.color_right_top_.value_.e_.g_		* BM3_DST(VertexElement::PC3)->e_.a_	,pdi.parent_color_->e_.g_)	* color_scale.e_.g_);
		BM3_DST(VertexElement::PC3)->e_.b_ = ml::numeric_cast<ml::ui8>(BM3_BLEND_PARENTCOLOR(prim.color_right_top_.value_.e_.b_		* BM3_DST(VertexElement::PC3)->e_.a_	,pdi.parent_color_->e_.b_)	* color_scale.e_.b_);
	}
};
#undef BM3_BLEND_PARENTALPHA
#undef BM3_BLEND_PARENTCOLOR
#undef BM3_DST

}	// namespace policy
}	// namespace draw
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_COLOR4_H
