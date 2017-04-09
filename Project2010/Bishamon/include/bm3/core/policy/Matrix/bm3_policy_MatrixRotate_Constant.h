#ifndef BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_MATRIXROTATE_CONSTANT_H
#define BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_MATRIXROTATE_CONSTANT_H

#include "../../bm3_Const.h"
#include <ml/type/ml_vector3d.h>

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

namespace policy{

namespace impl{

/// @brief MatrixRotate_ConstantƒNƒ‰ƒX
class MatrixRotate_Constant{

public:
	template<typename ResourceType>
	static BM3_FORCEINLINE void Initialize(const ResourceType &res, ml::random &random, ml::vector3d &value){

//		if(res.Rotate_Basic_RotateType() == RotateTypeConst_Constant)
		{
			value.random_range(random, res.Rotate_Value_Initial(), res.Rotate_Value_Range());
			value = value.to_radian();
		}
	}

	template<typename ResourceType>
	static BM3_FORCEINLINE void Initialize(const ResourceType &res, ml::random &random, float &value){

//		if(res.Rotate_Basic_RotateType() == RotateTypeConst_Constant)
		{
			value = random.f32_range(res.Rotate_Value_InitialX(), res.Rotate_Value_RangeX());
			value = ml::to_radian(value);
		}
	}

	template<typename ResourceType, typename UpdateInfo>
	static BM3_FORCEINLINE void Update(UpdateInfo &/*update_info*/, const ResourceType &/*res*/, ml::vector3d &/*value*/){

		// ‰½‚à‚µ‚È‚¢
		;
	}

	template<typename ResourceType>
	static BM3_FORCEINLINE void Prepare(const ResourceType & /*res*/, const ml::vector3d &value, ml::matrix44 &m){

//		if(res.Rotate_Basic_RotateType() == RotateTypeConst_Constant)
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

class MatrixRotate_Constant{

public:
	template<typename ResourceType>
	void BM3_FORCEINLINE Initialize(const ResourceType &res, ml::random &random, ml::vector3d &value){

		impl::MatrixRotate_Constant::Initialize(res, random, value);
	}

	template<typename ResourceType, typename UpdateInfo>
	void BM3_FORCEINLINE Update(UpdateInfo &update_info, const ResourceType &res, ml::vector3d &value){

		impl::MatrixRotate_Constant::Update(update_info, res, value);
	}

	template<typename ResourceType>
	void BM3_FORCEINLINE Prepare(const ResourceType &res, const ml::vector3d &value, ml::matrix44 &m){

		impl::MatrixRotate_Constant::Prepare(res, value, m);
	}
};

}	// namespace policy
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_MATRIXROTATE_CONSTANT_H
