#ifndef BM3_SDK_INC_BM3_CORE_POLICY_LIFE_BM3_POLICY_LIFE_GROUP_H
#define BM3_SDK_INC_BM3_CORE_POLICY_LIFE_BM3_POLICY_LIFE_GROUP_H

#include <ml/math/ml_arithmetic.h>		// ml::maximum
#include <ml/math/ml_random.h>
#include "../../fileformat/bm3_bmb.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

namespace policy{

/// @brief Life_Groupクラス。現状マスパーティクル用
class Life_Group{

public:
	BM3_FORCEINLINE bool	IsActive() const{		return (0 < age_) && (age_ <= life_);	}	// 
	BM3_FORCEINLINE bool	IsExpired() const{		return (life_ < age_);					}	// 
	BM3_FORCEINLINE float	RestLifeRate() const{	return rest_life_rate_;					}
	BM3_FORCEINLINE float	LifeRate() const{		return 1.0f - rest_life_rate_;			}
	BM3_FORCEINLINE float	LifeFrameStep() const{	return rest_life_rate_step_;			}	// 1フレームあたりのステップ
	BM3_FORCEINLINE bool	IsFirstFrame() const{	return false;							}

	template<typename ResourceType>
	BM3_FORCEINLINE void	Initialize(const ResourceType &res, ml::random &random){

		ML_ASSERT(res.Basic_Basic_LifeRange() >= 0);

		DetermineLife(res, random);

		age_	= 0;
		rest_life_rate_			= 1.0f;
		is_repeat_				= res.Generate_Constant_EndlessRepeat();
	}

	template<typename UpdateInfo>
	BM3_FORCEINLINE void	Update(UpdateInfo &info){

		age_ += info.GetFixPointUpdateFrame();
		rest_life_rate_ -= rest_life_rate_step_ * info.GetUpdateFrame();
		if(IsExpired()){	
			if(is_repeat_){
				rest_life_rate_ = 1.0f;
				age_	= age_ - life_;
			}
		}
	}

	template<typename ResourceType>
	BM3_FORCEINLINE void	SwitchResource(const ResourceType &res, ml::random &random){
		DetermineLife(res, random);
	}

private:
	template<typename ResourceType>
	BM3_FORCEINLINE void	DetermineLife(const ResourceType &res, ml::random & /*random*/){
		life_ = UpdateFixPoint::IntToFixPoint(
				res.Basic_Basic_Life() + res.Basic_Basic_LifeRange() 
				+ res.Basic_Basic_ActiveStart() + res.Basic_Basic_ActiveStartRange()
				+ res.Generate_Constant_GenerateTime()
			);
		
		rest_life_rate_step_	= ml::inverse(UpdateFixPoint::FixPointToFloat(life_));
	}

	UpdateFixPoint::Type	life_;
	UpdateFixPoint::Type	age_;
	float					rest_life_rate_;
	float					rest_life_rate_step_;
	bool					is_repeat_;
};

}	// namespace policy
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_POLICY_LIFE_BM3_POLICY_LIFE_GROUP_H
