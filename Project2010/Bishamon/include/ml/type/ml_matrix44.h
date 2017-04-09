#ifndef LIBMATCHLOCK_INC_ML_TYPE_ML_MATRIX44_H
#define LIBMATCHLOCK_INC_ML_TYPE_ML_MATRIX44_H

#include "../../libmatchlock_config.h"
#include "detail/ml_matrix44_detail.h"
#include "internal/ml_type_internal.h"
#include "../math/ml_triangular.h"	// sin_cos

namespace ml{

class vector3d;

///	@addtogroup	type
///	@{

///	@addtogroup	type_matrix
///	@{

///	@brief	�s��N���X(4x4)
class matrix44 : public detail::matrix44{

public:
	///	@brief				�s�^�C�v(4x4)
	struct row{
		enum type{
			x	= 0,	///<X�s
			y,			///<Y�s
			z,			///<Z�s
			translate	///<���s�ړ��s
		};
	};

	///	@brief			��^�C�v(4x4)
	struct column{
		enum type{
			x	= 0,	///<X��
			y,			///<Y��
			z			///<Z��
		};
	};

	struct scale_init{
	};
	struct translate_init{
	};

	///	@brief				�R���X�g���N�^
	matrix44();

	///	@brief				�R���X�g���N�^
	///	@param[in]	m00		�s��v�f
	///	@param[in]	m01		�s��v�f
	///	@param[in]	m02		�s��v�f
	///	@param[in]	m03		�s��v�f
	///	@param[in]	m10		�s��v�f
	///	@param[in]	m11		�s��v�f
	///	@param[in]	m12		�s��v�f
	///	@param[in]	m13		�s��v�f
	///	@param[in]	m20		�s��v�f
	///	@param[in]	m21		�s��v�f
	///	@param[in]	m22		�s��v�f
	///	@param[in]	m23		�s��v�f
	///	@param[in]	m30		�s��v�f
	///	@param[in]	m31		�s��v�f
	///	@param[in]	m32		�s��v�f
	///	@param[in]	m33		�s��v�f
	matrix44(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33);

	///	@brief				�R���X�g���N�^
	///	@param[in]	v		�s��
	matrix44(const detail::matrix44 &v){	static_cast<detail::matrix44 &>(*this) = v;	}

	matrix44(scale_init, float x, float y, float z){
		e_.m00_ = x;	e_.m01_ = 0.0f;	e_.m02_ = 0.0f;	e_.m03_ = 0.0f;
		e_.m10_ = 0.0f;	e_.m11_ = y;	e_.m12_ = 0.0f;	e_.m13_ = 0.0f;
		e_.m20_ = 0.0f;	e_.m21_ = 0.0f;	e_.m22_ = z;	e_.m23_ = 0.0f;
		e_.m30_ = 0.0f;	e_.m31_ = 0.0f;	e_.m32_ = 0.0f;	e_.m33_ = 1.0f;
	}

	matrix44(translate_init, float x, float y, float z){
		e_.m00_ = 1.0f;	e_.m01_ = 0.0f;	e_.m02_ = 0.0f;	e_.m03_ = 0.0f;
		e_.m10_ = 0.0f;	e_.m11_ = 1.0f;	e_.m12_ = 0.0f;	e_.m13_ = 0.0f;
		e_.m20_ = 0.0f;	e_.m21_ = 0.0f;	e_.m22_ = 1.0f;	e_.m23_ = 0.0f;
		e_.m30_ = x;	e_.m31_ = y;	e_.m32_ = z;	e_.m33_ = 1.0f;
	}

	///	@cond internal
	explicit matrix44(internal::not_initialize){;}
	///	@endcond

	///	@brief				�e�v�f��ݒ肷��
	///	@param[in]	m00		�s��v�f
	///	@param[in]	m01		�s��v�f
	///	@param[in]	m02		�s��v�f
	///	@param[in]	m03		�s��v�f
	///	@param[in]	m10		�s��v�f
	///	@param[in]	m11		�s��v�f
	///	@param[in]	m12		�s��v�f
	///	@param[in]	m13		�s��v�f
	///	@param[in]	m20		�s��v�f
	///	@param[in]	m21		�s��v�f
	///	@param[in]	m22		�s��v�f
	///	@param[in]	m23		�s��v�f
	///	@param[in]	m30		�s��v�f
	///	@param[in]	m31		�s��v�f
	///	@param[in]	m32		�s��v�f
	///	@param[in]	m33		�s��v�f
	///	@return				*this
	matrix44 &	set(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33);

