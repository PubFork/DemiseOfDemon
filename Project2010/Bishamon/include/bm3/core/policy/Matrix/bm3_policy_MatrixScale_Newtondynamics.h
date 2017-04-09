#ifndef BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_MATRIXSCALE_NEWTONDYNAMICS_H
#define BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_MATRIXSCALE_NEWTONDYNAMICS_H

#include "../../bm3_Const.h"
#include <ml/type/ml_vector3d.h>

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

namespace policy{

namespace impl{

/// @brief MatrixScale_NewtondynamicsƒNƒ‰ƒX
template<bool AllowFor_Scale_Basic_Square = true>
class MatrixScale_Newtondynamics{

public:
	template<typename ResourceType>
	static BM3_FORCEINLINE void Initialize(const ResourceType &res, ml::random &random, ml::vector3d &value, ml::vector3d &speed, ml::vector3d &acceleration);

	template<typename ResourceType>
	static BM3_FORCEINLINE void Initialize(const ResourceType &res, ml::random &random, float &value, float &speed, float &acceleration);

	template<typename ResourceType, typename UpdateInfo>
	static BM3_FORCEINLINE void Update(UpdateInfo &update_info, const ResourceType &res, ml::vector3d &value, ml::vector3d &speed, ml::vector3d &acceleration);

	template<typename ResourceType>
	static BM3_FORCEINLINE void Prepare(const ResourceType &res, const ml::vector3d &value, ml::matrix44 &m);

	template<typename ResourceType>
	static BM3_FORCEINLINE void SwitchResource(const ResourceType &res, const ResourceType &old_res, ml::random &random, ml::vector3d &value);
};

template<>
class MatrixScale_Newtondynamics<true>{

public:
	template<typename ResourceType>
	static BM3_FORCEINLINE void Initialize(const ResourceType &res, ml::random &random, ml::vector3d &value, ml::vector3d &speed, ml::vector3d &acceleration){

//		if(res.Scale_Basic_ScaleType() == ScaleTypeConst_NewtonDynamics)
		{
			value.random_range(			res.Scale_Basic_Square(), random, res.Scale_Value_Initial(),		res.Scale_Value_Range());
			speed.random_range(			res.Scale_Basic_Square(), random, res.Scale_Speed_Initial(),		res.Scale_Speed_Range());
			acceleration.random_range(	res.Scale_Basic_Square(), random, res.Scale_Acceleration_Initial(),	res.Scale_Acceleration_Range());
			value.maximum0();
		}
	}

	template<typename ResourceType>
	static BM3_FORCEINLINE void Initialize(const ResourceType &res, ml::random &random, float &value, float &speed, float &acceleration){

//		if(res.Scale_Basic_ScaleType() == ScaleTypeConst_NewtonDynamics)
		{
			value = random.f32_range(			res.Scale_Value_InitialX(),			res.Scale_Value_RangeX());
			speed = random.f32_range(			res.Scale_Speed_InitialX(),			res.Scale_Speed_RangeX());
			acceleration = random.f32_range(	res.Scale_Acceleration_InitialX(),	res.Scale_Acceleration_RangeX());
			value = ml::maximum(value, 0.0f);
		}
	}

	template<typename ResourceType, typename UpdateInfo>
	static BM3_FORCEINLINE void Update(UpdateInfo &/*update_info*/, const ResourceType &res, ml::vector3d &value, ml::vector3d &speed, ml::vector3d &acceleration){

//		if(res.Scale_Basic_ScaleType() == ScaleTypeConst_NewtonDynamics)
		{
			speed.add(acceleration);
			value.add(speed);
			value.maximum0();
		}
	}

	template<typename ResourceType>
	static BM3_FORCEINLINE void Prepare(const ResourceType &res, const ml::vector3d &value, ml::matrix44 &m){

		if(res.Scale_Basic_ScaleType() == ScaleTypeConst_NewtonDynamics)
		{
			m.mul_scale(value.e_.x_, value.e_.y_, value.e_.z_);
		}
	}
};

template<>
class MatrixScale_Newtondynamics<false>{

public:
	template<typename ResourceType>
	static BM3_FORCEINLINE void Initialize(const ResourceType &res, ml::random &random, ml::vector3d &value, ml::vector3d &speed, ml::vector3d &acceleration){

//		if(res.Scale_Basic_ScaleType() == ScaleTypeConst_NewtonDynamics)
		{
			value.random_range(			random, res.Scale_Value_Initial(),			res.Scale_Value_Range());
			speed.random_range(			random, res.Scale_Speed_Initial(),			res.Scale_Speed_Range());
			acceleration.random_range(	random, res.Scale_Acceleration_Initial(),	res.Scale_Acceleration_Range());
			value.maximum0();
		}
	}

	template<typename ResourceType, typename UpdateInfo>
	static BM3_FORCEINLINE void Update(UpdateInfo &/*update_info*/, const ResourceType &res, ml::vector3d &value, ml::vector3d &speed, ml::vector3d &acceleration){

		if(res.Scale_Basic_ScaleType() == ScaleTypeConst_NewtonDynamics)
		{
			speed.add(acceleration);
			value.add(speed);
			value.maximum0();
		}
	}

	template<typename ResourceType>
	static BM3_FORCEINLINE void Prepare(const ResourceType &res, const ml::vector3d &value, ml::matrix44 &m){

		if(res.Scale_Basic_ScaleType() == ScaleTypeConst_NewtonDynamics)
		{
			m.mul_scale(value.e_.x_, value.e_.y_, value.e_.z_);
		}
	}
};

}	// namespace impl

template<bool AllowFor_Scale_Basic_Square = true>
class MatrixScale_Newtondynamics{

public:
	template<typename ResourceType>
	void BM3_FORCEINLINE Initialize(const ResourceType &res, ml::random &random, ml::vector3d &value){

		impl::MatrixScale_Newtondynamics<AllowFor_Scale_Basic_Square>::Initialize(res, random, value, speed_, acceleration_);
	}

	template<typename ResourceType, typename UpdateInfo>
	void BM3_FORCEINLINE Update(UpdateInfo &update_info, const ResourceType &res, ml::vector3d &value){

		impl::MatrixScale_Newtondynamics<AllowFor_Scale_Basic_Square>::Update(update_info, res, value, speed_, acceleration_);
	}

	template<typename ResourceType>
	void BM3_FORCEINLINE Prepare(const ResourceType &res, const ml::vector3d &value, ml::matrix44 &m){

		impl::MatrixScale_Newtondynamics<AllowFor_Scale_Basic_Square>::Prepare(res, value, m);
	}

private:
	ml::ni::vector3d	speed_;
	ml::ni::vector3d	acceleration_;
};

}	// namespace policy
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_MATRIXSCALE_NEWTONDYNAMICS_H
