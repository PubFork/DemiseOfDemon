#ifndef LIBMATCHLOCK_INC_ML_TYPE_TRAITS_ML_IS_POINTER_H
#define LIBMATCHLOCK_INC_ML_TYPE_TRAITS_ML_IS_POINTER_H

#include "internal/ml_type_traits_internal.h"
#include "../ml_basic.h"

namespace ml{

///	@addtogroup	type
///	@{

///	@addtogroup	type_trait
///	@{

///	@brief	ポインターかどうかを判定する
ML_INTERNAL_BOOL_TRAIT(is_pointer);

///	@cond internal

ML_INTERNAL_BOOL_TRAIT_TRUE_ARG(is_pointer, T *);
ML_INTERNAL_BOOL_TRAIT_TRUE_ARG(is_pointer, T * const);
ML_INTERNAL_BOOL_TRAIT_TRUE_ARG(is_pointer, T * volatile);
ML_INTERNAL_BOOL_TRAIT_TRUE_ARG(is_pointer, T * const volatile);

///	@endcond

///	@}

///	@}

}	// namespace ml

#endif	// #ifndef LIBMATCHLOCK_INC_ML_TYPE_TRAITS_ML_IS_POINTER_H
