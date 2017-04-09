#ifndef BM3_SDK_INC_BM3_SYSTEM_BM3_VERSION_H
#define BM3_SDK_INC_BM3_SYSTEM_BM3_VERSION_H

#include "bm3_Config.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

///	@addtogroup	system
///	@{

///	@addtogroup	system_version
///	@{

enum{
	VersionMajor	= 0,		///<バージョン番号（メジャー）
	VersionMinor	= 1,		///<バージョン番号（マイナー）
	VersionPatch	= 0			///<バージョン番号（パッチ）
};

///	@}

///	@}

BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_SYSTEM_BM3_VERSION_H
