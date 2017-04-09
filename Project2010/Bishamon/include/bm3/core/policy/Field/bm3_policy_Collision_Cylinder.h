#ifndef BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_COLLISION_CYLINDER_H
#define BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_COLLISION_CYLINDER_H

#include "../../bm3_Const.h"
#include "../../bm3_DrawInfo.h"
#include <ml/type/ml_vector3d.h>

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

namespace policy{

namespace impl{

/// @brief Collision_CylinderƒNƒ‰ƒX
class Collision_Cylinder{

public:
	template<typename ResourceType>
	static BM3_FORCEINLINE void	Initialize(const ResourceType &res, ml::random &/*random*/, ml::vector3d &/*value*/){
//		if(res.Field_Basic_FieldCollisionShapeType()==FieldCollisionShapeTypeConst_Cylinder){
//		}
	}

	template<typename ResourceType, typename UpdateInfo>
	static BM3_FORCEINLINE void	Update(UpdateInfo &update_info, const ResourceType &res, ml::vector3d &size, ml::vector3d &position, ml::vector3d &slope, ml::vector3d &plane, const PrimitiveUpdateInfo &primitive_update_info){
//		if(res.Field_Basic_FieldCollisionShapeType()==FieldCollisionShapeTypeConst_Cylinder){
//		}
	}

	template<typename ResourceType ,typename FieldType, typename PrimitiveType>
	static void	Prepare(ml::vector3d &dst, DrawInfo &draw_info, const FieldType &field, const ResourceType &res, const ml::matrix44 &m, const PrimitiveType &prim){
//		if(res.Field_Basic_FieldCollisionShapeType()==FieldCollisionShapeTypeConst_Cylinder){
//		}
/*		{
			ml::ni::vector3d	position;
			position.mul(prim.translate_.value_ ,m);
			position.add(prim.field_info_.position_);

			ml::ni::vector3d	intersect;
			if(intersectLineAndPlane(intersect, res.Field_Option_NormalDirection(), prim.last_position_, position)){
			}
		}
*/	}

private:
	bool intersectLineAndPlane(ml::vector3d &dst, ml::vector3d &normal, ml::vector3d &pre, ml::vector3d last){
		float p = normal.dot(pre);
		float l = normal.dot(last);
		if(p*l<0){
			float i = ml::abs(p)/(ml::abs(p) + ml::abs(l));
			dst.add(pre.mul(pre ,1-i) ,last.mul(last ,i));
			return true;
		}
		return false;
	}
};

}	// namespace impl

}	// namespace policy
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_COLLISION_CYLINDER_H
