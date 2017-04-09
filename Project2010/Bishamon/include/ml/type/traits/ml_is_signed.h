#ifndef LIBMATCHLOCK_INC_ML_TYPE_TRAITS_ML_IS_SIGNED_H
#define LIBMATCHLOCK_INC_ML_TYPE_TRAITS_ML_IS_SIGNED_H

#include "internal/ml_type_traits_internal.h"
#include "../ml_basic.h"

namespace ml{

///	@addtogroup	type
///	@{

///	@addtogroup	type_trait
///	@{

///	@brief	ïÑçÜÇ†ÇËå^Ç©Ç«Ç§Ç©ÇîªíËÇ∑ÇÈ
ML_INTERNAL_BOOL_TRAIT(is_signed);

///	@cond internal

ML_INTERNAL_BOOL_TRAIT_TRUE(is_signed, signed char);
ML_INTERNAL_BOOL_TRAIT_TRUE(is_signed, signed short);
ML_INTERNAL_BOOL_TRAIT_TRUE(is_signed, signed int);
ML_INTERNAL_BOOL_TRAIT_TRUE(is_signed, signed long);
ML_INTERNAL_BOOL_TRAIT_TRUE(is_signed, si64);

///	@endcond

///	@}

///	@}

}	// namespace ml

#endif	// #ifndef LIBMATCHLOCK_INC_ML_TYPE_TRAITS_ML_IS_SIGNED_H
