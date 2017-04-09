#ifndef LIBMATCHLOCK_INC_ML_TYPE_ML_COLOR_H
#define LIBMATCHLOCK_INC_ML_TYPE_ML_COLOR_H

#include "../detail/ml_color_detail.h"
#include "../internal/ml_type_internal.h"
#include "../../math/ml_arithmetic.h"
#include "../../math/ml_random.h"

namespace ml{

///	@addtogroup	type
///	@{

///	@addtogroup	type_color
///	@{

///	@brief	色クラス(RGB)
template<typename T>
class color_rgb : public detail::color_rgb<T>{

public:
	///	@brief			コンストラクタ
	color_rgb(){
		set(T(), T(), T());
	}

	///	@brief			コンストラクタ
	///	@param[in]	r	赤
	///	@param[in]	g	緑
	///	@param[in]	b	青
	color_rgb(T r, T g, T b){
		set(r, g, b);
	}

//	///	@brief			コンストラクタ
//	///	@param[in]	v	色
//	color_rgb(const detail::color_rgb<T> &v){
//		static_cast<detail::color_rgb &>(*this) = v;
//	}

	///	@cond internal
	explicit color_rgb(internal::not_initialize){;}
	///	@endcond

	///	@brief			各要素を設定する
	///	@param[in]	r	赤
	///	@param[in]	g	緑
	///	@param[in]	b	青
	///	@return			*this
	color_rgb &	set(T r, T g, T b){
		detail::color_rgb<T>::e_.r_ = r;
		detail::color_rgb<T>::e_.g_ = g;
		detail::color_rgb<T>::e_.b_ = b;
		return *this;
	}

	///	@brief			各要素を指定した値で丸めこんだ値を設定する
	///	@param[in]	min	最小値
	///	@param[in]	max	最大値
	///	@return			*this
	color_rgb &	clamp(T min, T max){
		detail::color_rgb<T>::e_.r_ = ml::clamp(detail::color_rgb<T>::e_.r_, min, max);
		detail::color_rgb<T>::e_.g_ = ml::clamp(detail::color_rgb<T>::e_.g_, min, max);
		detail::color_rgb<T>::e_.b_ = ml::clamp(detail::color_rgb<T>::e_.b_, min, max);
		return *this;
	}

	///	@brief				各要素をランダム設定する
	///	@param[in]	r		ランダム
	///	@param[in]	base	ベース
	///	@param[in]	range	ランダム幅
	///	@return				*this
	color_rgb &	random_range_rgb(ml::random &r, const color_rgb &base, const color_rgb &range){
		detail::color_rgb<T>::e_.r_ = r.f32_range(base.e_.r_, range.e_.r_);
		detail::color_rgb<T>::e_.g_ = r.f32_range(base.e_.g_, range.e_.g_);
		detail::color_rgb<T>::e_.b_ = r.f32_range(base.e_.b_, range.e_.b_);

		return *this;
	}
};

///	@brief	色クラス(ARGB)
template<typename T>
class color_argb : public detail::color_argb<T>{

public:
	///	@brief			コンストラクタ
	color_argb(){
		set(T(), T(), T(), T());
	}

	///	@brief			コンストラクタ
	///	@param[in]	a	アルファ
	///	@param[in]	r	赤
	///	@param[in]	g	緑
	///	@param[in]	b	青
	color_argb(T a, T r, T g, T b){
		set(a, r, g, b);
	}

//	///	@brief			コンストラクタ
//	///	@param[in]	v	色
//	color_argb(const detail::color_argb<T> &v){
//		static_cast<detail::color_argb &>(*this) = v;
//	}

	///	@cond internal
	explicit color_argb(internal::not_initialize){;}
	///	@endcond

	///	@brief			各要素を設定する
	///	@param[in]	a	アルファ
	///	@param[in]	r	赤
	///	@param[in]	g	緑
	///	@param[in]	b	青
	///	@return			*this
	color_argb &	set(T a, T r, T g, T b){
		detail::color_argb<T>::e_.a_ = a;
		detail::color_argb<T>::e_.r_ = r;
		detail::color_argb<T>::e_.g_ = g;
		detail::color_argb<T>::e_.b_ = b;
		return *this;
	}

