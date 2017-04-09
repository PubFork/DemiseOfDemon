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

///	@addtogroup	math	���w
///	@{

///	@addtogroup	math_random
///	@{

///	@brief	�����N���X
class random{

public:
	///	@brief	������
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

	///	@brief	�R���X�g���N�^
	LIBMATCHLOCK_FORCEINLINE random() :
		is_enable_auto_shuffle_(true){

		seed_.data_[0] = 123456789;
		seed_.data_[1] = 362436069;
		seed_.data_[2] = 521288629;
		seed_.data_[3] = 88675123;
	}

	///	@brief	�f�X�g���N�^
	LIBMATCHLOCK_FORCEINLINE ~random(){
		;
	}

	///	@brief	������
	///	@note	i32(),f32()���ŌĂ΂��B
	LIBMATCHLOCK_FORCEINLINE void shuffle(){

		ml::ui32 t = (seed_.data_[0] ^ (seed_.data_[0] << 11));

		seed_.data_[0] = seed_.data_[1];
		seed_.data_[1] = seed_.data_[2];
		seed_.data_[2] = seed_.data_[3];
		seed_.data_[3] = (seed_.data_[3] ^ (seed_.data_[3] >> 19)) ^ (t ^ (t >> 8));
	}

	///	@brief	�����擾���ɃV���b�t�����s���悤�ɂ���
	LIBMATCHLOCK_FORCEINLINE void enable_auto_shuffle(){				is_enable_auto_shuffle_ = true;		}

	///	@brief	�����擾���ɃV���b�t�����s��Ȃ��悤�ɂ���
	LIBMATCHLOCK_FORCEINLINE void disable_auto_shuffle(){			is_enable_auto_shuffle_ = false;	}

	///	@brief	�����擾���ɃV���b�t�����s�����ǂ������擾����
	LIBMATCHLOCK_FORCEINLINE bool is_enable_auto_shuffle() const{	return is_enable_auto_shuffle_;		}

	///	@brief			���������߂�(32�r�b�g�������萮���l)
	///	@param[in]	max	�ő�l
	///	@return			����
	///	@note			0�ȏ�(0�܂�)�Amax����(max�͊܂܂Ȃ�)�l���o�͂��܂�
	LIBMATCHLOCK_FORCEINLINE int i32(int max){
		auto_shuffle();

		return
			(max == 0) ?
				0 :
				residue(seed_.data_[3], max);
	}

	///	@brief			���������߂�(32�r�b�g�������萮���l)
	///	@param[in]	min	�ŏ��l
	///	@param[in]	max	�ő�l
	///	@return			����
	///	@note			min�ȏ�(min�܂�)�Amax����(max�͊܂܂Ȃ�)�l���o�͂��܂�
	LIBMATCHLOCK_FORCEINLINE int i32(int min, int max){
		ML_ASSERT(min <= max);

		auto_shuffle();

		return
			(min == max) ?
				min :
				residue(seed_.data_[3], max - min) + min;
	}

	///	@brief				���������߂�(32�r�b�g�������萮���l)
	///	@param[in]	base	��l
	///	@param[in]	range	��
	///	@return				����
	///	@note				base - range �ȏ�Abase + range�����l���o�͂��܂�
	LIBMATCHLOCK_FORCEINLINE int i32_range(int base, int range){

		ML_ASSERT(range >= 0);

		return
			i32(
				base - range,
				base + range
			);
	}

	///	@brief				���������߂�(32�r�b�g�������萮���l)
	///	@param[in]	range	��
	///	@return				����
	///	@note				-range �ȏ�A+range�����l���o�͂��܂�
	LIBMATCHLOCK_FORCEINLINE int i32_range_base0(int range){

		ML_ASSERT(range >= 0);

		return
			i32(
				- range,
				+ range
			);
	}

	///	@brief			���������߂�(32�r�b�g���������_�l)
	///	@param[in]	max	�ő�l
	///	@return			����
	///	@note			0�ȏ�(0�܂�)�Amax����(max�͊܂܂Ȃ�)�l���o�͂��܂�
	LIBMATCHLOCK_FORCEINLINE float f32(float max){
		auto_shuffle();

		return (float(seed_.data_[3]) / float(0xFFFFFFFFUL)) * max;
//		return float(seed_.data_[3]) * max * float(1.0/4294967296.0);
	}

	///	@brief			���������߂�(32�r�b�g���������_�l)
	///	@param[in]	min	�ŏ��l
	///	@param[in]	max	�ő�l
	///	@return			����
	///	@note			min�ȏ�(min�܂�)�Amax����(max�͊܂܂Ȃ�)�l���o�͂��܂�
	LIBMATCHLOCK_FORCEINLINE float f32(float min, float max){
		ML_ASSERT(min <= max);

		auto_shuffle();

		return (float(seed_.data_[3]) / float(0xFFFFFFFFUL)) * (max - min) + min;
//		return float(seed_.data_[3]) * (max - min) * float(1.0/4294967296.0) + min;
	}

	///	@brief				���������߂�(32�r�b�g���������_�l)
	///	@param[in]	base	��l
	///	@param[in]	range	��
	///	@return				����
	///	@note				base - range �ȏ�Abase + range�����l���o�͂��܂�
	LIBMATCHLOCK_FORCEINLINE float f32_range(float base, float range){

		ML_ASSERT(range >= 0.0f);

		return
			f32(
				base - range,
				base + range
			);
	}

	///	@brief				���������߂�(32�r�b�g���������_�l)
	///	@param[in]	base	��l
	///	@param[in]	range	��
	///	@return				����
	///	@note				-range �ȏ�A+range�����l���o�͂��܂�
	LIBMATCHLOCK_FORCEINLINE float f32_range_base0(float range){

		ML_ASSERT(range >= 0.0f);

		return
			f32(
				- range,
				+ range
			);
	}

	///	@brief			����������߂�
	///	@return			������
	LIBMATCHLOCK_FORCEINLINE const seed_info	&seed() const{						return seed_;	}

	///	@brief			�������ݒ肷��
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

