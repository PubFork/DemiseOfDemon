#ifndef BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_MatrixRotate_CURVESPEED_H
#define BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_MatrixRotate_CURVESPEED_H

#include "../../bm3_Const.h"
#include "../../bm3_CoreType.h"
#include <ml/type/ml_vector3d.h>

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

namespace policy{

namespace impl{

/// @brief MatrixRotate_CurveSpeedƒNƒ‰ƒX
class MatrixRotate_CurveSpeed{

public:
	template<typename ResourceType>
	static BM3_FORCEINLINE void Initialize(const ResourceType &res, ml::random &random, ml::vector3d &value, ml::vector3d &speed, const PrimitiveInitializeInfo &primitive_initialize_info){

//		if(res.Rotate_Basic_RotateType() == RotateTypeConst_CurveSpeed)
		{
			speed	= res.Rotate_Speed_Curve().Value(0.0f, primitive_initialize_info.random_seed_, res.Rotate_Speed_Range());
			value.random_range(random, res.Rotate_Value_Initial(), res.Rotate_Value_Range());
			value = value.to_radian();
		}
	}

	template<typename ResourceType>
	static BM3_FORCEINLINE void Initialize(const ResourceType &res, ml::random &random, float &value, float &speed, const PrimitiveInitializeInfo &primitive_initialize_info){

//		if(res.Rotate_Basic_RotateType() == RotateTypeConst_CurveSpeed)
		{
			speed	= res.Rotate_Speed_CurveX().Value(0.0f, primitive_initialize_info.random_seed_, res.Rotate_Speed_RangeX());
			value	= random.f32_range(res.Rotate_Value_InitialX(), res.Rotate_Value_RangeX());
			value	= ml::to_radian(value);
		}
	}

	template<typename ResourceType, typename UpdateInfo>
	static BM3_FORCEINLINE void Update(UpdateInfo &/*update_info*/, const ResourceType &res, ml::vector3d &value, ml::vector3d &speed, const PrimitiveUpdateInfo &primitive_update_info){

//		if(res.Rotate_Basic_RotateType() == RotateTypeConst_CurveSpeed)
		{
			speed = res.Rotate_Speed_Curve().Value(primitive_update_info.life_rate_, primitive_update_info.random_seed_, res.Rotate_Speed_Range());
			speed = speed.to_radian();
			value.add(speed);
		}
	}

	template<typename ResourceType>
	static BM3_FORCEINLINE void Prepare(const ResourceType & /*res*/, const ml::vector3d &value, ml::matrix44 &m, const PrimitivePrepareInfo &/*primitive_prepare_info*/){

//		if(res.Rotate_Basic_RotateType() == RotateTypeConst_CurveSpeed)
		{
			m.mul_rot_xyz(value.e_.x_, value.e_.y_, value.e_.z_);
		}
	}
	template<typename ResourceType>
	static BM3_FORCEINLINE void PrepareBm2(const ResourceType & /*res*/, const ml::vector3d &value, ml::matrix44 &m, const PrimitivePrepareInfo &/*primitive_prepare_info*/){

		m.mul_rot_zxy(value.e_.x_, value.e_.y_, value.e_.z_);
	}
};

}	// namespace impl

class MatrixRotate_CurveSpeed{

public:
	template<typename ResourceType>
	void BM3_FORCEINLINE Initialize(const ResourceType &res, ml::random &random, ml::vector3d &value, const PrimitiveInitializeInfo &primitive_initialize_info){

		impl::MatrixRotate_CurveSpeed::Initialize(res, random, value, primitive_initialize_info);
	}

	template<typename ResourceType, typename UpdateInfo>
	void BM3_FORCEINLINE Update(UpdateInfo &update_info, const ResourceType &res, ml::vector3d &value, const PrimitiveUpdateInfo &primitive_update_info){

		impl::MatrixRotate_CurveSpeed::Update(update_info, res, value, primitive_update_info);
	}

	template<typename ResourceType>
	void BM3_FORCEINLINE Prepare(const ResourceType &res, const ml::vector3d &value, ml::matrix44 &m, const PrimitivePrepareInfo &primitive_prepare_info){

		impl::MatrixRotate_CurveSpeed::Prepare(res, value, m, primitive_prepare_info);
	}
};

}	// namespace policy
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_MatrixRotate_CURVESPEED_H
