#ifndef BM3_SDK_INC_BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_TEXTURE2_H
#define BM3_SDK_INC_BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_TEXTURE2_H

#include "bm3/system/bm3_Config.h"
#include "bm3_draw_policy_Common.h"
#include "../../bm3_DrawInfo.h"
#include "../../bm3_CoreType.h"
#include "../../bm3_Const.h"
#include "../../bm3_CoreUtility.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE
namespace draw{
namespace policy{

template<
	DrawTexture::Type DrawTextureType
>
struct Texture2{
	template<typename PrimitiveType, typename ResourceType>
	BM3_FORCEINLINE void Push(TextureCoordinate *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, TextureLayer::Type lt, float v);

	template<typename PrimitiveType, typename ResourceType>
	BM3_FORCEINLINE void Push(TextureCoordinate *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, TextureLayer::Type lt, float v, int stride);

	template<typename PrimitiveType, typename ResourceType>
	static void StaticPush(TextureCoordinate *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, TextureLayer::Type lt, float v){
		Texture2().Push(dst, prim, draw_info, res, lt, v);
	}

	template<typename PrimitiveType, typename ResourceType>
	static void StaticPush(TextureCoordinate *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, TextureLayer::Type lt, float v, int stride){
		Texture2().Push(dst, prim, draw_info, res, lt, v, stride);
	}
};

#define	BM3_DST(INDEX)	(reinterpret_cast<TextureCoordinate *>(int(dst) + stride * (INDEX)))

template<>
struct Texture2<DrawTexture::Nothing>{

	template<typename PrimitiveType, typename ResourceType>
	BM3_FORCEINLINE void Push(TextureCoordinate * /*dst*/, const PrimitiveType &/*prim*/, DrawInfo &/*draw_info*/, const ResourceType &/*res*/, TextureLayer::Type /*lt*/, float /*v*/, int /*stride*/){

		// ‰½‚à‚µ‚È‚¢
	}

	template<typename PrimitiveType, typename ResourceType>
	static void StaticPush(TextureCoordinate *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, TextureLayer::Type lt, float v){
		Texture2().Push(dst, prim, draw_info, res, lt, v);
	}

	template<typename PrimitiveType, typename ResourceType>
	static void StaticPush(TextureCoordinate *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, TextureLayer::Type lt, float v, int stride){
		Texture2().Push(dst, prim, draw_info, res, lt, v, stride);
	}
};

template<>
struct Texture2<DrawTexture::Normal>{

	template<typename PrimitiveType, typename ResourceType>
	BM3_FORCEINLINE void Push(TextureCoordinate *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, TextureLayer::Type lt, float v, int stride){
		ML_PTR_ASSERT(dst);

		BM3_DST(0)->u_	= 0.0f;
		BM3_DST(0)->v_	= 0.0f + v;
		BM3_DST(1)->u_	= 1.0f;
		BM3_DST(1)->v_	= 0.0f + v;
	}

	template<typename PrimitiveType, typename ResourceType>
	static void StaticPush(TextureCoordinate *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, TextureLayer::Type lt, float v){
		Texture2().Push(dst, prim, draw_info, res, lt, v);
	}

	template<typename PrimitiveType, typename ResourceType>
	static void StaticPush(TextureCoordinate *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, TextureLayer::Type lt, float v, int stride){
		Texture2().Push(dst, prim, draw_info, res, lt, v, stride);
	}
};

template<>
struct Texture2<DrawTexture::Crop>{

	template<typename PrimitiveType, typename ResourceType>
	BM3_FORCEINLINE void Push(TextureCoordinate *dst, const PrimitiveType &prim, DrawInfo & /*draw_info*/, const ResourceType &res, TextureLayer::Type lt, float v, int stride){

		ML_PTR_ASSERT(dst);

//		const AnimCrop	*ac			= anim_crop_array[Texture_Layer_TextureCropAnimationType(res, lt)];
		const AnimCrop	*ac			= Texture_Layer_AnimCrop(Texture_Layer_TextureCropAnimationType(res, lt));
		int				anim_frame	= prim.texture_[lt].frame_ & (ac->count_ - 1);

		ML_ASSERT(ml::is_pow2(ac->count_));

		float ou = ac->origin_[anim_frame].u_;
		float ov = ac->origin_[anim_frame].v_;
		float w  = ac->size_.u_;
		float h  = ac->size_.v_;
		
		BM3_DST(0)->u_	= ou;
		BM3_DST(0)->v_	= SET_V(ov + v * h);
		BM3_DST(1)->u_	= ou + w;
		BM3_DST(1)->v_	= SET_V(ov + v * h);
	}

	template<typename PrimitiveType, typename ResourceType>
	static void StaticPush(TextureCoordinate *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, TextureLayer::Type lt, float v){
		Texture2().Push(dst, prim, draw_info, res, lt, v);
	}

	template<typename PrimitiveType, typename ResourceType>
	static void StaticPush(TextureCoordinate *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, TextureLayer::Type lt, float v, int stride){
		Texture2().Push(dst, prim, draw_info, res, lt, v, stride);
	}

