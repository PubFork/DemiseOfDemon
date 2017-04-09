#ifndef LIBMATCHLOCK_INC_ML_UTILITY_ML_CAST_H
#define LIBMATCHLOCK_INC_ML_UTILITY_ML_CAST_H

#include "ml_assert.h"
#include "ml/type/traits/ml_is_numeric.h"

namespace ml{

///	@addtogroup	utility
///	@{

///	@addtogroup	utility_cast
///	@{

///	@brief				’l‚ÌŒ^•ÏŠ·
///	@param[in]	src		“ü—Í’l
///	@return				•ÏŠ·Œã‚Ì’l
///	@note				DstType‚É‚µ‚½‚¢•ÏŠ·Œ^‚ðŽw’è‚·‚é
template<typename DstType, typename SrcType>
inline DstType numeric_cast(SrcType src){

	ML_STATIC_ASSERT(ml::is_numeric<SrcType>::value);
	ML_STATIC_ASSERT(ml::is_numeric<DstType>::value);

	return static_cast<DstType>(src);
}

///	@}

///	@}

}	// namespace ml

#endif	// #ifndef LIBMATCHLOCK_INC_ML_UTILITY_ML_CAST_H
