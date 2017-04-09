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

///	@brief	ベクトルクラス(3D向き)
class vector3d : public detail::vector3d{

public:
	///	@brief				コンストラクタ
	vector3d(){
		set_unit();
	}

	///	@brief				コンストラクタ
	///	@param[in]	x		ベクトル要素
	///	@param[in]	y		ベクトル要素
	///	@param[in]	z		ベクトル要素
	///	@param[in]	w		ベクトル要素
	vector3d(float x, float y, float z, float w){
		set(x, y, z, w);
	}

	///	@brief				コンストラクタ
	///	@brief				コンストラクタ
	///	@param[in]	v		ベクトル
	vector3d(const detail::vector3d &v){
		static_cast<detail::vector3d &>(*this) = v;
	}

	///	@cond internal
	explicit vector3d(internal::not_initialize){;}
	///	@endcond

	///	@brief				各要素を設定する
	///	@param[in]	x		ベクトル要素
	///	@param[in]	y		ベクトル要素
	///	@param[in]	z		ベクトル要素
	///	@param[in]	w		ベクトル要素
	///	@return				*this
	vector3d &	set(float x, float y, float z, float w){
		e_.x_ = x;
		e_.y_ = y;
		e_.z_ = z;
		e_.w_ = w;

		return *this;
	}

	///	@brief				正規化する
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

	///	@brief				外積を求める
	///	@param[in]	v		入力ベクトル
	///	@return				外積
	vector3d	cross(const vector3d &v) const{
		return
			vector3d(
				e_.y_ * v.e_.z_ - e_.z_ * v.e_.y_,
				e_.z_ * v.e_.x_ - e_.x_ * v.e_.z_,
				e_.x_ * v.e_.y_ - e_.y_ * v.e_.x_,
				1.0f
			);
	}

	///	@brief				内積を求める
	///	@param[in]	v		入力ベクトル
	///	@return				内積
	float		dot(const vector3d &v) const{
		return e_.x_ * v.e_.x_ + e_.y_ * v.e_.y_ + e_.z_ * v.e_.z_;
	}

	///	@brief				長さを求める
	///	@return				長さ
	float		length() const{
		return ml::sqrtf(length_sq());
	}

	///	@brief				長さの平方を求める
	///	@return				長さの平方
	float		length_sq() const{
		return e_.x_ * e_.x_ + e_.y_ * e_.y_ + e_.z_ * e_.z_;
	}

	///	@brief				長さを設定する
	///	@param[in]	l		長さ
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

	///	@brief				距離を求める
	///	@param[in]	v		入力ベクトル
	///	@return 			距離
	float		distance(const vector3d &v) const{
		return ml::sqrtf(distance_sq(v));
	}

	///	@brief				距離の平方を求める
	///	@param[in]	v		入力ベクトル
	///	@return 			距離の平方
	float		distance_sq(const vector3d &v) const{
		return
			vector3d(
				e_.x_ - v.e_.x_,
				e_.y_ - v.e_.y_,
				e_.z_ - v.e_.z_,
				1.0f
			).length_sq();
	}

	///	@brief				逆ベクトルに設定する
	///	@return				*this
	vector3d &	negative(){
		e_.x_ = -e_.x_;
		e_.y_ = -e_.y_;
		e_.z_ = -e_.z_;

		return *this;
	}

	///	@brief				逆ベクトルに設定する
	///	@param[in]	v		入力ベクトル
	///	@return				*this
	vector3d &	negative(const vector3d &v){
		e_.x_ = -v.e_.x_;
		e_.y_ = -v.e_.y_;
		e_.z_ = -v.e_.z_;

		return *this;
	}

	///	@brief				単位ベクトルに設定する
	///	@return				*this
	vector3d &	set_unit(){
//		*this = unit;
		e_.x_ = 0.0f;
		e_.y_ = 0.0f;
		e_.z_ = 0.0f;
		e_.w_ = 1.0f;

		return *this;
	}

	///	@brief				行列の姿勢部とかける
	///	@param[in]	m		入力行列
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

	///	@brief				z要素が0であるベクトルと行列の姿勢部とかける
	///	@param[in]	m		入力行列
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

