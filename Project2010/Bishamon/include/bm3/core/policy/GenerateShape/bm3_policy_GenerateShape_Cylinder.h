#ifndef BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_GENERATESHAPE_CYLINDER_H
#define BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_GENERATESHAPE_CYLINDER_H

#include "../../bm3_Const.h"
#include <ml/type/ml_vector3d.h>
#include <ml/math/ml_triangular.h>
#include "../bm3_policy_PositionMaker.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

namespace policy{

namespace impl{

/// @brief GenerateShape_CylinderƒNƒ‰ƒX
class GenerateShape_Cylinder{

public:
	template<typename ResourceType>
	static BM3_FORCEINLINE void Initialize(const ResourceType &res, ml::random &random, ml::vector3d &value, ml::vector3d &direction){

//		if(res.GenerateShape_Basic_GenerateShapeType() == GenerateShapeTypeConst_Cylinder)
		{
			PositionMaker::Cylinder(
				value, random,
				res.GenerateShape_Area_Inside(), res.GenerateShape_Area_Outside(),
				res.GenerateShape_HorizontalArea_StartAngle(), res.GenerateShape_HorizontalArea_EndAngle()
			);

			{
				ml::matrix44		slope;
				slope.mul_rot_xyz(res.GenerateShape_Area_Slope());

				direction.set(value.e_.x_ ,0 ,value.e_.z_ ,1.0f);
				direction.mul(slope);
				direction.normalize();

				value.mul(slope);
			}
		}
	}
};

}	// namespace impl

class GenerateShape_Cylinder{

public:
	template<typename ResourceType>
	void BM3_FORCEINLINE Initialize(const ResourceType &res, ml::random &random ,ml::vector3d &position ,ml::vector3d &direction){

		impl::GenerateShape_Cylinder::Initialize(res, random, position, direction);
	}

private:

};

}	// namespace policy
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_GENERATESHAPE_CYLINDER_H
