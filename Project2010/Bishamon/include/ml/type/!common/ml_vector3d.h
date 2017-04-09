#ifndef LIBMATCHLOCK_INC_ML_TYPE_ML_VECTOR3D_H
#define LIBMATCHLOCK_INC_ML_TYPE_ML_VECTOR3D_H

#include "../../../libmatchlock_config.h"
#include "../detail/ml_vector3d_detail.h"
#include "../internal/ml_type_internal.h"
#include "../ml_matrix44.h"
#include "../../math/ml_arithmetic.h"		// inverse, inverse_sqrtf, to_degree, to_radian
#include "../../math/ml_random.h"

namespace ml{

class matrix44;
class random;

///	@addtogroup	type
///	@{

///	@addtogroup	type_vector
///	@{

///	@brief	�x�N�g���N���X(3D����)
class vector3d : public detail::vector3d{

public:
	///	@brief				�R���X�g���N�^
	vector3d(){
		set_unit();
	}

	///	@brief				�R���X�g���N�^
	///	@param[in]	x		�x�N�g���v�f
	///	@param[in]	y		�x�N�g���v�f
	///	@param[in]	z		�x�N�g���v�f
	///	@param[in]	w		�x�N�g���v�f
	vector3d(float x, float y, float z, float w){
		set(x, y, z, w);
	}

	///	@brief				�R���X�g���N�^
	///	@brief				�R���X�g���N�^
	///	@param[in]	v		�x�N�g��
	vector3d(const detail::vector3d &v){
		static_cast<detail::vector3d &>(*this) = v;
	}

	///	@cond internal
	explicit vector3d(internal::not_initialize){;}
	///	@endcond

	///	@brief				�e�v�f��ݒ肷��
	///	@param[in]	x		�x�N�g���v�f
	///	@param[in]	y		�x�N�g���v�f
	///	@param[in]	z		�x�N�g���v�f
	///	@param[in]	w		�x�N�g���v�f
	///	@return				*this
	vector3d &	set(float x, float y, float z, float w){
		e_.x_ = x;
		e_.y_ = y;
		e_.z_ = z;
		e_.w_ = w;

		return *this;
	}

	///	@brief				���K������
	///	@return				*this
	vector3d &	normalize(){
		float l_sq = length_sq();

		if(l_sq > 0.0f){
			float l = ml::inverse_sqrtf(l_sq);
			e_.x_ *= l;
			e_.y_ *= l;
			e_.z_ *= l;
		}

		return *this;
	}

	///	@brief				�O�ς����߂�
	///	@param[in]	v		���̓x�N�g��
	///	@return				�O��
	vector3d	cross(const vector3d &v) const{
		return
			vector3d(
				e_.y_ * v.e_.z_ - e_.z_ * v.e_.y_,
				e_.z_ * v.e_.x_ - e_.x_ * v.e_.z_,
				e_.x_ * v.e_.y_ - e_.y_ * v.e_.x_,
				1.0f
			);
	}

	///	@brief				���ς����߂�
	///	@param[in]	v		���̓x�N�g��
	///	@return				����
	float		dot(const vector3d &v) const{
		return e_.x_ * v.e_.x_ + e_.y_ * v.e_.y_ + e_.z_ * v.e_.z_;
	}

	///	@brief				���������߂�
	///	@return				����
	float		length() const{
		return ml::sqrtf(length_sq());
	}

	///	@brief				�����̕��������߂�
	///	@return				�����̕���
	float		length_sq() const{
		return e_.x_ * e_.x_ + e_.y_ * e_.y_ + e_.z_ * e_.z_;
	}

	///	@brief				������ݒ肷��
	///	@param[in]	l		����
	///	@return				*this
	vector3d &	set_length(float l){
		float len_sq = length_sq();
		if(len_sq > 0.0f)
		{
			float len = l * inverse_sqrtf(len_sq);

			e_.x_ *= len;
			e_.y_ *= len;
			e_.z_ *= len;
		}

		return *this;
	}

	///	@brief				���������߂�
	///	@param[in]	v		���̓x�N�g��
	///	@return 			����
	float		distance(const vector3d &v) const{
		return ml::sqrtf(distance_sq(v));
	}

