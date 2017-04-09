#ifndef BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_BOUNDING_SHPERE_H
#define BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_BOUNDING_SHPERE_H

#include "bm3/core/field/bm3_field.h"
#include "bm3/core/bm3_Const.h"
#include "bm3/core/bm3_DrawInfo.h"
#include <ml/type/ml_vector3d.h>
#include "bm3/core/primitive/bm3_Primitive_Common.h"
#include "bm3/core/primitive/bm3_Primitive.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

namespace policy{

namespace impl{

/// @brief Bounding_SphereƒNƒ‰ƒX
class Bounding_Sphere{
public:
	template<
		typename FieldType,
		typename ResourceType
	>
	static BM3_FORCEINLINE void	Initialize(const ResourceType & /*res*/, ml::random & /*random*/, FieldType &field, const prim::MatrixParam &translate ,const prim::MatrixParam &scale){
		field.bounding_.center_	= translate.value_;
		field.bounding_.size_	= ml::maximum(ml::maximum(ml::abs(scale.value_.e_.x_), ml::abs(scale.value_.e_.y_)) ,ml::abs(scale.value_.e_.z_)); 
	}

	template<
		typename FieldType,
		typename ResourceType
	>
	static BM3_FORCEINLINE void	Initialize(const ResourceType & /*res*/, ml::random & /*random*/, FieldType &field, const prim::MatrixParam &translate ,const prim::ScalarParam &scale){
		field.bounding_.center_	= translate.value_;
		field.bounding_.size_	= scale.value_; 
	}
#if 0
	template<
		typename FieldType,
		typename ResourceType,
		typename UpdateInfo
	>
	static BM3_FORCEINLINE void	Update(UpdateInfo & /*update_info*/, const ResourceType & /*res*/, FieldType &field, const prim::MatrixParam &scale){
	}

	template<
		typename FieldType,
		typename ResourceType,
		typename UpdateInfo
	>
	static BM3_FORCEINLINE void	Update(UpdateInfo & /*update_info*/, const ResourceType & /*res*/, FieldType &field, const prim::ScalarParam &scale){
	}
#endif

	template<
		typename FieldType,
		typename ResourceType
	>
	static BM3_FORCEINLINE void	Prepare(const ResourceType & /*res*/, FieldType &field, const prim::MatrixParam &scale ,const ml::vector3d &parent_scale){
		ml::vector3d	temp;
		temp.mul(scale.value_, parent_scale);
		field.bounding_.size_	= ml::maximum(ml::maximum(ml::abs(temp.e_.x_), ml::abs(temp.e_.y_)) ,ml::abs(temp.e_.z_)); 
	}

	template<
		typename FieldType,
		typename ResourceType
	>
	static BM3_FORCEINLINE void	Prepare(const ResourceType & /*res*/, FieldType &field, const prim::ScalarParam &scale ,const ml::vector3d &parent_scale){
		ml::vector3d	temp;
		temp.mul(parent_scale, scale.value_);
		field.bounding_.size_	= ml::maximum(ml::maximum(ml::abs(temp.e_.x_), ml::abs(temp.e_.y_)) ,ml::abs(temp.e_.z_)); 
	}

};

}	// namespace impl

}	// namespace policy
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_BOUNDING_SHPERE_H
