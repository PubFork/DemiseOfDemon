#ifndef BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_COLLISION_SPHERE_H
#define BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_COLLISION_SPHERE_H

#include "bm3/core/bm3_Const.h"
#include "bm3/core/bm3_DrawInfo.h"
#include <ml/type/ml_vector3d.h>
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

/// @brief Collision_Sphereクラス
class Collision_Sphere{

public:
	template<typename ResourceType>
	static BM3_FORCEINLINE void	Initialize(const ResourceType &res, ml::random &random, ml::vector3d & /*plane*/, ml::vector3d &size, ml::vector3d &position, ml::vector3d &/*slope*/){
		if(res.Field_Basic_FieldCollisionShapeType()==FieldCollisionShapeTypeConst_Sphere){

			policy::impl::Collision_Position_Constant::Initialize(	res, random, position);
			policy::impl::Collision_Position_Curve::Initialize(		res, random, position);

			policy::impl::Collision_Size_Constant::Initialize(		res, random, size);
			policy::impl::Collision_Size_Curve::Initialize(			res, random, size);
		}
	}

	template<typename ResourceType, typename UpdateInfo>
	static BM3_FORCEINLINE void	Update(UpdateInfo &update_info, const ResourceType &res, ml::vector3d &value){
		if(res.Field_Basic_FieldCollisionShapeType()==FieldCollisionShapeTypeConst_Sphere){
		}
	}

	template<typename ResourceType ,typename FieldType, typename PrimitiveType>
	static void	Prepare(ml::vector3d &dst, FieldType &field, const ResourceType &res, const ml::matrix44 &m, PrimitiveType &prim){
		if(res.Field_Basic_FieldCollisionShapeType()==FieldCollisionShapeTypeConst_Sphere){

			ml::ni::vector3d	position, field_pos;
			position.mul(prim.translate_.value_ ,m);
			position.add(dst);
			position.e_.w_	= 1.0f;
			position.add(field.pos_diff_);
			field_pos.mul(field.position_, m);
			field_pos.e_.w_	= 1.0f;

			if(field.detected_flag_==0.0f){
				field.detected_flag_	= (field_pos - position).length_sq() - field.size_.e_.x_ * field.size_.e_.x_;
			}

			if(prim.GetPreviousPosition().is_unit())	return;

			ml::ni::vector3d	intersect;
			ml::vector3d		pos_diff;
			if(	IntersectSphereSphere(intersect, pos_diff, position, field.bounding_.size_, field_pos, field.size_.e_.x_) ||
				IntersectRaySphere(intersect, prim.GetPreviousPosition(), position, field_pos, field.size_.e_.x_)){

				// 法線方向の計算
				ml::vector3d	plane = intersect - field.position_;
				plane.normalize();
				if(field.detected_flag_<0)		plane.mul(-1.0f);

				{	// 位置の反射
					float reflection = pos_diff.dot(plane) * (-2.0f);
					field.pos_diff_.add(ml::vector3d().mul(plane, reflection));
				}
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
	static bool IntersectSphereSphere(ml::vector3d &dst, ml::vector3d &dst_diff, const ml::vector3d &obj_center, const float &obj_size, const ml::vector3d &field_center, const float &field_size){
		ml::vector3d	d		= obj_center - field_center;
		float			dist	= d.dot(d);

		float	diff = 0.0f;
		if(dist<field_size*field_size){
			// 球の中
			diff = obj_size + ml::sqrtf(dist) - field_size;
			if(diff<=0) return false;
		}
		else{
			// 球の外
			float rad_sum = obj_size + field_size;
			if(rad_sum*rad_sum < dist) return false;
			diff	= (ml::sqrtf(dist) - obj_size) - field_size;
		}

		d.normalize();
		dst.mul(d, field_size);
		dst_diff.mul(d, diff);
		return true;
	}


	static bool IntersectRaySphere(ml::vector3d &dst, const ml::vector3d &ray_start, const ml::vector3d &ray_end, const ml::vector3d &field_center, const float field_size){
		ml::vector3d	vec_m	= ray_start - field_center;
		ml::vector3d	vec_d	= ray_end - ray_start;
		ml::vector3d	vec_nd	= vec_d;
		vec_nd.normalize();

		float	b = vec_m.dot(vec_nd);
		float	ac = (vec_m.dot(vec_m) - field_size * field_size);
		if(0.0f<ac && 0.0f<b) return false;	//rayの始点が球の外にあり、　rayが球から離れていく方向にあるとき

		float	d = b*b - ac;
		if(d<0)	return false;	// 球を貫かないとき

		float	d_sq	= ml::sqrtf(d);
		float	t		= - b - d_sq;
		float	t_plus	= - b + d_sq;
		if(t_plus<=0){	return false;											}	// 光線が球から遠ざかるとき
		else if(t < 0){	if(ml::abs(t) < ml::abs(t_plus)){	t	= t_plus;	}	}	// 光線は球の中から
		else{			;														}	// 光線は球の外から

		if(vec_d.length_sq()<t*t) return false;	// 線分より先に球があるとき
		dst.add(ray_start, vec_nd.mul(t));

		return true;
	}

};

}	// namespace impl

}	// namespace policy
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_COLLISION_SPHERE_H
