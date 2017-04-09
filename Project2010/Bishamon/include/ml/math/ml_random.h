#ifndef LIBMATCHLOCK_INC_MATH_ML_RANDOM_H
#define LIBMATCHLOCK_INC_MATH_ML_RANDOM_H

#if defined(BM3_PLATFORM_nw4c)
#pragma diag_suppress 2530
#endif

#include "../../libmatchlock_config.h"
#include "../type/ml_basic.h"
#include "../utility/ml_assert.h"
#include "ml_arithmetic.h"

namespace ml{

///	@addtogroup	math	数学
///	@{

///	@addtogroup	math_random
///	@{

///	@brief	乱数クラス
class random{

public:
	///	@brief	乱数種
	struct seed_info{

		seed_info(){}
		seed_info(ml::ui32 d){
			set1(d);
		}

		ml::ui32	data_[4];

		LIBMATCHLOCK_FORCEINLINE void	set1(ml::ui32 d){
#if 0
			data_[0] = 0x19770322 * (d ^ (d >> 30)) + 0;
			d = data_[0];

			data_[1] = 0x19770322 * (d ^ (d >> 30)) + 1;
			d = data_[1];

			data_[2] = 0x19770322 * (d ^ (d >> 30)) + 2;
			d = data_[2];

			data_[3] = 0x19770322 * (d ^ (d >> 30)) + 3;
			d = data_[3];
#else
			ml::ui32 i = 0x19770322 * (d ^ (d >> 30)) + 0;
			ml::ui32 j = 0x19770322 * (i ^ (i >> 30)) + 1;
			ml::ui32 k = 0x19770322 * (j ^ (j >> 30)) + 2;
			ml::ui32 l = 0x19770322 * (k ^ (k >> 30)) + 3;

			data_[0] = i;
			data_[1] = j;
			data_[2] = k;
			data_[3] = l;
#endif
		}
	};

	///	@brief	コンストラクタ
	LIBMATCHLOCK_FORCEINLINE random() :
		is_enable_auto_shuffle_(true){

		seed_.data_[0] = 123456789;
		seed_.data_[1] = 362436069;
		seed_.data_[2] = 521288629;
		seed_.data_[3] = 88675123;
	}

	///	@brief	デストラクタ
	LIBMATCHLOCK_FORCEINLINE ~random(){
		;
	}

	///	@brief	混ぜる
	///	@note	i32(),f32()内で呼ばれる。
	LIBMATCHLOCK_FORCEINLINE void shuffle(){

		ml::ui32 t = (seed_.data_[0] ^ (seed_.data_[0] << 11));

		seed_.data_[0] = seed_.data_[1];
		seed_.data_[1] = seed_.data_[2];
		seed_.data_[2] = seed_.data_[3];
		seed_.data_[3] = (seed_.data_[3] ^ (seed_.data_[3] >> 19)) ^ (t ^ (t >> 8));
	}

	///	@brief	乱数取得時にシャッフルを行うようにする
	LIBMATCHLOCK_FORCEINLINE void enable_auto_shuffle(){				is_enable_auto_shuffle_ = true;		}

	///	@brief	乱数取得時にシャッフルを行わないようにする
	LIBMATCHLOCK_FORCEINLINE void disable_auto_shuffle(){			is_enable_auto_shuffle_ = false;	}

	///	@brief	乱数取得時にシャッフルを行うかどうかを取得する
	LIBMATCHLOCK_FORCEINLINE bool is_enable_auto_shuffle() const{	return is_enable_auto_shuffle_;		}

	///	@brief			乱数を求める(32ビット符号あり整数値)
	///	@param[in]	max	最大値
	///	@return			乱数
	///	@note			0以上(0含む)、max未満(maxは含まない)値を出力します
	LIBMATCHLOCK_FORCEINLINE int i32(int max){
		auto_shuffle();

		return
			(max == 0) ?
				0 :
				residue(seed_.data_[3], max);
	}

