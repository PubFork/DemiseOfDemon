#ifndef LIBMATCHLOCK_INC_ML_TYPE_ML_STATIC_ALIGN_H
#define LIBMATCHLOCK_INC_ML_TYPE_ML_STATIC_ALIGN_H

#include "../utility/ml_assert.h"

namespace ml{

///	@addtogroup	type
///	@{

///	@addtogroup	type_static_align
///	@{

///	@brief			�ÓI�ɃA���C�������l�����߂�
template<int align, int a>
struct static_align{

///	@cond internal
	ML_STATIC_ASSERT(align >  0);
	ML_STATIC_ASSERT(a     >= 0);
///	@endcond

	enum{
		value = ((a + (align - 1)) / align) * align			///< �A���C�������l
	};
};

///	@}

///	@}

}	// namespace ml

#endif	// #ifndef LIBMATCHLOCK_INC_ML_TYPE_ML_STATIC_ALIGN_H
