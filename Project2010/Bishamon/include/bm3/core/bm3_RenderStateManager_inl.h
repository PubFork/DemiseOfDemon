#ifndef BM3_SDK_INC_BM3_CORE_BM3_RENDERSTATEMANAGER_INL_H
#define BM3_SDK_INC_BM3_CORE_BM3_RENDERSTATEMANAGER_INL_H

#include "../system/bm3_Config.h"
#include "bm3_Const.h"
#include "bm3_CoreType.h"
#include "bm3_Shader.h"
#include <ml/type/ml_type2type.h>
#include <ml/type/ml_basic.h>

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE
namespace internal{

template<typename StateChangePolicyType, typename RenderDevicePolicyType>
RenderStateManagerImpl<StateChangePolicyType, RenderDevicePolicyType>::RenderStateManagerImpl() :
	is_initialized_(false){

	;
}

template<typename StateChangePolicyType, typename RenderDevicePolicyType>
RenderStateManagerImpl<StateChangePolicyType, RenderDevicePolicyType>::~RenderStateManagerImpl(){

	;
}

template<typename StateChangePolicyType, typename RenderDevicePolicyType>
void	RenderStateManagerImpl<StateChangePolicyType, RenderDevicePolicyType>::Initialize(RenderStateInitializeInfo &info){

	StateChangePolicyType::InitializeChangePolicy(info);
	RenderDevicePolicyType::InitializeRenderDevicePolicy(info);
	is_initialized_ = true;
}

template<typename StateChangePolicyType, typename RenderDevicePolicyType>
void	RenderStateManagerImpl<StateChangePolicyType, RenderDevicePolicyType>::Finalize(){

	StateChangePolicyType::FinalizeChangePolicy();
	RenderDevicePolicyType::FinalizeRenderDevicePolicy();
	is_initialized_ = false;
}

/*!
	RenderStateの初期化
	@param	draw_info 描画情報
	@return	なし
*/
template<typename StateChangePolicyType, typename RenderDevicePolicyType>
void	RenderStateManagerImpl<StateChangePolicyType, RenderDevicePolicyType>::BeginRender(DrawInfo &draw_info){

	// レンタリング状態の取得
	StateChangePolicyType::PreBeginRender(draw_info);
	RenderDevicePolicyType::PreBeginRender(draw_info);
	{
		// 後で無駄に設定しないようにするため、BM用にレンダリング状態を設定しておく（この値と比較して設定するか決める）
		RenderDevicePolicyType::SetViewMatrix(draw_info.ViewMatrix());
		RenderDevicePolicyType::SetProjectionMatrix(draw_info.ProjectionMatrix());

		// 最後の引数は強制書き込みするかどうか
		enable_bits_ = 0;
		SetEnable<RenderState::Culling>(draw_info, true, true);
		SetEnable<RenderState::DepthTest>(draw_info, true, true);
		SetEnable<RenderState::DepthWrite>(draw_info, false, true);
		SetEnable<RenderState::AlphaTest>(draw_info, false, true);

#ifdef BM3_TARGET_IDE
		if(draw_info.IsDepthLess()){	
			depth_test_type_		= DepthTestTypeConst_LessEqual;		
			SetDepthTestType<DepthTestTypeConst_LessEqual>(draw_info, true);
		}
		else{
			depth_test_type_		= DepthTestTypeConst_GreaterEqual;	
			SetDepthTestType<DepthTestTypeConst_GreaterEqual>(draw_info, true);
		}
#else
		depth_test_type_		= DepthTestTypeConst_LessEqual;
		SetDepthTestType<DepthTestTypeConst_LessEqual>(draw_info, true);
#endif

		blend_type_				= BlendTypeConst_Non;
		SetBlendType<BlendTypeConst_Non>(draw_info, true);
		
		alpha_test_type_	= AlphaTestTypeConst_LessEqual;
		alpha_ref_			= 0.0f;
		SetAlphaTestType<AlphaTestTypeConst_LessEqual>(draw_info, alpha_ref_, true);

		for(int i = 0;i != TextureLayer::Max;++ i){
			ML_STATIC_ASSERT(TextureLayer::_1 == 0);
			ML_STATIC_ASSERT(TextureLayer::_2 == 1);

#if defined(BM3_PLATFORM_nw4c)

			texture_address_type_[i]	= TextureAddressTypeConst_Repeat;
			texture_filter_type_[i]	= TextureFilterTypeConst_Linear;
			SetTextureAddressFilterType<TextureAddressTypeConst_Repeat, TextureFilterTypeConst_Linear>(draw_info, TextureLayer::Type(TextureLayer::_1 + i), true);
#else
			
#if defined(BM3_PLATFORM_playstation3)
			bm3::Shader::SetTexture(draw_info, TextureLayer::Type(TextureLayer::_1 + i), NULL);
#endif
			texture_address_type_[i]	= TextureAddressTypeConst_Repeat;
			SetTextureAddressType<TextureAddressTypeConst_Repeat>(draw_info, TextureLayer::Type(TextureLayer::_1 + i), true);

			texture_filter_type_[i]	= TextureFilterTypeConst_Linear;
			SetTextureFilterType<TextureFilterTypeConst_Linear>(draw_info, TextureLayer::Type(TextureLayer::_1 + i), true);
#endif

		}
	}
	// レンタリング状態を戻す
	StateChangePolicyType::PostBeginRender(draw_info);
	RenderDevicePolicyType::PostBeginRender(draw_info);
}

template<typename StateChangePolicyType, typename RenderDevicePolicyType>
void	RenderStateManagerImpl<StateChangePolicyType, RenderDevicePolicyType>::EndRender(DrawInfo &draw_info){

	StateChangePolicyType::PreEndRender(draw_info);
	RenderDevicePolicyType::PreEndRender(draw_info);
	{
		;
	}
	StateChangePolicyType::PostEndRender(draw_info);
	RenderDevicePolicyType::PostEndRender(draw_info);
}

template<typename StateChangePolicyType, typename RenderDevicePolicyType>
template<RenderState::Type t>
bool	RenderStateManagerImpl<StateChangePolicyType, RenderDevicePolicyType>::IsEnable(){

	return (enable_bits_ & (1 << t)) != 0;
}

template<typename StateChangePolicyType, typename RenderDevicePolicyType>
template<RenderState::Type t>
void	RenderStateManagerImpl<StateChangePolicyType, RenderDevicePolicyType>::SetEnable(DrawInfo &draw_info, bool i, bool is_direct){

	if(is_direct || (IsEnable<t>() != i)){
		SetIsEnable<t>(i);

		RenderStateChangePolicy::SP_DirectSetEnable(draw_info, i, ml::enum2type<RenderState::Type, t>());
	}
}

template<typename StateChangePolicyType, typename RenderDevicePolicyType>
template<RenderState::Type t>
void	RenderStateManagerImpl<StateChangePolicyType, RenderDevicePolicyType>::SetIsEnable(bool i){

	ML_STATIC_ASSERT(
		(t == RenderState::Culling			) ||
		(t == RenderState::DepthTest		) ||
		(t == RenderState::DepthWrite		) ||
		(t == RenderState::AlphaTest		)
	);

	if(i){	enable_bits_ |=  (1 << t);	}
	else{	enable_bits_ &= ~(1 << t);	}
}

template<typename StateChangePolicyType, typename RenderDevicePolicyType>
template<DepthTestTypeConst t>
void	RenderStateManagerImpl<StateChangePolicyType, RenderDevicePolicyType>::SetDepthTestType(DrawInfo &draw_info, bool is_direct){

	if(is_direct || (depth_test_type_ != t)){
		depth_test_type_ = t;
		RenderStateChangePolicy::SP_DirectSetDepthTestType(draw_info ,ml::enum2type<DepthTestTypeConst, t>());
	}
}

template<typename StateChangePolicyType, typename RenderDevicePolicyType>
template<BlendTypeConst t>
void	RenderStateManagerImpl<StateChangePolicyType, RenderDevicePolicyType>::SetBlendType(DrawInfo &draw_info, bool is_direct){

	if(is_direct || (blend_type_ != t)){
		blend_type_ = t;
		RenderStateChangePolicy::SP_DirectSetBlendType(draw_info ,ml::enum2type<BlendTypeConst, t>());
	}
}

template<typename StateChangePolicyType, typename RenderDevicePolicyType>
template<AlphaTestTypeConst t>
void	RenderStateManagerImpl<StateChangePolicyType, RenderDevicePolicyType>::SetAlphaTestType(DrawInfo &draw_info, float ref, bool is_direct){

	if(is_direct || (alpha_test_type_ != t) || (alpha_ref_ != ref)){
		alpha_test_type_	= t;
		alpha_ref_			= ref;
		RenderStateChangePolicy::SP_DirectSetAlphaTestType(draw_info ,ml::enum2type<AlphaTestTypeConst, t>(), ref);
	}
}

/*!
	SetTextureAddressの初期化(PS3のみ)
	@param	draw_info 描画情報
	@param	layer インデックス
	@param	is_direct 有効/無効
	@return	なし
*/
#if defined(BM3_PLATFORM_nw4c)
template<typename StateChangePolicyType, typename RenderDevicePolicyType>
template<TextureAddressTypeConst type_address, TextureFilterTypeConst type_filter>
void	RenderStateManagerImpl<StateChangePolicyType, RenderDevicePolicyType>::SetTextureAddressFilterType(DrawInfo &draw_info, TextureLayer::Type layer, bool is_direct){

	if(is_direct || (texture_address_type_[layer] != type_address) || (texture_filter_type_[layer] != type_filter)){
		texture_address_type_[layer] = type_address;
		texture_filter_type_[layer] = type_filter;
		RenderStateChangePolicy::SP_DirectSetTextureAddressFilterType(draw_info, layer, ml::enum2type<TextureAddressTypeConst, type_address>(), ml::enum2type<TextureFilterTypeConst, type_filter>());
	}
}
#else
template<typename StateChangePolicyType, typename RenderDevicePolicyType>
template<TextureAddressTypeConst t>
void	RenderStateManagerImpl<StateChangePolicyType, RenderDevicePolicyType>::SetTextureAddressType(DrawInfo &draw_info, TextureLayer::Type layer, bool is_direct){

	if(is_direct || (texture_address_type_[layer] != t)){
		texture_address_type_[layer] = t;
		RenderStateChangePolicy::SP_DirectSetTextureAddressType(draw_info, layer, ml::enum2type<TextureAddressTypeConst, t>());
	}
}

/*!
	TextureFilterの初期化(PS3のみ)
	@param	draw_info 描画情報
	@param	layer インデックス
	@param	is_direct 有効/無効
	@return	なし
*/
template<typename StateChangePolicyType, typename RenderDevicePolicyType>
template<TextureFilterTypeConst t>
void	RenderStateManagerImpl<StateChangePolicyType, RenderDevicePolicyType>::SetTextureFilterType(DrawInfo &draw_info, TextureLayer::Type layer, bool is_direct){

	if(is_direct || (texture_filter_type_[layer] != t)){
		texture_filter_type_[layer] = t;
		RenderStateChangePolicy::SP_DirectSetTextureFilterType(draw_info, layer, ml::enum2type<TextureFilterTypeConst, t>());
	}
}
#endif

template<typename StateChangePolicyType, typename RenderDevicePolicyType>
void	RenderStateManagerImpl<StateChangePolicyType, RenderDevicePolicyType>::SetEnable(DrawInfo &draw_info, bool i, RenderState::Type t, bool is_direct){

	ML_STATIC_ASSERT(RenderState::Culling	 	== 0);
	ML_STATIC_ASSERT(RenderState::DepthTest	 	== 1);
	ML_STATIC_ASSERT(RenderState::DepthWrite 	== 2);
	ML_STATIC_ASSERT(RenderState::AlphaTest 	== 3);
	ML_STATIC_ASSERT(RenderState::MAX		 	== 4);

	switch(t){
		case RenderState::Culling:			SetEnable<RenderState::Culling>(draw_info, is_direct);			break;
		case RenderState::DepthTest:		SetEnable<RenderState::DepthTest>(draw_info, is_direct);		break;
		case RenderState::DepthWrite:		SetEnable<RenderState::DepthWrite>(draw_info, is_direct);		break;
		case RenderState::AlphaTest:		SetEnable<RenderState::AlphaTest>(draw_info, is_direct);		break;
		default:							BM3_ASSUME(0);	ML_ASSERT(false);								break;
	}
}

template<typename StateChangePolicyType, typename RenderDevicePolicyType>
void	RenderStateManagerImpl<StateChangePolicyType, RenderDevicePolicyType>::SetDepthTestType(DrawInfo &draw_info, DepthTestTypeConst t, bool is_direct){

	ML_STATIC_ASSERT(DepthTestTypeConst_Less			== 0);
	ML_STATIC_ASSERT(DepthTestTypeConst_LessEqual		== 1);
	ML_STATIC_ASSERT(DepthTestTypeConst_Greater			== 2);
	ML_STATIC_ASSERT(DepthTestTypeConst_GreaterEqual	== 3);
	ML_STATIC_ASSERT(DepthTestTypeConst_Equal			== 4);
	ML_STATIC_ASSERT(DepthTestTypeConst_NotEqual		== 5);
	ML_STATIC_ASSERT(DepthTestTypeConst_Never			== 6);
	ML_STATIC_ASSERT(DepthTestTypeConst_Always			== 7);
	ML_STATIC_ASSERT(DepthTestTypeConst_MAX				== 8);

	switch(t){
		case DepthTestTypeConst_Less:			SetDepthTestType<DepthTestTypeConst_Less>(draw_info, is_direct);			break;
		case DepthTestTypeConst_LessEqual:		SetDepthTestType<DepthTestTypeConst_LessEqual>(draw_info, is_direct);		break;
		case DepthTestTypeConst_Greater:		SetDepthTestType<DepthTestTypeConst_Greater>(draw_info, is_direct);			break;
		case DepthTestTypeConst_GreaterEqual:	SetDepthTestType<DepthTestTypeConst_GreaterEqual>(draw_info, is_direct);	break;
		case DepthTestTypeConst_Equal:			SetDepthTestType<DepthTestTypeConst_Equal>(draw_info, is_direct);			break;
		case DepthTestTypeConst_NotEqual:		SetDepthTestType<DepthTestTypeConst_NotEqual>(draw_info, is_direct);		break;
		case DepthTestTypeConst_Never:			SetDepthTestType<DepthTestTypeConst_Never>(draw_info, is_direct);			break;
		case DepthTestTypeConst_Always:			SetDepthTestType<DepthTestTypeConst_Always>(draw_info, is_direct);			break;
		default:								ML_ASSERT(false);	BM3_ASSUME(0);											break;
	}
}

template<typename StateChangePolicyType, typename RenderDevicePolicyType>
void	RenderStateManagerImpl<StateChangePolicyType, RenderDevicePolicyType>::SetBlendType(DrawInfo &draw_info, BlendTypeConst t, bool is_direct){

	ML_STATIC_ASSERT(BlendTypeConst_Non			== 0);
	ML_STATIC_ASSERT(BlendTypeConst_Semitrans	== 1);
	ML_STATIC_ASSERT(BlendTypeConst_SemiAdd		== 2);
	ML_STATIC_ASSERT(BlendTypeConst_SemiSub		== 3);
	ML_STATIC_ASSERT(BlendTypeConst_Add			== 4);
	ML_STATIC_ASSERT(BlendTypeConst_Sub			== 5);
	ML_STATIC_ASSERT(BlendTypeConst_Reverse		== 6);
	ML_STATIC_ASSERT(BlendTypeConst_MAX			== 7);

	switch(t){
		case BlendTypeConst_Non:			SetBlendType<BlendTypeConst_Non>(draw_info, is_direct);			break;
		case BlendTypeConst_Semitrans:		SetBlendType<BlendTypeConst_Semitrans>(draw_info, is_direct);	break;
		case BlendTypeConst_SemiAdd:		SetBlendType<BlendTypeConst_SemiAdd>(draw_info, is_direct);		break;
		case BlendTypeConst_SemiSub:		SetBlendType<BlendTypeConst_SemiSub>(draw_info, is_direct);		break;
		case BlendTypeConst_Add:			SetBlendType<BlendTypeConst_Add>(draw_info, is_direct);			break;
		case BlendTypeConst_Sub:			SetBlendType<BlendTypeConst_Sub>(draw_info, is_direct);			break;
		case BlendTypeConst_Reverse:		SetBlendType<BlendTypeConst_Reverse>(draw_info, is_direct);		break;
		default:							ML_ASSERT(false);	BM3_ASSUME(0);								break;
	}
}

template<typename StateChangePolicyType, typename RenderDevicePolicyType>
void	RenderStateManagerImpl<StateChangePolicyType, RenderDevicePolicyType>::SetAlphaTestType(DrawInfo &draw_info, AlphaTestTypeConst t, float ref, bool is_direct){

	ML_STATIC_ASSERT(AlphaTestTypeConst_Less			== 0);
	ML_STATIC_ASSERT(AlphaTestTypeConst_LessEqual		== 1);
	ML_STATIC_ASSERT(AlphaTestTypeConst_Greater			== 2);
	ML_STATIC_ASSERT(AlphaTestTypeConst_GreaterEqual	== 3);
	ML_STATIC_ASSERT(AlphaTestTypeConst_Equal			== 4);
	ML_STATIC_ASSERT(AlphaTestTypeConst_NotEqual		== 5);
	ML_STATIC_ASSERT(AlphaTestTypeConst_Never			== 6);
	ML_STATIC_ASSERT(AlphaTestTypeConst_Always			== 7);
	ML_STATIC_ASSERT(AlphaTestTypeConst_MAX				== 8);

	switch(t){
		case AlphaTestTypeConst_Less:			SetAlphaTestType<AlphaTestTypeConst_Less>(			draw_info, ref, is_direct);	break;
		case AlphaTestTypeConst_LessEqual:		SetAlphaTestType<AlphaTestTypeConst_LessEqual>(		draw_info, ref, is_direct);	break;
		case AlphaTestTypeConst_Greater:		SetAlphaTestType<AlphaTestTypeConst_Greater>(		draw_info, ref, is_direct);	break;
		case AlphaTestTypeConst_GreaterEqual:	SetAlphaTestType<AlphaTestTypeConst_GreaterEqual>(	draw_info, ref, is_direct);	break;
		case AlphaTestTypeConst_Equal:			SetAlphaTestType<AlphaTestTypeConst_Equal>(			draw_info, ref, is_direct);	break;
		case AlphaTestTypeConst_NotEqual:		SetAlphaTestType<AlphaTestTypeConst_NotEqual>(		draw_info, ref, is_direct);	break;
		case AlphaTestTypeConst_Never:			SetAlphaTestType<AlphaTestTypeConst_Never>(			draw_info, ref, is_direct);	break;
		case AlphaTestTypeConst_Always:			SetAlphaTestType<AlphaTestTypeConst_Always>(		draw_info, ref, is_direct);	break;
		default:								ML_ASSERT(false);	BM3_ASSUME(0);											break;
	}
}

#if defined(BM3_PLATFORM_nw4c)
template<typename StateChangePolicyType, typename RenderDevicePolicyType>
void	RenderStateManagerImpl<StateChangePolicyType, RenderDevicePolicyType>::SetTextureAddressFilterType(DrawInfo &draw_info, TextureLayer::Type layer, TextureAddressTypeConst type_address, TextureFilterTypeConst type_filter, bool is_direct){

	ML_STATIC_ASSERT(TextureAddressTypeConst_Clamp	== 0);
	ML_STATIC_ASSERT(TextureAddressTypeConst_Repeat	== 1);
	ML_STATIC_ASSERT(TextureAddressTypeConst_Mirror	== 2);
	ML_STATIC_ASSERT(TextureAddressTypeConst_MAX	== 3);

	ML_STATIC_ASSERT(TextureFilterTypeConst_Nearest	== 0);
	ML_STATIC_ASSERT(TextureFilterTypeConst_Linear	== 1);
	ML_STATIC_ASSERT(TextureFilterTypeConst_MAX		== 2);

	switch(type_address){
		case TextureAddressTypeConst_Clamp:
			switch(type_filter){
				case TextureFilterTypeConst_Nearest:	SetTextureAddressFilterType<TextureAddressTypeConst_Clamp, TextureFilterTypeConst_Nearest>(draw_info, layer, is_direct);	break;
				case TextureFilterTypeConst_Linear:		SetTextureAddressFilterType<TextureAddressTypeConst_Clamp, TextureFilterTypeConst_Linear>(draw_info, layer, is_direct);	break;
				default:								ML_ASSERT(false);	BM3_ASSUME(0);													break;
			}
			break;
		case TextureAddressTypeConst_Repeat:
			switch(type_filter){
				case TextureFilterTypeConst_Nearest:	SetTextureAddressFilterType<TextureAddressTypeConst_Repeat, TextureFilterTypeConst_Nearest>(draw_info, layer, is_direct);	break;
				case TextureFilterTypeConst_Linear:		SetTextureAddressFilterType<TextureAddressTypeConst_Repeat, TextureFilterTypeConst_Linear>(draw_info, layer, is_direct);	break;
				default:								ML_ASSERT(false);	BM3_ASSUME(0);													break;
			}
			break;
		case TextureAddressTypeConst_Mirror:
			switch(type_filter){
				case TextureFilterTypeConst_Nearest:	SetTextureAddressFilterType<TextureAddressTypeConst_Mirror, TextureFilterTypeConst_Nearest>(draw_info, layer, is_direct);	break;
				case TextureFilterTypeConst_Linear:		SetTextureAddressFilterType<TextureAddressTypeConst_Mirror, TextureFilterTypeConst_Linear>(draw_info, layer, is_direct);	break;
				default:								ML_ASSERT(false);	BM3_ASSUME(0);													break;
			}
			break;
		default:								ML_ASSERT(false);	BM3_ASSUME(0);													break;
	}


}
#else

template<typename StateChangePolicyType, typename RenderDevicePolicyType>
void	RenderStateManagerImpl<StateChangePolicyType, RenderDevicePolicyType>::SetTextureAddressType(DrawInfo &draw_info, TextureLayer::Type layer, TextureAddressTypeConst t, bool is_direct){

	ML_STATIC_ASSERT(TextureAddressTypeConst_Clamp	== 0);
	ML_STATIC_ASSERT(TextureAddressTypeConst_Repeat	== 1);
	ML_STATIC_ASSERT(TextureAddressTypeConst_Mirror	== 2);
	ML_STATIC_ASSERT(TextureAddressTypeConst_MAX	== 3);

	switch(t){
		case TextureAddressTypeConst_Clamp:		SetTextureAddressType<TextureAddressTypeConst_Clamp>( draw_info, layer, is_direct);	break;
		case TextureAddressTypeConst_Repeat:	SetTextureAddressType<TextureAddressTypeConst_Repeat>(draw_info, layer, is_direct);	break;
		case TextureAddressTypeConst_Mirror:	SetTextureAddressType<TextureAddressTypeConst_Mirror>(draw_info, layer, is_direct);	break;
		default:								ML_ASSERT(false);	BM3_ASSUME(0);													break;
	}
}

template<typename StateChangePolicyType, typename RenderDevicePolicyType>
void	RenderStateManagerImpl<StateChangePolicyType, RenderDevicePolicyType>::SetTextureFilterType(DrawInfo &draw_info, TextureLayer::Type layer, TextureFilterTypeConst t, bool is_direct){

	ML_STATIC_ASSERT(TextureFilterTypeConst_Nearest	== 0);
	ML_STATIC_ASSERT(TextureFilterTypeConst_Linear	== 1);
	ML_STATIC_ASSERT(TextureFilterTypeConst_MAX		== 2);

	switch(t){
		case TextureFilterTypeConst_Nearest:	SetTextureFilterType<TextureFilterTypeConst_Nearest>(draw_info, layer, is_direct);	break;
		case TextureFilterTypeConst_Linear:		SetTextureFilterType<TextureFilterTypeConst_Linear>(draw_info, layer, is_direct);	break;
		default:								ML_ASSERT(false);	BM3_ASSUME(0);													break;
	}
}
#endif

}	// namespace internal

BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_BM3_RENDERSTATEMANAGER_INL_H
