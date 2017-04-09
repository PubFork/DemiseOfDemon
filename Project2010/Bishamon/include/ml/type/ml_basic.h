#ifndef LIBMATCHLOCK_INC_ML_TYPE_ML_BASIC_H
#define LIBMATCHLOCK_INC_ML_TYPE_ML_BASIC_H

#include "../utility/ml_assert.h"

namespace ml{

///	@addtogroup	type
///	@{

///	@addtogroup	type_basic
///	@{

typedef signed char		si8;			///<8ビット符号あり整数
typedef unsigned char	ui8;			///<8ビット符号なし整数

typedef signed short	si16;			///<16ビット符号あり整数
typedef unsigned short	ui16;			///<16ビット符号なし整数

#ifdef BM3_64
typedef signed int		si32;
typedef unsigned int	ui32;
#else
typedef signed long		si32;			///<32ビット符号あり整数
typedef unsigned long	ui32;			///<32ビット符号なし整数
#endif

#if defined(_MSC_VER)
	typedef signed __int64		si64;	///<64ビット符号あり整数
	typedef unsigned __int64	ui64;	///<64ビット符号なし整数
#elif defined(__MWERKS__)
	typedef signed long long	si64;	///<64ビット符号あり整数
	typedef unsigned long long	ui64;	///<63ビット符号なし整数
#elif defined(__GNUC__)
	typedef signed long long	si64;	///<64ビット符号あり整数
	typedef unsigned long long	ui64;	///<63ビット符号なし整数
#elif defined(__SNC__)
	typedef signed long long	si64;	///<64ビット符号あり整数
	typedef unsigned long long	ui64;	///<63ビット符号なし整数
#else
///	@cond internal
	ML_STATIC_ASSERT(false);
///	@endcond
#endif

typedef float			f32;			///<32ビット浮動小数点
typedef double			f64;			///<64ビット浮動小数点

typedef	ui8		min_size_basic_type;	///<最小サイズ型
typedef	ui64	max_size_basic_type;	///<最大サイズ型

#ifdef BM3_64
typedef si64	ptr_size_si;
typedef ui64	ptr_size_ui;
#else
typedef	si32	ptr_size_si;	///<ポインタが収まる符号あり整数
typedef	ui32	ptr_size_ui;	///<ポインタが収まる符号なし整数
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