	///	@brief				ベクトルと行列の姿勢部をかけあわせた結果に設定する
	///	@param[in]	v		入力ベクトル
	///	@param[in]	m		入力行列
	///	@return				*this
	LIBMATCHLOCK_FORCEINLINE vector3d &	mul33(const vector3d &v, const matrix44 &m){
		e_.x_ = v.e_.x_ * m.e_.m00_ + v.e_.y_ * m.e_.m10_ + v.e_.z_ * m.e_.m20_;
		e_.y_ = v.e_.x_ * m.e_.m01_ + v.e_.y_ * m.e_.m11_ + v.e_.z_ * m.e_.m21_;
		e_.z_ = v.e_.x_ * m.e_.m02_ + v.e_.y_ * m.e_.m12_ + v.e_.z_ * m.e_.m22_;
		e_.w_ = 1.0;

		return *this;
	}

	///	@brief				行列とかける
	///	@param[in]	m		入力行列
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

	///	@brief				ベクトルと行列をかけあわせた結果に設定する
	///	@param[in]	v		入力ベクトル
	///	@param[in]	m		入力行列
	///	@return				*this
	LIBMATCHLOCK_FORCEINLINE vector3d &	mul(const vector3d &v, const matrix44 &m){

		e_.x_ = v.e_.x_ * m.e_.m00_ + v.e_.y_ * m.e_.m10_ + v.e_.z_ * m.e_.m20_ + v.e_.w_ * m.e_.m30_;
		e_.y_ = v.e_.x_ * m.e_.m01_ + v.e_.y_ * m.e_.m11_ + v.e_.z_ * m.e_.m21_ + v.e_.w_ * m.e_.m31_;
		e_.z_ = v.e_.x_ * m.e_.m02_ + v.e_.y_ * m.e_.m12_ + v.e_.z_ * m.e_.m22_ + v.e_.w_ * m.e_.m32_;
		e_.w_ = v.e_.x_ * m.e_.m03_ + v.e_.y_ * m.e_.m13_ + v.e_.z_ * m.e_.m23_ + v.e_.w_ * m.e_.m33_;

		return *this;
	}

	///	@brief				ベクトルと行列をかけあわせた結果に設定する
	///	@param[in]	v		入力ベクトル
	///	@param[in]	m		入力行列
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

	///	@brief				ベクトルを加算する
	///	@param[in]	v		入力ベクトル
	///	@return				*this
	vector3d &	add(const vector3d &v){
		e_.x_ += v.e_.x_;
		e_.y_ += v.e_.y_;
		e_.z_ += v.e_.z_;

		return *this;
	}

	///	@brief				２つのベクトルを加算した結果に設定する
	///	@param[in]	v0		入力ベクトル
	///	@param[in]	v1		入力ベクトル
	///	@return				*this
	vector3d &	add(const vector3d &v0, const vector3d &v1){
		e_.x_ = v0.e_.x_ + v1.e_.x_;
		e_.y_ = v0.e_.y_ + v1.e_.y_;
		e_.z_ = v0.e_.z_ + v1.e_.z_;

		return *this;
	}

	///	@brief				スカラーを加算する
	///	@param[in]	f		入力スカラー
	///	@return				*this
	vector3d &	add(float f){
		e_.x_ += f;
		e_.y_ += f;
		e_.z_ += f;

		return *this;
	}

	///	@brief				ベクトルとスカラーを加算した結果に設定する
	///	@param[in]	v		入力ベクトル
	///	@param[in]	f		入力スカラー
	///	@return				*this
	vector3d &	add(const vector3d &v, float f){
		e_.x_ = v.e_.x_ + f;
		e_.y_ = v.e_.y_ + f;
		e_.z_ = v.e_.z_ + f;

		return *this;
	}

	///	@brief				ベクトルとスカラーの積和演算を行う
	///	@param[in]	v		入力ベクトル
	///	@param[in]	f		入力スカラー
	///	@return				*this
	vector3d &	mac(const vector3d &v, float f){
		e_.x_ += v.e_.x_ * f;
		e_.y_ += v.e_.y_ * f;
		e_.z_ += v.e_.z_ * f;

		return *this;
	}


