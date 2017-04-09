#ifndef BM3_SDK_INC_SDK_INC_BM3_CORE_TEXTURE_BM3_TEX_MOVIEHANDLE_H
#define BM3_SDK_INC_SDK_INC_BM3_CORE_TEXTURE_BM3_TEX_MOVIEHANDLE_H

#if defined(BM3_PLATFORM_win_opengl)
	#include "!opengl/bm3_tex_MovieHandle.h"
#endif
#if defined(BM3_PLATFORM_win_directx9)
	#include "!directx9/bm3_tex_MovieHandle.h"
#endif
#if defined(BM3_PLATFORM_win_directx10)
	#include "!directx10/bm3_tex_MovieHandle.h"
#endif
#if defined(BM3_PLATFORM_playstation3)
//	#include "!playstation3/bm3_tex_MovieHandle.h"
#endif
#if defined(BM3_PLATFORM_xbox360)
	#include "!xbox360/bm3_tex_MovieHandle.h"
#endif
#if defined(BM3_PLATFORM_psp)
//	#include "!psp/bm3_tex_MovieHandle.h"
#endif
#if defined(BM3_PLATFORM_gamebryodx9)
//	#include "!gamebryodx9/bm3_tex_MovieHandle.h"
#endif
#if defined(BM3_PLATFORM_opengles)
//	#include "!opengles/bm3_tex_MovieHandle.h"
#endif
#if defined(BM3_PLATFORM_nw4c)
//	#include "!nw4c/bm3_tex_MovieHandle.h"
#endif
#if defined(BM3_PLATFORM_psvita)
//	#include "!psvita/bm3_tex_MovieHandle.h"
#endif

#endif	// BM3_SDK_INC_SDK_INC_BM3_CORE_TEXTURE_BM3_TEX_MOVIEHANDLE_H
