#ifndef LIBMATCHLOCK_INC_ML_TYPE_ML_BASIC_H
#define LIBMATCHLOCK_INC_ML_TYPE_ML_BASIC_H

#include "../utility/ml_assert.h"

namespace ml{

///	@addtogroup	type
///	@{

///	@addtogroup	type_basic
///	@{

typedef signed char		si8;			///<8�r�b�g�������萮��
typedef unsigned char	ui8;			///<8�r�b�g�����Ȃ�����

typedef signed short	si16;			///<16�r�b�g�������萮��
typedef unsigned short	ui16;			///<16�r�b�g�����Ȃ�����

#ifdef BM3_64
typedef signed int		si32;
typedef unsigned int	ui32;
#else
typedef signed long		si32;			///<32�r�b�g�������萮��
typedef unsigned long	ui32;			///<32�r�b�g�����Ȃ�����
#endif

#if defined(_MSC_VER)
	typedef signed __int64		si64;	///<64�r�b�g�������萮��
	typedef unsigned __int64	ui64;	///<64�r�b�g�����Ȃ�����
#elif defined(__MWERKS__)
	typedef signed long long	si64;	///<64�r�b�g�������萮��
	typedef unsigned long long	ui64;	///<63�r�b�g�����Ȃ�����
#elif defined(__GNUC__)
	typedef signed long long	si64;	///<64�r�b�g�������萮��
	typedef unsigned long long	ui64;	///<63�r�b�g�����Ȃ�����
#elif defined(__SNC__)
	typedef signed long long	si64;	///<64�r�b�g�������萮��
	typedef unsigned long long	ui64;	///<63�r�b�g�����Ȃ�����
#else
///	@cond internal
	ML_STATIC_ASSERT(false);
///	@endcond
#endif

typedef float			f32;			///<32�r�b�g���������_
typedef double			f64;			///<64�r�b�g���������_

typedef	ui8		min_size_basic_type;	///<�ŏ��T�C�Y�^
typedef	ui64	max_size_basic_type;	///<�ő�T�C�Y�^

#ifdef BM3_64
typedef si64	ptr_size_si;
typedef ui64	ptr_size_ui;
#else
typedef	si32	ptr_size_si;	///<�|�C���^�����܂镄�����萮��
typedef	ui32	ptr_size_ui;	///<�|�C���^�����܂镄���Ȃ�����
#endif

///	@cond internal

ML_STATIC_ASSERT(sizeof(si8)  ==  8 / 8);
ML_STATIC_ASSERT(sizeof(ui8)  ==  8 / 8);
ML_STATIC_ASSERT(sizeof(si16) == 16 / 8);
ML_STATIC_ASSERT(sizeof(ui16) == 16 / 8);
ML_STATIC_ASSERT(sizeof(si32) == 32 / 8);
ML_STATIC_ASSERT(sizeof(ui32) == 32 / 8);
ML_STATIC_ASSERT(sizeof(f32)  == 32 / 8);
ML_STATIC_ASSERT(sizeof(f64)  == 64 / 8);
ML_STATIC_ASSERT(sizeof(si64) == 64 / 8);
ML_STATIC_ASSERT(sizeof(ui64) == 64 / 8);

ML_STATIC_ASSERT(sizeof(ptr_size_si) >= sizeof(si8 *) );
ML_STATIC_ASSERT(sizeof(ptr_size_si) >= sizeof(ui8 *) );
ML_STATIC_ASSERT(sizeof(ptr_size_si) >= sizeof(si16 *));
ML_STATIC_ASSERT(sizeof(ptr_size_si) >= sizeof(ui16 *));
ML_STATIC_ASSERT(sizeof(ptr_size_si) >= sizeof(si32 *));
ML_STATIC_ASSERT(sizeof(ptr_size_si) >= sizeof(ui32 *));
ML_STATIC_ASSERT(sizeof(ptr_size_si) >= sizeof(f32 *) );
ML_STATIC_ASSERT(sizeof(ptr_size_si) >= sizeof(f64 *) );
ML_STATIC_ASSERT(sizeof(ptr_size_si) >= sizeof(si64 *));
ML_STATIC_ASSERT(sizeof(ptr_size_si) >= sizeof(ui64 *));

ML_STATIC_ASSERT(sizeof(ptr_size_ui) >= sizeof(si8 *) );
ML_STATIC_ASSERT(sizeof(ptr_size_ui) >= sizeof(ui8 *) );
ML_STATIC_ASSERT(sizeof(ptr_size_ui) >= sizeof(si16 *));
ML_STATIC_ASSERT(sizeof(ptr_size_ui) >= sizeof(ui16 *));
ML_STATIC_ASSERT(sizeof(ptr_size_ui) >= sizeof(si32 *));
ML_STATIC_ASSERT(sizeof(ptr_size_ui) >= sizeof(ui32 *));
ML_STATIC_ASSERT(sizeof(ptr_size_ui) >= sizeof(f32 *) );
ML_STATIC_ASSERT(sizeof(ptr_size_ui) >= sizeof(f64 *) );
ML_STATIC_ASSERT(sizeof(ptr_size_ui) >= sizeof(si64 *));
ML_STATIC_ASSERT(sizeof(ptr_size_ui) >= sizeof(ui64 *));

ML_STATIC_ASSERT(sizeof(ptr_size_si) >= sizeof(ptr_size_ui));

///	@endcond

///	@}

///	@}

}	// namespace ml

#endif	// #ifndef LIBMATCHLOCK_INC_ML_TYPE_ML_BASIC_H