	///	@brief				ベクトルを減算する
	///	@param[in]	v		入力ベクトル
	///	@return				*this
	vector3d &	sub(const vector3d &v){
		e_.x_ -= v.e_.x_;
		e_.y_ -= v.e_.y_;
		e_.z_ -= v.e_.z_;

		return *this;
	}

	///	@brief				２つのベクトルを減算した結果に設定する
	///	@param[in]	v0		入力ベクトル
	///	@param[in]	v1		入力ベクトル
	///	@return				*this
	vector3d &	sub(const vector3d &v0, const vector3d &v1){
		e_.x_ = v0.e_.x_ - v1.e_.x_;
		e_.y_ = v0.e_.y_ - v1.e_.y_;
		e_.z_ = v0.e_.z_ - v1.e_.z_;

		return *this;
	}

	///	@brief				スカラーを減算する
	///	@param[in]	f		入力スカラー
	///	@return				*this
	vector3d &	sub(float f){
		e_.x_ -= f;
		e_.y_ -= f;
		e_.z_ -= f;

		return *this;
	}

	///	@brief				ベクトルとスカラーを減算した結果に設定する
	///	@param[in]	v		入力ベクトル
	///	@param[in]	f		入力スカラー
	///	@return				*this
	vector3d &	sub(const vector3d &v, float f){
		e_.x_ = v.e_.x_ - f;
		e_.y_ = v.e_.y_ - f;
		e_.z_ = v.e_.z_ - f;

		return *this;
	}

	///	@brief				ベクトルを乗算する
	///	@param[in]	v		入力ベクトル
	///	@return				*this
	vector3d &	mul(const vector3d &v){
		e_.x_ *= v.e_.x_;
		e_.y_ *= v.e_.y_;
		e_.z_ *= v.e_.z_;

		return *this;
	}

	///	@brief				２つのベクトルを乗算した結果に設定する
	///	@param[in]	v0		入力ベクトル
	///	@param[in]	v1		入力ベクトル
	///	@return				*this
	vector3d &	mul(const vector3d &v0, const vector3d &v1){
		e_.x_ = v0.e_.x_ * v1.e_.x_;
		e_.y_ = v0.e_.y_ * v1.e_.y_;
		e_.z_ = v0.e_.z_ * v1.e_.z_;

		return *this;
	}

	///	@brief				スカラーを乗算する
	///	@param[in]	f		入力スカラー
	///	@return				*this
	vector3d &	mul(float f){
		e_.x_ *= f;
		e_.y_ *= f;
		e_.z_ *= f;

		return *this;
	}

	///	@brief				ベクトルとスカラーを乗算した結果に設定する
	///	@param[in]	v		入力ベクトル
	///	@param[in]	f		入力スカラー
	///	@return				*this
	vector3d &	mul(const vector3d &v, float f){
		e_.x_ = v.e_.x_ * f;
		e_.y_ = v.e_.y_ * f;
		e_.z_ = v.e_.z_ * f;

		return *this;
	}

	///	@brief				ベクトルを除算する
	///	@param[in]	v		入力ベクトル
	///	@return				*this
	vector3d &	div(const vector3d &v){
		e_.x_ *= ml::inverse(v.e_.x_);
		e_.y_ *= ml::inverse(v.e_.y_);
		e_.z_ *= ml::inverse(v.e_.z_);

		return *this;
	}

	///	@brief				２つのベクトルを除算した結果に設定する
	///	@param[in]	v0		入力ベクトル
	///	@param[in]	v1		入力ベクトル
	///	@return				*this
	vector3d &	div(const vector3d &v0, const vector3d &v1){
		e_.x_ = v0.e_.x_ * ml::inverse(v1.e_.x_);
		e_.y_ = v0.e_.y_ * ml::inverse(v1.e_.y_);
		e_.z_ = v0.e_.z_ * ml::inverse(v1.e_.z_);

		return *this;
	}

	///	@brief				スカラーを除算する
	///	@param[in]	f		入力スカラー
	///	@return				*this
	vector3d &	div(float f){
		float inv_f = ml::inverse(f);

		e_.x_ *= inv_f;
		e_.y_ *= inv_f;
		e_.z_ *= inv_f;

		return *this;
	}

