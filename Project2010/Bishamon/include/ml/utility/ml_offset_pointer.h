#ifndef LIBMATCHLOCK_INC_ML_UTILITY_ML_POINTER_H
#define LIBMATCHLOCK_INC_ML_UTILITY_ML_POINTER_H

#include "ml_assert.h"
#include "../type/traits/ml_is_interger.h"
#include "../type/ml_basic.h"

namespace ml{

///	@addtogroup	utility
///	@{

///	@addtogroup	utility_pointer
///	@{

///	@brief				�I�t�Z�b�g�����Z�����|�C���^�����߂�
///	@param[in]	pointer	�|�C���^
///	@param[in]	offset	�I�t�Z�b�g
///	@return				�I�t�Z�b�g�����Z�����|�C���^
template<
	typename DstPtrType,
	typename SrcPtrType,
	typename OffsetType
>
DstPtrType offset_pointer(SrcPtrType pointer, OffsetType offset){

	ML_STATIC_ASSERT(ml::is_interger<OffsetType>::value);

	DstPtrType p =
		reinterpret_cast<DstPtrType>(
			reinterpret_cast<OffsetType>(pointer) + offset
		);

	ML_PTR_ASSERT(p);

	return p;
}


///	@brief						�I�t�Z�b�g�����Z�����|�C���^�����߂�
///	@param[in]	pointer			�|�C���^
///	@param[in]	offset			�I�t�Z�b�g
///	@param[in]	invalid_offset	�����ȃI�t�Z�b�g
///	@param[in]	invalid_pointer	�����ȃ|�C���^
///	@return						�I�t�Z�b�g�����Z�����|�C���^�B(offset == invalid_offset)����invalid_pointer
template<
	typename DstPtrType,
	typename SrcPtrType,
	typename OffsetType
>
DstPtrType offset_pointer(SrcPtrType pointer, OffsetType offset, OffsetType invalid_offset, DstPtrType invalid_pointer){

	return
		(offset == invalid_offset) ?
			invalid_pointer :
			offset_pointer<DstPtrType, SrcPtrType, OffsetType>(pointer, offset);
}


///	@}

///	@}

}	// namespace ml

#endif	// #ifndef LIBMATCHLOCK_INC_ML_UTILITY_ML_POINTER_H
