#ifndef BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_COLLISION_PLANE_H
#define BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_COLLISION_PLANE_H

#include "bm3/core/field/bm3_field.h"
#include "bm3/core/bm3_Const.h"
#include "bm3/core/bm3_DrawInfo.h"
#include <ml/type/ml_vector3d.h>
#include <float.h>
#include "bm3/core/policy/Field/bm3_policy_Collision_Position_Constant.h"
#include "bm3/core/policy/Field/bm3_policy_Collision_Position_Curve.h"
#include "bm3/core/policy/Field/bm3_policy_Collision_Size_Constant.h"
#include "bm3/core/policy/Field/bm3_policy_Collision_Size_Curve.h"
#include "bm3/core/policy/Field/bm3_policy_Collision_Slope_Constant.h"
#include "bm3/core/policy/Field/bm3_policy_Collision_Slope_Curve.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

namespace policy{

namespace impl{

/// @brief Collision_Planeクラス
class Collision_Plane{

public:
	template<typename ResourceType>
	static BM3_FORCEINLINE void	Initialize(const ResourceType &res, ml::random &random, ml::vector3d &plane, ml::vector3d &size, ml::vector3d &position, ml::vector3d &slope){
		if(res.Field_Basic_FieldCollisionShapeType()==FieldCollisionShapeTypeConst_Plane){

			switch(res.Field_Basic_FieldPlaneType()){
				case FieldPlaneTypeConst_PlaneXY:	plane = ml::vector3d(0.0f, 0.0f, 1.0f ,1.0f);			break;
				case FieldPlaneTypeConst_PlaneYZ:	plane = ml::vector3d(1.0f, 0.0f, 0.0f ,1.0f);			break;
				case FieldPlaneTypeConst_PlaneZX:	plane = ml::vector3d(0.0f, 1.0f, 0.0f ,1.0f);			break;
				default:							ML_ASSERT(false);BM3_ASSUME(0);							break;
			}

			policy::impl::Collision_Position_Constant::Initialize(	res, random, position);
			policy::impl::Collision_Position_Curve::Initialize(		res, random, position);
			policy::impl::Collision_Slope_Constant::Initialize(		res, random, slope);
			policy::impl::Collision_Slope_Curve::Initialize(		res, random, slope);
			if(res.Field_Basic_FieldPlaneShapeType()==FieldPlaneShapeTypeConst_Infinit){
			}
			else{
				policy::impl::Collision_Size_Constant::Initialize(		res, random, size);
				policy::impl::Collision_Size_Curve::Initialize(			res, random, size);
			}

			ml::matrix44	m;
			m.set_unit();
			m.mul_rot_xyz(slope);
			plane.mul33(m);
		}
	}

	template<typename ResourceType, typename UpdateInfo>
	static BM3_FORCEINLINE void	Update(UpdateInfo &update_info, const ResourceType &res, ml::vector3d &plane, ml::vector3d &size, ml::vector3d &position, ml::vector3d &slope, const PrimitiveUpdateInfo &pui){
		if(res.Field_Basic_FieldCollisionShapeType()==FieldCollisionShapeTypeConst_Plane){
			policy::impl::Collision_Position_Curve::Update(	update_info, res, position, pui);
			policy::impl::Collision_Slope_Curve::Update(	update_info, res, slope,	pui);
			if(res.Field_Basic_FieldPlaneShapeType()==FieldPlaneShapeTypeConst_Infinit){
			}
			else{
				policy::impl::Collision_Size_Curve::Update(	update_info, res, size,		pui);
			}
		}
	}

	template<typename ResourceType ,typename FieldType, typename PrimitiveType>
	static void	Prepare(ml::vector3d &dst, FieldType &field, const ResourceType &res, const ml::matrix44 &m, PrimitiveType &prim){
		if(res.Field_Basic_FieldCollisionShapeType()==FieldCollisionShapeTypeConst_Plane){

			ml::ni::vector3d	position, field_pos;
			position.mul(prim.translate_.value_ ,m);
			position.add(dst);
			position.e_.w_	= 1.0f;
			position.add(field.pos_diff_);
			field_pos.mul(field.position_, m);
			field_pos.e_.w_	= 1.0f;

			if(field.detected_flag_==0.0f){
				field.detected_flag_	= field.plane_.dot(position);
			}

			if(prim.GetPreviousPosition().is_unit())	return;

			ml::ni::vector3d	intersect,pos_diff;
			if(	IntersectSpherePlane(intersect, pos_diff, field_pos, field.plane_, position, prim.GetPreviousPosition(), field.bounding_.size_) ||
				IntersectRayPlane(intersect, pos_diff, field_pos, field.plane_, prim.GetPreviousPosition(), position, field.detected_flag_, field.bounding_.size_)){

				// 法線方向の計算
				ml::vector3d	plane	= field.plane_;
				if(field.detected_flag_<0)	plane.mul(-1.0f);

				{	// 位置の反射
					float reflection = pos_diff.dot(plane) * (-2.0f);
					field.pos_diff_.add(ml::vector3d().mul(plane, reflection));
				}
				{	//　速度の反射分
					ml::vector3d	diff = res.Field_Judge_Reflection();
					float			rate = -1.0f * prim.translate_.speed_.dot(plane);

					diff.add(1.0f);
					diff.mul(plane);
					diff.mul(rate);

					prim.translate_.speed_.add(diff);
				}
			}
			dst.add(field.pos_diff_);
		}
	}

private:
	static bool IntersectSpherePlane(ml::vector3d &dst, ml::vector3d &dst_diff, const ml::vector3d plane_pos, const ml::vector3d &plane_normal, const ml::vector3d &obj_pos, const ml::vector3d &obj_pre_pos, float obj_size){
		float dist = obj_pos.dot(plane_normal) - plane_pos.dot(plane_normal);
		if(ml::abs(dist)<=obj_size){
			ml::vector3d	n = plane_normal;
			dst.sub(obj_pos, n.mul(dist));

			dst_diff = obj_pos - obj_pre_pos;
			dst_diff.normalize();
			dst_diff.mul(obj_size - ml::abs(dist));
			return true;
		}
		return false;
	}

	static bool IntersectRayPlane(ml::vector3d &dst, ml::vector3d &dst_diff, const ml::vector3d plane_pos, const ml::vector3d &plane_normal, const ml::vector3d &before, const ml::vector3d &after, float & /*detected_flag*/, float /*field_bouding_size*/){

		ml::vector3d	ab = after - before;
		float	t = (plane_pos.dot(plane_normal) - plane_normal.dot(before)) / plane_normal.dot(ab);
		if(0.0f<=t && t <=1.0f){
			dst.add(before, ab.mul(t));
			dst_diff.sub(after, dst);
			return true;
		}
		return false;
	}
};

}	// namespace impl

}	// namespace policy
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_COLLISION_PLANE_H
