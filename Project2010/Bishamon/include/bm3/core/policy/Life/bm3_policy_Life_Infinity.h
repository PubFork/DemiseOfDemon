#ifndef BM3_SDK_INC_BM3_CORE_POLICY_LIFE_BM3_POLICY_LIFE_INFINITY_H
#define BM3_SDK_INC_BM3_CORE_POLICY_LIFE_BM3_POLICY_LIFE_INFINITY_H

namespace ml{
class random;
}

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

namespace policy{

/// @brief Life_Infinityクラス
class Life_Infinity{

public:
	BM3_FORCEINLINE bool	IsActive() const{		return true;	}
	BM3_FORCEINLINE bool	IsExpired() const{		return false;	}
	BM3_FORCEINLINE float	RestLifeRate() const{	return 1.0f;	}
	BM3_FORCEINLINE float	LifeRate() const{		return 0.0f;	}
	BM3_FORCEINLINE float	LifeFrameStep() const{	return 0.0f;	}		// 1フレームあたりのステップ
	BM3_FORCEINLINE bool	IsFirstFrame() const{	return false;	}

	template<typename ResourceType>
	BM3_FORCEINLINE void Initialize(const ResourceType &/*res*/, ml::random &/*random*/){

		// 何もしない
		;
	}

	template<typename UpdateInfo>
	BM3_FORCEINLINE void Update(UpdateInfo &/*info*/){

		// 何もしない
		;
	}

	template<typename ResourceType>
	BM3_FORCEINLINE void	SwitchResource(const ResourceType & /*res*/, ml::random & /*random*/){
	}

	BM3_FORCEINLINE void	ForceActive(){	;	}

};

}	// namespace policy
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_POLICY_LIFE_BM3_POLICY_LIFE_INFINITY_H
