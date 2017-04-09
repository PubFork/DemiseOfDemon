#ifndef LIBMATCHLOCK_INC_MATH_ML_ARITHMETIC_H
#define LIBMATCHLOCK_INC_MATH_ML_ARITHMETIC_H

#include <cmath>

#if defined(_XBOX_VER)
	#include <PPCIntrinsics.h>
#endif

#include "../../utility/ml_assert.h"
#include "../../utility/ml_cast.h"
#include "../../type/traits/ml_is_interger.h"
#include "../../type/traits/ml_is_float.h"
#include "../../type/traits/ml_is_unsigned.h"
#include "../../type/ml_bool.h"
#include "../../type/ml_basic.h"

namespace ml{

class vector3d;

template<typename T>
T select(T cond, T positive, T negative);
	
	
///	@addtogroup	math
///	@{

///	@addtogroup	math_arithmetic
///	@{

///	@cond internal
namespace internal{

template<typename T>
inline T minimum(T a, T b, true_type){

	return (a < b) ? a : b;
}

template<typename T>
inline T minimum(T a, T b, false_type){

	return select(T(a - b), b, a);
}

template<typename T>
inline T maximum(T a, T b, true_type){

	return (a > b) ? a : b;
}

template<typename T>
inline T maximum(T a, T b, false_type){

	return select(T(a - b), a, b);
}

}	// namespace internal
///	@endcond

///	@brief			小さい方の値を求める
///	@param[in]	a	入力値
///	@param[in]	b	入力値
///	@return			小さい方の値
template<typename T>
inline T minimum(T a, T b){

	return internal::minimum(a, b, typename bool_type<is_unsigned<T>::value>::type());
}

///	@brief			大きい方の値を求める
///	@param[in]	a	入力値
///	@param[in]	b	入力値
///	@return			大きい方の値
template<typename T>
inline T maximum(T a, T b){

	return internal::maximum(a, b, typename bool_type<is_unsigned<T>::value>::type());
}

///	@brief				丸め込んだ値を求める
///	@param[in]	a		入力値
///	@param[in]	min_v	最小値
///	@param[in]	max_v	最大値
///	@return				丸め込んだ値
template<typename T>
inline T clamp(T a, T min_v, T max_v){

	ML_ASSERT(min_v <= max_v);

#if 0
	if(a < min_v){
		return min_v;
	}else if(a > max_v){
		return max_v;
	}else{
		return a;
	}
#else
	return
		minimum(
			maximum(
				a,
				min_v
			),
			max_v
		);
#endif
}

///	@brief			アラインした値を求める
///	@param[in]	a	入力値
///	@return			アラインした値
template<unsigned int align_size, typename T>
inline T align(T a){

	ML_STATIC_ASSERT(is_interger<T>::value);

	if(a == 0){
		return 0;
	}else{
		if(a > 0){	a += ((T)align_size - 1);	}
		else{		a -= ((T)align_size - 1);	}

		a /= (T)align_size;
		a *= (T)align_size;

		return a;
	}
}

///	@brief			値が指定内に収まっているかを判定する
///	@param[in]	a	入力値
///	@param[in]	min	最小値
///	@param[in]	max	最大値
///	@return			true / false
template<typename T>
inline bool is_between(T a, T min, T max){

	ML_ASSERT(min <= max);

	if(a < min){
		return false;
	}else if(a > max){
		return false;
	}else{
		return true;
	}
}

///	@brief			値が冪数かどうかを判定する
///	@param[in]	a	入力値
///	@return			true / false
template<typename T>
inline bool is_pow2(T a){

	ML_STATIC_ASSERT(is_interger<T>::value);

	return (unsigned(a) & (unsigned(a) - 1)) == 0;
}

///	@brief			値同士を交換する
///	@param[in]	a	入力値
///	@param[in]	b	入力値
template<typename T>
inline void swap(T &a, T &b){

	T t = a;
	a = b;
	b = t;
}

#if 0
///	@brief			値の逆数を求める
///	@param[in]	a	入力値
///	@return			入力値の逆数
template<typename T>
inline T inverse(T a){

#if defined(_XBOX_VER)
//	return __fres(a);
#else
	return T(1) / a;
#endif
}
#else

#if defined(_XBOX_VER)
inline float inverse(float a){

	float ia = __fres(a);
	return 2.0f * ia - a * ia * ia;
}
#endif

///	@brief			値の逆数を求める
///	@param[in]	a	入力値
///	@return			入力値の逆数
template<typename T>
inline T inverse(T a){

	return T(1) / a;
}
#endif

///	@brief			商を求める
///	@param[in]	a	入力値
///	@param[in]	b	入力値
///	@return			商
template<typename T>
inline T division(T a, T b){

	return a / b;
}

///	@brief			割り算の余りを求める
///	@param[in]	a	入力値
///	@param[in]	b	入力値
///	@return			余り
template<typename T, typename U>
inline int residue(T a, U b){

	ML_STATIC_ASSERT(is_interger<T>::value);
	ML_STATIC_ASSERT(is_interger<U>::value);

	return a % b;
}

///	@brief			値の絶対値を求める
///	@param[in]	a	入力値
///	@return			入力値の絶対値
template<typename T>
inline T abs(T a){

	return (a < T()) ? T(-a) : a;
}

///	@brief					条件によって値を選択する
///	@param[in]	cond		選択条件
///	@param[in]	positive	cond >= 0 時の値
///	@param[in]	negative	cond <  0 時の値
///	@return					判定後の値
///	@note					cond が 0 以上であれば positive、そうでなければ negative を返す
template<typename T>
inline T select(T cond, T positive, T negative){

	return (cond >= T()) ? positive : negative;
}

#if defined(_XBOX_VER)
template<>
inline float select(float cond, float positive, float negative){

	return __fself(cond, positive, negative);
}
#endif


///	@brief			平方根を求める
///	@param[in]	a	入力値
///	@return			入力値の平方根
inline float sqrtf(float a){

	ML_ASSERT(a >= 0.0f);

#if defined(_XBOX_VER)
	return __fsqrts(a);

#else

#if defined(_MSC_VER)
	return std::sqrtf(a);

#elif defined(__GNUC__)
	return numeric_cast<float>(std::sqrt(numeric_cast<double>(a)));

#elif defined(__SNC__)
	return std::sqrtf(a);

#else
	ML_STATIC_ASSERT(false);
#endif

#endif
}

///	@brief			平方根の逆数を求める
///	@param[in]	a	入力値
///	@return			入力値の平方根の逆数
inline float inverse_sqrtf(float a){

	ML_ASSERT(a > 0.0f);

#if defined(_XBOX_VER)
//	return numeric_cast<float>(__frsqrte(numeric_cast<double>(a)));

	float rsqrt = numeric_cast<float>(__frsqrte(numeric_cast<double>(a)));
	return (rsqrt * 0.5f) * (3.0f - rsqrt * rsqrt * a);

#else
	
	return inverse(sqrtf(a));

#endif
}

///	@brief			階乗を求める
///	@param[in]	a	底
///	@param[in]	b	指数
///	@return			冪乗
inline float pow(float a, float b){

#if defined(_MSC_VER)
	return std::powf(a, b);

#else
	return numeric_cast<float>(std::pow(numeric_cast<float>(a), numeric_cast<float>(b)));

#endif
}

///	@brief			ラジアンから度を求める
///	@param[in]	a	入力値
///	@return			度
inline float to_degree(float a){

	return a * (180.0f / 3.14159265358979323846f);
}

///	@brief			度からラジアンを求める
///	@param[in]	a	入力値
///	@return			ラジアン
inline float to_radian(float a){

	return a * (3.14159265358979323846f / 180.0f);
}

///	@brief				値が0に近いかを判定する
///	@param[in]	a		入力値
///	@param[in]	maxUlps	閾値
///	@return				true  / false
bool is_near_zero(float a, int maxUlps = 150);

///	@brief				値同士が近い値かを判定する
///	@param[in]	a		入力値
///	@param[in]	b		入力値
///	@param[in]	maxUlps	閾値
///	@return				true  / false
#if defined(_XBOX_VER)	// 精度が低いため誤差範囲を広げておく
bool is_near(float a, float b, int maxUlps = 100000);
#else
bool is_near(float a, float b, int maxUlps = 150);
#endif

///	@brief				冪数に繰り上げた値を求める
///	@param[in]	a		入力値
ui32 make_forword_pow2(ui32 a);

///	@cond internal
namespace internal{
template<int Count>
inline float expansion_pow(float a){
	ML_STATIC_ASSERT(Count >= 0);
	return a * expansion_pow<Count - 1>(a);
}

template<>
inline float expansion_pow<0>(float/* a*/){
	return 1.0f;
}

template<int Count>
inline int expansion_pow(int a){
	ML_STATIC_ASSERT(Count >= 0);
	return a * expansion_pow<Count - 1>(a);
}

template<>
inline int expansion_pow<0>(int/* a*/){
	return 1;
}

}	// namespace internal
///	@endcond

///	@brief				冪数を求める
template<int Count>
inline float expansion_pow(float a){
	return internal::expansion_pow<Count>(a);
}

///	@brief				冪数を求める
template<int Count>
inline int expansion_pow(int a){
	return internal::expansion_pow<Count>(a);
}

///	@cond internal
namespace internal{

template<int size>
struct endian_reverse{
	template<typename T>
	inline static T execute(T a);
};

template<>
struct endian_reverse<1>{
	template<typename T>
	inline static T execute(T a){
		return a;
	}
};

template<>
struct endian_reverse<2>{
	template<typename T>
	inline static T execute(T a){
		ui8 *p = reinterpret_cast<ui8 *>(&a);
		swap(p[0], p[1]);
		return a;
	}
};

template<>
struct endian_reverse<4>{
	template<typename T>
	inline static T execute(T a){
		ui8 *p = reinterpret_cast<ui8 *>(&a);
		swap(p[0], p[3]);
		swap(p[1], p[2]);
		return a;
	}
};

template<>
struct endian_reverse<8>{
	template<typename T>
	inline static T execute(T a){
		ui8 *p = reinterpret_cast<ui8 *>(&a);
		swap(p[0], p[7]);
		swap(p[1], p[6]);
		swap(p[2], p[5]);
		swap(p[3], p[4]);
		return a;
	}
};

}	// namespace internal
///	@endcond

///	@brief			エンディアンを反転させる
///	@param[in]	a	入力値
///	@return			エンディアンを反転した値
template<typename T>
inline T endian_reverse(T a){

	return internal::endian_reverse<sizeof(a)>::execute(a);
}

// x86ではf[32|64]_endian_reverse をつかう必要がある
// もしくは変換先、変換元を受け付けるendian_reverse()をつかう必要がある
// http://0xcc.net/blog/archives/000164.html
template<>
inline float endian_reverse(float a);
template<>
inline double endian_reverse(double a);

///	@brief			f32変数のエンディアンを反転させる
///	@param[in]	a	入力値
///	@return			エンディアンを反転した値
inline ui32 f32_endian_reverse(const f32 &a){

	union U{
		ui8		v_ui8_[4];
		ui32	v_ui32_;
		f32		v_f32_;
	}u;

	u.v_f32_ = a;

	swap(u.v_ui8_[0], u.v_ui8_[3]);
	swap(u.v_ui8_[1], u.v_ui8_[2]);

	return u.v_ui32_;
}

///	@brief			f64変数のエンディアンを反転させる
///	@param[in]	a	入力値
///	@return			エンディアンを反転した値
inline ui64 f64_endian_reverse(const f64 &a){

	union U{
		ui8		v_ui8_[8];
		ui64	v_ui64_;
		f64		v_f64_;
	}u;

	u.v_f64_ = a;

	swap(u.v_ui8_[0], u.v_ui8_[7]);
	swap(u.v_ui8_[1], u.v_ui8_[6]);
	swap(u.v_ui8_[2], u.v_ui8_[5]);
	swap(u.v_ui8_[3], u.v_ui8_[4]);

	return u.v_ui64_;
}

///	@cond internal
namespace internal{

template<typename T>
inline void endian_reverse_impl(T &dst, T src, f32){

	((ui32 &)dst) = f32_endian_reverse(src);
}

template<typename T>
inline void endian_reverse_impl(T &dst, T src, f64){

	((ui64 &)dst) = f64_endian_reverse(src);
}

template<typename T, typename U>
inline void endian_reverse_impl(T &dst, T src, U){

	dst = ml::endian_reverse(src);
}

}	// namespace internal
///	@endcond

template<typename T>
inline void endian_reverse(T &dst, T src){

	internal::endian_reverse_impl(dst, src, T());
}

///	@brief			三角形の面積を求める
///	@param[in]	p0	頂点
///	@param[in]	p1	頂点
///	@param[in]	p2	頂点
///	@return			三角形の面積
float	triangle_measure(const vector3d &p0, const vector3d &p1, const vector3d &p2);

///	@}

///	@}

}	// namespace ml

#endif	// #ifndef LIBMATCHLOCK_INC_MATH_ML_ARITHMETIC_H
