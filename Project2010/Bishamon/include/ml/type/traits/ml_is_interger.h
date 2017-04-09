#ifndef LIBMATCHLOCK_INC_ML_TYPE_TRAITS_ML_IS_INTERGER_H
#define LIBMATCHLOCK_INC_ML_TYPE_TRAITS_ML_IS_INTERGER_H

#include "internal/ml_type_traits_internal.h"
#include "../ml_basic.h"

namespace ml{

///	@addtogroup	type
///	@{

///	@addtogroup	type_trait
///	@{

///	@brief	êÆêîílå^Ç©Ç«Ç§Ç©ÇîªíËÇ∑ÇÈ
ML_INTERNAL_BOOL_TRAIT(is_interger);

///	@cond internal

ML_INTERNAL_BOOL_TRAIT_TRUE(is_interger, char);
ML_INTERNAL_BOOL_TRAIT_TRUE(is_interger, signed char);
ML_INTERNAL_BOOL_TRAIT_TRUE(is_interger, unsigned char);

ML_INTERNAL_BOOL_TRAIT_TRUE(is_interger, signed short);
ML_INTERNAL_BOOL_TRAIT_TRUE(is_interger, unsigned short);

ML_INTERNAL_BOOL_TRAIT_TRUE(is_interger, signed int);
ML_INTERNAL_BOOL_TRAIT_TRUE(is_interger, unsigned int);

ML_INTERNAL_BOOL_TRAIT_TRUE(is_interger, signed long);
ML_INTERNAL_BOOL_TRAIT_TRUE(is_interger, unsigned long);

ML_INTERNAL_BOOL_TRAIT_TRUE(is_interger, si64);
ML_INTERNAL_BOOL_TRAIT_TRUE(is_interger, ui64);

///	@endcond

///	@}

///	@}

}	// namespace ml

#endif	// #ifndef LIBMATCHLOCK_INC_ML_TYPE_TRAITS_ML_IS_INTERGER_H
