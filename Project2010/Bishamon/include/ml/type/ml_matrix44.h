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

///	@brief	行列クラス(4x4)
class matrix44 : public detail::matrix44{

public:
	///	@brief				行タイプ(4x4)
	struct row{
		enum type{
			x	= 0,	///<X行
			y,			///<Y行
			z,			///<Z行
			translate	///<平行移動行
		};
	};

	///	@brief			列タイプ(4x4)
	struct column{
		enum type{
			x	= 0,	///<X列
			y,			///<Y列
			z			///<Z列
		};
	};

	struct scale_init{
	};
	struct translate_init{
	};

	///	@brief				コンストラクタ
	matrix44();

	///	@brief				コンストラクタ
	///	@param[in]	m00		行列要素
	///	@param[in]	m01		行列要素
	///	@param[in]	m02		行列要素
	///	@param[in]	m03		行列要素
	///	@param[in]	m10		行列要素
	///	@param[in]	m11		行列要素
	///	@param[in]	m12		行列要素
	///	@param[in]	m13		行列要素
	///	@param[in]	m20		行列要素
	///	@param[in]	m21		行列要素
	///	@param[in]	m22		行列要素
	///	@param[in]	m23		行列要素
	///	@param[in]	m30		行列要素
	///	@param[in]	m31		行列要素
	///	@param[in]	m32		行列要素
	///	@param[in]	m33		行列要素
	matrix44(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33);

	///	@brief				コンストラクタ
	///	@param[in]	v		行列
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

	///	@brief				各要素を設定する
	///	@param[in]	m00		行列要素
	///	@param[in]	m01		行列要素
	///	@param[in]	m02		行列要素
	///	@param[in]	m03		行列要素
	///	@param[in]	m10		行列要素
	///	@param[in]	m11		行列要素
	///	@param[in]	m12		行列要素
	///	@param[in]	m13		行列要素
	///	@param[in]	m20		行列要素
	///	@param[in]	m21		行列要素
	///	@param[in]	m22		行列要素
	///	@param[in]	m23		行列要素
	///	@param[in]	m30		行列要素
	///	@param[in]	m31		行列要素
	///	@param[in]	m32		行列要素
	///	@param[in]	m33		行列要素
	///	@return				*this
	matrix44 &	set(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33);

	///	@brief				各要素を設定する
	///	@param[in]	m00		行列要素
	///	@param[in]	m01		行列要素
	///	@param[in]	m02		行列要素
	///	@param[in]	m03		行列要素
	///	@param[in]	m10		行列要素
	///	@param[in]	m11		行列要素
	///	@param[in]	m12		行列要素
	///	@param[in]	m13		行列要素
	///	@param[in]	m20		行列要素
	///	@param[in]	m21		行列要素
	///	@param[in]	m22		行列要素
	///	@param[in]	m23		行列要素
	///	@return				*this
	matrix44 &	set43(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23);


	///	@brief				各要素を設定する
	///	@param[in]	m00		行列要素
	///	@param[in]	m01		行列要素
	///	@param[in]	m02		行列要素
	///	@param[in]	m10		行列要素
	///	@param[in]	m11		行列要素
	///	@param[in]	m12		行列要素
	///	@param[in]	m20		行列要素
	///	@param[in]	m21		行列要素
	///	@param[in]	m22		行列要素
	///	@return				*this
	matrix44 &	set33(
		float m00, float m01, float m02,
		float m10, float m11, float m12,
		float m20, float m21, float m22);

	///	@brief				単位行列に設定する
	///	@return				*this
	matrix44 &	set_unit();

	///	@brief				4x3部分のみ単位行列に設定する
	///	@return				*this
	matrix44 &	set_unit43();

	///	@brief				3x3部分のみ単位行列に設定する
	///	@return				*this
	matrix44 &	set_unit33();

	///	@brief			X軸回転に設定する
	///	@param[in]	r	回転ラジアン
	///	@return			*this
	///	@note			平行移動部は変更しない
	matrix44 &	set_rot_x(float r);

	///	@brief			Y軸回転に設定する
	///	@param[in]	r	回転ラジアン
	///	@return			*this
	///	@note			平行移動部は変更しない
	matrix44 &	set_rot_y(float r);

	///	@brief			Z軸回転に設定する
	///	@param[in]	r	回転ラジアン
	///	@return			*this
	///	@note			平行移動部は変更しない
	matrix44 &	set_rot_z(float r);

