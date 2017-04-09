#ifndef LIBMATCHLOCK_INC_MATH_ML_INTERPOLATE_H
#define LIBMATCHLOCK_INC_MATH_ML_INTERPOLATE_H

#include "../type/ml_vector3d.h"
#include "../type/ml_color.h"

namespace ml{

///	@addtogroup	math
///	@{

///	@addtogroup	math_interpolate
///	@{

///	@brief				���`���
///	@param[in]	rate	����(0.0�`1.0)
///	@param[in]	x0		���͒l
///	@param[in]	x1		���͒l
///	@return				��Ԓl
///	@note				rate��0.0�̎�x0, 1.0�̎�x1��Ԃ�
inline float lerp(float rate, float x0, float x1){

	return x0 + (x1 - x0) * rate;
}

///	@brief				�G���~�[�g���
///	@param[in]	rate	����(0.0�`1.0)
///	@param[in]	x0		���͒l
///	@param[in]	x1		���͒l
///	@param[in]	v0		���͒l(x0�̃x�N�g��)
///	@param[in]	v1		���͒l(x1�̃x�N�g��)
///	@return				��Ԓl
///	@note				rate��0.0�̎�x0, 1.0�̎�x1��Ԃ�
inline float hermite(float rate, float x0, float x1, float v0, float v1){

	float t		= rate;
	float tt	= t * t;
	float ttt	= t * t * t;

	return
		( 2.0f * ttt - 3.0f * tt + 1.0f) * x0 +
		(-2.0f * ttt + 3.0f * tt       ) * x1 +
		(ttt - 2.0f * tt + t) * v0 +
		(ttt - tt           ) * v1;
}

///	@brief				�x�W�F���
///	@param[in]	rate	����(0.0�`1.0)
///	@param[in]	x0		���͒l
///	@param[in]	x1		���͒l(x0�̐���l)
///	@param[in]	x2		���͒l(x3�̐���l)
///	@param[in]	x3		���͒l
///	@return				��Ԓl
///	@note				rate��0.0�̎�x0, 1.0�̎�x3��Ԃ�
inline float bezier(float rate, float x0, float x1, float x2, float x3){

	float	t1 = 1.0f - rate,
			t2 = rate * t1 * 3.0f;

	return
		x0 * t1 * t1 * t1 +
		x1 * t2 * t1 +
		x2 * t2 * rate +
		x3 * rate * rate * rate;
}

///	@brief				���`���
///	@param[in]	rate	����(0.0�`1.0)
///	@param[in]	x0		���͒l
///	@param[in]	x1		���͒l
///	@return				��Ԓl
///	@note				rate��0.0�̎�x0, 1.0�̎�x1��Ԃ�
inline vector3d lerp(float rate, const vector3d &x0, const vector3d &x1){

	return
		vector3d(
			lerp(rate, x0.e_.x_, x1.e_.x_),
			lerp(rate, x0.e_.y_, x1.e_.y_),
			lerp(rate, x0.e_.z_, x1.e_.z_),
			1.0f
		);
}

///	@brief				�G���~�[�g���
///	@param[in]	rate	����(0.0�`1.0)
///	@param[in]	x0		���͒l
///	@param[in]	x1		���͒l
///	@param[in]	v0		���͒l(x0�̃x�N�g��)
///	@param[in]	v1		���͒l(x1�̃x�N�g��)
///	@return				��Ԓl
///	@note				rate��0.0�̎�x0, 1.0�̎�x1��Ԃ�
inline vector3d hermite(float rate, const vector3d &x0, const vector3d &x1, float v0, float v1){

	return
		vector3d(
			hermite(rate, x0.e_.x_, x1.e_.x_, v0, v1),
			hermite(rate, x0.e_.y_, x1.e_.y_, v0, v1),
			hermite(rate, x0.e_.z_, x1.e_.z_, v0, v1),
			1.0f
		);
}

///	@brief				�G���~�[�g���
///	@param[in]	rate	����(0.0�`1.0)
///	@param[in]	x0		���͒l
///	@param[in]	x1		���͒l
///	@param[in]	v0		���͒l(x0�̃x�N�g��)
///	@param[in]	v1		���͒l(x1�̃x�N�g��)
///	@return				��Ԓl
///	@note				rate��0.0�̎�x0, 1.0�̎�x1��Ԃ�
inline vector3d hermite(float rate, const vector3d &x0, const vector3d &x1, const vector3d &v0, const vector3d &v1){

	return
		vector3d(
			hermite(rate, x0.e_.x_, x1.e_.x_, v0.e_.x_, v1.e_.x_),
			hermite(rate, x0.e_.y_, x1.e_.y_, v0.e_.y_, v1.e_.y_),
			hermite(rate, x0.e_.z_, x1.e_.z_, v0.e_.z_, v1.e_.z_),
			1.0f
		);
}

///	@brief				�x�W�F���
///	@param[in]	rate	����(0.0�`1.0)
///	@param[in]	x0		���͒l
///	@param[in]	x1		���͒l(x0�̐���l)
///	@param[in]	x2		���͒l(x3�̐���l)
///	@param[in]	x3		���͒l
///	@return				��Ԓl
///	@note				rate��0.0�̎�x0, 1.0�̎�x3��Ԃ�
inline vector3d bezier(float rate, const vector3d &x0, const vector3d &x1, const vector3d &x2, const vector3d &x3){

	return
		vector3d(
			bezier(rate, x0.e_.x_, x1.e_.x_, x2.e_.x_, x3.e_.x_),
			bezier(rate, x0.e_.y_, x1.e_.y_, x2.e_.y_, x3.e_.y_),
			bezier(rate, x0.e_.z_, x1.e_.z_, x2.e_.z_, x3.e_.z_),
			1.0f
		);
}

///	@brief				���`���
///	@param[in]	rate	����(0.0�`1.0)
///	@param[in]	x0		���͒l
///	@param[in]	x1		���͒l
///	@return				��Ԓl
///	@note				rate��0.0�̎�x0, 1.0�̎�x1��Ԃ�
inline color_rgb<float> lerp(float rate, const color_rgb<float> &x0, const color_rgb<float> &x1){

	return
		color_rgb<float>(
			lerp(rate, x0.e_.r_, x1.e_.r_),
			lerp(rate, x0.e_.g_, x1.e_.g_),
			lerp(rate, x0.e_.b_, x1.e_.b_)
		);
}

///	@brief				���`���
///	@param[in]	rate	����(0.0�`1.0)
///	@param[in]	x0		���͒l
///	@param[in]	x1		���͒l
///	@return				��Ԓl
///	@note				rate��0.0�̎�x0, 1.0�̎�x1��Ԃ�
inline color_argb<float> lerp(float rate, const color_argb<float> &x0, const color_argb<float> &x1){

	return
		color_argb<float>(
			lerp(rate, x0.e_.a_, x1.e_.a_),
			lerp(rate, x0.e_.r_, x1.e_.r_),
			lerp(rate, x0.e_.g_, x1.e_.g_),
			lerp(rate, x0.e_.b_, x1.e_.b_)
		);
}

///	@brief				���`���
///	@param[in]	rate	����(0.0�`1.0)
///	@param[in]	x0		���͒l
///	@param[in]	x1		���͒l
///	@return				��Ԓl
///	@note				rate��0.0�̎�x0, 1.0�̎�x1��Ԃ�
inline color_rgba<float> lerp(float rate, const color_rgba<float> &x0, const color_rgba<float> &x1){

	return
		color_rgba<float>(
			lerp(rate, x0.e_.r_, x1.e_.r_),
			lerp(rate, x0.e_.g_, x1.e_.g_),
			lerp(rate, x0.e_.b_, x1.e_.b_),
			lerp(rate, x0.e_.a_, x1.e_.a_)
		);
}

///	@brief				�G���~�[�g���
///	@param[in]	rate	����(0.0�`1.0)
///	@param[in]	x0		���͒l
///	@param[in]	x1		���͒l
///	@param[in]	v0		���͒l(x0�̃x�N�g��)
///	@param[in]	v1		���͒l(x1�̃x�N�g��)
///	@return				��Ԓl
///	@note				rate��0.0�̎�x0, 1.0�̎�x1��Ԃ�
inline color_rgb<float> hermite(float rate, const color_rgb<float> &x0, const color_rgb<float> &x1, float v0, float v1){

	return
		color_rgb<float>(
			hermite(rate, x0.e_.r_, x1.e_.r_, v0, v1),
			hermite(rate, x0.e_.g_, x1.e_.g_, v0, v1),
			hermite(rate, x0.e_.b_, x1.e_.b_, v0, v1)
		);
}

///	@brief				�G���~�[�g���
///	@param[in]	rate	����(0.0�`1.0)
///	@param[in]	x0		���͒l
///	@param[in]	x1		���͒l
///	@param[in]	v0		���͒l(x0�̃x�N�g��)
///	@param[in]	v1		���͒l(x1�̃x�N�g��)
///	@return				��Ԓl
///	@note				rate��0.0�̎�x0, 1.0�̎�x1��Ԃ�
inline color_argb<float> hermite(float rate, const color_argb<float> &x0, const color_argb<float> &x1, float v0, float v1){

	return
		color_argb<float>(
			hermite(rate, x0.e_.a_, x1.e_.a_, v0, v1),
			hermite(rate, x0.e_.r_, x1.e_.r_, v0, v1),
			hermite(rate, x0.e_.g_, x1.e_.g_, v0, v1),
			hermite(rate, x0.e_.b_, x1.e_.b_, v0, v1)
		);
}


///	@brief				�G���~�[�g���
///	@param[in]	rate	����(0.0�`1.0)
///	@param[in]	x0		���͒l
///	@param[in]	x1		���͒l
///	@param[in]	v0		���͒l(x0�̃x�N�g��)
///	@param[in]	v1		���͒l(x1�̃x�N�g��)
///	@return				��Ԓl
///	@note				rate��0.0�̎�x0, 1.0�̎�x1��Ԃ�
inline color_rgba<float> hermite(float rate, const color_rgba<float> &x0, const color_rgba<float> &x1, float v0, float v1){

	return
		color_rgba<float>(
			hermite(rate, x0.e_.r_, x1.e_.r_, v0, v1),
			hermite(rate, x0.e_.g_, x1.e_.g_, v0, v1),
			hermite(rate, x0.e_.b_, x1.e_.b_, v0, v1),
			hermite(rate, x0.e_.a_, x1.e_.a_, v0, v1)
		);
}


///	@brief				�G���~�[�g���
///	@param[in]	rate	����(0.0�`1.0)
///	@param[in]	x0		���͒l
///	@param[in]	x1		���͒l
///	@param[in]	v0		���͒l(x0�̃x�N�g��)
///	@param[in]	v1		���͒l(x1�̃x�N�g��)
///	@return				��Ԓl
///	@note				rate��0.0�̎�x0, 1.0�̎�x1��Ԃ�
inline color_rgb<float> hermite(float rate, const color_rgb<float> &x0, const color_rgb<float> &x1, const color_rgb<float> &v0, const color_rgb<float> &v1){

	return
		color_rgb<float>(
			hermite(rate, x0.e_.r_, x1.e_.r_, v0.e_.r_, v1.e_.r_),
			hermite(rate, x0.e_.g_, x1.e_.g_, v0.e_.g_, v1.e_.g_),
			hermite(rate, x0.e_.b_, x1.e_.b_, v0.e_.b_, v1.e_.b_)
		);
}

///	@brief				�G���~�[�g���
///	@param[in]	rate	����(0.0�`1.0)
///	@param[in]	x0		���͒l
///	@param[in]	x1		���͒l
///	@param[in]	v0		���͒l(x0�̃x�N�g��)
///	@param[in]	v1		���͒l(x1�̃x�N�g��)
///	@return				��Ԓl
///	@note				rate��0.0�̎�x0, 1.0�̎�x1��Ԃ�
inline color_argb<float> hermite(float rate, const color_argb<float> &x0, const color_argb<float> &x1, const color_argb<float> &v0, const color_argb<float> &v1){

	return
		color_argb<float>(
			hermite(rate, x0.e_.a_, x1.e_.a_, v0.e_.a_, v1.e_.a_),
			hermite(rate, x0.e_.r_, x1.e_.r_, v0.e_.r_, v1.e_.r_),
			hermite(rate, x0.e_.g_, x1.e_.g_, v0.e_.g_, v1.e_.g_),
			hermite(rate, x0.e_.b_, x1.e_.b_, v0.e_.b_, v1.e_.b_)
		);
}


///	@brief				�G���~�[�g���
///	@param[in]	rate	����(0.0�`1.0)
///	@param[in]	x0		���͒l
///	@param[in]	x1		���͒l
///	@param[in]	v0		���͒l(x0�̃x�N�g��)
///	@param[in]	v1		���͒l(x1�̃x�N�g��)
///	@return				��Ԓl
///	@note				rate��0.0�̎�x0, 1.0�̎�x1��Ԃ�
inline color_rgba<float> hermite(float rate, const color_rgba<float> &x0, const color_rgba<float> &x1, const color_rgba<float> &v0, const color_rgba<float> &v1){

	return
		color_rgba<float>(
			hermite(rate, x0.e_.r_, x1.e_.r_, v0.e_.r_, v1.e_.r_),
			hermite(rate, x0.e_.g_, x1.e_.g_, v0.e_.g_, v1.e_.g_),
			hermite(rate, x0.e_.b_, x1.e_.b_, v0.e_.b_, v1.e_.b_),
			hermite(rate, x0.e_.a_, x1.e_.a_, v0.e_.a_, v1.e_.a_)
		);
}

///	@brief				�x�W�F���
///	@param[in]	rate	����(0.0�`1.0)
///	@param[in]	x0		���͒l
///	@param[in]	x1		���͒l(x0�̐���l)
///	@param[in]	x2		���͒l(x3�̐���l)
///	@param[in]	x3		���͒l
///	@return				��Ԓl
///	@note				rate��0.0�̎�x0, 1.0�̎�x3��Ԃ�
inline color_rgb<float> bezier(float rate, const color_rgb<float> &x0, const color_rgb<float> &x1, const color_rgb<float> &x2, const color_rgb<float> &x3){

	return
		color_rgb<float>(
			bezier(rate, x0.e_.r_, x1.e_.r_, x2.e_.r_, x3.e_.r_),
			bezier(rate, x0.e_.g_, x1.e_.g_, x2.e_.g_, x3.e_.g_),
			bezier(rate, x0.e_.b_, x1.e_.b_, x2.e_.b_, x3.e_.b_)
		);
}

///	@brief				�x�W�F���
///	@param[in]	rate	����(0.0�`1.0)
///	@param[in]	x0		���͒l
///	@param[in]	x1		���͒l(x0�̐���l)
///	@param[in]	x2		���͒l(x3�̐���l)
///	@param[in]	x3		���͒l
///	@return				��Ԓl
///	@note				rate��0.0�̎�x0, 1.0�̎�x3��Ԃ�
inline color_argb<float> bezier(float rate, const color_argb<float> &x0, const color_argb<float> &x1, const color_argb<float> &x2, const color_argb<float> &x3){

	return
		color_argb<float>(
			bezier(rate, x0.e_.a_, x1.e_.a_, x2.e_.a_, x3.e_.a_),
			bezier(rate, x0.e_.r_, x1.e_.r_, x2.e_.r_, x3.e_.r_),
			bezier(rate, x0.e_.g_, x1.e_.g_, x2.e_.g_, x3.e_.g_),
			bezier(rate, x0.e_.b_, x1.e_.b_, x2.e_.b_, x3.e_.b_)
		);
}

///	@brief				�x�W�F���
///	@param[in]	rate	����(0.0�`1.0)
///	@param[in]	x0		���͒l
///	@param[in]	x1		���͒l(x0�̐���l)
///	@param[in]	x2		���͒l(x3�̐���l)
///	@param[in]	x3		���͒l
///	@return				��Ԓl
///	@note				rate��0.0�̎�x0, 1.0�̎�x3��Ԃ�
inline color_rgba<float> bezier(float rate, const color_rgba<float> &x0, const color_rgba<float> &x1, const color_rgba<float> &x2, const color_rgba<float> &x3){

	return
		color_rgba<float>(
			bezier(rate, x0.e_.r_, x1.e_.r_, x2.e_.r_, x3.e_.r_),
			bezier(rate, x0.e_.g_, x1.e_.g_, x2.e_.g_, x3.e_.g_),
			bezier(rate, x0.e_.b_, x1.e_.b_, x2.e_.b_, x3.e_.b_),
			bezier(rate, x0.e_.a_, x1.e_.a_, x2.e_.a_, x3.e_.a_)
		);
}

///	@}

///	@}

}	// namespace ml

#endif	// #ifndef LIBMATCHLOCK_INC_MATH_ML_INTERPOLATE_H
