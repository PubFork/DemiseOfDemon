#ifndef BM3_SDK_INC_BM3_CORE_BM3_RENDERSTATEMANAGER_H
#define BM3_SDK_INC_BM3_CORE_BM3_RENDERSTATEMANAGER_H

#include <ml/type/ml_color.h>
#include <ml/type/ml_basic.h>
#include <ml/type/ml_matrix44.h>
#include "../system/bm3_Config.h"
#include "bm3_Const.h"
#include "texture/bm3_tex_HandleBase.h"
#include "bm3_CoreType.h"
#include "bm3_DrawInfo.h"
#include "bm3_RenderStateInitializeInfo.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

namespace internal{

/// @brief RenderStateManagerImplƒNƒ‰ƒX
template<typename StateChangePolicyType, typename RenderDevicePolicyType>
class RenderStateManagerImpl :
	public StateChangePolicyType,
	public RenderDevicePolicyType{

public:
	RenderStateManagerImpl();
	~RenderStateManagerImpl();

	void	Initialize(RenderStateInitializeInfo &info);
	void	Finalize();
	bool	IsInitialized() const{	return is_initialized_;	}

	void	BeginRender(DrawInfo &draw_info);
	void	EndRender(DrawInfo &draw_info);

	template<RenderState::Type t>
	bool	IsEnable();

	template<RenderState::Type t>
	void	SetEnable(DrawInfo &draw_info, bool i, bool is_direct = false);

	template<DepthTestTypeConst t>
	void	SetDepthTestType(DrawInfo &draw_info, bool is_direct = false);

	template<BlendTypeConst t>
	void	SetBlendType(DrawInfo &draw_info, bool is_direct = false);

	template<AlphaTestTypeConst t>
	void	SetAlphaTestType(DrawInfo &draw_info, float ref, bool is_direct = false);
		

#if defined(BM3_PLATFORM_nw4c)
	template<TextureAddressTypeConst type_address, TextureFilterTypeConst type_filter>
	void	SetTextureAddressFilterType(DrawInfo &draw_info, TextureLayer::Type layer, bool is_direct = false);
#else
	template<TextureAddressTypeConst t>
	void	SetTextureAddressType(DrawInfo &draw_info, TextureLayer::Type layer, bool is_direct = false);

	template<TextureFilterTypeConst t>
	void	SetTextureFilterType(DrawInfo &draw_info, TextureLayer::Type layer, bool is_direct = false);
#endif

	void	SetEnable(DrawInfo &draw_info, bool i, RenderState::Type t, bool is_direct = false);
	void	SetDepthTestType(DrawInfo &draw_info, DepthTestTypeConst t, bool is_direct = false);
	void	SetBlendType(DrawInfo &draw_info, BlendTypeConst t, bool is_direct = false);
	void	SetAlphaTestType(DrawInfo &draw_info, AlphaTestTypeConst t, float ref, bool is_direct = false);

#if defined(BM3_PLATFORM_nw4c)
	void	SetTextureAddressFilterType(DrawInfo &draw_info, TextureLayer::Type layer, TextureAddressTypeConst type_address, TextureFilterTypeConst type_filter, bool is_direct = false);
#else
	void	SetTextureAddressType(DrawInfo &draw_info, TextureLayer::Type layer, TextureAddressTypeConst t, bool is_direct = false);
	void	SetTextureFilterType(DrawInfo &draw_info, TextureLayer::Type layer, TextureFilterTypeConst t, bool is_direct = false);
#endif

	BlendTypeConst			BlendType() const{									return blend_type_;						}
	TextureAddressTypeConst	TextureAddressType(TextureLayer::Type layer) const{	return texture_address_type_[layer];	}
	TextureFilterTypeConst	TextureFilterType(TextureLayer::Type layer) const{	return texture_filter_type_[layer];		}

protected:
	bool	is_initialized_;

	ml::ui32				enable_bits_;
	DepthTestTypeConst		depth_test_type_;
	BlendTypeConst			blend_type_;
	AlphaTestTypeConst		alpha_test_type_;
	float					alpha_ref_;
	TextureAddressTypeConst	texture_address_type_[TextureLayer::Max];
	TextureFilterTypeConst	texture_filter_type_[TextureLayer::Max];

	template<RenderState::Type t>
	void	SetIsEnable(bool i);
};

}	// namespace internal

BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3


#if defined(BM3_PLATFORM_win_opengl)
	#include "!opengl/bm3_RenderStateChangePolicy.h"
	#include "!opengl/bm3_RenderDevicePolicy.h"
#endif
#if defined(BM3_PLATFORM_win_directx9)
	#include "!directx9/bm3_RenderStateChangePolicy.h"
	#include "!directx9/bm3_RenderDevicePolicy.h"
#endif
#if defined(BM3_PLATFORM_win_directx10)
	#include "!directx10/bm3_RenderStateChangePolicy.h"
	#include "!directx10/bm3_RenderDevicePolicy.h"
#endif
#if defined(BM3_PLATFORM_playstation3)
	#include "!playstation3/bm3_RenderStateChangePolicy.h"
	#include "!playstation3/bm3_RenderDevicePolicy.h"
#endif
#if defined(BM3_PLATFORM_xbox360)
	#include "!xbox360/bm3_RenderStateChangePolicy.h"
	#include "!xbox360/bm3_RenderDevicePolicy.h"
#endif	
#if defined(BM3_PLATFORM_psp)
	#include "!psp/bm3_RenderStateChangePolicy.h"
	#include "!psp/bm3_RenderDevicePolicy.h"
#endif
#if defined(BM3_PLATFORM_gamebryodx9)
	#include "!gamebryodx9/bm3_RenderStateChangePolicy.h"
	#include "!gamebryodx9/bm3_RenderDevicePolicy.h"
#endif
#if defined(BM3_PLATFORM_opengles)
	#include "!opengles/bm3_RenderStateChangePolicy.h"
	#include "!opengles/bm3_RenderDevicePolicy.h"
#endif
#if defined(BM3_PLATFORM_nw4c)
	#include "!nw4c/bm3_RenderStateChangePolicy.h"
	#include "!nw4c/bm3_RenderDevicePolicy.h"
#endif
#if defined(BM3_PLATFORM_psvita)
	#include "!psvita/bm3_RenderStateChangePolicy.h"
	#include "!psvita/bm3_RenderDevicePolicy.h"
#endif

#include "bm3_RenderStateManager_inl.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

#if defined(BM3_TARGET_IDE) && defined(BM3_PLATFORM_win_directx9)
	typedef	internal::RenderStateManagerImpl<internal::RenderStateChangePolicy, internal::ide::RenderDevicePolicy>	RenderStateManagerImpl;
#else
	typedef	internal::RenderStateManagerImpl<internal::RenderStateChangePolicy, internal::RenderDevicePolicy>		RenderStateManagerImpl;
#endif

typedef	ml::singleton<RenderStateManagerImpl>																RenderStateManager;

BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_BM3_RENDERSTATEMANAGER_H
