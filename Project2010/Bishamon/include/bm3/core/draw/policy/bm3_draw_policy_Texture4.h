#ifndef BM3_SDK_INC_BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_TEXTURE4_H
#define BM3_SDK_INC_BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_TEXTURE4_H

#include "bm3/system/bm3_Config.h"
#include "bm3_draw_policy_Common.h"
#include "../../bm3_DrawInfo.h"
#include "../../bm3_CoreType.h"
#include "../../bm3_Const.h"
#include "../bm3_draw_Common.h"
#include "../../bm3_CoreUtility.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

namespace draw{
namespace policy{

#define	BM3_DST(INDEX)	(reinterpret_cast<DstType *>(int(dst) + stride * (INDEX)))

struct Texture4_V_TYPE_UP{};
struct Texture4_V_TYPE_DOWN{};

#if defined(BM3_PLATFORM_win_opengl)
	typedef Texture4_V_TYPE_UP		V_TYPE;
#endif
#if defined(BM3_PLATFORM_win_directx9)
	typedef Texture4_V_TYPE_UP		V_TYPE;
#endif
#if defined(BM3_PLATFORM_playstation3)
	typedef Texture4_V_TYPE_DOWN	V_TYPE;
#endif
#if defined(BM3_PLATFORM_xbox360)
	typedef Texture4_V_TYPE_UP		V_TYPE;
#endif
#if defined(BM3_PLATFORM_psp)
	typedef Texture4_V_TYPE_UP		V_TYPE;
#endif
#if defined(BM3_PLATFORM_gamebryodx9)
	typedef Texture4_V_TYPE_UP		V_TYPE;
#endif
#if defined(BM3_PLATFORM_opengles)
	typedef Texture4_V_TYPE_UP		V_TYPE;
#endif
#if defined(BM3_PLATFORM_nw4c)
	typedef Texture4_V_TYPE_UP		V_TYPE;
#endif
#if defined(BM3_PLATFORM_psvita)
	typedef Texture4_V_TYPE_UP		V_TYPE;
#endif

template<
	DrawTexture::Type DrawTextureType
>
struct Texture4{
	template<typename PrimitiveType, typename ResourceType, typename DstType, typename V_TYPE>
	BM3_FORCEINLINE void Push(DstType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, TextureLayer::Type lt, V_TYPE vt, int stride);

	template<typename PrimitiveType, typename ResourceType, typename DstType, typename V_TYPE>
	static BM3_FORCEINLINE void StaticPush(DstType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, TextureLayer::Type lt, V_TYPE vt, int stride){

		Texture4().Push(dst, prim, draw_info, res, lt, vt, stride);
	}
};

template<>
struct Texture4<DrawTexture::Nothing>{

	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void Push(DstType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, TextureLayer::Type lt, Texture4_V_TYPE_UP, int stride){

		Push<0, 1, 2, 3, PrimitiveType>(dst, prim, draw_info, res, lt, stride);
	}

	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void Push(DstType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, TextureLayer::Type lt, Texture4_V_TYPE_DOWN, int stride){

		Push<1, 0, 3, 2, PrimitiveType>(dst, prim, draw_info, res, lt, stride);
	}

	template<typename PrimitiveType, typename ResourceType, typename DstType>
	static BM3_FORCEINLINE void StaticPush(DstType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, TextureLayer::Type lt, Texture4_V_TYPE_UP vt, int stride){

		Texture4().Push(dst, prim, draw_info, res, lt, vt, stride);
	}

	template<typename PrimitiveType, typename ResourceType, typename DstType>
	static BM3_FORCEINLINE void StaticPush(DstType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, TextureLayer::Type lt, Texture4_V_TYPE_DOWN vt, int stride){

		Texture4().Push(dst, prim, draw_info, res, lt, vt, stride);
	}

private:
	template<int v_index0, int v_index1, int v_index2, int v_index3, typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void Push(DstType * /*dst*/, const PrimitiveType &/*prim*/, DrawInfo &/*draw_info*/, const ResourceType &/*res*/, TextureLayer::Type /*lt*/, int /*stride*/){

		// ‰½‚à‚µ‚È‚¢
	}
};

template<>
struct Texture4<DrawTexture::Normal>{

	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void Push(DstType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, TextureLayer::Type lt, Texture4_V_TYPE_UP, int stride){

		Push<0, 1, 2, 3, PrimitiveType>(dst, prim, draw_info, res, lt, stride);
	}

	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void Push(DstType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, TextureLayer::Type lt, Texture4_V_TYPE_DOWN, int stride){

		Push<1, 0, 3, 2, PrimitiveType>(dst, prim, draw_info, res, lt, stride);
	}

