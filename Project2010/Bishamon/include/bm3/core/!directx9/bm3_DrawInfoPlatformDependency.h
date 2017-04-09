#ifndef BM3_SDK_INC_BM3_CORE_DIRECTX9_BM3_DRAWINFOPLATFORMDEPENDENCY_H
#define BM3_SDK_INC_BM3_CORE_DIRECTX9_BM3_DRAWINFOPLATFORMDEPENDENCY_H

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

namespace internal{

class DrawInfoPlatformDependency{
public:
	DrawInfoPlatformDependency() :
		is_enabled_softn_(false){
			;
	}
	
	void	EnableSoftN(){		is_enabled_softn_ = true;	}
	void	DisableSoftN(){		is_enabled_softn_ = false;	}
	bool	IsEnabledSoftN()	const {	return is_enabled_softn_;	}
	
private:
	bool			is_enabled_softn_;	// 正規化されたソフトパーティクルを使うかどうか
};

}	// namespace internal
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_DIRECTX9_BM3_DRAWINFOPLATFORMDEPENDENCY_H
