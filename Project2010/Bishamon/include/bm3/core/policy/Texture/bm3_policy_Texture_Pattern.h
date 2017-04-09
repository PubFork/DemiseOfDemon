#ifndef BM3_SDK_INC_BM3_CORE_TEXTURE_BM3_POLICY_TEXTURE_PATTERN_H
#define BM3_SDK_INC_BM3_CORE_TEXTURE_BM3_POLICY_TEXTURE_PATTERN_H

#include "../../bm3_Const.h"
#include "../../bm3_CoreType.h"
#include "../../bm3_CoreUtility.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

namespace policy{

namespace impl{

/// @brief Texture_Patternクラス
class Texture_Pattern{

public:
	template<typename ResourceType>
	static BM3_FORCEINLINE void	Initialize(const ResourceType &res, ml::random &random, TextureCrop &/*value*/, int &frame, float &step, TextureLayer::Type lt){

//		if(type == TextureAnimationTypeConst_Pattern)	// 将来的にこのif文はなくなる
		{
			if(Texture_Layer_CropRandom(res, lt)){
				frame	= random.i32(Texture_Layer_Pattern(res, lt).PatternCount());
			}
			else{
				frame	= 0;
			}
			step	= (float)(Texture_Layer_AnimationSpeed(res, lt) * Texture_Layer_Pattern(res, lt).Pattern(0).frame_count_);
		}

#if defined(BM3_TARGET_IDE)
		// IDEの時、途中でタイプが変わったときに参照されてしまう。
		frame	= 0;
		step	= (float)(Texture_Layer_AnimationSpeed(res, lt) * Texture_Layer_Pattern(res, lt).Pattern(0).frame_count_);
#endif
	}

	template<typename ResourceType>
	static BM3_FORCEINLINE void	InitializeLayer1(const ResourceType &res, ml::random &/*random*/, TextureCrop &/*value*/, int &frame, float &step, TextureAnimationTypeConst type){

		if(type == TextureAnimationTypeConst_Pattern)	// 将来的にこのif文はなくなる
		{
			frame	= 0;
			step	= (float)(res.Texture_Layer1_AnimationSpeed() * res.Texture_Layer1_Pattern().Pattern(0).frame_count_);
		}

#if defined(BM3_TARGET_IDE)
		// IDEの時、途中でタイプが変わったときに参照されてしまう。
		frame	= 0;
		step	= (float)(res.Texture_Layer1_AnimationSpeed() * res.Texture_Layer1_Pattern().Pattern(0).frame_count_);
#endif
	}
	
	template<typename ResourceType, typename UpdateInfo>
	static BM3_FORCEINLINE void	Update(UpdateInfo &update_info, const ResourceType &res, TextureCrop &/*value*/, int &frame, float &step, ml::random &random, TextureAnimationTypeConst /*type*/, TextureLayer::Type lt){

//		if(type == TextureAnimationTypeConst_Pattern)	// 将来的にこのif文はなくなる
		{
			ML_ASSERT(0<=step);
	
			//-- step;
			step -= update_info.GetUpdateFrame();
			while(step <= 0){

				if(Texture_Layer_CropRandom(res, lt)){
					
					frame	= random.i32(Texture_Layer_Pattern(res, lt).PatternCount());
				}
				else{
					++ frame;

					// 最終フレームを過ぎたら
					if(frame >= Texture_Layer_Pattern(res, lt).PatternCount()){
						frame =
							Texture_Layer_Pattern(res, lt).IsRepeatPattern() ?
								0 :
								Texture_Layer_Pattern(res, lt).PatternCount() - 1;
					}
				}

				step = (Texture_Layer_AnimationSpeed(res, lt) * Texture_Layer_Pattern(res, lt).Pattern(frame).frame_count_) + step;
			}
		}
	}
	template<typename ResourceType, typename UpdateInfo>
	static BM3_FORCEINLINE void	UpdateLayer1(UpdateInfo &update_info, const ResourceType &res, TextureCrop &/*value*/, int &frame, float &step, TextureAnimationTypeConst type){

		if(type == TextureAnimationTypeConst_Pattern)	// 将来的にこのif文はなくなる
		{
			ML_ASSERT(0<=step);
	
			//-- step;
			step -= update_info.GetUpdateFrame();
			while(step <= 0){
				++ frame;

				// 最終フレームを過ぎたら
				if(frame >= res.Texture_Layer1_Pattern().PatternCount()){
					frame =
						res.Texture_Layer1_Pattern().IsRepeatPattern() ?
							0 :
							res.Texture_Layer1_Pattern().PatternCount() - 1;
				}

				step = (res.Texture_Layer1_AnimationSpeed() * res.Texture_Layer1_Pattern().Pattern(frame).frame_count_) + step;
			}
		}
	}	
};

}	// namespace impl

class Texture_Pattern{

public:
	template<typename ResourceType>
	BM3_FORCEINLINE void	Initialize(const ResourceType &res, ml::random &random, TextureCrop &value, TextureAnimationTypeConst type, TextureLayer::Type lt){

		impl::Texture_Pattern::Initialize(res, random, value, frame_, step_, type, lt);
	}

	template<typename ResourceType, typename UpdateInfo>
	BM3_FORCEINLINE void	Update(UpdateInfo &update_info, const ResourceType &res, TextureCrop &value, TextureAnimationTypeConst type, TextureLayer::Type lt){

		impl::Texture_Pattern::Update(update_info, res, value, frame_, step_, type, lt);
	}

private:
	int	frame_;
	int	step_;
};

}	// namespace policy
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_TEXTURE_BM3_POLICY_TEXTURE_Pattern_H