	///	@brief			平行移動を設定する
	///	@param[in]	x	平行移動x
	///	@param[in]	y	平行移動y
	///	@param[in]	z	平行移動z
	///	@return			*this
	///	@note			姿勢部は変更しない
	matrix44 &	set_translate(float x, float y, float z){
		e_.m30_ = x;		e_.m31_ = y;		e_.m32_ = z;		e_.m33_ = 1.0;
		return *this;
	}

	///	@brief			平行移動を加算する
	///	@param[in]	x	平行移動x
	///	@param[in]	y	平行移動y
	///	@param[in]	z	平行移動z
	///	@return			*this
	///	@note			姿勢部は変更しない
	matrix44 &	add_translate(float x, float y, float z){
		e_.m30_ += x;		e_.m31_ += y;		e_.m32_ += z;		e_.m33_ = 1.0;
		return *this;
	}

	///	@brief			平行移動を減算する
	///	@param[in]	x	平行移動x
	///	@param[in]	y	平行移動y
	///	@param[in]	z	平行移動z
	///	@return			*this
	///	@note			姿勢部は変更しない
	matrix44 &	sub_translate(float x, float y, float z){
		e_.m30_ -= x;		e_.m31_ -= y;		e_.m32_ -= z;		e_.m33_ = 1.0;
		return *this;
	}

	///	@brief			拡大縮小を設定する
	///	@param[in]	x	拡大縮小x
	///	@param[in]	y	拡大縮小y
	///	@param[in]	z	拡大縮小z
	///	@return			*this
	///	@note			平行移動部は変更しない
	matrix44 &	set_scale(float x, float y, float z){
		e_.m00_ = x;	e_.m01_ = 0.0f;	e_.m02_ = 0.0f;	e_.m03_ = 0.0f;
		e_.m10_ = 0.0f;	e_.m11_ = y;	e_.m12_ = 0.0f;	e_.m13_ = 0.0f;
		e_.m20_ = 0.0f;	e_.m21_ = 0.0f;	e_.m22_ = z;	e_.m23_ = 0.0f;
		return *this;
	}

	///	@brief			X軸回転をかける
	///	@param[in]	r	回転ラジアン
	///	@return			*this
	LIBMATCHLOCK_FORCEINLINE matrix44 &	mul_rot_x(float r){
		if(r != 0.0f){
			mul_rot_x_nozero(r);
		}
		return *this;
	}

	///	@brief			X軸回転をかける
	///	@param[in]	r	回転ラジアン
	///	@return			*this
	matrix44 &	mul_rot_x_nozero(float r);

	///	@brief			Y軸回転をかける
	///	@param[in]	r	回転ラジアン
	///	@return			*this
	LIBMATCHLOCK_FORCEINLINE matrix44 &	mul_rot_y(float r){
		if(r != 0.0f){
			mul_rot_y_nozero(r);
		}
		return *this;
	}

	///	@brief			Y軸回転をかける
	///	@param[in]	r	回転ラジアン
	///	@return			*this
	matrix44 &	mul_rot_y_nozero(float r);

	///	@brief			Z軸回転をかける
	///	@param[in]	r	回転ラジアン
	///	@return			*this
	LIBMATCHLOCK_FORCEINLINE matrix44 &	mul_rot_z(float r){
		if(r != 0.0f){
			mul_rot_z_nozero(r);
		}
		return *this;
	}

	///	@brief			Z軸回転をかける
	///	@param[in]	r	回転ラジアン
	///	@return			*this
	matrix44 &	mul_rot_z_nozero(float r);

	///	@brief			XYZの順番に回転をかける
	///	@param[in]	x	回転ラジアン
	///	@param[in]	y	回転ラジアン
	///	@param[in]	z	回転ラジアン
	///	@return			*this
	matrix44 &	mul_rot_xyz(float x, float y, float z);

	///	@brief			XZYの順番に回転をかける
	///	@param[in]	x	回転ラジアン
	///	@param[in]	y	回転ラジアン
	///	@param[in]	z	回転ラジアン
	///	@return			*this
	matrix44 &	mul_rot_xzy(float x, float y, float z);

	///	@brief			YXZの順番に回転をかける
	///	@param[in]	x	回転ラジアン
	///	@param[in]	y	回転ラジアン
	///	@param[in]	z	回転ラジアン
	///	@return			*this
	matrix44 &	mul_rot_yxz(float x, float y, float z);