	///	@brief				�e�v�f��ݒ肷��
	///	@param[in]	m00		�s��v�f
	///	@param[in]	m01		�s��v�f
	///	@param[in]	m02		�s��v�f
	///	@param[in]	m03		�s��v�f
	///	@param[in]	m10		�s��v�f
	///	@param[in]	m11		�s��v�f
	///	@param[in]	m12		�s��v�f
	///	@param[in]	m13		�s��v�f
	///	@param[in]	m20		�s��v�f
	///	@param[in]	m21		�s��v�f
	///	@param[in]	m22		�s��v�f
	///	@param[in]	m23		�s��v�f
	///	@return				*this
	matrix44 &	set43(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23);


	///	@brief				�e�v�f��ݒ肷��
	///	@param[in]	m00		�s��v�f
	///	@param[in]	m01		�s��v�f
	///	@param[in]	m02		�s��v�f
	///	@param[in]	m10		�s��v�f
	///	@param[in]	m11		�s��v�f
	///	@param[in]	m12		�s��v�f
	///	@param[in]	m20		�s��v�f
	///	@param[in]	m21		�s��v�f
	///	@param[in]	m22		�s��v�f
	///	@return				*this
	matrix44 &	set33(
		float m00, float m01, float m02,
		float m10, float m11, float m12,
		float m20, float m21, float m22);

	///	@brief				�P�ʍs��ɐݒ肷��
	///	@return				*this
	matrix44 &	set_unit();

	///	@brief				4x3�����̂ݒP�ʍs��ɐݒ肷��
	///	@return				*this
	matrix44 &	set_unit43();

	///	@brief				3x3�����̂ݒP�ʍs��ɐݒ肷��
	///	@return				*this
	matrix44 &	set_unit33();

	///	@brief			X����]�ɐݒ肷��
	///	@param[in]	r	��]���W�A��
	///	@return			*this
	///	@note			���s�ړ����͕ύX���Ȃ�
	matrix44 &	set_rot_x(float r);

	///	@brief			Y����]�ɐݒ肷��
	///	@param[in]	r	��]���W�A��
	///	@return			*this
	///	@note			���s�ړ����͕ύX���Ȃ�
	matrix44 &	set_rot_y(float r);

	///	@brief			Z����]�ɐݒ肷��
	///	@param[in]	r	��]���W�A��
	///	@return			*this
	///	@note			���s�ړ����͕ύX���Ȃ�
	matrix44 &	set_rot_z(float r);

	///	@brief			���s�ړ���ݒ肷��
	///	@param[in]	x	���s�ړ�x
	///	@param[in]	y	���s�ړ�y
	///	@param[in]	z	���s�ړ�z
	///	@return			*this
	///	@note			�p�����͕ύX���Ȃ�
	matrix44 &	set_translate(float x, float y, float z){
		e_.m30_ = x;		e_.m31_ = y;		e_.m32_ = z;		e_.m33_ = 1.0;
		return *this;
	}

	///	@brief			���s�ړ������Z����
	///	@param[in]	x	���s�ړ�x
	///	@param[in]	y	���s�ړ�y
	///	@param[in]	z	���s�ړ�z
	///	@return			*this
	///	@note			�p�����͕ύX���Ȃ�
	matrix44 &	add_translate(float x, float y, float z){
		e_.m30_ += x;		e_.m31_ += y;		e_.m32_ += z;		e_.m33_ = 1.0;
		return *this;
	}

	///	@brief			���s�ړ������Z����
	///	@param[in]	x	���s�ړ�x
	///	@param[in]	y	���s�ړ�y
	///	@param[in]	z	���s�ړ�z
	///	@return			*this
	///	@note			�p�����͕ύX���Ȃ�
	matrix44 &	sub_translate(float x, float y, float z){
		e_.m30_ -= x;		e_.m31_ -= y;		e_.m32_ -= z;		e_.m33_ = 1.0;
		return *this;
	}

	///	@brief			�g��k����ݒ肷��
	///	@param[in]	x	�g��k��x
	///	@param[in]	y	�g��k��y
	///	@param[in]	z	�g��k��z
	///	@return			*this
	///	@note			���s�ړ����͕ύX���Ȃ�
	matrix44 &	set_scale(float x, float y, float z){
		e_.m00_ = x;	e_.m01_ = 0.0f;	e_.m02_ = 0.0f;	e_.m03_ = 0.0f;
		e_.m10_ = 0.0f;	e_.m11_ = y;	e_.m12_ = 0.0f;	e_.m13_ = 0.0f;
		e_.m20_ = 0.0f;	e_.m21_ = 0.0f;	e_.m22_ = z;	e_.m23_ = 0.0f;
		return *this;
	}

