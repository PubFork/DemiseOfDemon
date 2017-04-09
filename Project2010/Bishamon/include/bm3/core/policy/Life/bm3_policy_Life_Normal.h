#ifndef BM3_SDK_INC_BM3_CORE_POLICY_LIFE_BM3_POLICY_LIFE_NORMAL_H
#define BM3_SDK_INC_BM3_CORE_POLICY_LIFE_BM3_POLICY_LIFE_NORMAL_H

#include <ml/math/ml_arithmetic.h>		// ml::maximum
#include <ml/math/ml_random.h>
#include "../../fileformat/bm3_bmb.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

namespace policy{

/// @brief Life_Normalクラス
class Life_Normal{

public:
	BM3_FORCEINLINE bool	IsActive() const{		return (0 < age_) && (age_ <= lifespan_);		}	// 寿命が0のときはまだ生まれていないとする
	BM3_FORCEINLINE bool	IsExpired() const{		return (lifespan_ < age_);						}	// 寿命と等しい場合はまだ生きている
	BM3_FORCEINLINE float	RestLifeRate() const{	return rest_life_rate_;						}
	BM3_FORCEINLINE float	LifeRate() const{		return 1.0f - rest_life_rate_;				}
	BM3_FORCEINLINE float	LifeFrameStep() const{	return rest_life_rate_step_;				}		// 1フレームあたりのステップ
	BM3_FORCEINLINE float	LifeFrame() const { return UpdateFixPoint::FixPointToFloat(age_); }


	BM3_FORCEINLINE bool	IsFirstFrame() const{	return is_first_frame_;						}
	BM3_FORCEINLINE bool	IsStarted() const{		return (0 < age_);							}

	template<typename ResourceType>
	BM3_FORCEINLINE void	Initialize(const ResourceType &res, ml::random &random){

		ML_ASSERT(res.Basic_Basic_LifeRange() >= 0);
		ML_ASSERT(res.Basic_Basic_ActiveStartRange() >= 0);

#if defined(BM3_TARGET_EMBEDDED)
		is_infinity_ = (res.node_header_.flag_ & fileformat::bmb::NodeHeader::Flag_is_life_infinity) != 0;
#else
		is_infinity_ = false;
#endif

		lifespan_ =
			UpdateFixPoint::IntToFixPoint(
				ml::maximum(
					0,
					random.i32_range(
						res.Basic_Basic_Life(),
						res.Basic_Basic_LifeRange()
					)
				)
			);

		// 寿命0より多いとアクティブ
		age_	= 0;
		age_	-=
			UpdateFixPoint::IntToFixPoint(
				ml::maximum(
					0,
					random.i32_range(
						res.Basic_Basic_ActiveStart(),
						res.Basic_Basic_ActiveStartRange()
					)
				)
			);

		is_first_frame_			= true;
		rest_life_rate_			= 1.0f;
		rest_life_rate_step_	= ml::inverse(UpdateFixPoint::FixPointToFloat(lifespan_));
	}

	template<typename UpdateInfo>
	BM3_FORCEINLINE void	Update(UpdateInfo &info){
		if(is_infinity_ == false){

			if(IsExpired()==false){
				age_	+= info.GetFixPointUpdateFrame();
				if(IsActive()){
					is_first_frame_	= false;

					rest_life_rate_ -= rest_life_rate_step_ * info.GetUpdateFrame();
				}
			}
		}
	}

	BM3_FORCEINLINE void	ForceActive(){	if(IsActive()==false)	age_ = 0;	}

	template<typename ResourceType>
	BM3_FORCEINLINE void	SwitchResource(const ResourceType &res, ml::random &random){

		lifespan_ =
			UpdateFixPoint::IntToFixPoint(
				ml::maximum(
					0,
					random.i32_range(
						res.Basic_Basic_Life(),
						res.Basic_Basic_LifeRange()
					)
				)
			);

		if(IsExpired()==false){
			rest_life_rate_step_	= ml::inverse(UpdateFixPoint::FixPointToFloat(lifespan_ - age_));
		}
	}

private:
	UpdateFixPoint::Type	lifespan_;
	UpdateFixPoint::Type	age_;

	float					rest_life_rate_;
	float					rest_life_rate_step_;
	bool					is_infinity_;
	bool					is_first_frame_;
};

}	// namespace policy
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_POLICY_LIFE_BM3_POLICY_LIFE_NORMAL_H
