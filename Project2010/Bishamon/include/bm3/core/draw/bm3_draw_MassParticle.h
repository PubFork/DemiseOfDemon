#ifndef BM3_SDK_INC_BM3_CORE_DRAW_BM3_DRAW_MASSPARTICLE_H
#define BM3_SDK_INC_BM3_CORE_DRAW_BM3_DRAW_MASSPARTICLE_H

#if defined(BM3_PLATFORM_win_opengl)
#ifdef BM3_PLATFORM_opengl_glsl
	#include "bm3_draw_MassParticleImpl.h"
#else
// ↓マスパーティクルの実装が無いプラットフォーム用のコード
	#include "bm3_draw_NonMassParticle.h"
#endif
#elif defined(BM3_PLATFORM_win_directx9)
	#include "!directx9/bm3_draw_MassParticle.h"
#elif defined(BM3_PLATFORM_win_directx10)
	#include "!directx10/bm3_draw_MassParticle.h"
#elif defined(BM3_PLATFORM_playstation3)
	#include "!playstation3/bm3_draw_MassParticle.h"
#elif defined(BM3_PLATFORM_xbox360)
	#include "!xbox360/bm3_draw_MassParticle.h"
#elif defined(BM3_PLATFORM_psp)
	#include "bm3_draw_NonMassParticle.h"
#elif defined(BM3_PLATFORM_gamebryodx9)
	#include "bm3_draw_NonMassParticle.h"
#elif defined(BM3_PLATFORM_opengles)
	#include "bm3_draw_NonMassParticle.h"
#elif defined(BM3_PLATFORM_psvita)
	#include "bm3_draw_MassParticleImpl.h"
#else
// ↓マスパーティクルの実装が無いプラットフォーム用のコード
	#include "bm3_draw_NonMassParticle.h"
#endif

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_DRAW_BM3_DRAW_MASSPARTICLE_H