	///	@brief				�����̕��������߂�
	///	@param[in]	v		���̓x�N�g��
	///	@return 			�����̕���
	float		distance_sq(const vector3d &v) const{
		return
			vector3d(
				e_.x_ - v.e_.x_,
				e_.y_ - v.e_.y_,
				e_.z_ - v.e_.z_,
				1.0f
			).length_sq();
	}

	///	@brief				�t�x�N�g���ɐݒ肷��
	///	@return				*this
	vector3d &	negative(){
		e_.x_ = -e_.x_;
		e_.y_ = -e_.y_;
		e_.z_ = -e_.z_;

		return *this;
	}

	///	@brief				�t�x�N�g���ɐݒ肷��
	///	@param[in]	v		���̓x�N�g��
	///	@return				*this
	vector3d &	negative(const vector3d &v){
		e_.x_ = -v.e_.x_;
		e_.y_ = -v.e_.y_;
		e_.z_ = -v.e_.z_;

		return *this;
	}

	///	@brief				�P�ʃx�N�g���ɐݒ肷��
	///	@return				*this
	vector3d &	set_unit(){
//		*this = unit;
		e_.x_ = 0.0f;
		e_.y_ = 0.0f;
		e_.z_ = 0.0f;
		e_.w_ = 1.0f;

		return *this;
	}

	///	@brief				�s��̎p�����Ƃ�����
	///	@param[in]	m		���͍s��
	///	@return				*this
	LIBMATCHLOCK_FORCEINLINE vector3d &	mul33(const matrix44 &m){
#if 0
		*this = vector3d(internal::not_initialize()).mul33(*this, m);
#else
		float x = e_.x_;
		float y = e_.y_;
		float z = e_.z_;

		e_.x_ = x * m.e_.m00_ + y * m.e_.m10_ + z * m.e_.m20_;
		e_.y_ = x * m.e_.m01_ + y * m.e_.m11_ + z * m.e_.m21_;
		e_.z_ = x * m.e_.m02_ + y * m.e_.m12_ + z * m.e_.m22_;
		e_.w_ = 1.0;
#endif
		return *this;
	}

	///	@brief				z�v�f��0�ł���x�N�g���ƍs��̎p�����Ƃ�����
	///	@param[in]	m		���͍s��
	///	@return				*this
	LIBMATCHLOCK_FORCEINLINE vector3d &	mul33xy(const matrix44 &m){

		float x = e_.x_;
		float y = e_.y_;

		e_.x_ = x * m.e_.m00_ + y * m.e_.m10_;
		e_.y_ = x * m.e_.m01_ + y * m.e_.m11_;
		e_.z_ = x * m.e_.m02_ + y * m.e_.m12_;
		e_.w_ = 1.0;

		return *this;
	}

	///	@brief				�x�N�g���ƍs��̎p�������������킹�����ʂɐݒ肷��
	///	@param[in]	v		���̓x�N�g��
	///	@param[in]	m		���͍s��
	///	@return				*this
	LIBMATCHLOCK_FORCEINLINE vector3d &	mul33(const vector3d &v, const matrix44 &m){
		e_.x_ = v.e_.x_ * m.e_.m00_ + v.e_.y_ * m.e_.m10_ + v.e_.z_ * m.e_.m20_;
		e_.y_ = v.e_.x_ * m.e_.m01_ + v.e_.y_ * m.e_.m11_ + v.e_.z_ * m.e_.m21_;
		e_.z_ = v.e_.x_ * m.e_.m02_ + v.e_.y_ * m.e_.m12_ + v.e_.z_ * m.e_.m22_;
		e_.w_ = 1.0;

		return *this;
	}

	///	@brief				�s��Ƃ�����
	///	@param[in]	m		���͍s��
	///	@return				*this
	LIBMATCHLOCK_FORCEINLINE vector3d &	mul(const matrix44 &m){
#if 0
		*this = vector3d(internal::not_initialize()).mul(*this, m);
#else
		float x = e_.x_;
		float y = e_.y_;
		float z = e_.z_;
		float w = e_.w_;

		e_.x_ = x * m.e_.m00_ + y * m.e_.m10_ + z * m.e_.m20_ + w * m.e_.m30_;
		e_.y_ = x * m.e_.m01_ + y * m.e_.m11_ + z * m.e_.m21_ + w * m.e_.m31_;
		e_.z_ = x * m.e_.m02_ + y * m.e_.m12_ + z * m.e_.m22_ + w * m.e_.m32_;
		e_.w_ = x * m.e_.m03_ + y * m.e_.m13_ + z * m.e_.m23_ + w * m.e_.m33_;
#endif

		return *this;
	}