	template<typename PrimitiveType, typename ResourceType, typename DstType>
	static BM3_FORCEINLINE void StaticPush(DstType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, TextureLayer::Type lt, Texture4_V_TYPE_UP vt, int stride){

		Texture4().Push(dst, prim, draw_info, res, lt, vt, stride);
	}

	template<typename PrimitiveType, typename ResourceType, typename DstType>
	static BM3_FORCEINLINE void StaticPush(DstType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, TextureLayer::Type lt, Texture4_V_TYPE_DOWN vt, int stride){

		Texture4().Push(dst, prim, draw_info, res, lt, vt, stride);
	}

private:
	template<int v_index0, int v_index1, int v_index2, int v_index3, typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void Push(DstType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, TextureLayer::Type lt, int stride){
		ML_PTR_ASSERT(dst);

		BM3_DST(VertexElement::T0)->u_	= 0.0f;
		BM3_DST(VertexElement::T0)->v_	= 0.0f;
		BM3_DST(VertexElement::T1)->u_	= 0.0f;
		BM3_DST(VertexElement::T1)->v_	= 1.0f;
		BM3_DST(VertexElement::T2)->u_	= 1.0f;
		BM3_DST(VertexElement::T2)->v_	= 1.0f;
		BM3_DST(VertexElement::T3)->u_	= 1.0f;
		BM3_DST(VertexElement::T3)->v_	= 0.0f;
	}
};

template<>
struct Texture4<DrawTexture::Crop>{

	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void Push(DstType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, TextureLayer::Type lt, Texture4_V_TYPE_UP, int stride){

		Push<0, 1, 2, 3, PrimitiveType>(dst, prim, draw_info, res, lt, stride);
	}

	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void Push(DstType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, TextureLayer::Type lt, Texture4_V_TYPE_DOWN, int stride){

		Push<1, 0, 3, 2, PrimitiveType>(dst, prim, draw_info, res, lt, stride);
	}

	template<typename PrimitiveType, typename ResourceType, typename DstType>
	static BM3_FORCEINLINE void StaticPush(DstType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, TextureLayer::Type lt, Texture4_V_TYPE_UP vt, int stride){

		Texture4().Push(dst, prim, draw_info, res, lt, vt, stride);
	}

	template<typename PrimitiveType, typename ResourceType, typename DstType>
	static BM3_FORCEINLINE void StaticPush(DstType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, TextureLayer::Type lt, Texture4_V_TYPE_DOWN vt, int stride){

		Texture4().Push(dst, prim, draw_info, res, lt, vt, stride);
	}

	template<typename DstType>
	static BM3_FORCEINLINE void StaticPush(DstType *dst, DrawInfo & /*draw_info*/, int stride){

		BM3_DST(VertexElement::T0)->u_	= 0.0f;
		BM3_DST(VertexElement::T0)->v_	= 0.0f;
		BM3_DST(VertexElement::T1)->u_	= 0.0f;
		BM3_DST(VertexElement::T1)->v_	= 1.0f;
		BM3_DST(VertexElement::T2)->u_	= 1.0f;
		BM3_DST(VertexElement::T2)->v_	= 1.0f;
		BM3_DST(VertexElement::T3)->u_	= 1.0f;
		BM3_DST(VertexElement::T3)->v_	= 0.0f;
	}

private:
	template<int v_index0, int v_index1, int v_index2, int v_index3, typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void Push(DstType *dst, const PrimitiveType &prim, DrawInfo &/*draw_info*/, const ResourceType &res, TextureLayer::Type lt, int stride){
		ML_PTR_ASSERT(dst);

//		const AnimCrop	*ac			= anim_crop_array[Texture_Layer_TextureCropAnimationType(res, lt)];
		const AnimCrop	*ac			= Texture_Layer_AnimCrop(Texture_Layer_TextureCropAnimationType(res, lt));
		int				anim_frame	= prim.texture_[lt].frame_ & (ac->count_ - 1);

		ML_ASSERT(ml::is_pow2(ac->count_));

		float ou = ac->origin_[anim_frame].u_;
		float ov = ac->origin_[anim_frame].v_;
		float w  = ac->size_.u_;
		float h  = ac->size_.v_;

		BM3_DST(VertexElement::T0)->u_	= ou;
		BM3_DST(VertexElement::T0)->v_	= SET_V(ov);
		BM3_DST(VertexElement::T1)->u_	= ou;
		BM3_DST(VertexElement::T1)->v_	= SET_V(ov + h);
		BM3_DST(VertexElement::T2)->u_	= ou + w;
		BM3_DST(VertexElement::T2)->v_	= SET_V(ov + h);
		BM3_DST(VertexElement::T3)->u_	= ou + w;
		BM3_DST(VertexElement::T3)->v_	= SET_V(ov);
	}
};

template<>
struct Texture4<DrawTexture::Scroll>{

	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void Push(DstType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, TextureLayer::Type lt, Texture4_V_TYPE_UP, int stride){

		Push<0, 1, 2, 3, PrimitiveType>(dst, prim, draw_info, res, lt, stride);
	}

	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void Push(DstType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, TextureLayer::Type lt, Texture4_V_TYPE_DOWN, int stride){

		Push<1, 0, 3, 2, PrimitiveType>(dst, prim, draw_info, res, lt, stride);
	}