	///	@brief			YZXの順番に回転をかける
	///	@param[in]	x	回転ラジアン
	///	@param[in]	y	回転ラジアン
	///	@param[in]	z	回転ラジアン
	///	@return			*this
	matrix44 &	mul_rot_yzx(float x, float y, float z);

	///	@brief			ZXYの順番に回転をかける
	///	@param[in]	x	回転ラジアン
	///	@param[in]	y	回転ラジアン
	///	@param[in]	z	回転ラジアン
	///	@return			*this
	matrix44 &	mul_rot_zxy(float x, float y, float z);

	///	@brief			ZYXの順番に回転をかける
	///	@param[in]	x	回転ラジアン
	///	@param[in]	y	回転ラジアン
	///	@param[in]	z	回転ラジアン
	///	@return			*this
	matrix44 &	mul_rot_zyx(float x, float y, float z);

	///	@brief			平行移動をかける
	///	@param[in]	x	平行移動
	///	@param[in]	y	平行移動
	///	@param[in]	z	平行移動
	///	@return			*this
	matrix44 &	mul_translate(float x, float y, float z);

	///	@brief			拡大縮小をかける
	///	@param[in]	x	拡大縮小
	///	@param[in]	y	拡大縮小
	///	@param[in]	z	拡大縮小
	///	@return			*this
	matrix44 &	mul_scale(float x, float y, float z);

	///	@brief			行列をかける
	///	@param[in]	m	入力行列
	///	@return			*this
	///	@note			*this = *this x m
	matrix44 &	mul(const matrix44 &m);

	///	@brief			行列をかける
	///	@param[in]	m	入力行列
	///	@return			*this
	///	@note			*this = *this x m
	matrix44 &	mul33(const matrix44 &m);

	///	@brief			指定軸を回転軸とした行列をかける
	///	@param[in]	r	回転ラジアン
	///	@param[in]	v	回転軸
	///	@return			*this
	///	@attention		vは正規化してある状態でなければいけない
	///	@note			平行移動部は変更しない
	matrix44 &	mul_rot_axis(float r, const vector3d &v);

	///	@brief			行列同士をかけた結果に設定する
	///	@param[in]	s	入力行列
	///	@param[in]	d	入力行列
	///	@return			*this
	///	@note			*this = s x d
	matrix44 &	mul(const matrix44 &s, const matrix44 &d);

	///	@brief			行列同士をかけた結果に設定する
	///	@param[in]	s	入力行列
	///	@param[in]	d	入力行列
	///	@return			*this
	///	@note			*this = s x d
	///	@note			s と d の実態は違う必要がある
	matrix44 &	mul_restrict(const matrix44 &s, const matrix44 &d);

	///	@brief			行列同士をかけた結果に設定する
	///	@param[in]	s	入力行列
	///	@param[in]	d	入力行列
	///	@return			*this
	///	@note			*this = s x d
	matrix44 &	mul33(const matrix44 &s, const matrix44 &d);

	///	@brief			指定軸を回転軸とした行列に設定する
	///	@param[in]	r	回転ラジアン
	///	@param[in]	v	回転軸
	///	@return			*this
	///	@attention		vは正規化してある状態でなければいけない
	///	@note			平行移動部は変更しない
	matrix44 &	set_rot_axis(float r, const vector3d &v);

	///	@brief			平行移動を設定する
	///	@param[in]	v	平行移動
	///	@return			*this
	///	@note			姿勢部は変更しない
	matrix44 &	set_translate(const vector3d &v);

	///	@brief			平行移動を加算する
	///	@param[in]	v	平行移動
	///	@return			*this
	///	@note			姿勢部は変更しない
	matrix44 &	add_translate(const vector3d &v);

	///	@brief			平行移動を減算する
	///	@param[in]	v	平行移動
	///	@return			*this
	///	@note			姿勢部は変更しない
	matrix44 &	sub_translate(const vector3d &v);

	///	@brief			拡大縮小を設定する
	///	@param[in]	v	拡大縮小
	///	@return			*this
	///	@note			平行移動部は変更しない
	matrix44 &	set_scale(const vector3d &v);

