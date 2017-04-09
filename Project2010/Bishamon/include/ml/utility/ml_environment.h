#ifndef LIBMATCHLOCK_INC_ML_UTILITY_ML_ENVIRONMENT_CHECK_H
#define LIBMATCHLOCK_INC_ML_UTILITY_ML_ENVIRONMENT_CHECK_H

#if defined(BM3_PLATFORM_nw4c)
#pragma diag_suppress 1301
#pragma diag_suppress 2530
#endif

#include "../system/ml_version.h"
#include "../type/ml_basic.h"
#include "ml_assert.h"
#include <stddef.h>				// offsetof

namespace ml{

///	@addtogroup	utility
///	@{

///	@addtogroup	utility_environment
///	@{

///	@cond internal

namespace internal{

struct struct_menber_offset{
	ui8		ui8_;
	ui16	ui16_;
	ui32	ui32_;
	ui64	ui64_;
	f32		f32_;
	f64		f64_;
};

enum env_check_const{

	env_check_version_major	= ml::version_major,
	env_check_version_minor	= ml::version_minor,
	env_check_version_patch	= ml::version_patch,

#if defined(ML_ASSERT_ENABLE)
	env_check_compile_option_assert	= 1<<0,
#else
	env_check_compile_option_assert	= 0,
#endif

	env_check_version			= (env_check_version_major << (10*2)) | (env_check_version_minor << (10*1)) | (env_check_version_patch << (10*0)),
	env_check_compile_option	= env_check_compile_option_assert
};

ML_STATIC_ASSERT(env_check_version_major < (1<<10));
ML_STATIC_ASSERT(env_check_version_minor < (1<<10));
ML_STATIC_ASSERT(env_check_version_patch < (1<<10));

}	// namespace internal

///	@endcond

///	@brief									環境の妥当性を判定する
///	@param[in]	version						内部で使用
///	@param[in]	flag						内部で使用
///	@param[in]	struct_menber_offset_i8		内部で使用
///	@param[in]	struct_menber_offset_i16	内部で使用
///	@param[in]	struct_menber_offset_i32	内部で使用
///	@param[in]	struct_menber_offset_i64	内部で使用
///	@param[in]	struct_menber_offset_f32	内部で使用
///	@param[in]	struct_menber_offset_f64	内部で使用
///	@return									問題なければ true
///	@attention								アサート有効時、環境に問題があるときは停止します
///	@note									引数はデフォルト引数を指定してください
bool check_environment(
	internal::env_check_const version	= internal::env_check_version,
	internal::env_check_const flag		= internal::env_check_compile_option,
	int	struct_menber_offset_i8			= offsetof(internal::struct_menber_offset, ui8_),
	int	struct_menber_offset_i16		= offsetof(internal::struct_menber_offset, ui16_),
	int	struct_menber_offset_i32		= offsetof(internal::struct_menber_offset, ui32_),
	int	struct_menber_offset_i64		= offsetof(internal::struct_menber_offset, ui64_),
	int	struct_menber_offset_f32		= offsetof(internal::struct_menber_offset, f32_),
	int	struct_menber_offset_f64		= offsetof(internal::struct_menber_offset, f64_)
);

///	@}

///	@}

}	// namespace ml

#endif	// #ifndef LIBMATCHLOCK_INC_ML_UTILITY_ML_ENVIRONMENT_CHECK_H