	///	@brief			X����]��������
	///	@param[in]	r	��]���W�A��
	///	@return			*this
	LIBMATCHLOCK_FORCEINLINE matrix44 &	mul_rot_x(float r){
		if(r != 0.0f){
			mul_rot_x_nozero(r);
		}
		return *this;
	}

	///	@brief			X����]��������
	///	@param[in]	r	��]���W�A��
	///	@return			*this
	matrix44 &	mul_rot_x_nozero(float r);

	///	@brief			Y����]��������
	///	@param[in]	r	��]���W�A��
	///	@return			*this
	LIBMATCHLOCK_FORCEINLINE matrix44 &	mul_rot_y(float r){
		if(r != 0.0f){
			mul_rot_y_nozero(r);
		}
		return *this;
	}

	///	@brief			Y����]��������
	///	@param[in]	r	��]���W�A��
	///	@return			*this
	matrix44 &	mul_rot_y_nozero(float r);

	///	@brief			Z����]��������
	///	@param[in]	r	��]���W�A��
	///	@return			*this
	LIBMATCHLOCK_FORCEINLINE matrix44 &	mul_rot_z(float r){
		if(r != 0.0f){
			mul_rot_z_nozero(r);
		}
		return *this;
	}

	///	@brief			Z����]��������
	///	@param[in]	r	��]���W�A��
	///	@return			*this
	matrix44 &	mul_rot_z_nozero(float r);

	///	@brief			XYZ�̏��Ԃɉ�]��������
	///	@param[in]	x	��]���W�A��
	///	@param[in]	y	��]���W�A��
	///	@param[in]	z	��]���W�A��
	///	@return			*this
	matrix44 &	mul_rot_xyz(float x, float y, float z);

	///	@brief			XZY�̏��Ԃɉ�]��������
	///	@param[in]	x	��]���W�A��
	///	@param[in]	y	��]���W�A��
	///	@param[in]	z	��]���W�A��
	///	@return			*this
	matrix44 &	mul_rot_xzy(float x, float y, float z);

	///	@brief			YXZ�̏��Ԃɉ�]��������
	///	@param[in]	x	��]���W�A��
	///	@param[in]	y	��]���W�A��
	///	@param[in]	z	��]���W�A��
	///	@return			*this
	matrix44 &	mul_rot_yxz(float x, float y, float z);

	///	@brief			YZX�̏��Ԃɉ�]��������
	///	@param[in]	x	��]���W�A��
	///	@param[in]	y	��]���W�A��
	///	@param[in]	z	��]���W�A��
	///	@return			*this
	matrix44 &	mul_rot_yzx(float x, float y, float z);

	///	@brief			ZXY�̏��Ԃɉ�]��������
	///	@param[in]	x	��]���W�A��
	///	@param[in]	y	��]���W�A��
	///	@param[in]	z	��]���W�A��
	///	@return			*this
	matrix44 &	mul_rot_zxy(float x, float y, float z);

	///	@brief			ZYX�̏��Ԃɉ�]��������
	///	@param[in]	x	��]���W�A��
	///	@param[in]	y	��]���W�A��
	///	@param[in]	z	��]���W�A��
	///	@return			*this
	matrix44 &	mul_rot_zyx(float x, float y, float z);

	///	@brief			���s�ړ���������
	///	@param[in]	x	���s�ړ�
	///	@param[in]	y	���s�ړ�
	///	@param[in]	z	���s�ړ�
	///	@return			*this
	matrix44 &	mul_translate(float x, float y, float z);

	///	@brief			�g��k����������
	///	@param[in]	x	�g��k��
	///	@param[in]	y	�g��k��
	///	@param[in]	z	�g��k��
	///	@return			*this
	matrix44 &	mul_scale(float x, float y, float z);

	///	@brief			�s���������
	///	@param[in]	m	���͍s��
	///	@return			*this
	///	@note			*this = *this x m
	matrix44 &	mul(const matrix44 &m);

	///	@brief			�s���������
	///	@param[in]	m	���͍s��
	///	@return			*this
	///	@note			*this = *this x m
	matrix44 &	mul33(const matrix44 &m);