	///	@brief			XYZの順番に回転をかける
	///	@param[in]	v	回転ラジアン
	///	@return			*this
	matrix44 &	mul_rot_xyz(const vector3d &v);

	///	@brief			XZYの順番に回転をかける
	///	@param[in]	v	回転ラジアン
	///	@return			*this
	matrix44 &	mul_rot_xzy(const vector3d &v);

	///	@brief			YXZの順番に回転をかける
	///	@param[in]	v	回転ラジアン
	///	@return			*this
	matrix44 &	mul_rot_yxz(const vector3d &v);

	///	@brief			YZXの順番に回転をかける
	///	@param[in]	v	回転ラジアン
	///	@return			*this
	matrix44 &	mul_rot_yzx(const vector3d &v);

	///	@brief			ZXYの順番に回転をかける
	///	@param[in]	v	回転ラジアン
	///	@return			*this
	matrix44 &	mul_rot_zxy(const vector3d &v);

	///	@brief			ZYXの順番に回転をかける
	///	@param[in]	v	回転ラジアン
	///	@return			*this
	matrix44 &	mul_rot_zyx(const vector3d &v);

	///	@brief			転地行列に設定する
	///	@return			*this
	matrix44 &	transpose();

	///	@brief			逆行列に設定する
	///	@return			*this
	matrix44 &	inverse();

	///	@brief			逆行列に設定する（正規直行行列として）
	///	@return			*this
	matrix44 &	inverse_ortho();

	///	@brief			平行移動を求める
	///	@return			平行移動
	const vector3d &translate() const{
		return reinterpret_cast<const vector3d &>(e_.m30_);
	}

	///	@brief			行を得る
	///	@param[in]	row	行タイプ
	///	@return			行
	const vector3d &row(row::type row) const{
		return reinterpret_cast<const vector3d &>(m_[row][0]);
	}

	///	@brief			行を得る
	///	@param[in]	row	行タイプ
	///	@return			行
	vector3d &row(row::type row){
		return reinterpret_cast<vector3d &>(m_[row][0]);
	}

	///	@brief			各行の長さを求める
	///	@return			各行の長さ
	vector3d	row_length() const;

	///	@brief			各行の長さの平方を求める
	///	@return			各行の長さの平方
	vector3d	row_length_sq() const;

	///	@brief			列を得る
	///	@param[in]	column	列タイプ
	///	@return			列
	vector3d column(column::type column) const;

	///	@brief			右手系ビューマトリックスを作る
	///	@param[in]	eye	位置
	///	@param[in]	at	注視点
	///	@param[in]	up	アップ
	///	@return			ビューマトリックス
	void lookat_r(const vector3d &eye, const vector3d &at, const vector3d &up);

	///	@brief			左手系ビューマトリックスを作る
	///	@param[in]	eye	位置
	///	@param[in]	at	注視点
	///	@param[in]	up	アップ
	///	@return			ビューマトリックス
	void lookat_l(const vector3d &eye, const vector3d &at, const vector3d &up);

	///	@brief			右手系射影行列を作る
	///	@param[in]	field_of_view	視野
	///	@param[in]	aspect			アスペクト比
	///	@param[in]	near_clip		ニアクリップ面
	///	@param[in]	far_clip		ファークリップ面
	///	@return		右手系射影行列
	void perspectivefov_r(float field_of_view, float aspect, float near_clip, float far_clip);

	///	@brief			左手系射影行列を作る
	///	@param[in]	field_of_view	視野
	///	@param[in]	aspect			アスペクト比
	///	@param[in]	near_clip		ニアクリップ面
	///	@param[in]	far_clip		ファークリップ面
	///	@return		左手系射影行列
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

///	@brief	精度を犠牲にして高速に動作する行列クラス(4x4)
class matrix44 : public ml::matrix44{

public:
};

}	// namespace fast

namespace ni{

///	@addtogroup	type
///	@{

///	@addtogroup	type_matrix
///	@{

///	@brief	デフォルトコンストラクタで初期化処理をしない行列クラス(4x4)
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

///	@brief	精度を犠牲にして高速に動作するし、デフォルトコンストラクタで初期化処理をしない行列クラス(4x4)
class matrix44 : public ni::matrix44{

public:
};

}	// namespace fast

}	// namespace ni

///	@}

///	@}

}	// namespace ml

#endif	// #ifndef LIBMATCHLOCK_INC_ML_TYPE_ML_MATRIX44_H
