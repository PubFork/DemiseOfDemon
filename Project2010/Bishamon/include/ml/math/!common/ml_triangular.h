#ifndef LIBMATCHLOCK_INC_MATH_ML_TRIANGULAR_H
#define LIBMATCHLOCK_INC_MATH_ML_TRIANGULAR_H

#include <cmath>
#include "../../utility/ml_assert.h"
#include "../../utility/ml_cast.h"
#include "ml_arithmetic.h"

namespace ml{

///	@addtogroup	math
///	@{

///	@addtogroup	math_triangular
///	@{

///	@brief			サイン値を求める
///	@param[in]	a	入力値(単位：ラジアン)
///	@return			サイン値
inline float sinf(float a){

#if defined(_MSC_VER)
	return std::sinf(a);
#elif defined(__GNUC__)
	return numeric_cast<float>(std::sin(numeric_cast<double>(a)));
#elif defined(__SNC__)
	return std::sinf(a);
#else
	ML_STATIC_ASSERT(false);
#endif
}

///	@brief			コサイン値を求める
///	@param[in]	a	入力値(単位：ラジアン)
///	@return			コサイン値
inline float cosf(float a){

#if defined(_MSC_VER)
	return std::cosf(a);
#elif defined(__GNUC__)
	return numeric_cast<float>(std::cos(numeric_cast<double>(a)));
#elif defined(__SNC__)
	return std::cosf(a);
#else
	ML_STATIC_ASSERT(false);
#endif
}

///	@brief			タンジェント値を求める
///	@param[in]	a	入力値(単位：ラジアン)
///	@return			タンジェント値
inline float tanf(float a){

#if defined(_MSC_VER)
	return std::tanf(a);
#elif defined(__GNUC__)
	return numeric_cast<float>(std::tan(numeric_cast<double>(a)));
#elif defined(__SNC__)
	return std::tanf(a);
#else
	ML_STATIC_ASSERT(false);
#endif
}

///	@brief				サイン値コサイン値を求める
///	@param[in]		a	入力値(単位：ラジアン)
///	@param[out]	dst_s	サイン値
///	@param[out]	dst_c	コサイン値
inline void  sin_cos(float a, float &dst_s, float &dst_c){

	dst_s = sinf(a);
	dst_c = cosf(a);
}

///	@brief			アークタンジェント値を求める
///	@param[in]	a	入力値
///	@return			アークタンジェント値
inline float atanf(float a){

#if defined(_MSC_VER)
	return std::atanf(a);
#elif defined(__GNUC__)
	return numeric_cast<float>(std::atan(numeric_cast<double>(a)));
#elif defined(__SNC__)
	return std::atanf(a);
#else
	ML_STATIC_ASSERT(false);
#endif
}

///	@brief			アークタンジェント値を求める
///	@param[in]	b	入力値
///	@param[in]	a	入力値
///	@return			アークタンジェント値
inline float atan2f(float b, float a){

#if defined(_MSC_VER)
	return std::atan2f(b, a);
#elif defined(__GNUC__)
	return numeric_cast<float>(std::atan2(numeric_cast<double>(b), numeric_cast<double>(a)));
#elif defined(__SNC__)
	return std::atan2f(b, a);
#else
	ML_STATIC_ASSERT(false);
#endif
}

///	@brief			アークサイン値を求める
///	@param[in]	a	入力値
///	@return			アークサイン値
inline float asinf(float a){

	ML_ASSERT(is_between(a, -1.0f, +1.0f));

#if defined(_MSC_VER)
	return std::asinf(a);
#elif defined(__GNUC__)
	return numeric_cast<float>(std::asin(numeric_cast<double>(a)));
#elif defined(__SNC__)
	return std::asinf(a);
#else
	ML_STATIC_ASSERT(false);
#endif
}

///	@brief			アークコサインを求める
///	@param[in]	a	入力値
///	@return			アークコサイン値
inline float acosf(float a){

	ML_ASSERT(is_between(a, -1.0f, +1.0f));

#if defined(_MSC_VER)
	return std::acosf(a);
#elif defined(__GNUC__)
	return numeric_cast<float>(std::acos(numeric_cast<double>(a)));
#elif defined(__SNC__)
	return std::acosf(a);
#else
	ML_STATIC_ASSERT(false);
#endif
}

namespace fast{

///	@brief			サイン値を求める
///	@param[in]	a	入力値(単位：ラジアン)
///	@return			サイン値
inline float sinf(float a){

	return ml::sinf(a);
}

///	@brief			コサイン値を求める
///	@param[in]	a	入力値(単位：ラジアン)
///	@return			コサイン値
inline float cosf(float a){

	return ml::cosf(a);
}

///	@brief			タンジェント値を求める
///	@param[in]	a	入力値(単位：ラジアン)
///	@return			タンジェント値
inline float tanf(float a){

	return ml::tanf(a);
}

///	@brief				サイン値コサイン値を求める
///	@param[in]		a	入力値(単位：ラジアン)
///	@param[out]	dst_s	サイン値
///	@param[out]	dst_c	コサイン値
inline void  sin_cos(float a, float &dst_s, float &dst_c){

	return ml::sin_cos(a, dst_s, dst_c);
}

///	@brief			アークタンジェント値を求める
///	@param[in]	a	入力値
///	@return			アークタンジェント値
inline float atanf(float a){

	return ml::atanf(a);
}

///	@brief			アークタンジェント値を求める
///	@param[in]	b	入力値
///	@param[in]	a	入力値
///	@return			アークタンジェント値
inline float atan2f(float b, float a){

	return ml::atan2f(b, a);
}

///	@brief			アークサイン値を求める
///	@param[in]	a	入力値
///	@return			アークサイン値
inline float asinf(float a){

	return ml::asinf(a);
}

///	@brief			アークコサインを求める
///	@param[in]	a	入力値
///	@return			アークコサイン値
inline float acosf(float a){

	return ml::acosf(a);
}

}	// namespace fast

///	@}

///	@}

}	// namespace ml

#endif	// #ifndef LIBMATCHLOCK_INC_MATH_ML_TRIANGULAR_H