	///	@brief				�x�N�g���ƍs����������킹�����ʂɐݒ肷��
	///	@param[in]	v		���̓x�N�g��
	///	@param[in]	m		���͍s��
	///	@return				*this
	LIBMATCHLOCK_FORCEINLINE vector3d &	mul(const vector3d &v, const matrix44 &m){

		e_.x_ = v.e_.x_ * m.e_.m00_ + v.e_.y_ * m.e_.m10_ + v.e_.z_ * m.e_.m20_ + v.e_.w_ * m.e_.m30_;
		e_.y_ = v.e_.x_ * m.e_.m01_ + v.e_.y_ * m.e_.m11_ + v.e_.z_ * m.e_.m21_ + v.e_.w_ * m.e_.m31_;
		e_.z_ = v.e_.x_ * m.e_.m02_ + v.e_.y_ * m.e_.m12_ + v.e_.z_ * m.e_.m22_ + v.e_.w_ * m.e_.m32_;
		e_.w_ = v.e_.x_ * m.e_.m03_ + v.e_.y_ * m.e_.m13_ + v.e_.z_ * m.e_.m23_ + v.e_.w_ * m.e_.m33_;

		return *this;
	}

	///	@brief				�x�N�g���ƍs����������킹�����ʂɐݒ肷��
	///	@param[in]	v		���̓x�N�g��
	///	@param[in]	m		���͍s��
	///	@return				*this
	LIBMATCHLOCK_FORCEINLINE vector3d &	mulw1(const vector3d &v, const matrix44 &m){
		e_.x_ = v.e_.x_ * m.e_.m00_ + v.e_.y_ * m.e_.m10_ + v.e_.z_ * m.e_.m20_ + m.e_.m30_;
		e_.y_ = v.e_.x_ * m.e_.m01_ + v.e_.y_ * m.e_.m11_ + v.e_.z_ * m.e_.m21_ + m.e_.m31_;
		e_.z_ = v.e_.x_ * m.e_.m02_ + v.e_.y_ * m.e_.m12_ + v.e_.z_ * m.e_.m22_ + m.e_.m32_;
		e_.w_ = v.e_.x_ * m.e_.m03_ + v.e_.y_ * m.e_.m13_ + v.e_.z_ * m.e_.m23_ + m.e_.m33_;

		return *this;
	}

	LIBMATCHLOCK_FORCEINLINE vector3d &	mul_xyz_restrict(const vector3d &v, const matrix44 &m){

		e_.x_ = v.e_.x_ * m.e_.m00_ + v.e_.y_ * m.e_.m10_ + v.e_.z_ * m.e_.m20_ + m.e_.m30_;
		e_.y_ = v.e_.x_ * m.e_.m01_ + v.e_.y_ * m.e_.m11_ + v.e_.z_ * m.e_.m21_ + m.e_.m31_;
		e_.z_ = v.e_.x_ * m.e_.m02_ + v.e_.y_ * m.e_.m12_ + v.e_.z_ * m.e_.m22_ + m.e_.m32_;

		return *this;
	}

	///	@brief				�x�N�g�������Z����
	///	@param[in]	v		���̓x�N�g��
	///	@return				*this
	vector3d &	add(const vector3d &v){
		e_.x_ += v.e_.x_;
		e_.y_ += v.e_.y_;
		e_.z_ += v.e_.z_;

		return *this;
	}

	///	@brief				�Q�̃x�N�g�������Z�������ʂɐݒ肷��
	///	@param[in]	v0		���̓x�N�g��
	///	@param[in]	v1		���̓x�N�g��
	///	@return				*this
	vector3d &	add(const vector3d &v0, const vector3d &v1){
		e_.x_ = v0.e_.x_ + v1.e_.x_;
		e_.y_ = v0.e_.y_ + v1.e_.y_;
		e_.z_ = v0.e_.z_ + v1.e_.z_;

		return *this;
	}

