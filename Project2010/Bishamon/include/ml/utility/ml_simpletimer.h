#ifndef LIBMATCHLOCK_INC_ML_UTILITY_ML_SIMPLETIMER_H
#define LIBMATCHLOCK_INC_ML_UTILITY_ML_SIMPLETIMER_H

#include <string>

#if defined(WIN32)
#include <windows.h>
#elif defined(BM3_PLATFORM_psp)
#include <kernel.h>
#include <psptypes.h>
#endif

namespace ml{

///	@addtogroup	timer
///	@{

///	@addtogroup	utility_timer
///	@{

class simpletimerimpl;

///	@brief				コンストラクタで開始し、デストラクタで終了するだけの簡単なタイマー
class simpletimer {
public:
	simpletimer(const char* func_name);	
	~simpletimer();

private:
	simpletimerimpl *impl;

};
	
///	@}

///	@}

}	// namespace ml


#define ML_UTILITY_TIMER	ml::simpletimer	ml_timer(__FUNCTION__);

#if defined (WIN32)
#define DEBUG_OUTPUT(msg)		OutputDebugStringA(msg);
#endif
#if defined (BM3_PLATFORM_psp)
#define DEBUG_OUTPUT(msg)		printf(msg);
#endif



#ifdef BM3_TIMER_ENABLE
#define ML_TIMER	ML_UTILITY_TIMER
#else
#define ML_TIMER
#endif

#ifdef BM3_TIMER_ENABLE_INITIALIZE
#define ML_INITIALIZE_TIMER	ML_UTILITY_TIMER
#define ML_INITIALIZE_TIMER_SEP		DEBUG_OUTPUT("------------------------------------ init\n");
#else
#define ML_INITIALIZE_TIMER
#define ML_INITIALIZE_TIMER_SEP
#endif

#ifdef BM3_TIMER_ENABLE_UPDATE
#define ML_UPDATE_TIMER	ML_UTILITY_TIMER
#define ML_UPDATE_TIMER_SEP		DEBUG_OUTPUT("------------------------------------ update\n");
#else
#define ML_UPDATE_TIMER
#define ML_UPDATE_TIMER_SEP
#endif

#ifdef BM3_TIMER_ENABLE_MAKEVERTEX
#define ML_MAKEVERTEX_TIMER	ML_UTILITY_TIMER
#define ML_MAKEVERTEX_TIMER_SEP		DEBUG_OUTPUT("------------------------------------ make vertex\n");
#else
#define ML_MAKEVERTEX_TIMER
#define ML_MAKEVERTEX_TIMER_SEP
#endif

#ifdef BM3_TIMER_ENABLE_DRAW
#define ML_DRAWTIMER	ML_UTILITY_TIMER
#define ML_DRAWTIMER_SEP		DEBUG_OUTPUT("------------------------------------\n");
#else
#define ML_DRAWTIMER
#define ML_DRAWTIMER_SEP
#endif



#endif	// #ifndef LIBMATCHLOCK_INC_ML_UTILITY_ML_SIMPLETIMER_H
