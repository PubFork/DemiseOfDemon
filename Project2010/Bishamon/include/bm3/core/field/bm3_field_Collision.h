#ifndef BM3_SDK_INC_BM3_CORE_PRIMITIVE_BM3_FIELD_COLLISION_H
#define BM3_SDK_INC_BM3_CORE_PRIMITIVE_BM3_FIELD_COLLISION_H

#include "bm3_field_Common.h"
#include "bm3/core/policy/Field/bm3_policy_Collision_Plane.h"
#include "bm3/core/policy/Field/bm3_policy_Collision_Sphere.h"
#include "bm3/core/policy/Field/bm3_policy_Collision_Cube.h"
#include "bm3/core/policy/Field/bm3_policy_Bounding_Sphere.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

class DrawInfo;

namespace field{
namespace collision{

/// @brief BoundingSphere構造体
struct BoundingSphere{
	float			size_;
	ml::vector3d	center_;
};

/// @brief Field構造体
struct Field{
	ml::vector3d	value_;
	ml::vector3d	plane_;
	ml::vector3d	size_;
	ml::vector3d	position_;
	ml::vector3d	slope_;
	BoundingSphere	bounding_;
	float			detected_flag_;

	ml::vector3d	pos_diff_;
};

/// @brief FieldManipulatorクラス
class FieldManipulator{

public:
	template<
		typename FieldType,
		typename PrimitiveType,
		typename ResourceType
	>
	static void Initialize(FieldType &field, const ResourceType &res, ml::random &random, PrimitiveType & /*prim*/){

		policy::impl::Collision_Plane::Initialize(		res, random ,field.plane_, field.size_, field.position_, field.slope_);
		policy::impl::Collision_Sphere::Initialize(		res, random ,field.plane_, field.size_, field.position_, field.slope_);
//		policy::impl::Collision_Cylinder::Initialize(	res, random ,field.value_);
		policy::impl::Collision_Cube::Initialize(		res, random ,field.plane_, field.size_, field.position_, field.slope_);

		field.detected_flag_	= 0.0f;
		field.pos_diff_.set_unit();
	}

	template<
		typename UpdateInfo,
		typename FieldType,
		typename PrimitiveType,
		typename ResourceType
	>
	static void Update(UpdateInfo & /*update_info*/, FieldType & /*field*/, const ResourceType & /*res*/, const PrimitiveType & /*prim*/, const PrimitiveUpdateInfo &/*primitive_update_info*/){

//		policy::impl::Collision_Plane::Update(		update_info, res, field.value_);

//		policy::impl::Collision_Sphere::Update(		update_info, res, field.value_);
//		policy::impl::Collision_Cylinder::Update(	update_info, res, field.value_);
//		policy::impl::Collision_Cube::Update(		update_info, res, field.value_);


	}

	template<
		typename FieldType,
		typename PrimitiveType,
		typename ResourceType
	>
	static void Prepare(FieldInfo &dst, FieldType &field, const ResourceType &res, const ml::matrix44 &matrix, UpdateInfo & /*update_info*/, const obj::RootNodeBase &/*root*/, PrimitiveType &prim){

		ml::ni::vector3d	temp;
		temp.mul(prim.translate_.value_, matrix);
		field.bounding_.center_.add(temp, dst.position_);

		policy::impl::Collision_Plane::Prepare(		dst.position_, field, res, matrix, prim);
		policy::impl::Collision_Sphere::Prepare(	dst.position_, field, res, matrix, prim);
//		policy::impl::Collision_Cylinder::Prepare(	res, random ,field.value_);
		policy::impl::Collision_Cube::Prepare(	dst.position_, field, res, matrix, prim);
	}
	
	template<
		typename FieldType,
		typename ResourceType
	>
	static void SwitchResource(FieldType & /*field*/, const ResourceType & /*res*/, const ResourceType & /*old_res*/, ml::random & /*random*/){

//		policy::impl::Collision_Plane::Initialize(		res, random ,field.plane_, field.size_, field.position_, field.slope_);
//		policy::impl::Collision_Sphere::Initialize(		res, random ,field.value_);
//		policy::impl::Collision_Cylinder::Initialize(	res, random ,field.value_);
//		policy::impl::Collision_Cube::Initialize(		res, random ,field.value_);
	}

	template<
		typename ResourceType,
		typename LifePolicy
	>
	static void Draw(DrawInfo &draw_info, const ResourceType &res, const ml::matrix44 &matrix, LifePolicy &life){
#if defined(BM3_TARGET_IDE)
		if(res.Basic_Meta_Enable() && res.Basic_Meta_Display() && res.Basic_Basic_AreaDisplay()){
		}
#endif
	}

	
};

}	// namespace collision
}	// namespace field
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_PRIMITIVE_BM3_FIELD_COLLISION_H
