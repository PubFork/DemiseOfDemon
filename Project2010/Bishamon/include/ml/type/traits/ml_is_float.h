#ifndef LIBMATCHLOCK_INC_ML_TYPE_TRAITS_ML_IS_FLOAT_H
#define LIBMATCHLOCK_INC_ML_TYPE_TRAITS_ML_IS_FLOAT_H

#include "internal/ml_type_traits_internal.h"
#include "../ml_basic.h"

namespace ml{

///	@addtogroup	type
///	@{

///	@addtogroup	type_trait
///	@{

///	@brief	ïÇìÆè¨êîílå^Ç©Ç«Ç§Ç©ÇîªíËÇ∑ÇÈ
ML_INTERNAL_BOOL_TRAIT(is_float);

///	@cond internal

ML_INTERNAL_BOOL_TRAIT_TRUE(is_float, float);
ML_INTERNAL_BOOL_TRAIT_TRUE(is_float, double);

///	@endcond

///	@}

///	@}

}	// namespace ml

#endif	// #ifndef LIBMATCHLOCK_INC_ML_TYPE_TRAITS_ML_IS_FLOAT_H