	///	@brief			乱数を求める(32ビット符号あり整数値)
	///	@param[in]	min	最小値
	///	@param[in]	max	最大値
	///	@return			乱数
	///	@note			min以上(min含む)、max未満(maxは含まない)値を出力します
	LIBMATCHLOCK_FORCEINLINE int i32(int min, int max){
		ML_ASSERT(min <= max);

		auto_shuffle();

		return
			(min == max) ?
				min :
				residue(seed_.data_[3], max - min) + min;
	}

	///	@brief				乱数を求める(32ビット符号あり整数値)
	///	@param[in]	base	基準値
	///	@param[in]	range	幅
	///	@return				乱数
	///	@note				base - range 以上、base + range未満値を出力します
	LIBMATCHLOCK_FORCEINLINE int i32_range(int base, int range){

		ML_ASSERT(range >= 0);

		return
			i32(
				base - range,
				base + range
			);
	}

	///	@brief				乱数を求める(32ビット符号あり整数値)
	///	@param[in]	range	幅
	///	@return				乱数
	///	@note				-range 以上、+range未満値を出力します
	LIBMATCHLOCK_FORCEINLINE int i32_range_base0(int range){

		ML_ASSERT(range >= 0);

		return
			i32(
				- range,
				+ range
			);
	}

	///	@brief			乱数を求める(32ビット浮動小数点値)
	///	@param[in]	max	最大値
	///	@return			乱数
	///	@note			0以上(0含む)、max未満(maxは含まない)値を出力します
	LIBMATCHLOCK_FORCEINLINE float f32(float max){
		auto_shuffle();

		return (float(seed_.data_[3]) / float(0xFFFFFFFFUL)) * max;
//		return float(seed_.data_[3]) * max * float(1.0/4294967296.0);
	}

	///	@brief			乱数を求める(32ビット浮動小数点値)
	///	@param[in]	min	最小値
	///	@param[in]	max	最大値
	///	@return			乱数
	///	@note			min以上(min含む)、max未満(maxは含まない)値を出力します
	LIBMATCHLOCK_FORCEINLINE float f32(float min, float max){
		ML_ASSERT(min <= max);

		auto_shuffle();

		return (float(seed_.data_[3]) / float(0xFFFFFFFFUL)) * (max - min) + min;
//		return float(seed_.data_[3]) * (max - min) * float(1.0/4294967296.0) + min;
	}

	///	@brief				乱数を求める(32ビット浮動小数点値)
	///	@param[in]	base	基準値
	///	@param[in]	range	幅
	///	@return				乱数
	///	@note				base - range 以上、base + range未満値を出力します
	LIBMATCHLOCK_FORCEINLINE float f32_range(float base, float range){

		ML_ASSERT(range >= 0.0f);

		return
			f32(
				base - range,
				base + range
			);
	}

	///	@brief				乱数を求める(32ビット浮動小数点値)
	///	@param[in]	base	基準値
	///	@param[in]	range	幅
	///	@return				乱数
	///	@note				-range 以上、+range未満値を出力します
	LIBMATCHLOCK_FORCEINLINE float f32_range_base0(float range){

		ML_ASSERT(range >= 0.0f);

		return
			f32(
				- range,
				+ range
			);
	}

	///	@brief			乱数種を求める
	///	@return			乱数種
	LIBMATCHLOCK_FORCEINLINE const seed_info	&seed() const{						return seed_;	}

	///	@brief			乱数種を設定する
	LIBMATCHLOCK_FORCEINLINE void			set_seed(const seed_info &seed){	seed_ = seed;	}

private:
	seed_info	seed_;
	bool		is_enable_auto_shuffle_;

	LIBMATCHLOCK_FORCEINLINE void auto_shuffle(){
		if(is_enable_auto_shuffle_){
			shuffle();
		}
	}
};

///	@}

///	@}

}	// namespace ml

#endif	// #ifndef LIBMATCHLOCK_INC_MATH_ML_RANDOM_H