	///	@brief				�X�J���[�����Z����
	///	@param[in]	f		���̓X�J���[
	///	@return				*this
	vector3d &	add(float f){
		e_.x_ += f;
		e_.y_ += f;
		e_.z_ += f;

		return *this;
	}

	///	@brief				�x�N�g���ƃX�J���[�����Z�������ʂɐݒ肷��
	///	@param[in]	v		���̓x�N�g��
	///	@param[in]	f		���̓X�J���[
	///	@return				*this
	vector3d &	add(const vector3d &v, float f){
		e_.x_ = v.e_.x_ + f;
		e_.y_ = v.e_.y_ + f;
		e_.z_ = v.e_.z_ + f;

		return *this;
	}

	///	@brief				�x�N�g���ƃX�J���[�̐Ϙa���Z���s��
	///	@param[in]	v		���̓x�N�g��
	///	@param[in]	f		���̓X�J���[
	///	@return				*this
	vector3d &	mac(const vector3d &v, float f){
		e_.x_ += v.e_.x_ * f;
		e_.y_ += v.e_.y_ * f;
		e_.z_ += v.e_.z_ * f;

		return *this;
	}


	///	@brief				�x�N�g�������Z����
	///	@param[in]	v		���̓x�N�g��
	///	@return				*this
	vector3d &	sub(const vector3d &v){
		e_.x_ -= v.e_.x_;
		e_.y_ -= v.e_.y_;
		e_.z_ -= v.e_.z_;

		return *this;
	}

	///	@brief				�Q�̃x�N�g�������Z�������ʂɐݒ肷��
	///	@param[in]	v0		���̓x�N�g��
	///	@param[in]	v1		���̓x�N�g��
	///	@return				*this
	vector3d &	sub(const vector3d &v0, const vector3d &v1){
		e_.x_ = v0.e_.x_ - v1.e_.x_;
		e_.y_ = v0.e_.y_ - v1.e_.y_;
		e_.z_ = v0.e_.z_ - v1.e_.z_;

		return *this;
	}

	///	@brief				�X�J���[�����Z����
	///	@param[in]	f		���̓X�J���[
	///	@return				*this
	vector3d &	sub(float f){
		e_.x_ -= f;
		e_.y_ -= f;
		e_.z_ -= f;

		return *this;
	}

	///	@brief				�x�N�g���ƃX�J���[�����Z�������ʂɐݒ肷��
	///	@param[in]	v		���̓x�N�g��
	///	@param[in]	f		���̓X�J���[
	///	@return				*this
	vector3d &	sub(const vector3d &v, float f){
		e_.x_ = v.e_.x_ - f;
		e_.y_ = v.e_.y_ - f;
		e_.z_ = v.e_.z_ - f;

		return *this;
	}

	///	@brief				�x�N�g������Z����
	///	@param[in]	v		���̓x�N�g��
	///	@return				*this
	vector3d &	mul(const vector3d &v){
		e_.x_ *= v.e_.x_;
		e_.y_ *= v.e_.y_;
		e_.z_ *= v.e_.z_;

		return *this;
	}

	///	@brief				�Q�̃x�N�g������Z�������ʂɐݒ肷��
	///	@param[in]	v0		���̓x�N�g��
	///	@param[in]	v1		���̓x�N�g��
	///	@return				*this
	vector3d &	mul(const vector3d &v0, const vector3d &v1){
		e_.x_ = v0.e_.x_ * v1.e_.x_;
		e_.y_ = v0.e_.y_ * v1.e_.y_;
		e_.z_ = v0.e_.z_ * v1.e_.z_;

		return *this;
	}

	///	@brief				�X�J���[����Z����
	///	@param[in]	f		���̓X�J���[
	///	@return				*this
	vector3d &	mul(float f){
		e_.x_ *= f;
		e_.y_ *= f;
		e_.z_ *= f;

		return *this;
	}

	///	@brief				�x�N�g���ƃX�J���[����Z�������ʂɐݒ肷��
	///	@param[in]	v		���̓x�N�g��
	///	@param[in]	f		���̓X�J���[
	///	@return				*this
	vector3d &	mul(const vector3d &v, float f){
		e_.x_ = v.e_.x_ * f;
		e_.y_ = v.e_.y_ * f;
		e_.z_ = v.e_.z_ * f;

		return *this;
	}

