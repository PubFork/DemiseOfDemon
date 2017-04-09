#ifndef LIBMATCHLOCK_INC_MATH_ML_MATH_CONST_H
#define LIBMATCHLOCK_INC_MATH_ML_MATH_CONST_H

namespace ml{

///	@addtogroup	math
///	@{

///	@addtogroup	math_constant
///	@{

static const float	pi			= 3.14159265358979323846f;			///<円周率
static const float	pi2			= 3.14159265358979323846f*2.0f;		///<円周率x2
static const float	sqrt2		= 1.41421356237309504880f;			///<√2
static const float	sqrt3		= 1.73205080756887729352f;			///<√3

static const float	degree0		= 0.0f;			///<0度
static const float	degree45	= pi / 4.0f;	///<45度
static const float	degree90	= pi / 2.0f;	///<90度
static const float	degree180	= pi;			///<180度
static const float	degree270	= pi * 1.5f;	///<270度
static const float	degree360	= pi2;			///<360度

static const float	inverse3	= 0.33333333333333333333f;			/// 1/3

///	@}

///	@}

}	// namespace ml

#endif	// #ifndef LIBMATCHLOCK_INC_MATH_ML_MATH_CONST_H
