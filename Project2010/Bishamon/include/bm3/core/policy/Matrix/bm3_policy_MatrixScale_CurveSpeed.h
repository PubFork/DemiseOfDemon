#ifndef BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_MatrixScale_CURVESPEED_H
#define BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_MatrixScale_CURVESPEED_H

#include "../../bm3_Const.h"
#include "../../bm3_CoreType.h"
#include <ml/type/ml_vector3d.h>

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

namespace policy{

namespace impl{

/// @brief MatrixScale_CurveSpeedƒNƒ‰ƒX
template<bool AllowFor_Scale_Basic_Square = true>
class MatrixScale_CurveSpeed{

public:
	template<typename ResourceType>
	static BM3_FORCEINLINE void Initialize(const ResourceType &res, ml::random &random, ml::vector3d &value, ml::vector3d &speed, const PrimitiveInitializeInfo &primitive_initialize_info);

	template<typename ResourceType>
	static BM3_FORCEINLINE void Initialize(const ResourceType &res, ml::random &random, float &value, float &speed, const PrimitiveInitializeInfo &primitive_initialize_info);

	template<typename ResourceType, typename UpdateInfo>
	static BM3_FORCEINLINE void Update(UpdateInfo &update_info, const ResourceType &res, ml::vector3d &value, ml::vector3d &speed, const PrimitiveUpdateInfo &primitive_update_info);

	template<typename ResourceType>
	static BM3_FORCEINLINE void Prepare(const ResourceType &res, const ml::vector3d &value, ml::matrix44 &m, const PrimitivePrepareInfo &primitive_prepare_info);
};

template<>
class MatrixScale_CurveSpeed<true>{

public:
	template<typename ResourceType>
	static BM3_FORCEINLINE void Initialize(const ResourceType &res, ml::random &random, ml::vector3d &value, ml::vector3d &speed, const PrimitiveInitializeInfo &primitive_initialize_info){

//		if(res.Scale_Basic_ScaleType() == ScaleTypeConst_CurveSpeed)
		{
			speed	= res.Scale_Speed_Curve().Value(0.0f, primitive_initialize_info.random_seed_, res.Scale_Speed_Range());
			value.random_range(res.Scale_Basic_Square(), random, res.Scale_Value_Initial(), res.Scale_Value_Range());
			value.maximum0();
		}
	}

	template<typename ResourceType>
	static BM3_FORCEINLINE void Initialize(const ResourceType &res, ml::random &random, float &value, float &speed, const PrimitiveInitializeInfo &primitive_initialize_info){

//		if(res.Scale_Basic_ScaleType() == ScaleTypeConst_CurveSpeed)
		{
			speed	= res.Scale_Speed_CurveX().Value(0.0f, primitive_initialize_info.random_seed_, res.Scale_Speed_RangeX());
			value = random.f32_range(res.Scale_Value_InitialX(), res.Scale_Value_RangeX());
			value = ml::maximum(value, 0.0f);
		}
	}

	template<typename ResourceType, typename UpdateInfo>
	static BM3_FORCEINLINE void Update(UpdateInfo &/*update_info*/, const ResourceType &res, ml::vector3d &value, ml::vector3d &speed, const PrimitiveUpdateInfo &primitive_update_info){

//		if(res.Scale_Basic_ScaleType() == ScaleTypeConst_CurveSpeed)
		{
			speed = res.Scale_Speed_Curve().Value(primitive_update_info.life_rate_, primitive_update_info.random_seed_, res.Scale_Speed_Range());

			if(res.Scale_Basic_Square()){
				speed.e_.y_ = speed.e_.x_;
				speed.e_.z_ = speed.e_.x_;
			}

			value.add(speed);
			value.maximum0();
		}
	}

	template<typename ResourceType>
	static BM3_FORCEINLINE void Prepare(const ResourceType &res, const ml::vector3d &value, ml::matrix44 &m, const PrimitivePrepareInfo &/*primitive_prepare_info*/){

		if(res.Scale_Basic_ScaleType() == ScaleTypeConst_CurveSpeed)
		{
			m.mul_scale(value.e_.x_, value.e_.y_, value.e_.z_);
		}
	}
};

template<>
class MatrixScale_CurveSpeed<false>{

public:
	template<typename ResourceType>
	static BM3_FORCEINLINE void Initialize(const ResourceType &res, ml::random &random, ml::vector3d &value, ml::vector3d &speed, const PrimitiveInitializeInfo &primitive_initialize_info){

//		if(res.Scale_Basic_ScaleType() == ScaleTypeConst_CurveSpeed)
		{
			speed	= res.Scale_Speed_Curve().Value(0.0f, primitive_initialize_info.random_seed_, res.Scale_Speed_Range());
			value.random_range(random, res.Scale_Value_Initial(), res.Scale_Value_Range());
			value.maximum0();
		}
	}

	template<typename ResourceType, typename UpdateInfo>
	static BM3_FORCEINLINE void Update(UpdateInfo &/*update_info*/, const ResourceType &res, ml::vector3d &value, ml::vector3d &speed, const PrimitiveUpdateInfo &primitive_update_info){

		if(res.Scale_Basic_ScaleType() == ScaleTypeConst_CurveSpeed)
		{
			speed = res.Scale_Speed_Curve().Value(primitive_update_info.life_rate_, primitive_update_info.random_seed_, res.Scale_Speed_Range());
			value.add(speed);
			value.maximum0();
		}
	}

	template<typename ResourceType>
	static BM3_FORCEINLINE void Prepare(const ResourceType &res, const ml::vector3d &value, ml::matrix44 &m, const PrimitivePrepareInfo &primitive_prepare_info){

		if(res.Scale_Basic_ScaleType() == ScaleTypeConst_CurveSpeed)
		{
			m.mul_scale(value.e_.x_, value.e_.y_, value.e_.z_);
		}
	}
};

}	// namespace impl

template<bool AllowFor_Scale_Basic_Square = true>
class MatrixScale_CurveSpeed{

public:
	template<typename ResourceType>
	void BM3_FORCEINLINE Initialize(const ResourceType &res, ml::random &random, ml::vector3d &value, ml::vector3d &speed, const PrimitiveInitializeInfo &primitive_initialize_info){

		impl::MatrixScale_CurveSpeed<AllowFor_Scale_Basic_Square>::Initialize(res, random, value, speed, primitive_initialize_info);
	}

	template<typename ResourceType, typename UpdateInfo>
	void BM3_FORCEINLINE Update(UpdateInfo &update_info, const ResourceType &res, ml::vector3d &value, ml::vector3d &speed, const PrimitiveUpdateInfo &primitive_update_info){

		impl::MatrixScale_CurveSpeed<AllowFor_Scale_Basic_Square>::Update(update_info, res, value, speed, primitive_update_info);
	}

	template<typename ResourceType>
	void BM3_FORCEINLINE Prepare(const ResourceType &res, const ml::vector3d &value, ml::matrix44 &m, const PrimitivePrepareInfo &primitive_prepare_info){

		impl::MatrixScale_CurveSpeed<AllowFor_Scale_Basic_Square>::Prepare(res, value, m, primitive_prepare_info);
	}
};

}	// namespace policy
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_MatrixScale_CURVESPEED_H
