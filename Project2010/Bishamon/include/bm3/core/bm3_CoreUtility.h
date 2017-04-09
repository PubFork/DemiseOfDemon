#ifndef BM3_SDK_INC_BM3_CORE_BM3_COREUTILITY_h
#define BM3_SDK_INC_BM3_CORE_BM3_COREUTILITY_h

#include "../system/bm3_Config.h"
#include "bm3_CoreType.h"
#include "bm3_CoreTypeSwitch.h"
#include "../utility/bm3_String.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

inline int TextureReferenceCount(TextureDrawTypeConst t){

	ML_ASSERT(
		(t == TextureDrawTypeConst_Basic)   ||		// 基本
		(t == TextureDrawTypeConst_Alpha)   ||		// アルファ
		(t == TextureDrawTypeConst_Add)     ||		// 加算
		(t == TextureDrawTypeConst_Sub)     ||		// 減算
		(t == TextureDrawTypeConst_Reverse) ||		// 反転
		(t == TextureDrawTypeConst_Normalmapping)	// 法線マッピング
	);

	ML_STATIC_ASSERT(TextureDrawTypeConst_Basic			== 0);
	ML_STATIC_ASSERT(TextureDrawTypeConst_Alpha			== 1);
	ML_STATIC_ASSERT(TextureDrawTypeConst_Add			== 2);
	ML_STATIC_ASSERT(TextureDrawTypeConst_Sub			== 3);
	ML_STATIC_ASSERT(TextureDrawTypeConst_Reverse		== 4);
	ML_STATIC_ASSERT(TextureDrawTypeConst_Normalmapping	== 5);
	ML_STATIC_ASSERT(TextureDrawTypeConst_MAX			== 6);

	static const int count[TextureDrawTypeConst_MAX] = {
		1,	// TextureDrawTypeConst_Basic
		2,	// TextureDrawTypeConst_Alpha
		2,	// TextureDrawTypeConst_Add
		2,	// TextureDrawTypeConst_Sub
		2,	// TextureDrawTypeConst_Reverse
		2	// TextureDrawTypeConst_Normalmapping
	};

	return count[t];
}

template<typename ResourceType>
inline int GetTextureReferenceCount(const ResourceType &res, TextureDrawTypeConst t) {
	
	if( ( res.Texture_Layer1_PictureName().IsEmpty() ) && ( res.Texture_Layer2_PictureName().IsEmpty() ) ) {
#if defined(BM3_PLATFORM_nw4c)
		
		//j ダミーデータが仕込んであるので、基本描画となる。
		t = TextureDrawTypeConst_Basic;		
#else
		return 0;
#endif
	}
	
	//j 二枚目のレイヤーにテクスチャが無い場合でもtの値がTextureDrawTypeConst_Basic以外に設定されてしまうので、無い場合は基本描画に切り替える。
	if (res.Texture_Layer2_PictureName().IsEmpty()) {
		t = TextureDrawTypeConst_Basic;
	}
	
	return TextureReferenceCount(t);
}
	
ML_STATIC_ASSERT(TextureLayer::Max == 2);

template<typename ResourceType>
inline const StringType & Texture_Layer_PictureName(const ResourceType &res, TextureLayer::Type tl){

	return
		(tl == TextureLayer::_1) ?
			res.Texture_Layer1_PictureName() :
			res.Texture_Layer2_PictureName();
}

template<typename ResourceType>
inline TextureAddressTypeConst Texture_Layer_TextureAddressType(const ResourceType &res, TextureLayer::Type tl){

	return
		(tl == TextureLayer::_1) ?
			res.Texture_Layer1_TextureAddressType() :
			res.Texture_Layer2_TextureAddressType();
}

template<typename ResourceType>
inline TextureFilterTypeConst Texture_Layer_TextureFilterType(const ResourceType &res, TextureLayer::Type tl){

	return
		(tl == TextureLayer::_1) ?
			res.Texture_Layer1_TextureFilterType() :
			res.Texture_Layer2_TextureFilterType();
}

template<typename ResourceType>
inline TextureAnimationTypeConst Texture_Layer_TextureAnimationType(const ResourceType &res, TextureLayer::Type tl){

	return
		(tl == TextureLayer::_1) ?
			res.Texture_Layer1_TextureAnimationType() :
			res.Texture_Layer2_TextureAnimationType();
}

template<typename ResourceType>
inline int Texture_Layer_AnimationSpeed(const ResourceType &res, TextureLayer::Type tl){

	return
		(tl == TextureLayer::_1) ?
			res.Texture_Layer1_AnimationSpeed() :
			res.Texture_Layer2_AnimationSpeed();
}

