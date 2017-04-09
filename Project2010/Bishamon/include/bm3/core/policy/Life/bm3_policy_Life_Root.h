#ifndef BM3_SDK_INC_BM3_CORE_POLICY_LIFE_BM3_POLICY_LIFE_Root_H
#define BM3_SDK_INC_BM3_CORE_POLICY_LIFE_BM3_POLICY_LIFE_Root_H

namespace ml{
class random;
}

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

namespace policy{

/// @brief Life_RootƒNƒ‰ƒX
class Life_Root{

public:
	BM3_FORCEINLINE bool	IsExpired() const{	return (lifespan_==0) ? false : (UpdateFixPoint::IntToFixPoint(lifespan_)<age_);		}

	BM3_FORCEINLINE void Initialize(int life){

		lifespan_	= life;
		age_		= 0;
	}

	template<typename UpdateInfo>
	BM3_FORCEINLINE void Update(UpdateInfo &info){
		if(IsExpired()==false){
			age_	+= info.GetFixPointUpdateFrame();
		}
	}

	BM3_FORCEINLINE void	SwitchResource(int life){
		lifespan_ = life;
	}

private:
	int						lifespan_;
	UpdateFixPoint::Type	age_;
};

}	// namespace policy
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_POLICY_LIFE_BM3_POLICY_LIFE_Root_H