	///	@brief			各要素を指定した値で丸めこんだ値を設定する
	///	@param[in]	min	最小値
	///	@param[in]	max	最大値
	///	@return			*this
	color_argb &	clamp(T min, T max){
		detail::color_argb<T>::e_.a_ = ml::clamp(detail::color_argb<T>::e_.a_, min, max);
		detail::color_argb<T>::e_.r_ = ml::clamp(detail::color_argb<T>::e_.r_, min, max);
		detail::color_argb<T>::e_.g_ = ml::clamp(detail::color_argb<T>::e_.g_, min, max);
		detail::color_argb<T>::e_.b_ = ml::clamp(detail::color_argb<T>::e_.b_, min, max);
		return *this;
	}

	///	@brief				各要素をランダム設定する
	///	@param[in]	r		ランダム
	///	@param[in]	base	ベース
	///	@param[in]	range	ランダム幅
	///	@return				*this
	color_argb &	random_range_rgb(ml::random &r, const color_argb &base, const color_argb &range){
		detail::color_argb<T>::e_.r_ = r.f32_range(base.e_.r_, range.e_.r_);
		detail::color_argb<T>::e_.g_ = r.f32_range(base.e_.g_, range.e_.g_);
		detail::color_argb<T>::e_.b_ = r.f32_range(base.e_.b_, range.e_.b_);
		detail::color_argb<T>::e_.a_ = 1.0f;

		return *this;
	}
};


///	@brief	色クラス(RGBA)
template<typename T>
class color_rgba : public detail::color_rgba<T>{

public:
	///	@brief			コンストラクタ
	color_rgba(){
		set(T(), T(), T(), T());
	}

	///	@brief			コンストラクタ
	///	@param[in]	a	アルファ
	///	@param[in]	r	赤
	///	@param[in]	g	緑
	///	@param[in]	b	青
	color_rgba(T r, T g, T b, T a){
		set(r, g, b, a);
	}

//	///	@brief			コンストラクタ
//	///	@param[in]	v	色
//	color_rgba(const detail::color_rgba<T> &v){
//		static_cast<detail::color_rgba &>(*this) = v;
//	}

	///	@cond internal
	explicit color_rgba(internal::not_initialize){;}
	///	@endcond

	///	@brief			各要素を設定する
	///	@param[in]	a	アルファ
	///	@param[in]	r	赤
	///	@param[in]	g	緑
	///	@param[in]	b	青
	///	@return			*this
	color_rgba &	set(T r, T g, T b, T a){
		detail::color_rgba<T>::e_.r_ = r;
		detail::color_rgba<T>::e_.g_ = g;
		detail::color_rgba<T>::e_.b_ = b;
		detail::color_rgba<T>::e_.a_ = a;
		return *this;
	}

	///	@brief			各要素を掛け合わせる
	///	@param[in]	a	アルファ
	///	@param[in]	r	赤
	///	@param[in]	g	緑
	///	@param[in]	b	青
	///	@return			*this
	color_rgba &	mul(T r, T g, T b, T a){
		detail::color_rgba<T>::e_.r_ *= r;
		detail::color_rgba<T>::e_.g_ *= g;
		detail::color_rgba<T>::e_.b_ *= b;
		detail::color_rgba<T>::e_.a_ *= a;
		return *this;
	}

	///	@brief			各要素を掛け合わせる
	///	@param[in]	c	カラー
	///	@return			*this
	color_rgba &	mul(color_rgba &c){
		return this->mul(c.e_.r_, c.e_.g_, c.e_.b_, c.e_.a_);
	}

	///	@brief			各要素に掛け合わせる
	///	@param[in]	c	カラー
	///	@return			*this
	color_rgba &	mul(T &c){
		return this->mul(c, c, c, c);
	}