template<typename ResourceType>
inline TextureCropAnimationTypeConst Texture_Layer_TextureCropAnimationType(const ResourceType &res, TextureLayer::Type tl){

	return
		(tl == TextureLayer::_1) ?
			res.Texture_Layer1_TextureCropAnimationType() :
			res.Texture_Layer2_TextureCropAnimationType();
}
#ifdef BM3_TARGET_IDE
template<typename ResourceType>
inline int Texture_Layer_TextureCropAnimationCount(const ResourceType &res, TextureLayer::Type tl){

	switch(
		(tl == TextureLayer::_1) ?
			res.Texture_Layer1_TextureCropAnimationType() :
			res.Texture_Layer2_TextureCropAnimationType()){
				case TextureCropAnimationTypeConst__1x1: return 1;
				case TextureCropAnimationTypeConst__2x1: return 2;
				case TextureCropAnimationTypeConst__2x2: return 4;
				case TextureCropAnimationTypeConst__4x1: return 4;
				case TextureCropAnimationTypeConst__4x4: return 16;
				case TextureCropAnimationTypeConst__8x8: return 64;
				default: ML_ASSERT(false);	return 1;
	}
}
#else
template<typename ResourceType>
inline int Texture_Layer_TextureCropAnimationCount(const ResourceType &res, TextureLayer::Type tl){

	return(
		(tl == TextureLayer::_1) ?
			res.Texture_Layer1_CropCount() :
			res.Texture_Layer2_CropCount());
}
#endif


template<typename ResourceType>
inline float Texture_Layer_HScrollSpeed(const ResourceType &res, TextureLayer::Type tl){

	return
		(tl == TextureLayer::_1) ?
			res.Texture_Layer1_HScrollSpeed() :
			res.Texture_Layer2_HScrollSpeed();
}

template<typename ResourceType>
inline float Texture_Layer_VScrollSpeed(const ResourceType &res, TextureLayer::Type tl){

	return
		(tl == TextureLayer::_1) ?
			res.Texture_Layer1_VScrollSpeed() :
			res.Texture_Layer2_VScrollSpeed();
}

template<typename ResourceType>
inline const TextureType &Texture_Layer_Pattern(const ResourceType &res, TextureLayer::Type tl){

	return
		(tl == TextureLayer::_1) ?
			res.Texture_Layer1_Pattern() :
			res.Texture_Layer2_Pattern();
}

template<typename ResourceType>
inline bool Texture_Layer_CropRandom(const ResourceType &res, TextureLayer::Type tl){

	return
		(tl == TextureLayer::_1) ?
			res.Texture_Layer1_CropRandom() :
			res.Texture_Layer2_CropRandom();
}

struct AnimCrop{
	int							count_;
	TextureCoordinate			size_;
	const TextureCoordinate *	origin_;
};

