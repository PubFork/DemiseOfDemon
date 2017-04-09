#ifndef BM3_SDK_INC_BM3_CORE_TYPE_INTERNAL_BM3_INTERPOLATEKEYFRAME_H
#define BM3_SDK_INC_BM3_CORE_TYPE_INTERNAL_BM3_INTERPOLATEKEYFRAME_H

#include "../../../system/bm3_Config.h"
#include "../../bm3_CoreType.h"

#include <ml/math/ml_interpolate.h>
#include <ml/type/ml_int2type.h>

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

namespace internal{

ML_STATIC_ASSERT(KeyframeTypeConst_Constant == 0);
ML_STATIC_ASSERT(KeyframeTypeConst_Linear == 1);
ML_STATIC_ASSERT(KeyframeTypeConst_Spline == 2);
ML_STATIC_ASSERT(KeyframeTypeConst_BreakSpline == 3);
ML_STATIC_ASSERT(KeyframeTypeConst_Bezier == 4);
ML_STATIC_ASSERT(KeyframeTypeConst_MAX == 5);


template<int index>
BM3_FORCEINLINE void InterpolateKeyframe(
	float *dst_array,
	float rate,
	const float *src_left_array,
	const float *src_right_array,
	const KeyframeElementInfo *left_info_array,
	const KeyframeElementInfo *right_info_array
){
	switch(left_info_array[index].keyframe_type_){
//		case KeyframeTypeConst_Constant:	dst_array[index] = (rate < 1.0f) ? src_left_array[index] : src_right_array[index];																			break;
		case KeyframeTypeConst_Constant:	dst_array[index] = ml::select(rate - 1.0f,  src_right_array[index], src_left_array[index]);																	break;
		case KeyframeTypeConst_Linear:		dst_array[index] = ml::lerp(	rate, src_left_array[index], src_right_array[index]);																		break;
		case KeyframeTypeConst_Spline:		dst_array[index] = ml::hermite(	rate, src_left_array[index], src_right_array[index], left_info_array[index].slope_l_, right_info_array[index].slope_r_);	break;
		case KeyframeTypeConst_BreakSpline:	dst_array[index] = ml::hermite(	rate, src_left_array[index], src_right_array[index], left_info_array[index].slope_l_, right_info_array[index].slope_r_);	break;
		case KeyframeTypeConst_Bezier:		dst_array[index] = ml::bezier(rate, src_left_array[index], left_info_array[index].slope_r_, right_info_array[index].slope_l_, src_right_array[index]);		break;
		default:							ML_ASSERT(false);		dst_array[index] = 0.0f;	BM3_ASSUME(0);																							break;
	}
}

BM3_FORCEINLINE void InterpolateKeyframe(
	float *dst_array,
	float rate,
	const float *src_left_array,
	const float *src_right_array,
	const KeyframeElementInfo *left_info_array,
	const KeyframeElementInfo *right_info_array,
	ml::int2type<1>
){
	InterpolateKeyframe<0>(dst_array, rate, src_left_array, src_right_array, left_info_array, right_info_array);
}

BM3_FORCEINLINE void InterpolateKeyframe(
	float *dst_array,
	float rate,
	const float *src_left_array,
	const float *src_right_array,
	const KeyframeElementInfo *left_info_array,
	const KeyframeElementInfo *right_info_array,
	ml::int2type<2>
){
	InterpolateKeyframe<0>(dst_array, rate, src_left_array, src_right_array, left_info_array, right_info_array);
	InterpolateKeyframe<1>(dst_array, rate, src_left_array, src_right_array, left_info_array, right_info_array);
}

BM3_FORCEINLINE void InterpolateKeyframe(
	float *dst_array,
	float rate,
	const float *src_left_array,
	const float *src_right_array,
	const KeyframeElementInfo *left_info_array,
	const KeyframeElementInfo *right_info_array,
	ml::int2type<3>
){
	InterpolateKeyframe<0>(dst_array, rate, src_left_array, src_right_array, left_info_array, right_info_array);
	InterpolateKeyframe<1>(dst_array, rate, src_left_array, src_right_array, left_info_array, right_info_array);
	InterpolateKeyframe<2>(dst_array, rate, src_left_array, src_right_array, left_info_array, right_info_array);
}

BM3_FORCEINLINE void InterpolateKeyframe(
	float *dst_array,
	float rate,
	const float *src_left_array,
	const float *src_right_array,
	const KeyframeElementInfo *left_info_array,
	const KeyframeElementInfo *right_info_array,
	ml::int2type<4>
){
	InterpolateKeyframe<0>(dst_array, rate, src_left_array, src_right_array, left_info_array, right_info_array);
	InterpolateKeyframe<1>(dst_array, rate, src_left_array, src_right_array, left_info_array, right_info_array);
	InterpolateKeyframe<2>(dst_array, rate, src_left_array, src_right_array, left_info_array, right_info_array);
	InterpolateKeyframe<3>(dst_array, rate, src_left_array, src_right_array, left_info_array, right_info_array);
}

}	// namespace internal

BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_TYPE_INTERNAL_BM3_INTERPOLATEKEYFRAME_H
