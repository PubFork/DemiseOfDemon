#ifndef BM3_SDK_INC_BM3_CORE_BM3_SHADER_H
#define BM3_SDK_INC_BM3_CORE_BM3_SHADER_H

#if defined(BM3_PLATFORM_win_opengl)
#if defined(BM3_PLATFORM_opengl_glsl)
	#include "!opengl/bm3_Shader.h"
#endif
#endif
#if defined(BM3_PLATFORM_win_directx9)
	#include "!directx9/bm3_Shader.h"
#endif
#if defined(BM3_PLATFORM_win_directx10)
	#include "!directx10/bm3_Shader.h"
#endif
#if defined(BM3_PLATFORM_playstation3)
	#include "!playstation3/bm3_Shader.h"
#endif
#if defined(BM3_PLATFORM_xbox360)
	#include "!xbox360/bm3_Shader.h"
#endif
#if defined(BM3_PLATFORM_psp)
//	#include "!psp/bm3_Shader.h"
#endif
#if defined(BM3_PLATFORM_gamebryodx9)
	#include "!gamebryodx9/bm3_Shader.h"
#endif
#if defined(BM3_PLATFORM_opengles)
	#include "!opengles/bm3_Shader.h"
#endif
#if defined(BM3_PLATFORM_nw4c)
	#include "!nw4c/bm3_Shader.h"
#endif
#if defined(BM3_PLATFORM_psvita)
	#include "!psvita/bm3_Shader.h"
#endif

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_BM3_SHADER_H
