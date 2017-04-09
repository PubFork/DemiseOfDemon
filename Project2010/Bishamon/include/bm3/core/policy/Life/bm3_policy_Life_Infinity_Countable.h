#ifndef BM3_SDK_INC_BM3_CORE_POLICY_LIFE_BM3_POLICY_LIFE_INFINITY_COUNTABLE_H
#define BM3_SDK_INC_BM3_CORE_POLICY_LIFE_BM3_POLICY_LIFE_INFINITY_COUNTABLE_H

#include <ml/math/ml_arithmetic.h>		// ml::maximum
#include <ml/math/ml_random.h>
#include "../../fileformat/bm3_bmb.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

namespace policy{


/// @brief Life_Infinity_Countableクラス。無限寿命だが、一定時間を持って時間が来たら元に戻す
class Life_Infinity_Countable{

public:
	BM3_FORCEINLINE bool	IsActive() const{		return true;					}	// 常にアクティブ
	BM3_FORCEINLINE bool	IsExpired() const{		return false;					}	// 常に期限なし
	BM3_FORCEINLINE float	RestLifeRate() const{	return rest_life_rate_;			}
	BM3_FORCEINLINE float	LifeRate() const{		return 1.0f - rest_life_rate_;	}
	BM3_FORCEINLINE float	LifeFrameStep() const{	return rest_life_rate_step_;	}	// 1フレームあたりのステップ
	BM3_FORCEINLINE bool	IsFirstFrame() const{	return false;					}

	template<typename ResourceType>
	BM3_FORCEINLINE void	Initialize(const ResourceType &res, ml::random &random){

		ML_ASSERT(res.Basic_Basic_LifeRange() >= 0);

		life_ =
			UpdateFixPoint::IntToFixPoint(
				ml::maximum(
					0,
					random.i32_range(
						res.Basic_Basic_Life(),
						res.Basic_Basic_LifeRange()
					)
				)
			);

		age_	= 0;

		rest_life_rate_			= 1.0f;
		rest_life_rate_step_	= ml::inverse(UpdateFixPoint::FixPointToFloat(life_));
	}

	template<typename UpdateInfo>
	BM3_FORCEINLINE void	Update(UpdateInfo &info){

		age_ += info.GetFixPointUpdateFrame();
		rest_life_rate_ -= rest_life_rate_step_ * info.GetUpdateFrame();
		if(life_ < age_){	
			rest_life_rate_ = 1.0f;
			age_	= life_ - age_;
		}
	}

	template<typename ResourceType>
	BM3_FORCEINLINE void	SwitchResource(const ResourceType &res, ml::random &random){
		life_ =
			UpdateFixPoint::IntToFixPoint(
				ml::maximum(
					0,
					random.i32_range(
						res.Basic_Basic_Life(),
						res.Basic_Basic_LifeRange()
					)
				)
			);
	}

private:
	UpdateFixPoint::Type	life_;
	UpdateFixPoint::Type	age_;
	float					rest_life_rate_;
	float					rest_life_rate_step_;
};

}	// namespace policy
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_POLICY_LIFE_BM3_POLICY_LIFE_INFINITY_COUNTABLE_H
