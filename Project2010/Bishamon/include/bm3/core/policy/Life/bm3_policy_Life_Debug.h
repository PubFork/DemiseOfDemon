#ifndef BM3_SDK_INC_BM3_CORE_POLICY_LIFE_BM3_POLICY_LIFE_DEBUG_H
#define BM3_SDK_INC_BM3_CORE_POLICY_LIFE_BM3_POLICY_LIFE_DEBUG_H

#include <ml/math/ml_arithmetic.h>		// ml::maximum
#include <ml/math/ml_random.h>
#include "../../fileformat/bm3_bmb.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

namespace policy{

// �f�o�b�O�v���p
// ��ɃA�N�e�B�u
// �K�����s�[�g����
// �����_�����͂Ȃ�
/// @brief Life_Debug�N���X
class Life_Debug{

public:
	BM3_FORCEINLINE bool	IsActive() const{		return (0 < age_) && (age_ <= life_);		}	// ������0�̂Ƃ��͂܂����܂�Ă��Ȃ��Ƃ���
	BM3_FORCEINLINE bool	IsExpired() const{		return (life_ < age_);						}	// �����Ɠ������ꍇ�͂܂������Ă���
	BM3_FORCEINLINE float	RestLifeRate() const{	return rest_life_rate_;						}
	BM3_FORCEINLINE float	LifeRate() const{		return 1.0f - rest_life_rate_;				}
	BM3_FORCEINLINE float	LifeFrameStep() const{	return rest_life_rate_step_;				}		// 1�t���[��������̃X�e�b�v
	BM3_FORCEINLINE bool	IsStarted() const{		return (0 < age_);							}

	template<typename ResourceType>
	BM3_FORCEINLINE void	Initialize(const ResourceType &res){

		life_ =
			UpdateFixPoint::IntToFixPoint(res.Basic_Basic_Life());

		Reset();

		rest_life_rate_step_	= ml::inverse(UpdateFixPoint::FixPointToFloat(life_));
	}

	template<typename UpdateInfo>
	BM3_FORCEINLINE void	Update(UpdateInfo &info){

		if(IsExpired()){	Reset();	}
		
		age_	+= info.GetFixPointUpdateFrame();
		rest_life_rate_ -= rest_life_rate_step_ * info.GetUpdateFrame();
	}

	BM3_FORCEINLINE void	ForceActive(){	if(IsActive()==false)	age_ = 0;	}

	template<typename ResourceType>
	BM3_FORCEINLINE void	SwitchResource(const ResourceType & /*res*/, ml::random & /*random*/){
	}

private:
	UpdateFixPoint::Type	life_;
	UpdateFixPoint::Type	age_;

	float					rest_life_rate_;
	float					rest_life_rate_step_;
	
	void Reset(){
		age_			= 0;
		rest_life_rate_	= 1.0f;
	}
};

}	// namespace policy
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_POLICY_LIFE_BM3_POLICY_LIFE_DEBUG_H
