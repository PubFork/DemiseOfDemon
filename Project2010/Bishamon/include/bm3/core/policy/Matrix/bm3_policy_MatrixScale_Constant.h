#ifndef BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_MATRIXSCALE_CONSTANT_H
#define BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_MATRIXSCALE_CONSTANT_H

#include "../../bm3_Const.h"
#include <ml/type/ml_vector3d.h>

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

namespace policy{

namespace impl{

/// @brief MatrixScale_ConstantƒNƒ‰ƒX
template<bool AllowFor_Scale_Basic_Square = true>
class MatrixScale_Constant{

public:
	template<typename ResourceType>
	static BM3_FORCEINLINE void Initialize(const ResourceType &res, ml::random &random, ml::vector3d &value);

	template<typename ResourceType>
	static BM3_FORCEINLINE void Initialize(const ResourceType &res, ml::random &random, float &value);

	template<typename ResourceType, typename UpdateInfo>
	static BM3_FORCEINLINE void Update(UpdateInfo &update_info, const ResourceType &res, ml::vector3d &value);

	template<typename ResourceType>
	static BM3_FORCEINLINE void Prepare(const ResourceType &res, const ml::vector3d &value, ml::matrix44 &m);
};

template<>
class MatrixScale_Constant<true>{

public:
	template<typename ResourceType>
	static BM3_FORCEINLINE void Initialize(const ResourceType &res, ml::random &random, ml::vector3d &value){

//		if(res.Scale_Basic_ScaleType() == ScaleTypeConst_Constant)
		{
			value.random_range(res.Scale_Basic_Square(), random, res.Scale_Value_Initial(), res.Scale_Value_Range());
			value.maximum0();
		}
	}

	template<typename ResourceType>
	static BM3_FORCEINLINE void Initialize(const ResourceType &res, ml::random &random, float &value){

//		if(res.Scale_Basic_ScaleType() == ScaleTypeConst_Constant)
		{
			value = random.f32_range(res.Scale_Value_InitialX(), res.Scale_Value_RangeX());
			value = ml::maximum(value, 0.0f);
		}
	}

	template<typename ResourceType, typename UpdateInfo>
	static BM3_FORCEINLINE void Update(UpdateInfo &/*update_info*/, const ResourceType &/*res*/, ml::vector3d &/*value*/){

		// ‰½‚à‚µ‚È‚¢
		;
	}

	template<typename ResourceType>
	static BM3_FORCEINLINE void Prepare(const ResourceType & /*res*/, const ml::vector3d &value, ml::matrix44 &m){

//		if(res.Scale_Basic_ScaleType() == ScaleTypeConst_Constant)
		{
			m.mul_scale(value.e_.x_, value.e_.y_, value.e_.z_);
		}
	}
};

template<>
class MatrixScale_Constant<false>{

public:
	template<typename ResourceType>
	static BM3_FORCEINLINE void Initialize(const ResourceType &res, ml::random &random, ml::vector3d &value){

//		if(res.Scale_Basic_ScaleType() == ScaleTypeConst_Constant)
		{
			value.random_range(random, res.Scale_Value_Initial(), res.Scale_Value_Range());
			value.maximum0();
		}
	}

	template<typename ResourceType, typename UpdateInfo>
	static BM3_FORCEINLINE void Update(UpdateInfo &/*update_info*/, const ResourceType &/*res*/, ml::vector3d &/*value*/){

		// ‰½‚à‚µ‚È‚¢
		;
	}

	template<typename ResourceType>
	static BM3_FORCEINLINE void Prepare(const ResourceType &res, const ml::vector3d &value, ml::matrix44 &m){

		m.mul_scale(value.e_.x_, value.e_.y_, value.e_.z_);
	}
};

}	// namespace impl

template<bool AllowFor_Scale_Basic_Square = true>
class MatrixScale_Constant{

public:
	template<typename ResourceType>
	void BM3_FORCEINLINE Initialize(const ResourceType &res, ml::random &random, ml::vector3d &value){

		impl::MatrixScale_Constant<AllowFor_Scale_Basic_Square>::Initialize(res, random, value);
	}

	template<typename ResourceType, typename UpdateInfo>
	void BM3_FORCEINLINE Update(UpdateInfo &update_info, const ResourceType &res, ml::vector3d &value){

		impl::MatrixScale_Constant<AllowFor_Scale_Basic_Square>::Update(update_info, res, value);
	}

	template<typename ResourceType, typename UpdateInfo>
	void BM3_FORCEINLINE Prepare(const ResourceType &res, const ml::vector3d &value, ml::matrix44 &m){

		impl::MatrixScale_Constant<AllowFor_Scale_Basic_Square>::Prepare(res, value, m);
	}
};

}	// namespace policy
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_MATRIXSCALE_CONSTANT_H
