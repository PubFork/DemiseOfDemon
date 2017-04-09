#ifndef LIBMATCHLOCK_INC_ML_TYPE_TRAITS_ML_IS_UNSIGNED_H
#define LIBMATCHLOCK_INC_ML_TYPE_TRAITS_ML_IS_UNSIGNED_H

#include "internal/ml_type_traits_internal.h"
#include "../ml_basic.h"

namespace ml{

///	@addtogroup	type
///	@{

///	@addtogroup	type_trait
///	@{

///	@brief	ïÑçÜÇ»Çµå^Ç©Ç«Ç§Ç©ÇîªíËÇ∑ÇÈ
ML_INTERNAL_BOOL_TRAIT(is_unsigned);

///	@cond internal

ML_INTERNAL_BOOL_TRAIT_TRUE(is_unsigned, unsigned char);
ML_INTERNAL_BOOL_TRAIT_TRUE(is_unsigned, unsigned short);
ML_INTERNAL_BOOL_TRAIT_TRUE(is_unsigned, unsigned int);
ML_INTERNAL_BOOL_TRAIT_TRUE(is_unsigned, unsigned long);
ML_INTERNAL_BOOL_TRAIT_TRUE(is_unsigned, ui64);

///	@endcond

///	@}

///	@}

}	// namespace ml

#endif	// #ifndef LIBMATCHLOCK_INC_ML_TYPE_TRAITS_ML_IS_UNSIGNED_H
