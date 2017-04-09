#ifndef LIBMATCHLOCK_INC_ML_UTILITY_ML_MAKE_SIGN_H
#define LIBMATCHLOCK_INC_ML_UTILITY_ML_MAKE_SIGN_H

namespace ml{

///	@addtogroup	utility
///	@{

///	@addtogroup	utility_make_sign
///	@{

///	@brief				32ビットの署名を作る
///	@param[in]	A		入力トークン
///	@param[in]	B		入力トークン
///	@param[in]	C		入力トークン
///	@param[in]	D		入力トークン
///	@return				署名
#define	ML_MAKE_SIGN_32(A,B,C,D)	\
	((A)<<(8*3)) |					\
	((B)<<(8*2)) |					\
	((C)<<(8*1)) |					\
	((D)<<(8*0))

///	@brief				32ビットの署名を作る
///	@param[in]	A		入力トークン
///	@param[in]	B		入力トークン
///	@param[in]	C		入力トークン
///	@param[in]	D		入力トークン
///	@return				署名
#define	ML_MAKE_SIGN_R32(A,B,C,D)	\
	((D)<<(8*3)) |					\
	((C)<<(8*2)) |					\
	((B)<<(8*1)) |					\
	((A)<<(8*0))

///	@}

///	@}

}	// namespace ml

#endif	// #ifndef LIBMATCHLOCK_INC_ML_UTILITY_ML_MAKE_SIGN_H