inline const AnimCrop *Texture_Layer_AnimCrop(TextureCropAnimationTypeConst type){

	static const TextureCoordinate	origin[]	= {
		// 1x1
		{0.00f*0.0f, 0.00f*0.0f},
		// 2x1
		{0.50f*0.0f, 0.00f*0.0f},
		{0.50f*1.0f, 0.00f*0.0f},
		// 4x1
		{0.25f*0.0f, 0.00f*0.0f},
		{0.25f*1.0f, 0.00f*0.0f},
		{0.25f*2.0f, 0.00f*0.0f},
		{0.25f*3.0f, 0.00f*0.0f},
		// 2x2
		{0.50f*0.0f, 0.50f*0.0f},
		{0.50f*1.0f, 0.50f*0.0f},
		{0.50f*0.0f, 0.50f*1.0f},
		{0.50f*1.0f, 0.50f*1.0f},
		// 4x4
		{0.25f*0.0f, 0.25f*0.0f},
		{0.25f*1.0f, 0.25f*0.0f},
		{0.25f*2.0f, 0.25f*0.0f},
		{0.25f*3.0f, 0.25f*0.0f},

		{0.25f*0.0f, 0.25f*1.0f},
		{0.25f*1.0f, 0.25f*1.0f},
		{0.25f*2.0f, 0.25f*1.0f},
		{0.25f*3.0f, 0.25f*1.0f},

		{0.25f*0.0f, 0.25f*2.0f},
		{0.25f*1.0f, 0.25f*2.0f},
		{0.25f*2.0f, 0.25f*2.0f},
		{0.25f*3.0f, 0.25f*2.0f},

		{0.25f*0.0f, 0.25f*3.0f},
		{0.25f*1.0f, 0.25f*3.0f},
		{0.25f*2.0f, 0.25f*3.0f},
		{0.25f*3.0f, 0.25f*3.0f},

		// 8x8
		{0.125f*0.0f, 0.125f*0.0f},
		{0.125f*1.0f, 0.125f*0.0f},
		{0.125f*2.0f, 0.125f*0.0f},
		{0.125f*3.0f, 0.125f*0.0f},
		{0.125f*4.0f, 0.125f*0.0f},
		{0.125f*5.0f, 0.125f*0.0f},
		{0.125f*6.0f, 0.125f*0.0f},
		{0.125f*7.0f, 0.125f*0.0f},

		{0.125f*0.0f, 0.125f*1.0f},
		{0.125f*1.0f, 0.125f*1.0f},
		{0.125f*2.0f, 0.125f*1.0f},
		{0.125f*3.0f, 0.125f*1.0f},
		{0.125f*4.0f, 0.125f*1.0f},
		{0.125f*5.0f, 0.125f*1.0f},
		{0.125f*6.0f, 0.125f*1.0f},
		{0.125f*7.0f, 0.125f*1.0f},

		{0.125f*0.0f, 0.125f*2.0f},
		{0.125f*1.0f, 0.125f*2.0f},
		{0.125f*2.0f, 0.125f*2.0f},
		{0.125f*3.0f, 0.125f*2.0f},
		{0.125f*4.0f, 0.125f*2.0f},
		{0.125f*5.0f, 0.125f*2.0f},
		{0.125f*6.0f, 0.125f*2.0f},
		{0.125f*7.0f, 0.125f*2.0f},

		{0.125f*0.0f, 0.125f*3.0f},
		{0.125f*1.0f, 0.125f*3.0f},
		{0.125f*2.0f, 0.125f*3.0f},
		{0.125f*3.0f, 0.125f*3.0f},
		{0.125f*4.0f, 0.125f*3.0f},
		{0.125f*5.0f, 0.125f*3.0f},
		{0.125f*6.0f, 0.125f*3.0f},
		{0.125f*7.0f, 0.125f*3.0f},

		{0.125f*0.0f, 0.125f*4.0f},
		{0.125f*1.0f, 0.125f*4.0f},
		{0.125f*2.0f, 0.125f*4.0f},
		{0.125f*3.0f, 0.125f*4.0f},
		{0.125f*4.0f, 0.125f*4.0f},
		{0.125f*5.0f, 0.125f*4.0f},
		{0.125f*6.0f, 0.125f*4.0f},
		{0.125f*7.0f, 0.125f*4.0f},

		{0.125f*0.0f, 0.125f*5.0f},
		{0.125f*1.0f, 0.125f*5.0f},
		{0.125f*2.0f, 0.125f*5.0f},
		{0.125f*3.0f, 0.125f*5.0f},
		{0.125f*4.0f, 0.125f*5.0f},
		{0.125f*5.0f, 0.125f*5.0f},
		{0.125f*6.0f, 0.125f*5.0f},
		{0.125f*7.0f, 0.125f*5.0f},

		{0.125f*0.0f, 0.125f*6.0f},
		{0.125f*1.0f, 0.125f*6.0f},
		{0.125f*2.0f, 0.125f*6.0f},
		{0.125f*3.0f, 0.125f*6.0f},
		{0.125f*4.0f, 0.125f*6.0f},
		{0.125f*5.0f, 0.125f*6.0f},
		{0.125f*6.0f, 0.125f*6.0f},
		{0.125f*7.0f, 0.125f*6.0f},

		{0.125f*0.0f, 0.125f*7.0f},
		{0.125f*1.0f, 0.125f*7.0f},
		{0.125f*2.0f, 0.125f*7.0f},
		{0.125f*3.0f, 0.125f*7.0f},
		{0.125f*4.0f, 0.125f*7.0f},
		{0.125f*5.0f, 0.125f*7.0f},
		{0.125f*6.0f, 0.125f*7.0f},
		{0.125f*7.0f, 0.125f*7.0f}
	};


	static const AnimCrop	data_1x1	= {
		1*1,
		{1.00f, 1.00f},
		origin + 0
	};
	static const AnimCrop	data_2x1	= {
		2*1,
		{0.50f, 1.00f},
		origin + 1
	};
	static const AnimCrop	data_4x1	= {
		4*1,
		{0.25f, 1.00f},
		origin + 1 + 2
	};
	static const AnimCrop	data_2x2	= {
		2*2,
		{0.50f, 0.50f},
		origin + 1 + 2 + 4
	};
	static const AnimCrop	data_4x4	= {
		4*4,
		{0.25f, 0.25f},
		origin + 1 + 2 + 4 + 4
	};
	static const AnimCrop	data_8x8	= {
		8*8,
		{0.125f, 0.125f},
		origin + 1 + 2 + 4 + 4 + 4*4
	};

	ML_STATIC_ASSERT(TextureCropAnimationTypeConst__1x1 == 0);
	ML_STATIC_ASSERT(TextureCropAnimationTypeConst__2x1 == 1);
	ML_STATIC_ASSERT(TextureCropAnimationTypeConst__4x1 == 2);
	ML_STATIC_ASSERT(TextureCropAnimationTypeConst__2x2 == 3);
	ML_STATIC_ASSERT(TextureCropAnimationTypeConst__4x4 == 4);
	ML_STATIC_ASSERT(TextureCropAnimationTypeConst__8x8 == 5);
	ML_STATIC_ASSERT(TextureCropAnimationTypeConst_MAX == 6);

	static const AnimCrop	*anim_crop_array[TextureCropAnimationTypeConst_MAX] = {
		&data_1x1,
		&data_2x1,
		&data_4x1,
		&data_2x2,
		&data_4x4,
		&data_8x8
	};


	return anim_crop_array[type];
}

BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// BM3_SDK_INC_BM3_CORE_BM3_COREUTILITY_h
