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

///	@brief			�T�C���l�����߂�
///	@param[in]	a	���͒l(�P�ʁF���W�A��)
///	@return			�T�C���l
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

///	@brief			�R�T�C���l�����߂�
///	@param[in]	a	���͒l(�P�ʁF���W�A��)
///	@return			�R�T�C���l
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

///	@brief			�^���W�F���g�l�����߂�
///	@param[in]	a	���͒l(�P�ʁF���W�A��)
///	@return			�^���W�F���g�l
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

///	@brief				�T�C���l�R�T�C���l�����߂�
///	@param[in]		a	���͒l(�P�ʁF���W�A��)
///	@param[out]	dst_s	�T�C���l
///	@param[out]	dst_c	�R�T�C���l
inline void  sin_cos(float a, float &dst_s, float &dst_c){

	dst_s = sinf(a);
	dst_c = cosf(a);
}

///	@brief			�A�[�N�^���W�F���g�l�����߂�
///	@param[in]	a	���͒l
///	@return			�A�[�N�^���W�F���g�l
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

///	@brief			�A�[�N�^���W�F���g�l�����߂�
///	@param[in]	b	���͒l
///	@param[in]	a	���͒l
///	@return			�A�[�N�^���W�F���g�l
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

///	@brief			�A�[�N�T�C���l�����߂�
///	@param[in]	a	���͒l
///	@return			�A�[�N�T�C���l
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

///	@brief			�A�[�N�R�T�C�������߂�
///	@param[in]	a	���͒l
///	@return			�A�[�N�R�T�C���l
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

///	@brief			�T�C���l�����߂�
///	@param[in]	a	���͒l(�P�ʁF���W�A��)
///	@return			�T�C���l
inline float sinf(float a){

	return ml::sinf(a);
}

///	@brief			�R�T�C���l�����߂�
///	@param[in]	a	���͒l(�P�ʁF���W�A��)
///	@return			�R�T�C���l
inline float cosf(float a){

	return ml::cosf(a);
}

///	@brief			�^���W�F���g�l�����߂�
///	@param[in]	a	���͒l(�P�ʁF���W�A��)
///	@return			�^���W�F���g�l
inline float tanf(float a){

	return ml::tanf(a);
}

///	@brief				�T�C���l�R�T�C���l�����߂�
///	@param[in]		a	���͒l(�P�ʁF���W�A��)
///	@param[out]	dst_s	�T�C���l
///	@param[out]	dst_c	�R�T�C���l
inline void  sin_cos(float a, float &dst_s, float &dst_c){

	return ml::sin_cos(a, dst_s, dst_c);
}

///	@brief			�A�[�N�^���W�F���g�l�����߂�
///	@param[in]	a	���͒l
///	@return			�A�[�N�^���W�F���g�l
inline float atanf(float a){

	return ml::atanf(a);
}

///	@brief			�A�[�N�^���W�F���g�l�����߂�
///	@param[in]	b	���͒l
///	@param[in]	a	���͒l
///	@return			�A�[�N�^���W�F���g�l
inline float atan2f(float b, float a){

	return ml::atan2f(b, a);
}

///	@brief			�A�[�N�T�C���l�����߂�
///	@param[in]	a	���͒l
///	@return			�A�[�N�T�C���l
inline float asinf(float a){

	return ml::asinf(a);
}

///	@brief			�A�[�N�R�T�C�������߂�
///	@param[in]	a	���͒l
///	@return			�A�[�N�R�T�C���l
inline float acosf(float a){

	return ml::acosf(a);
}

}	// namespace fast

///	@}

///	@}

}	// namespace ml

#endif	// #ifndef LIBMATCHLOCK_INC_MATH_ML_TRIANGULAR_H
