#ifndef BM3_SDK_INC_BM3_CORE_DRAW_BM3_TEX_IDEHANDLE_H
#define BM3_SDK_INC_BM3_CORE_DRAW_BM3_TEX_IDEHANDLE_H

#if defined(BM3_PLATFORM_win_opengl)
	#include "!opengl/bm3_tex_IdeHandle.h"
#endif
#if defined(BM3_PLATFORM_win_directx9)
	#include "!directx9/bm3_tex_IdeHandle.h"
#endif

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_DRAW_BM3_TEX_IDEHANDLE_H