	///	@brief			�w�莲����]���Ƃ����s���������
	///	@param[in]	r	��]���W�A��
	///	@param[in]	v	��]��
	///	@return			*this
	///	@attention		v�͐��K�����Ă����ԂłȂ���΂����Ȃ�
	///	@note			���s�ړ����͕ύX���Ȃ�
	matrix44 &	mul_rot_axis(float r, const vector3d &v);

	///	@brief			�s�񓯎m�����������ʂɐݒ肷��
	///	@param[in]	s	���͍s��
	///	@param[in]	d	���͍s��
	///	@return			*this
	///	@note			*this = s x d
	matrix44 &	mul(const matrix44 &s, const matrix44 &d);

	///	@brief			�s�񓯎m�����������ʂɐݒ肷��
	///	@param[in]	s	���͍s��
	///	@param[in]	d	���͍s��
	///	@return			*this
	///	@note			*this = s x d
	///	@note			s �� d �̎��Ԃ͈Ⴄ�K�v������
	matrix44 &	mul_restrict(const matrix44 &s, const matrix44 &d);

	///	@brief			�s�񓯎m�����������ʂɐݒ肷��
	///	@param[in]	s	���͍s��
	///	@param[in]	d	���͍s��
	///	@return			*this
	///	@note			*this = s x d
	matrix44 &	mul33(const matrix44 &s, const matrix44 &d);

	///	@brief			�w�莲����]���Ƃ����s��ɐݒ肷��
	///	@param[in]	r	��]���W�A��
	///	@param[in]	v	��]��
	///	@return			*this
	///	@attention		v�͐��K�����Ă����ԂłȂ���΂����Ȃ�
	///	@note			���s�ړ����͕ύX���Ȃ�
	matrix44 &	set_rot_axis(float r, const vector3d &v);

	///	@brief			���s�ړ���ݒ肷��
	///	@param[in]	v	���s�ړ�
	///	@return			*this
	///	@note			�p�����͕ύX���Ȃ�
	matrix44 &	set_translate(const vector3d &v);

	///	@brief			���s�ړ������Z����
	///	@param[in]	v	���s�ړ�
	///	@return			*this
	///	@note			�p�����͕ύX���Ȃ�
	matrix44 &	add_translate(const vector3d &v);

	///	@brief			���s�ړ������Z����
	///	@param[in]	v	���s�ړ�
	///	@return			*this
	///	@note			�p�����͕ύX���Ȃ�
	matrix44 &	sub_translate(const vector3d &v);

	///	@brief			�g��k����ݒ肷��
	///	@param[in]	v	�g��k��
	///	@return			*this
	///	@note			���s�ړ����͕ύX���Ȃ�
	matrix44 &	set_scale(const vector3d &v);

	///	@brief			XYZ�̏��Ԃɉ�]��������
	///	@param[in]	v	��]���W�A��
	///	@return			*this
	matrix44 &	mul_rot_xyz(const vector3d &v);

	///	@brief			XZY�̏��Ԃɉ�]��������
	///	@param[in]	v	��]���W�A��
	///	@return			*this
	matrix44 &	mul_rot_xzy(const vector3d &v);

	///	@brief			YXZ�̏��Ԃɉ�]��������
	///	@param[in]	v	��]���W�A��
	///	@return			*this
	matrix44 &	mul_rot_yxz(const vector3d &v);

	///	@brief			YZX�̏��Ԃɉ�]��������
	///	@param[in]	v	��]���W�A��
	///	@return			*this
	matrix44 &	mul_rot_yzx(const vector3d &v);

	///	@brief			ZXY�̏��Ԃɉ�]��������
	///	@param[in]	v	��]���W�A��
	///	@return			*this
	matrix44 &	mul_rot_zxy(const vector3d &v);

	///	@brief			ZYX�̏��Ԃɉ�]��������
	///	@param[in]	v	��]���W�A��
	///	@return			*this
	matrix44 &	mul_rot_zyx(const vector3d &v);

	///	@brief			�]�n�s��ɐݒ肷��
	///	@return			*this
	matrix44 &	transpose();

	///	@brief			�t�s��ɐݒ肷��
	///	@return			*this
	matrix44 &	inverse();

	///	@brief			�t�s��ɐݒ肷��i���K���s�s��Ƃ��āj
	///	@return			*this
	matrix44 &	inverse_ortho();

	///	@brief			���s�ړ������߂�
	///	@return			���s�ړ�
	const vector3d &translate() const{
		return reinterpret_cast<const vector3d &>(e_.m30_);
	}

	///	@brief			�s�𓾂�
	///	@param[in]	row	�s�^�C�v
	///	@return			�s
	const vector3d &row(row::type row) const{
		return reinterpret_cast<const vector3d &>(m_[row][0]);
	}