	static void StaticPush(TextureCoordinate *dst, DrawInfo & /*draw_info*/, float v, int stride){
		BM3_DST(0)->u_	= 0.0f;
		BM3_DST(0)->v_	= 0.0f + v;
		BM3_DST(1)->u_	= 1.0f;
		BM3_DST(1)->v_	= 0.0f + v;
	}

};

template<>
struct Texture2<DrawTexture::Scroll>{

	template<typename PrimitiveType, typename ResourceType>
	BM3_FORCEINLINE void Push(TextureCoordinate *dst, const PrimitiveType &prim, DrawInfo & /*draw_info*/, const ResourceType &res, TextureLayer::Type lt, float v, int stride){

		ML_PTR_ASSERT(dst);

		ML_STATIC_ASSERT(TextureCropAnimationTypeConst__1x1 == 0);
		ML_STATIC_ASSERT(TextureCropAnimationTypeConst__2x1 == 1);
		ML_STATIC_ASSERT(TextureCropAnimationTypeConst__4x1 == 2);
		ML_STATIC_ASSERT(TextureCropAnimationTypeConst__2x2 == 3);
		ML_STATIC_ASSERT(TextureCropAnimationTypeConst__4x4 == 4);
		ML_STATIC_ASSERT(TextureCropAnimationTypeConst__8x8 == 5);
		ML_STATIC_ASSERT(TextureCropAnimationTypeConst_MAX == 6);

		static const TextureCoordinate	size[TextureCropAnimationTypeConst_MAX] = {
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

		BM3_DST(0)->u_	= h_scroll;
		BM3_DST(0)->v_	= SET_V(v_scroll + v);
		BM3_DST(1)->u_	= h_scroll + w;
		BM3_DST(1)->v_	= SET_V(v_scroll + v);
	}

	template<typename PrimitiveType, typename ResourceType>
	static void StaticPush(TextureCoordinate *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, TextureLayer::Type lt, float v){
		Texture2().Push(dst, prim, draw_info, res, lt, v);
	}

	template<typename PrimitiveType, typename ResourceType>
	static void StaticPush(TextureCoordinate *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, TextureLayer::Type lt, float v, int stride){
		Texture2().Push(dst, prim, draw_info, res, lt, v, stride);
	}
};


template<>
struct Texture2<DrawTexture::Pattern>{

	template<typename PrimitiveType, typename ResourceType>
	BM3_FORCEINLINE void Push(TextureCoordinate *dst, const PrimitiveType &prim, DrawInfo & /*draw_info*/, const ResourceType &res, TextureLayer::Type lt, float v, int stride){
		ML_PTR_ASSERT(dst);
		const TexturePattern &tp = Texture_Layer_Pattern(res, lt).Pattern(prim.texture_[lt].frame_);

		BM3_DST(0)->u_	= tp.crop_.left_;
		BM3_DST(0)->v_	= SET_V(tp.crop_.top_  + v * tp.crop_.height_);
		BM3_DST(1)->u_	= tp.crop_.left_ + tp.crop_.width_;
		BM3_DST(1)->v_	= SET_V(tp.crop_.top_  + v * tp.crop_.height_);
	}

	template<typename PrimitiveType, typename ResourceType>
	static void StaticPush(TextureCoordinate *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, TextureLayer::Type lt, float v){
		Texture2().Push(dst, prim, draw_info, res, lt, v);
	}

	template<typename PrimitiveType, typename ResourceType>
	static void StaticPush(TextureCoordinate *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, TextureLayer::Type lt, float v, int stride){
		Texture2().Push(dst, prim, draw_info, res, lt, v, stride);
	}
};

template<>
struct Texture2<DrawTexture::Transform>{

	template<typename PrimitiveType, typename ResourceType>
	BM3_FORCEINLINE void Push(TextureCoordinate *dst, const PrimitiveType & /*prim*/, DrawInfo & /*draw_info*/, const ResourceType & /*res*/, TextureLayer::Type /*lt*/, float v, int stride){
		ML_PTR_ASSERT(dst);

		// –¢ŽÀ‘•

		BM3_DST(0)->u_	= 0.0f;
		BM3_DST(0)->v_	= 0.0f + v;
		BM3_DST(1)->u_	= 1.0f;
		BM3_DST(1)->v_	= 0.0f + v;
	}

	template<typename PrimitiveType, typename ResourceType>
	static void StaticPush(TextureCoordinate *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, TextureLayer::Type lt, float v){
		Texture2().Push(dst, prim, draw_info, res, lt, v);
	}

	template<typename PrimitiveType, typename ResourceType>
	static void StaticPush(TextureCoordinate *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, TextureLayer::Type lt, float v, int stride){
		Texture2().Push(dst, prim, draw_info, res, lt, v, stride);
	}
};

#undef BM3_DST

}	// namespace policy
}	// namespace draw
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_Texture2_H