	///	@brief				ベクトルとスカラーを除算した結果に設定する
	///	@param[in]	v		入力ベクトル
	///	@param[in]	f		入力スカラー
	///	@return				*this
	vector3d &	div(const vector3d &v, float f){
		float inv_f = ml::inverse(f);

		e_.x_ = v.e_.x_ * inv_f;
		e_.y_ = v.e_.y_ * inv_f;
		e_.z_ = v.e_.z_ * inv_f;

		return *this;
	}

	///	@brief				ベクトルの小さい方の要素を設定する
	///	@param[in]	v		入力ベクトル
	///	@return				*this
	vector3d &	minimum(const vector3d &v){
		e_.x_ = ml::minimum(e_.x_, v.e_.x_);
		e_.y_ = ml::minimum(e_.y_, v.e_.y_);
		e_.z_ = ml::minimum(e_.z_, v.e_.z_);
		e_.w_ = 1.0f;

		return *this;
	}

	///	@brief				２つのベクトルの小さい方の要素を結果に設定する
	///	@param[in]	v0		入力ベクトル
	///	@param[in]	v1		入力ベクトル
	///	@return				*this
	vector3d &	minimum(const vector3d &v0, const vector3d &v1){
		e_.x_ = ml::minimum(v0.e_.x_, v1.e_.x_);
		e_.y_ = ml::minimum(v0.e_.y_, v1.e_.y_);
		e_.z_ = ml::minimum(v0.e_.z_, v1.e_.z_);
		e_.w_ = 1.0f;

		return *this;
	}

	///	@brief				0ベクトルのとくらべて小さい方の要素を結果に設定する
	///	@return				*this
	vector3d &	minimum0(){
		e_.x_ = ml::minimum(e_.x_, 0.0f);
		e_.y_ = ml::minimum(e_.y_, 0.0f);
		e_.z_ = ml::minimum(e_.z_, 0.0f);
		e_.w_ = 1.0f;

		return *this;
	}

	///	@brief				0ベクトルのとくらべて小さい方の要素を結果に設定する
	///	@param[in]	v		入力ベクトル
	///	@return				*this
	vector3d &	minimum0(const vector3d &v){
		e_.x_ = ml::minimum(v.e_.x_, 0.0f);
		e_.y_ = ml::minimum(v.e_.y_, 0.0f);
		e_.z_ = ml::minimum(v.e_.z_, 0.0f);
		e_.w_ = 1.0f;

		return *this;
	}

	///	@brief				ベクトルの大きい方の要素を設定する
	///	@param[in]	v		入力ベクトル
	///	@return				*this
	vector3d &	maximum(const vector3d &v){
		e_.x_ = ml::maximum(e_.x_, v.e_.x_);
		e_.y_ = ml::maximum(e_.y_, v.e_.y_);
		e_.z_ = ml::maximum(e_.z_, v.e_.z_);
		e_.w_ = 1.0f;

		return *this;
	}

	///	@brief				２つのベクトルの大きい方の要素を結果に設定する
	///	@param[in]	v0		入力ベクトル
	///	@param[in]	v1		入力ベクトル
	///	@return				*this
	vector3d &	maximum(const vector3d &v0, const vector3d &v1){
		e_.x_ = ml::maximum(v0.e_.x_, v1.e_.x_);
		e_.y_ = ml::maximum(v0.e_.y_, v1.e_.y_);
		e_.z_ = ml::maximum(v0.e_.z_, v1.e_.z_);
		e_.w_ = 1.0f;

		return *this;
	}

	///	@brief				0ベクトルとくらべて大きい方の要素を結果に設定する
	///	@return				*this
	vector3d &	maximum0(){
		e_.x_ = ml::maximum(e_.x_, 0.0f);
		e_.y_ = ml::maximum(e_.y_, 0.0f);
		e_.z_ = ml::maximum(e_.z_, 0.0f);
		e_.w_ = 1.0f;

		return *this;
	}