	///	@brief			�s�𓾂�
	///	@param[in]	row	�s�^�C�v
	///	@return			�s
	vector3d &row(row::type row){
		return reinterpret_cast<vector3d &>(m_[row][0]);
	}

	///	@brief			�e�s�̒��������߂�
	///	@return			�e�s�̒���
	vector3d	row_length() const;

	///	@brief			�e�s�̒����̕��������߂�
	///	@return			�e�s�̒����̕���
	vector3d	row_length_sq() const;

	///	@brief			��𓾂�
	///	@param[in]	column	��^�C�v
	///	@return			��
	vector3d column(column::type column) const;

	///	@brief			�E��n�r���[�}�g���b�N�X�����
	///	@param[in]	eye	�ʒu
	///	@param[in]	at	�����_
	///	@param[in]	up	�A�b�v
	///	@return			�r���[�}�g���b�N�X
	void lookat_r(const vector3d &eye, const vector3d &at, const vector3d &up);

	///	@brief			����n�r���[�}�g���b�N�X�����
	///	@param[in]	eye	�ʒu
	///	@param[in]	at	�����_
	///	@param[in]	up	�A�b�v
	///	@return			�r���[�}�g���b�N�X
	void lookat_l(const vector3d &eye, const vector3d &at, const vector3d &up);

	///	@brief			�E��n�ˉe�s������
	///	@param[in]	field_of_view	����
	///	@param[in]	aspect			�A�X�y�N�g��
	///	@param[in]	near_clip		�j�A�N���b�v��
	///	@param[in]	far_clip		�t�@�[�N���b�v��
	///	@return		�E��n�ˉe�s��
	void perspectivefov_r(float field_of_view, float aspect, float near_clip, float far_clip);

	///	@brief			����n�ˉe�s������
	///	@param[in]	field_of_view	����
	///	@param[in]	aspect			�A�X�y�N�g��
	///	@param[in]	near_clip		�j�A�N���b�v��
	///	@param[in]	far_clip		�t�@�[�N���b�v��
	///	@return		����n�ˉe�s��
	void perspectivefov_l(float field_of_view, float aspect, float near_clip, float far_clip);

	/// @brief	orthographic
	void orthographic(float left, float right, float bottom, float top, float zNear, float zFar) {
		float sum_rl = right + left;
		float sum_tb = top + bottom;
		float sum_nf = zNear + zFar;
		float inv_rl = 1.0f / (right - left);
		float inv_tb = 1.0f / (top - bottom);
		float inv_nf = 1.0f / (zNear - zFar);
		set(
			inv_rl+inv_rl,	0,				0,				0,
			0,				inv_tb+inv_tb,	0,				0,
			0,				0,				inv_nf+inv_nf,	0,
			-sum_rl*inv_rl,	-sum_tb*inv_tb,	sum_nf*inv_nf,	1
		);
	}
};

///	@}

///	@}

namespace fast{

///	@addtogroup	type
///	@{

///	@addtogroup	type_matrix
///	@{

///	@brief	���x���]���ɂ��č����ɓ��삷��s��N���X(4x4)
class matrix44 : public ml::matrix44{

public:
};

}	// namespace fast

namespace ni{

///	@addtogroup	type
///	@{

///	@addtogroup	type_matrix
///	@{

///	@brief	�f�t�H���g�R���X�g���N�^�ŏ��������������Ȃ��s��N���X(4x4)
class matrix44 : public ml::matrix44{

public:
	inline matrix44() : ml::matrix44(internal::not_initialize()){;}
	matrix44(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33) :
		ml::matrix44(
			m00, m01, m02, m03,
			m10, m11, m12, m13,
			m20, m21, m22, m23,
			m30, m31, m32, m33
		){;}
	matrix44(const detail::matrix44 &v){	static_cast<detail::matrix44 &>(*this) = v;	}
};

///	@}

///	@}

namespace fast{

///	@addtogroup	type
///	@{

///	@addtogroup	type_matrix
///	@{

///	@brief	���x���]���ɂ��č����ɓ��삷�邵�A�f�t�H���g�R���X�g���N�^�ŏ��������������Ȃ��s��N���X(4x4)
class matrix44 : public ni::matrix44{

public:
};

}	// namespace fast

}	// namespace ni

///	@}

///	@}

}	// namespace ml

#endif	// #ifndef LIBMATCHLOCK_INC_ML_TYPE_ML_MATRIX44_H