	template<typename PrimitiveType, typename ResourceType, typename DstType>
	static BM3_FORCEINLINE void StaticPush(DstType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, TextureLayer::Type lt, Texture4_V_TYPE_UP vt, int stride){

		Texture4().Push(dst, prim, draw_info, res, lt, vt, stride);
	}

	template<typename PrimitiveType, typename ResourceType, typename DstType>
	static BM3_FORCEINLINE void StaticPush(DstType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, TextureLayer::Type lt, Texture4_V_TYPE_DOWN vt, int stride){

		Texture4().Push(dst, prim, draw_info, res, lt, vt, stride);
	}

private:
	template<int v_index0, int v_index1, int v_index2, int v_index3, typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void Push(DstType *dst, const PrimitiveType &prim, DrawInfo &/*draw_info*/, const ResourceType &res, TextureLayer::Type lt, int stride){
		ML_PTR_ASSERT(dst);

		ML_STATIC_ASSERT(TextureCropAnimationTypeConst__1x1 == 0);
		ML_STATIC_ASSERT(TextureCropAnimationTypeConst__2x1 == 1);
		ML_STATIC_ASSERT(TextureCropAnimationTypeConst__4x1 == 2);
		ML_STATIC_ASSERT(TextureCropAnimationTypeConst__2x2 == 3);
		ML_STATIC_ASSERT(TextureCropAnimationTypeConst__4x4 == 4);
		ML_STATIC_ASSERT(TextureCropAnimationTypeConst__8x8 == 5);
		ML_STATIC_ASSERT(TextureCropAnimationTypeConst_MAX == 6);

		static const DstType	size[TextureCropAnimationTypeConst_MAX] = {
			{1.00f, 1.00f},
			{0.50f, 1.00f},
			{0.25f, 1.00f},
			{0.50f, 0.50f},
			{0.25f, 0.25f},
			{0.125f, 0.125f}
		};

		float h_scroll = prim.texture_[lt].h_scroll_;
		float v_scroll = prim.texture_[lt].v_scroll_;
		float w = size[Texture_Layer_TextureCropAnimationType(res, lt)].u_;
		float h = size[Texture_Layer_TextureCropAnimationType(res, lt)].v_;

		if(Texture_Layer_Pattern(res, lt).PatternCount() > 0)
		{
			const TexturePattern &tp = Texture_Layer_Pattern(res, lt).Pattern(0);

			BM3_DST(VertexElement::T0)->u_	= h_scroll;
			BM3_DST(VertexElement::T0)->v_	= SET_V(v_scroll);
			BM3_DST(VertexElement::T1)->u_	= h_scroll;
			BM3_DST(VertexElement::T1)->v_	= SET_V(v_scroll + tp.crop_.height_);
			BM3_DST(VertexElement::T2)->u_	= h_scroll + tp.crop_.width_;
			BM3_DST(VertexElement::T2)->v_	= SET_V(v_scroll + tp.crop_.height_);
			BM3_DST(VertexElement::T3)->u_	= h_scroll + tp.crop_.width_;
			BM3_DST(VertexElement::T3)->v_	= SET_V(v_scroll);
		}
		else
		{
			BM3_DST(VertexElement::T0)->u_	= h_scroll;
			BM3_DST(VertexElement::T0)->v_	= SET_V(v_scroll);
			BM3_DST(VertexElement::T1)->u_	= h_scroll;
			BM3_DST(VertexElement::T1)->v_	= SET_V(v_scroll + h);
			BM3_DST(VertexElement::T2)->u_	= h_scroll + w;
			BM3_DST(VertexElement::T2)->v_	= SET_V(v_scroll + h);
			BM3_DST(VertexElement::T3)->u_	= h_scroll + w;
			BM3_DST(VertexElement::T3)->v_	= SET_V(v_scroll);
		}
	}
};

template<>
struct Texture4<DrawTexture::Pattern>{

	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void Push(DstType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, TextureLayer::Type lt, Texture4_V_TYPE_UP, int stride){

		Push<0, 1, 2, 3, PrimitiveType>(dst, prim, draw_info, res, lt, stride);
	}

	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void Push(DstType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, TextureLayer::Type lt, Texture4_V_TYPE_DOWN, int stride){

		Push<1, 0, 3, 2, PrimitiveType>(dst, prim, draw_info, res, lt, stride);
	}

