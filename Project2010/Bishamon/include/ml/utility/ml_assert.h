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

	///	@brief				アサート
	///	@param[in]	cond	判定
	///	@note				判定がfalseの時に停止します
	#define	ML_ASSERT(cond)					assert(cond)

	///	@brief				アサート用リザルト値
	#define	ML_ASSERT_RESULT(TYPE, NAME)	TYPE NAME =

#else	// #if defined(ML_ASSERT_ENABLE)

	///	@brief				アサート
	///	@param[in]	cond	判定
	///	@note				判定がfalseの時に停止します
	#define	ML_ASSERT(cond)					((void)0)

	///	@brief				アサート用リザルト値
	#define	ML_ASSERT_RESULT(TYPE, NAME)

#endif // #if defined(ML_ASSERT_ENABLE)


///	@brief				メッセージ付きアサート
///	@param[in]	cond	判定
///	@param[in]	message	メッセージ
///	@note				判定がfalseの時にメッセージを出力して停止します
#define	ML_ASSERT_MESSAGE(cond, message)	ML_ASSERT((cond) && ((bool)message))

///	@brief				ポインターアサート
///	@param[in]	ptr		入力ポインタ
///	@note				入力ポインタがポインタとして不正な時に停止します
#define	ML_PTR_ASSERT(ptr)					ML_ASSERT((ptr) != NULL)

///	@brief				メッセージ付きポインターアサート
///	@param[in]	ptr		入力ポインタ
///	@param[in]	message	メッセージ
///	@note				入力ポインタがポインタとして不正な時に停止します
#define	ML_PTR_ASSERT_MESSAGE(ptr, message)	ML_ASSERT_MESSAGE((ptr) != NULL, message)

///	@brief				静的アサート
///	@param[in]	cond	判定
///	@note				コンパイル時、判定がfalseであればコンパイルエラーになります
#define	ML_STATIC_ASSERT(cond)				typedef int ML_INTERNAL_ASSERT_PP_CAT(ML_STATIC_ASSERT, __LINE__)[::ml::internal::ml_static_assert<(cond)>::ml_static_assert_was_failed]

///	@cond internal

namespace internal{

#define ML_INTERNAL_ASSERT_PP_CAT(a, b)				ML_INTERNAL_ASSERT_PP_CAT_INTERNAL(a, b)
#define ML_INTERNAL_ASSERT_PP_CAT_INTERNAL(a, b)	a##b

// 参考:	http://cppemb.blog17.fc2.com/blog-entry-27.html

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