	///	@brief				�x�N�g�������Z����
	///	@param[in]	v		���̓x�N�g��
	///	@return				*this
	vector3d &	div(const vector3d &v){
		e_.x_ *= ml::inverse(v.e_.x_);
		e_.y_ *= ml::inverse(v.e_.y_);
		e_.z_ *= ml::inverse(v.e_.z_);

		return *this;
	}

	///	@brief				�Q�̃x�N�g�������Z�������ʂɐݒ肷��
	///	@param[in]	v0		���̓x�N�g��
	///	@param[in]	v1		���̓x�N�g��
	///	@return				*this
	vector3d &	div(const vector3d &v0, const vector3d &v1){
		e_.x_ = v0.e_.x_ * ml::inverse(v1.e_.x_);
		e_.y_ = v0.e_.y_ * ml::inverse(v1.e_.y_);
		e_.z_ = v0.e_.z_ * ml::inverse(v1.e_.z_);

		return *this;
	}

	///	@brief				�X�J���[�����Z����
	///	@param[in]	f		���̓X�J���[
	///	@return				*this
	vector3d &	div(float f){
		float inv_f = ml::inverse(f);

		e_.x_ *= inv_f;
		e_.y_ *= inv_f;
		e_.z_ *= inv_f;

		return *this;
	}

	///	@brief				�x�N�g���ƃX�J���[�����Z�������ʂɐݒ肷��
	///	@param[in]	v		���̓x�N�g��
	///	@param[in]	f		���̓X�J���[
	///	@return				*this
	vector3d &	div(const vector3d &v, float f){
		float inv_f = ml::inverse(f);

		e_.x_ = v.e_.x_ * inv_f;
		e_.y_ = v.e_.y_ * inv_f;
		e_.z_ = v.e_.z_ * inv_f;

		return *this;
	}

	///	@brief				�x�N�g���̏��������̗v�f��ݒ肷��
	///	@param[in]	v		���̓x�N�g��
	///	@return				*this
	vector3d &	minimum(const vector3d &v){
		e_.x_ = ml::minimum(e_.x_, v.e_.x_);
		e_.y_ = ml::minimum(e_.y_, v.e_.y_);
		e_.z_ = ml::minimum(e_.z_, v.e_.z_);
		e_.w_ = 1.0f;

		return *this;
	}

	///	@brief				�Q�̃x�N�g���̏��������̗v�f�����ʂɐݒ肷��
	///	@param[in]	v0		���̓x�N�g��
	///	@param[in]	v1		���̓x�N�g��
	///	@return				*this
	vector3d &	minimum(const vector3d &v0, const vector3d &v1){
		e_.x_ = ml::minimum(v0.e_.x_, v1.e_.x_);
		e_.y_ = ml::minimum(v0.e_.y_, v1.e_.y_);
		e_.z_ = ml::minimum(v0.e_.z_, v1.e_.z_);
		e_.w_ = 1.0f;

		return *this;
	}

	///	@brief				0�x�N�g���̂Ƃ���ׂď��������̗v�f�����ʂɐݒ肷��
	///	@return				*this
	vector3d &	minimum0(){
		e_.x_ = ml::minimum(e_.x_, 0.0f);
		e_.y_ = ml::minimum(e_.y_, 0.0f);
		e_.z_ = ml::minimum(e_.z_, 0.0f);
		e_.w_ = 1.0f;

		return *this;
	}

	///	@brief				0�x�N�g���̂Ƃ���ׂď��������̗v�f�����ʂɐݒ肷��
	///	@param[in]	v		���̓x�N�g��
	///	@return				*this
	vector3d &	minimum0(const vector3d &v){
		e_.x_ = ml::minimum(v.e_.x_, 0.0f);
		e_.y_ = ml::minimum(v.e_.y_, 0.0f);
		e_.z_ = ml::minimum(v.e_.z_, 0.0f);
		e_.w_ = 1.0f;

		return *this;
	}

	///	@brief				�x�N�g���̑傫�����̗v�f��ݒ肷��
	///	@param[in]	v		���̓x�N�g��
	///	@return				*this
	vector3d &	maximum(const vector3d &v){
		e_.x_ = ml::maximum(e_.x_, v.e_.x_);
		e_.y_ = ml::maximum(e_.y_, v.e_.y_);
		e_.z_ = ml::maximum(e_.z_, v.e_.z_);
		e_.w_ = 1.0f;

		return *this;
	}

