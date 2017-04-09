#ifndef LIBMATCHLOCK_INC_ML_UTILITY_ML_DEBUG_H
#define LIBMATCHLOCK_INC_ML_UTILITY_ML_DEBUG_H

#if defined(WIN32)
#include <windows.h>
#endif

namespace ml{

///	@addtogroup	debug
///	@{

///	@addtogroup	utility_debug
///	@{

#if defined(_DEBUG) && defined(WIN32)
	///	@brief				�����[�X�łł��o���������b�Z�[�W
	///	@param[in]	arg		���b�Z�[�W
	#define ML_FORCE_DEBUG_MESSAGE(arg)	OutputDebugStringA(arg)
#elif defined(SPU)
	///	@brief				�����[�X�łł��o���������b�Z�[�W
	///	@param[in]	arg		���b�Z�[�W
	#define ML_FORCE_DEBUG_MESSAGE(arg)
#else
	///	@brief				�����[�X�łł��o���������b�Z�[�W
	///	@param[in]	arg		���b�Z�[�W
	#define ML_FORCE_DEBUG_MESSAGE(arg)	printf(arg)
#endif

#if defined(_DEBUG) && defined(WIN32)
	///	@brief				�f�o�b�O���b�Z�[�W
	///	@param[in]	arg		���b�Z�[�W
	#define ML_DEBUG_MESSAGE(arg)	OutputDebugStringA(arg)
#else
	///	@brief				�f�o�b�O���b�Z�[�W
	///	@param[in]	arg		���b�Z�[�W
	#define ML_DEBUG_MESSAGE(arg)
#endif
	

///	@}

///	@}

}	// namespace ml

#endif	// #ifndef LIBMATCHLOCK_INC_ML_UTILITY_ML_DEBUG_H
