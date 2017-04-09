#ifndef LIBMATCHLOCK_INC_ML_TYPE_TRAITS_ML_IS_NUMERIC_H
#define LIBMATCHLOCK_INC_ML_TYPE_TRAITS_ML_IS_NUMERIC_H

#include "internal/ml_type_traits_internal.h"
#include "../ml_basic.h"

namespace ml{

///	@addtogroup	type
///	@{

///	@addtogroup	type_trait
///	@{

///	@brief	êîílå^Ç©Ç«Ç§Ç©ÇîªíËÇ∑ÇÈ
ML_INTERNAL_BOOL_TRAIT(is_numeric);

///	@cond internal

ML_INTERNAL_BOOL_TRAIT_TRUE(is_numeric, char);
ML_INTERNAL_BOOL_TRAIT_TRUE(is_numeric, signed char);
ML_INTERNAL_BOOL_TRAIT_TRUE(is_numeric, unsigned char);

ML_INTERNAL_BOOL_TRAIT_TRUE(is_numeric, signed short);
ML_INTERNAL_BOOL_TRAIT_TRUE(is_numeric, unsigned short);

ML_INTERNAL_BOOL_TRAIT_TRUE(is_numeric, signed int);
ML_INTERNAL_BOOL_TRAIT_TRUE(is_numeric, unsigned int);

ML_INTERNAL_BOOL_TRAIT_TRUE(is_numeric, signed long);
ML_INTERNAL_BOOL_TRAIT_TRUE(is_numeric, unsigned long);

ML_INTERNAL_BOOL_TRAIT_TRUE(is_numeric, si64);
ML_INTERNAL_BOOL_TRAIT_TRUE(is_numeric, ui64);

ML_INTERNAL_BOOL_TRAIT_TRUE(is_numeric, float);
ML_INTERNAL_BOOL_TRAIT_TRUE(is_numeric, double);

///	@endcond

///	@}

///	@}

}	// namespace ml

#endif	// #ifndef LIBMATCHLOCK_INC_ML_TYPE_TRAITS_ML_IS_NUMERIC_H