	template<typename PrimitiveType, typename ResourceType, typename DstType>
	static BM3_FORCEINLINE void StaticPush(DstType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, TextureLayer::Type lt, Texture4_V_TYPE_UP vt, int stride){

		Texture4().Push(dst, prim, draw_info, res, lt, vt, stride);
	}

	template<typename PrimitiveType, typename ResourceType, typename DstType>
	static BM3_FORCEINLINE void StaticPush(DstType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, TextureLayer::Type lt, Texture4_V_TYPE_DOWN vt, int stride){

		Texture4().Push(dst, prim, draw_info, res, lt, vt, stride);
	}

private:
	template<int v_index0, int v_index1, int v_index2, int v_index3, typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void Push(DstType *dst, const PrimitiveType &prim, DrawInfo &/*draw_info*/, const ResourceType &res, TextureLayer::Type lt, int stride){
		ML_PTR_ASSERT(dst);

		if(Texture_Layer_Pattern(res, lt).PatternCount() > 0){
			const TexturePattern &tp = Texture_Layer_Pattern(res, lt).Pattern(prim.texture_[lt].frame_);

			BM3_DST(VertexElement::T0)->u_	= tp.crop_.left_;
			BM3_DST(VertexElement::T0)->v_	= SET_V(tp.crop_.top_);
			BM3_DST(VertexElement::T1)->u_	= tp.crop_.left_;
			BM3_DST(VertexElement::T1)->v_	= SET_V(tp.crop_.top_  + tp.crop_.height_);
			BM3_DST(VertexElement::T2)->u_	= tp.crop_.left_ + tp.crop_.width_;
			BM3_DST(VertexElement::T2)->v_	= SET_V(tp.crop_.top_  + tp.crop_.height_);
			BM3_DST(VertexElement::T3)->u_	= tp.crop_.left_ + tp.crop_.width_;
			BM3_DST(VertexElement::T3)->v_	= SET_V(tp.crop_.top_);
		}
	}
};

template<>
struct Texture4<DrawTexture::Transform>{

	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void Push(DstType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, TextureLayer::Type lt, Texture4_V_TYPE_UP, int stride){

		Push<0, 1, 2, 3, PrimitiveType>(dst, prim, draw_info, res, lt, stride);
	}

	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void Push(DstType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, TextureLayer::Type lt, Texture4_V_TYPE_DOWN, int stride){

		Push<1, 0, 3, 2, PrimitiveType>(dst, prim, draw_info, res, lt, stride);
	}

	template<typename PrimitiveType, typename ResourceType, typename DstType>
	static BM3_FORCEINLINE void StaticPush(DstType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, TextureLayer::Type lt, Texture4_V_TYPE_UP vt, int stride){

		Texture4().Push(dst, prim, draw_info, res, lt, vt, stride);
	}

	template<typename PrimitiveType, typename ResourceType, typename DstType>
	static BM3_FORCEINLINE void StaticPush(DstType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, TextureLayer::Type lt, Texture4_V_TYPE_DOWN vt, int stride){

		Texture4().Push(dst, prim, draw_info, res, lt, vt, stride);
	}

private:
	template<int v_index0, int v_index1, int v_index2, int v_index3, typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void Push(DstType *dst, const PrimitiveType &/*prim*/, DrawInfo &/*draw_info*/, const ResourceType &/*res*/, TextureLayer::Type /*lt*/, int stride){
		ML_PTR_ASSERT(dst);

		// –¢ŽÀ‘•
		;

		BM3_DST(VertexElement::T0)->u_	= 0.0f;
		BM3_DST(VertexElement::T0)->v_	= 0.0f;
		BM3_DST(VertexElement::T1)->u_	= 0.0f;
		BM3_DST(VertexElement::T1)->v_	= 1.0f;
		BM3_DST(VertexElement::T2)->u_	= 1.0f;
		BM3_DST(VertexElement::T2)->v_	= 1.0f;
		BM3_DST(VertexElement::T3)->u_	= 1.0f;
		BM3_DST(VertexElement::T3)->v_	= 0.0f;
	}
};

#undef BM3_DST

}	// namespace policy
}	// namespace draw
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_Texture4_H
