#ifndef LIBMATCHLOCK_INC_ML_UTILITY_ML_PREPROCESSOR_H
#define LIBMATCHLOCK_INC_ML_UTILITY_ML_PREPROCESSOR_H

namespace ml{

///	@addtogroup	utility
///	@{

///	@addtogroup	utility_preprocessor
///	@{

///	@brief			文字列化
///	@param[in]	a	トークン
///	@return			文字列
#define	ML_PP_STR(a)				ML_PP_STR_INTERNAL(a)

///	@brief			ワイド文字列化
///	@param[in]	a	トークン
///	@return			ワイド文字列
#define	ML_PP_WSTR(a)				ML_PP_WIDESTRING(ML_PP_STR(a))

///	@brief			トークン結合
///	@param[in]	a	トークン
///	@param[in]	b	トークン
///	@return			トークン
#define	ML_PP_CAT(a, b)				ML_PP_CAT_INTERNAL(a, b)

///	@brief			文字列リテラルをワイド文字列リテラルにする
///	@param[in]	a	文字列
///	@return			ワイド文字列
#define	ML_PP_WIDESTRING(a)			ML_PP_CAT(L, a)

///	@brief				ユニークなIDを求める
///	@param[in]	base	入力トークン
///	@return				トークン
#define	ML_PP_UNIQUE_ID(base)		ML_PP_CAT(base, __LINE__)

///	@cond internal

namespace internal{

#define	ML_PP_STR_INTERNAL(a)		#a
#define	ML_PP_CAT_INTERNAL(a, b)	a##b

}	// namespace internal

///	@endcond

///	@}

///	@}

}	// namespace ml

#endif	// #ifndef LIBMATCHLOCK_INC_ML_UTILITY_ML_PREPROCESSOR_H