	///	@brief			各要素を指定した値で丸めこんだ値を設定する
	///	@param[in]	min	最小値
	///	@param[in]	max	最大値
	///	@return			*this
	color_rgba &	clamp(T min, T max){
		detail::color_rgba<T>::e_.r_ = ml::clamp(detail::color_rgba<T>::e_.r_, min, max);
		detail::color_rgba<T>::e_.g_ = ml::clamp(detail::color_rgba<T>::e_.g_, min, max);
		detail::color_rgba<T>::e_.b_ = ml::clamp(detail::color_rgba<T>::e_.b_, min, max);
		detail::color_rgba<T>::e_.a_ = ml::clamp(detail::color_rgba<T>::e_.a_, min, max);
		return *this;
	}

	///	@brief				各要素をランダム設定する
	///	@param[in]	r		ランダム
	///	@param[in]	base	ベース
	///	@param[in]	range	ランダム幅
	///	@return				*this
	color_rgba &	random_range_rgb(ml::random &r, const color_rgba &base, const color_rgba &range){

		detail::color_rgba<T>::e_.r_ = r.f32_range(base.e_.r_, range.e_.r_);
		detail::color_rgba<T>::e_.g_ = r.f32_range(base.e_.g_, range.e_.g_);
		detail::color_rgba<T>::e_.b_ = r.f32_range(base.e_.b_, range.e_.b_);
		detail::color_rgba<T>::e_.a_ = 1.0f;

		return *this;
	}

	///	@brief				各要素がすべてゼロかどうか
	///	@return				真偽
	bool	is_unit() const{
		if(0.0f!=detail::color_rgba<T>::e_.r_)	return false;
		if(0.0f!=detail::color_rgba<T>::e_.g_)	return false;
		if(0.0f!=detail::color_rgba<T>::e_.b_)	return false;

		return true;
	}

	///	@brief				各要素がすべてゼロかどうか
	///	@return				真偽
	bool	is_equal(const color_rgba &v) const{
		if(v.e_.r_!=detail::color_rgba<T>::e_.r_)	return false;
		if(v.e_.g_!=detail::color_rgba<T>::e_.g_)	return false;
		if(v.e_.b_!=detail::color_rgba<T>::e_.b_)	return false;

		return true;
	}

};

///	@}

///	@}

namespace ni{

///	@addtogroup	type
///	@{

///	@addtogroup	type_color
///	@{

///	@brief	デフォルトコンストラクタで初期化処理をしない色クラス(RGB)
template<typename T>
class color_rgb : public ml::color_rgb<T>{

public:
	color_rgb() : ml::color_rgb<T>(internal::not_initialize()){;}
	color_rgb(T r, T g, T b) : ml::color_rgb<T>(r, g, b){;}
	color_rgb(const detail::color_rgb<T> &v){
		static_cast<detail::color_rgb<T> &>(*this) = v;
	}
};

///	@brief	デフォルトコンストラクタで初期化処理をしない色クラス(ARGB)
template<typename T>
class color_argb : public ml::color_argb<T>{

public:
	color_argb() : ml::color_argb<T>(internal::not_initialize()){;}
	color_argb(T a, T r, T g, T b) : ml::color_argb<T>(a, r, g, b){;}
	color_argb(const detail::color_argb<T> &v){
		static_cast<detail::color_argb<T> &>(*this) = v;
	}
};

///	@brief	デフォルトコンストラクタで初期化処理をしない色クラス(RGBA)
template<typename T>
class color_rgba : public ml::color_rgba<T>{

public:
	color_rgba() : ml::color_rgba<T>(internal::not_initialize()){;}
	color_rgba(T r, T g, T b, T a) : ml::color_rgba<T>(r, g, b, a){;}
	color_rgba(const detail::color_rgba<T> &v){
		static_cast<detail::color_rgba<T> &>(*this) = v;
	}
};

///	@}

///	@}

}	// namespace ni

}	// namespace ml

#endif	// #ifndef LIBMATCHLOCK_INC_ML_TYPE_ML_COLOR_H
