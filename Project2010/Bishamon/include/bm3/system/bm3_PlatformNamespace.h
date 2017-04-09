#ifndef BM3_SDK_INC_BM3_SYSTEM_BM3_PLATFORM_NAMESPACE_H
#define BM3_SDK_INC_BM3_SYSTEM_BM3_PLATFORM_NAMESPACE_H

#include <ml/utility/ml_preprocessor.h>

///	@addtogroup	system
///	@{

///	@addtogroup	system_platform_namespace
///	@{

#if defined(BM3_USE_PLATFORM_NAMESPACE)

	#if !defined(BM3_PLATFORM_NAME)
		// プリプロセッサでBM3_PLATFORM_NAMEを定義する必要がある
		ML_STATIC_ASSERT(BM3_PLATFORM_NAME)
	#endif

	#define	BM3_BEGIN_PLATFORM_NAMESPACE	namespace BM3_PLATFORM_NAME{
	#define	BM3_END_PLATFORM_NAMESPACE		}

	#define	BM3_PLATFORM_NAME_STR			ML_PP_STR(BM3_PLATFORM_NAME)
	#define	BM3_PLATFORM_NAME_WSTR			ML_PP_WSTR(BM3_PLATFORM_NAME)

#else	// #if defined(BM3_USE_PLATFORM_NAMESPACE)

	#define	BM3_BEGIN_PLATFORM_NAMESPACE
	#define	BM3_END_PLATFORM_NAMESPACE

	#define	BM3_PLATFORM_NAME_STR			""
	#define	BM3_PLATFORM_NAME_WSTR			L""

#endif	// #if defined(BM3_USE_PLATFORM_NAMESPACE)


///	@}

///	@}

#endif	// #ifndef BM3_SDK_INC_BM3_SYSTEM_BM3_PLATFORM_NAMESPACE_H
