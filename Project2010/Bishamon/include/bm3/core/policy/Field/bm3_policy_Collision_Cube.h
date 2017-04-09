#ifndef BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_COLLISION_CUBE_H
#define BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_COLLISION_CUBE_H

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

/// @brief Collision_Cubeクラス
class Collision_Cube{

public:
	template<typename ResourceType>
	static BM3_FORCEINLINE void	Initialize(const ResourceType &res, ml::random &random, ml::vector3d & /*plane*/, ml::vector3d &size, ml::vector3d &position, ml::vector3d & /*slope*/){
		if(res.Field_Basic_FieldCollisionShapeType()==FieldCollisionShapeTypeConst_Cube){
			policy::impl::Collision_Position_Constant::Initialize(	res, random, position);
			policy::impl::Collision_Position_Curve::Initialize(		res, random, position);

			policy::impl::Collision_Size_Constant::Initialize(		res, random, size);
			policy::impl::Collision_Size_Curve::Initialize(			res, random, size);
		}
	}

	template<typename ResourceType, typename UpdateInfo>
	static BM3_FORCEINLINE void	Update(UpdateInfo &update_info, const ResourceType &res, ml::vector3d &size, ml::vector3d &position, ml::vector3d &slope, ml::vector3d &plane, const PrimitiveUpdateInfo &primitive_update_info){
		if(res.Field_Basic_FieldCollisionShapeType()==FieldCollisionShapeTypeConst_Cube){
		}
	}