	///	@brief				�Q�̃x�N�g���̑傫�����̗v�f�����ʂɐݒ肷��
	///	@param[in]	v0		���̓x�N�g��
	///	@param[in]	v1		���̓x�N�g��
	///	@return				*this
	vector3d &	maximum(const vector3d &v0, const vector3d &v1){
		e_.x_ = ml::maximum(v0.e_.x_, v1.e_.x_);
		e_.y_ = ml::maximum(v0.e_.y_, v1.e_.y_);
		e_.z_ = ml::maximum(v0.e_.z_, v1.e_.z_);
		e_.w_ = 1.0f;

		return *this;
	}

	///	@brief				0�x�N�g���Ƃ���ׂđ傫�����̗v�f�����ʂɐݒ肷��
	///	@return				*this
	vector3d &	maximum0(){
		e_.x_ = ml::maximum(e_.x_, 0.0f);
		e_.y_ = ml::maximum(e_.y_, 0.0f);
		e_.z_ = ml::maximum(e_.z_, 0.0f);
		e_.w_ = 1.0f;

		return *this;
	}

	///	@brief				0�x�N�g���Ƃ���ׂđ傫�����̗v�f�����ʂɐݒ肷��
	///	@param[in]	v		���̓x�N�g��
	///	@return				*this
	vector3d &	maximum0(const vector3d &v){
		e_.x_ = ml::maximum(v.e_.x_, 0.0f);
		e_.y_ = ml::maximum(v.e_.y_, 0.0f);
		e_.z_ = ml::maximum(v.e_.z_, 0.0f);
		e_.w_ = 1.0f;

		return *this;
	}

	///	@brief				���`���
	///	@param[in]	rate	����(0.0�`1.0)
	///	@param[in]	v		���̓x�N�g��
	///	@return				*this
	///	@note				rate��0.0�̎�*this, 1.0�̎�v��Ԃ�
	vector3d &	lerp(float rate, const vector3d &v);

	///	@brief				�Q�̃x�N�g������`��Ԃ������ʂɐݒ肷��
	///	@param[in]	rate	����(0.0�`1.0)
	///	@param[in]	v0		���̓x�N�g��
	///	@param[in]	v1		���̓x�N�g��
	///	@return				��Ԓl
	///	@note				rate��0.0�̎�v0, 1.0�̎�v1��Ԃ�
	vector3d &	lerp(float rate, const vector3d &v0, const vector3d &v1);

	///	@brief				�x�����߂�
	///	@return				�x
	vector3d	to_degree() const{
		return
			vector3d(
				ml::to_degree(e_.x_),
				ml::to_degree(e_.y_),
				ml::to_degree(e_.z_),
				e_.w_
			);
	}

	///	@brief				���W�A�������߂�
	///	@return				���W�A��
	vector3d	to_radian() const{
		return
			vector3d(
				ml::to_radian(e_.x_),
				ml::to_radian(e_.y_),
				ml::to_radian(e_.z_),
				e_.w_
			);
	}

	///	@brief				���������߂�
	///	@param[in]	r		����
	///	@param[in]	base	��l
	///	@param[in]	range	��
	///	@return				����
	///	@note				base - range �ȏ�Abase + range�����l���o�͂��܂��Bw_��1.0f��Ԃ��B
	vector3d &	random_range(ml::random &r, const vector3d &base, const vector3d &range){
		e_.x_ = r.f32_range(base.e_.x_, range.e_.x_);
		e_.y_ = r.f32_range(base.e_.y_, range.e_.y_);
		e_.z_ = r.f32_range(base.e_.z_, range.e_.z_);
		e_.w_ = 1.0f;
		return *this;
	}

	///	@brief				���������߂�
	///	@param[in]	r		����
	///	@param[in]	base	��l
	///	@param[in]	range	��
	///	@return				����
	///	@note				-range �ȏ�A+range�����l���o�͂��܂��Bw_��1.0f��Ԃ��B
	vector3d &	random_range_base0(ml::random &r, const vector3d &range){
		e_.x_ = r.f32_range_base0(range.e_.x_);
		e_.y_ = r.f32_range_base0(range.e_.y_);
		e_.z_ = r.f32_range_base0(range.e_.z_);
		e_.w_ = 1.0f;

		return *this;
	}

