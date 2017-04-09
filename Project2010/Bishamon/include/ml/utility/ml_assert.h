#ifndef LIBMATCHLOCK_INC_ML_UTILITY_ML_ASSERT_H
#define LIBMATCHLOCK_INC_ML_UTILITY_ML_ASSERT_H

#include <cassert>
#include <stddef.h>			// NULL

namespace ml{

///	@addtogroup	utility
///	@{

///	@addtogroup	utility_assert
///	@{

#if defined(ML_ASSERT_ENABLE)

	///	@brief				�A�T�[�g
	///	@param[in]	cond	����
	///	@note				���肪false�̎��ɒ�~���܂�
	#define	ML_ASSERT(cond)					assert(cond)

	///	@brief				�A�T�[�g�p���U���g�l
	#define	ML_ASSERT_RESULT(TYPE, NAME)	TYPE NAME =

#else	// #if defined(ML_ASSERT_ENABLE)

	///	@brief				�A�T�[�g
	///	@param[in]	cond	����
	///	@note				���肪false�̎��ɒ�~���܂�
	#define	ML_ASSERT(cond)					((void)0)

	///	@brief				�A�T�[�g�p���U���g�l
	#define	ML_ASSERT_RESULT(TYPE, NAME)

#endif // #if defined(ML_ASSERT_ENABLE)


///	@brief				���b�Z�[�W�t���A�T�[�g
///	@param[in]	cond	����
///	@param[in]	message	���b�Z�[�W
///	@note				���肪false�̎��Ƀ��b�Z�[�W���o�͂��Ē�~���܂�
#define	ML_ASSERT_MESSAGE(cond, message)	ML_ASSERT((cond) && ((bool)message))

///	@brief				�|�C���^�[�A�T�[�g
///	@param[in]	ptr		���̓|�C���^
///	@note				���̓|�C���^���|�C���^�Ƃ��ĕs���Ȏ��ɒ�~���܂�
#define	ML_PTR_ASSERT(ptr)					ML_ASSERT((ptr) != NULL)

///	@brief				���b�Z�[�W�t���|�C���^�[�A�T�[�g
///	@param[in]	ptr		���̓|�C���^
///	@param[in]	message	���b�Z�[�W
///	@note				���̓|�C���^���|�C���^�Ƃ��ĕs���Ȏ��ɒ�~���܂�
#define	ML_PTR_ASSERT_MESSAGE(ptr, message)	ML_ASSERT_MESSAGE((ptr) != NULL, message)

///	@brief				�ÓI�A�T�[�g
///	@param[in]	cond	����
///	@note				�R���p�C�����A���肪false�ł���΃R���p�C���G���[�ɂȂ�܂�
#define	ML_STATIC_ASSERT(cond)				typedef int ML_INTERNAL_ASSERT_PP_CAT(ML_STATIC_ASSERT, __LINE__)[::ml::internal::ml_static_assert<(cond)>::ml_static_assert_was_failed]

///	@cond internal

namespace internal{

#define ML_INTERNAL_ASSERT_PP_CAT(a, b)				ML_INTERNAL_ASSERT_PP_CAT_INTERNAL(a, b)
#define ML_INTERNAL_ASSERT_PP_CAT_INTERNAL(a, b)	a##b

// �Q�l:	http://cppemb.blog17.fc2.com/blog-entry-27.html

template<bool>
struct ml_static_assert{
	enum{
		ml_static_assert_was_failed = 1
	};
};

template<>
struct ml_static_assert<false>{
};

}	// namespace internal

///	@endcond

///	@}

///	@}

}	// namespace ml

#endif	// #ifndef LIBMATCHLOCK_INC_ML_UTILITY_ML_ASSERT_H