	///	@brief				0ベクトルとくらべて大きい方の要素を結果に設定する
	///	@param[in]	v		入力ベクトル
	///	@return				*this
	vector3d &	maximum0(const vector3d &v){
		e_.x_ = ml::maximum(v.e_.x_, 0.0f);
		e_.y_ = ml::maximum(v.e_.y_, 0.0f);
		e_.z_ = ml::maximum(v.e_.z_, 0.0f);
		e_.w_ = 1.0f;

		return *this;
	}

	///	@brief				線形補間
	///	@param[in]	rate	割合(0.0～1.0)
	///	@param[in]	v		入力ベクトル
	///	@return				*this
	///	@note				rateが0.0の時*this, 1.0の時vを返す
	vector3d &	lerp(float rate, const vector3d &v);

	///	@brief				２つのベクトルを線形補間した結果に設定する
	///	@param[in]	rate	割合(0.0～1.0)
	///	@param[in]	v0		入力ベクトル
	///	@param[in]	v1		入力ベクトル
	///	@return				補間値
	///	@note				rateが0.0の時v0, 1.0の時v1を返す
	vector3d &	lerp(float rate, const vector3d &v0, const vector3d &v1);

	///	@brief				度を求める
	///	@return				度
	vector3d	to_degree() const{
		return
			vector3d(
				ml::to_degree(e_.x_),
				ml::to_degree(e_.y_),
				ml::to_degree(e_.z_),
				e_.w_
			);
	}

	///	@brief				ラジアンを求める
	///	@return				ラジアン
	vector3d	to_radian() const{
		return
			vector3d(
				ml::to_radian(e_.x_),
				ml::to_radian(e_.y_),
				ml::to_radian(e_.z_),
				e_.w_
			);
	}

	///	@brief				乱数を求める
	///	@param[in]	r		乱数
	///	@param[in]	base	基準値
	///	@param[in]	range	幅
	///	@return				乱数
	///	@note				base - range 以上、base + range未満値を出力します。w_は1.0fを返す。
	vector3d &	random_range(ml::random &r, const vector3d &base, const vector3d &range){
		e_.x_ = r.f32_range(base.e_.x_, range.e_.x_);
		e_.y_ = r.f32_range(base.e_.y_, range.e_.y_);
		e_.z_ = r.f32_range(base.e_.z_, range.e_.z_);
		e_.w_ = 1.0f;
		return *this;
	}

	///	@brief				乱数を求める
	///	@param[in]	r		乱数
	///	@param[in]	base	基準値
	///	@param[in]	range	幅
	///	@return				乱数
	///	@note				-range 以上、+range未満値を出力します。w_は1.0fを返す。
	vector3d &	random_range_base0(ml::random &r, const vector3d &range){
		e_.x_ = r.f32_range_base0(range.e_.x_);
		e_.y_ = r.f32_range_base0(range.e_.y_);
		e_.z_ = r.f32_range_base0(range.e_.z_);
		e_.w_ = 1.0f;

		return *this;
	}

	///	@brief						乱数を求める
	///	@param[in]	is_same_elem	X == Y == Z か？
	///	@param[in]	random			乱数
	///	@param[in]	base			基準値
	///	@param[in]	range			幅
	///	@return						乱数
	///	@note						base - range 以上、base + range未満値を出力します。w_は1.0fを返す。
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

	///	@brief						乱数を求める
	///	@param[in]	is_same_elem	X == Y == Z か？
	///	@param[in]	r				乱数
	///	@param[in]	range			幅
	///	@return						乱数
	///	@note						-range 以上、+range未満値を出力します。w_は1.0fを返す。
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

	///	@brief			ベクトルの要素が同じかどうか
	///	@param[in]	a	対象のベクトル
	///	@return			同じ要素ならTRUE
	bool	is_equal(const ml::vector3d &a) const{
		if((e_.x_ != a.e_.x_) || (e_.y_ != a.e_.y_) || (e_.z_ != a.e_.z_) || (e_.w_ != a.e_.w_)) return false;
		return true;
	}

	///	@brief			ベクトルの要素が単位行列かどうか
	///	@return			すべての要素がゼロならTRUE
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

///	@brief	デフォルトコンストラクタで初期化処理をしないベクトルクラス(3D向き)
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