	///	@brief						���������߂�
	///	@param[in]	is_same_elem	X == Y == Z ���H
	///	@param[in]	random			����
	///	@param[in]	base			��l
	///	@param[in]	range			��
	///	@return						����
	///	@note						base - range �ȏ�Abase + range�����l���o�͂��܂��Bw_��1.0f��Ԃ��B
	vector3d &	random_range(bool is_same_elem, ml::random &r, const vector3d &base, const vector3d &range){
		if(is_same_elem){
			e_.x_ = r.f32_range(base.e_.x_, range.e_.x_);
			e_.y_ = e_.x_;
			e_.z_ = e_.x_;
			e_.w_ = 1.0f;

			return *this;
		}else{
			return random_range(r, base, range);
		}
	}

	///	@brief						���������߂�
	///	@param[in]	is_same_elem	X == Y == Z ���H
	///	@param[in]	r				����
	///	@param[in]	range			��
	///	@return						����
	///	@note						-range �ȏ�A+range�����l���o�͂��܂��Bw_��1.0f��Ԃ��B
	vector3d &	random_range_base0(bool is_same_elem, ml::random &r, const vector3d &range){
		if(is_same_elem){
			e_.x_ = r.f32_range_base0(range.e_.x_);
			e_.y_ = e_.x_;
			e_.z_ = e_.x_;
			e_.w_ = 1.0f;

			return *this;
		}else{
			return random_range_base0(r, range);
		}
	}

	///	@brief			�x�N�g���̗v�f���������ǂ���
	///	@param[in]	a	�Ώۂ̃x�N�g��
	///	@return			�����v�f�Ȃ�TRUE
	bool	is_equal(const ml::vector3d &a) const{
		if((e_.x_ != a.e_.x_) || (e_.y_ != a.e_.y_) || (e_.z_ != a.e_.z_) || (e_.w_ != a.e_.w_)) return false;
		return true;
	}

	///	@brief			�x�N�g���̗v�f���P�ʍs�񂩂ǂ���
	///	@return			���ׂĂ̗v�f���[���Ȃ�TRUE
	bool	is_unit() const{
		return
			(e_.x_ == 0.0f) &&
			(e_.y_ == 0.0f) &&
			(e_.z_ == 0.0f) &&
			(e_.w_ == 1.0f);
	}

//	static const vector3d	xp;			///<(+1.0f,  0.0f,  0.0f, 1.0f)
//	static const vector3d	xm;			///<(-1.0f,  0.0f,  0.0f, 1.0f)
//	static const vector3d	yp;			///<( 0.0f, +1.0f,  0.0f, 1.0f)
//	static const vector3d	ym;			///<( 0.0f, -1.0f,  0.0f, 1.0f)
//	static const vector3d	zp;			///<( 0.0f,  0.0f, +1.0f, 1.0f)
//	static const vector3d	zm;			///<( 0.0f,  0.0f, -1.0f, 1.0f)

	vector3d operator +(const vector3d &a) const {
		return vector3d(e_.x_+a.e_.x_, e_.y_+a.e_.y_, e_.z_+a.e_.z_, 1.0f);
	}
	vector3d operator -(const vector3d &a) const {
		return vector3d(e_.x_-a.e_.x_ ,e_.y_-a.e_.y_ ,e_.z_-a.e_.z_, 1.0f);
	}

};

///	@}

///	@}

namespace ni{

///	@addtogroup	type
///	@{

///	@addtogroup	type_vector
///	@{

///	@brief	�f�t�H���g�R���X�g���N�^�ŏ��������������Ȃ��x�N�g���N���X(3D����)
class vector3d : public ml::vector3d{

public:
	vector3d() : ml::vector3d(internal::not_initialize()){;}
	vector3d(float x, float y, float z, float w) : ml::vector3d(x, y, z, w){;}
	vector3d(const detail::vector3d &v){	static_cast<detail::vector3d &>(*this) = v;	}
};

///	@}

///	@}

}	// namespace ni

}	// namespace ml

#endif	// #ifndef LIBMATCHLOCK_INC_ML_TYPE_ML_VECTOR3D_H
