#ifndef BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_MATRIXSCALE_CURVEVALUE_H
#define BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_MATRIXSCALE_CURVEVALUE_H

#include "../../bm3_Const.h"
#include "../../bm3_CoreType.h"
#include <ml/type/ml_vector3d.h>

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

namespace policy{

namespace impl{

/// @brief MatrixScale_CurveValueƒNƒ‰ƒX
template<bool AllowFor_Scale_Basic_Square = true>
class MatrixScale_CurveValue{

public:
	template<typename ResourceType>
	static BM3_FORCEINLINE void Initialize(const ResourceType &res, ml::random &random, ml::vector3d &value, const PrimitiveInitializeInfo &primitive_initialize_info);

	template<typename ResourceType>
	static BM3_FORCEINLINE void Initialize(const ResourceType &res, ml::random &random, float &value, const PrimitiveInitializeInfo &primitive_initialize_info);

	template<typename ResourceType, typename UpdateInfo>
	static BM3_FORCEINLINE void Update(UpdateInfo &update_info, const ResourceType &res, ml::vector3d &value, const PrimitiveUpdateInfo &primitive_update_info);

	template<typename ResourceType>
	static BM3_FORCEINLINE void Prepare(const ResourceType &res, const ml::vector3d &value, ml::matrix44 &m, const PrimitivePrepareInfo &primitive_prepare_info);
};

template<>
class MatrixScale_CurveValue<true>{

public:
	template<typename ResourceType>
	static BM3_FORCEINLINE void Initialize(const ResourceType &res, ml::random &/*random*/, ml::vector3d &value, const PrimitiveInitializeInfo &primitive_initialize_info){

//		if(res.Scale_Basic_ScaleType() == ScaleTypeConst_CurveValue)
		{
			value = res.Scale_Value_Curve().Value(0.0f, primitive_initialize_info.random_seed_, res.Scale_Value_Range());

			if(res.Scale_Basic_Square()){
				value.e_.x_ = ml::maximum(value.e_.x_, 0.0f);

				value.e_.y_ = value.e_.x_;
				value.e_.z_ = value.e_.x_;
			}else{
				value.maximum0();
			}
		}
	}

	template<typename ResourceType>
	static BM3_FORCEINLINE void Initialize(const ResourceType &res, ml::random &/*random*/, float &value, const PrimitiveInitializeInfo &primitive_initialize_info){

//		if(res.Scale_Basic_ScaleType() == ScaleTypeConst_CurveValue)
		{
			value = res.Scale_Value_CurveX().Value(0.0f, primitive_initialize_info.random_seed_, res.Scale_Value_RangeX());
			value = ml::maximum(value, 0.0f);
		}
	}

	template<typename ResourceType, typename UpdateInfo>
	static BM3_FORCEINLINE void Update(UpdateInfo &/*update_info*/, const ResourceType &res, ml::vector3d &value, const PrimitiveUpdateInfo &primitive_update_info){

//		if(res.Scale_Basic_ScaleType() == ScaleTypeConst_CurveValue)
		{
			value = res.Scale_Value_Curve().Value(primitive_update_info.life_rate_, primitive_update_info.random_seed_, res.Scale_Value_Range());

			if(res.Scale_Basic_Square()){
				value.e_.x_ = ml::maximum(value.e_.x_, 0.0f);

				value.e_.y_ = value.e_.x_;
				value.e_.z_ = value.e_.x_;
			}else{
				value.maximum0();
			}
		}
	}

	template<typename ResourceType>
	static BM3_FORCEINLINE void Prepare(const ResourceType &res, const ml::vector3d &value, ml::matrix44 &m, const PrimitivePrepareInfo &/*primitive_prepare_info*/){

		if(res.Scale_Basic_ScaleType() == ScaleTypeConst_CurveValue)
		{
			m.mul_scale(value.e_.x_, value.e_.y_, value.e_.z_);
		}
	}
};

template<>
class MatrixScale_CurveValue<false>{

public:
	template<typename ResourceType>
	static BM3_FORCEINLINE void Initialize(const ResourceType &res, ml::random &/*random*/, ml::vector3d &value, const PrimitiveInitializeInfo &primitive_initialize_info){

//		if(res.Scale_Basic_ScaleType() == ScaleTypeConst_CurveValue)
		{
			value = res.Scale_Value_Curve().Value(0.0f, primitive_initialize_info.random_seed_, res.Scale_Value_Range());
			value.maximum0();
		}
	}

	template<typename ResourceType, typename UpdateInfo>
	static BM3_FORCEINLINE void Update(UpdateInfo &/*update_info*/, const ResourceType &res, ml::vector3d &value, const PrimitiveUpdateInfo &primitive_update_info){

		if(res.Scale_Basic_ScaleType() == ScaleTypeConst_CurveValue)
		{
			value = res.Scale_Value_Curve().Value(primitive_update_info.life_rate_, primitive_update_info.random_seed_, res.Scale_Value_Range());
			value.maximum0();
		}
	}

	template<typename ResourceType>
	static BM3_FORCEINLINE void Prepare(const ResourceType &res, const ml::vector3d &value, ml::matrix44 &m, const PrimitivePrepareInfo &primitive_prepare_info){

		if(res.Scale_Basic_ScaleType() == ScaleTypeConst_CurveValue)
		{
			m.mul_scale(value.e_.x_, value.e_.y_, value.e_.z_);
		}
	}
};

}	// namespace impl

template<bool AllowFor_Scale_Basic_Square = true>
class MatrixScale_CurveValue{

public:
	template<typename ResourceType>
	void BM3_FORCEINLINE Initialize(const ResourceType &res, ml::random &random, ml::vector3d &value, const PrimitiveInitializeInfo &primitive_initialize_info){

		impl::MatrixScale_CurveValue<AllowFor_Scale_Basic_Square>::Initialize(res, random, value, primitive_initialize_info);
	}

	template<typename ResourceType, typename UpdateInfo>
	void BM3_FORCEINLINE Update(UpdateInfo &update_info, const ResourceType &res, ml::vector3d &value, const PrimitiveUpdateInfo &primitive_update_info){

		impl::MatrixScale_CurveValue<AllowFor_Scale_Basic_Square>::Update(update_info, res, value, primitive_update_info);
	}

	template<typename ResourceType>
	void BM3_FORCEINLINE Prepare(const ResourceType &res, const ml::vector3d &value, ml::matrix44 &m, const PrimitivePrepareInfo &primitive_prepare_info){

		impl::MatrixScale_CurveValue<AllowFor_Scale_Basic_Square>::Prepare(res, value, m, primitive_prepare_info);
	}
};

}	// namespace policy
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_MATRIXSCALE_CURVEVALUE_H