	template<typename ResourceType ,typename FieldType, typename PrimitiveType>
	static void	Prepare(ml::vector3d &dst, FieldType &field, const ResourceType &res, const ml::matrix44 &m, PrimitiveType &prim){
		if(res.Field_Basic_FieldCollisionShapeType()==FieldCollisionShapeTypeConst_Cube){

			ml::ni::vector3d	position, field_pos;
			position.mul(prim.translate_.value_ ,m);
			position.add(dst);
			position.e_.w_	= 1.0f;
			position.add(field.pos_diff_);
			field_pos.mul(field.position_, m);
			field_pos.e_.w_	= 1.0f;

			ml::vector3d	box_min, box_max;
			box_min.sub(field_pos, field.size_);
			box_max.add(field_pos, field.size_);
			if(field.detected_flag_==0.0f){
				field.detected_flag_	= IsInsideAABB(position, box_min, box_max) ? 1.0f :-1.0f;
			}

			ml::ni::vector3d	intersect, pos_diff;
//			if(	IntersectSphereAABB(intersect, pos_diff, position, field.bounding_.size_, box_min, box_max, (field.detected_flag_<0)) ||
//				IntersectRayCube(intersect, prim.GetPreviousPosition(), position, box_min, box_max, field.detected_flag_, field.bounding_.size_)){
			if(IntersectRayCube(intersect, prim.GetPreviousPosition(), position, box_min, box_max, field.detected_flag_, field.bounding_.size_)){

				// 法線方向の計算
				ml::vector3d	plane;
				plane.set_unit();
				{
							if(box_min.e_.x_==intersect.e_.x_)	plane.e_.x_	= -1.0f;
					else	if(box_min.e_.y_==intersect.e_.y_)	plane.e_.y_	= -1.0f;
					else	if(box_min.e_.z_==intersect.e_.z_)	plane.e_.z_	= -1.0f;
					else	if(box_max.e_.x_==intersect.e_.x_)	plane.e_.x_	= 1.0f;
					else	if(box_max.e_.y_==intersect.e_.y_)	plane.e_.y_	= 1.0f;
					else	if(box_max.e_.z_==intersect.e_.z_)	plane.e_.z_	= 1.0f;
				}
#if 1
				{
					field.pos_diff_.sub(position, intersect);
					ml::vector3d	reflect_normal = field.pos_diff_;

					reflect_normal.normalize();
					field.pos_diff_.mul(reflect_normal.dot(plane));
				}
#else
				{	// 位置の反射
					float reflection = pos_diff.dot(plane) * (-2.0f);
					field.pos_diff_.add(ml::vector3d().mul(plane, reflection));
				}
#endif
				{
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
	static bool IsInsideAABB(ml::vector3d &point, const ml::vector3d &box_min, const ml::vector3d &box_max){
		if(point.e_.x_<box_min.e_.x_)	return false;
		if(point.e_.y_<box_min.e_.y_)	return false;
		if(point.e_.z_<box_min.e_.z_)	return false;
		if(point.e_.x_>box_max.e_.x_)	return false;
		if(point.e_.y_>box_max.e_.y_)	return false;
		if(point.e_.z_>box_max.e_.z_)	return false;
		return true;
	}

	static float IntersectSqDistanceOfPointAABB(const ml::vector3d &point, const ml::vector3d box_min, const ml::vector3d &box_max){
		float sqdist = 0.0f;

	#define ADD_MORE_LONG(a,b)		if(a<b)	sqdist += (b - a) * (b - a);
		ADD_MORE_LONG(point.e_.x_, box_min.e_.x_)
		ADD_MORE_LONG(point.e_.y_, box_min.e_.y_)
		ADD_MORE_LONG(point.e_.z_, box_min.e_.z_)
		ADD_MORE_LONG(box_max.e_.x_, point.e_.x_)
		ADD_MORE_LONG(box_max.e_.y_, point.e_.y_)
		ADD_MORE_LONG(box_max.e_.z_, point.e_.z_)
	#undef ADD_MORE_LONG

		return sqdist;
	}

	static void ClosestPointAABB(ml::vector3d &dst, const ml::vector3d &point, const ml::vector3d box_min, const ml::vector3d &box_max){
		for(int i=0 ; i<3 ; ++i){
			float v = point.v_[i];
			if(v < box_min.v_[i]) v = box_min.v_[i];
			if(v > box_max.v_[i]) v = box_max.v_[i];
			dst.v_[i] = v;
		}
	}

	static bool IntersectSphereAABB(ml::vector3d &dst, ml::vector3d &dst_diff, const ml::vector3d &obj_center, const float obj_size, const ml::vector3d &box_min, const ml::vector3d &box_max, bool /*is_inside*/){
		float sqdist = IntersectSqDistanceOfPointAABB(obj_center, box_min, box_max);
		float diff = (obj_size * obj_size) - sqdist;
		if(diff<0) return false;

		ClosestPointAABB(dst, obj_center, box_min, box_max);

		ml::vector3d	nd = dst - obj_center;
		nd.normalize();
		dst_diff.mul(nd, diff);

		return true;
	}

#if 0
	static bool IntersectRayCube(ml::vector3d &dst, const ml::vector3d &ray_start, const ml::vector3d &ray_end, const ml::vector3d &box_min, const ml::vector3d &box_max, float & /*detected_flag*/, float field_bouding_size){

		if(ray_start.is_unit())	return false;

		float min	= -FLT_MAX;
		float max	= FLT_MAX;
		ml::vector3d	d, dn;
		dn = d.sub(ray_end, ray_start);
		dn.normalize();

		for(int i=0 ; i<3 ; ++i){

			if(ml::abs(d.v_[i]) < FLT_EPSILON){
				// 光線がスラブに対して平行な場合。この場合は交差なし
				if(ray_start.v_[i] < box_min.v_[i] || ray_start.v_[i] > box_max.v_[i])	return false;
			}
			else{

				float	d_inv = 1.0f/dn.v_[i];
				float	t1	= (box_min.v_[i] - ray_start.v_[i]) * d_inv;
				float	t2	= (box_max.v_[i] - ray_start.v_[i]) * d_inv;

				if(t1 > t2)		ml::swap(t1, t2);

				if(t1 > min)	min = t1;
				if(t2 < max)	max = t2;

				if(min > max)	return false;	// 交差がない
			}
		}

		dst.add(ray_start, dn);
		return true;
	}
#else
	static bool IntersectRayCube(ml::vector3d &dst, const ml::vector3d &ray_start, const ml::vector3d &ray_end, const ml::vector3d &box_min, const ml::vector3d &box_max, float & /*detected_flag*/, float field_bouding_size){

		if(ray_start.is_unit())	return false;

		float min	= -FLT_MAX;
		float max	= FLT_MAX;

		ml::vector3d	d, dn;
		dn = d.sub(ray_end, ray_start);
		dn.normalize();
		{
			ml::vector3d	dir = dn;
			dir.mul(field_bouding_size);
			d.add(dir);
		}

		bool	is_inside = true;
		for(int i=0 ; i<3 ; ++i){

			if(ml::abs(d.v_[i]) < FLT_EPSILON){
				// 光線がスラブに対して平行な場合。この場合は交差なし
				if(ray_start.v_[i] < box_min.v_[i] || ray_start.v_[i] > box_max.v_[i])	return false;
			}
			else{

				float	d_inv = 1.0f/dn.v_[i];
				float	t1	= (box_min.v_[i] - ray_start.v_[i]) * d_inv;
				float	t2	= (box_max.v_[i] - ray_start.v_[i]) * d_inv;

				if(t1 > t2)		ml::swap(t1, t2);

				if(t1 > min)	min = t1;
				if(t2 < max)	max = t2;

				if(min > max)	return false;	// 交差がない
				is_inside = (is_inside) && (t1<0) && (t2<0);
			}
		}

		dn.mul(min);
		if(d.length_sq() <= dn.length_sq())	return false;
		if(is_inside){
		}
		else{
		}

		dst.add(ray_start, dn);
		return true;
	}
#endif
};

}	// namespace impl

}	// namespace policy
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_COLLISION_CUBE_H
