#ifndef BM3_SDK_INC_BM3_UTILITY_BM3_ENVIRONMEN_H
#define BM3_SDK_INC_BM3_UTILITY_BM3_ENVIRONMEN_H

#include <ml/type/ml_basic.h>
#include <ml/utility/ml_assert.h>
#include <ml/utility/ml_environment.h>
#include "../system/bm3_Version.h"
#include "../system/bm3_Config.h"

#if !defined(BM3_PLATFORM_win_opengl) && !defined(BM3_PLATFORM_win_directx9)  && !defined(BM3_PLATFORM_win_directx10) && !defined(BM3_PLATFORM_playstation3) && !defined(BM3_PLATFORM_xbox360) && !defined(BM3_PLATFORM_psp) && !defined(BM3_PLATFORM_gamebryodx9) && !defined(BM3_PLATFORM_opengles) && !defined(BM3_PLATFORM_nw4c) && !defined(BM3_PLATFORM_psvita)
	#error UNDEFINED 'BM3_PLATFORM_XXX'
#endif

#if !defined(BM3_TARGET_IDE) && !defined(BM3_TARGET_EMBEDDED)
	#error UNDEFINED 'BM3_TARGET_XXX'
#endif

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

///	@addtogroup	utility
///	@{

///	@addtogroup	utility_environment
///	@{

///	@cond internal

namespace internal{

struct struct_menber_offset{
	ml::ui8		ui8_;
	ml::ui16	ui16_;
	ml::ui32	ui32_;
	ml::ui64	ui64_;
	ml::f32		f32_;
	ml::f64		f64_;
};

enum EnvCheckConst{

	EnvCheckVersionMajor	= VersionMajor,
	EnvCheckVersionMinor	= VersionMinor,
	EnvCheckVersionPatch	= VersionPatch,

	EnvCheckCompileOptionDummy	= 0,

	EnvCheckVersion			= (EnvCheckVersionMajor << (10*2)) | (EnvCheckVersionMinor << (10*1)) | (EnvCheckVersionPatch << (10*0)),
	EnvCheckCompileOption	= EnvCheckCompileOptionDummy
};

ML_STATIC_ASSERT(EnvCheckVersionMajor < (1<<10));
ML_STATIC_ASSERT(EnvCheckVersionMinor < (1<<10));
ML_STATIC_ASSERT(EnvCheckVersionPatch < (1<<10));

}	// namespace internal

///	@endcond

///	@brief				環境の妥当性を判定する
///	@param[in]	version	バージョン
///	@param[in]	flag	flag
///	@return				問題なければ true
///	@attention			アサート有効時、環境に問題があるときは停止します
///	@note				引数はデフォルト引数を指定してください
bool CheckEnvironment(
	internal::EnvCheckConst version	= internal::EnvCheckVersion,
	internal::EnvCheckConst flag	= internal::EnvCheckCompileOption,
	int	struct_menber_offset_ui8	= offsetof(internal::struct_menber_offset, ui8_),
	int	struct_menber_offset_ui16	= offsetof(internal::struct_menber_offset, ui16_),
	int	struct_menber_offset_ui32	= offsetof(internal::struct_menber_offset, ui32_),
	int	struct_menber_offset_ui64	= offsetof(internal::struct_menber_offset, ui64_),
	int	struct_menber_offset_f32	= offsetof(internal::struct_menber_offset, f32_),
	int	struct_menber_offset_f64	= offsetof(internal::struct_menber_offset, f64_)
);

///	@}

///	@}

BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_UTILITY_BM3_ENVIRONMEN_H
