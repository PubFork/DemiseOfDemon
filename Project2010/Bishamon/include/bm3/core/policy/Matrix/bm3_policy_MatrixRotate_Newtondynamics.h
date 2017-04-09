#ifndef BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_MATRIXROTATE_NEWTONDYNAMICS_H
#define BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_MATRIXROTATE_NEWTONDYNAMICS_H

#include "../../bm3_Const.h"
#include <ml/type/ml_vector3d.h>

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

namespace policy{

namespace impl{

/// MatrixRotate_NewtondynamicsƒNƒ‰ƒX
class MatrixRotate_Newtondynamics{

public:
	template<typename ResourceType>
	static BM3_FORCEINLINE void Initialize(const ResourceType &res, ml::random &random, ml::vector3d &value, ml::vector3d &speed, ml::vector3d &acceleration){

//		if(res.Rotate_Basic_RotateType() == RotateTypeConst_NewtonDynamics)
		{
			value.random_range(			random, res.Rotate_Value_Initial(),			res.Rotate_Value_Range());
			speed.random_range(			random, res.Rotate_Speed_Initial(),			res.Rotate_Speed_Range());
			acceleration.random_range(	random, res.Rotate_Acceleration_Initial(),	res.Rotate_Acceleration_Range());

			value			= value.to_radian();
			speed			= speed.to_radian();
			acceleration	= acceleration.to_radian();
		}
	}

	template<typename ResourceType>
	static BM3_FORCEINLINE void Initialize(const ResourceType &res, ml::random &random, float &value, float &speed, float &acceleration){

//		if(res.Rotate_Basic_RotateType() == RotateTypeConst_NewtonDynamics)
		{
			value = random.f32_range(			res.Rotate_Value_InitialX(),		res.Rotate_Value_RangeX());
			speed = random.f32_range(			res.Rotate_Speed_InitialX(),		res.Rotate_Speed_RangeX());
			acceleration = random.f32_range(	res.Rotate_Acceleration_InitialX(),	res.Rotate_Acceleration_RangeX());

			value			= ml::to_radian(value);
			speed			= ml::to_radian(speed);
			acceleration	= ml::to_radian(acceleration);
		}
	}

	template<typename ResourceType, typename UpdateInfo>
	static BM3_FORCEINLINE void Update(UpdateInfo &/*update_info*/, const ResourceType &res, ml::vector3d &value, ml::vector3d &speed, ml::vector3d &acceleration){

//		if(res.Rotate_Basic_RotateType() == RotateTypeConst_NewtonDynamics)
		{
			speed.add(acceleration);
			value.add(speed);
		}
	}

	template<typename ResourceType>
	static BM3_FORCEINLINE void Prepare(const ResourceType & /*res*/, const ml::vector3d &value, ml::matrix44 &m){

//		if(res.Rotate_Basic_RotateType() == RotateTypeConst_NewtonDynamics)
		{
			m.mul_rot_xyz(value.e_.x_, value.e_.y_, value.e_.z_);
		}
	}
	template<typename ResourceType>
	static BM3_FORCEINLINE void PrepareBm2(const ResourceType & /*res*/, const ml::vector3d &value, ml::matrix44 &m){

		m.mul_rot_zxy(value.e_.x_, value.e_.y_, value.e_.z_);
	}
};

}	// namespace impl

class MatrixRotate_Newtondynamics{

public:
	template<typename ResourceType>
	void BM3_FORCEINLINE Initialize(const ResourceType &res, ml::random &random, ml::vector3d &value){

		impl::MatrixRotate_Newtondynamics::Initialize(res, random, value, speed_, acceleration_);
	}

	template<typename ResourceType, typename UpdateInfo>
	void BM3_FORCEINLINE Update(UpdateInfo &update_info, const ResourceType &res, ml::vector3d &value){

		impl::MatrixRotate_Newtondynamics::Update(update_info, res, value, speed_, acceleration_);
	}

	template<typename ResourceType>
	void BM3_FORCEINLINE Prepare(const ResourceType &res, const ml::vector3d &value, ml::matrix44 &m){

		impl::MatrixRotate_Newtondynamics::Prepare(res, value, m);
	}

private:
	ml::ni::vector3d	speed_;
	ml::ni::vector3d	acceleration_;
};

}	// namespace policy
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_MATRIXROTATE_NEWTONDYNAMICS_H
