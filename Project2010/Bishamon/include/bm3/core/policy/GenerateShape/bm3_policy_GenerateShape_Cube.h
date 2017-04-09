#ifndef BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_GENERATESHAPE_CUBE_H
#define BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_GENERATESHAPE_CUBE_H

#include "../../bm3_Const.h"
#include <ml/type/ml_vector3d.h>
#include "../bm3_policy_PositionMaker.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

namespace policy{

namespace impl{

/// @brief GenerateShape_CubeƒNƒ‰ƒX
class GenerateShape_Cube{

public:
	template<typename ResourceType>
	static BM3_FORCEINLINE void Initialize(const ResourceType &res, ml::random &random, ml::vector3d &value, ml::vector3d &direction){

//		if(res.GenerateShape_Basic_GenerateShapeType() == GenerateShapeTypeConst_Cube)
		{
			PositionMaker::Cube(
				value, random,
				res.GenerateShape_Area_Inside(), res.GenerateShape_Area_Outside()
			);

			{
				ml::matrix44		slope;
				slope.mul_rot_xyz(res.GenerateShape_Area_Slope());

				direction = value;
					 if(ml::abs(direction.e_.x_) < ml::abs(direction.e_.y_)){ (ml::abs(direction.e_.y_) < ml::abs(direction.e_.z_)) ?	direction.e_.x_ = direction.e_.y_ = 0 : direction.e_.x_ = direction.e_.z_ = 0;	}
				else if(ml::abs(direction.e_.x_) < ml::abs(direction.e_.z_)){ (ml::abs(direction.e_.y_) < ml::abs(direction.e_.z_)) ?	direction.e_.x_ = direction.e_.y_ = 0 : direction.e_.x_ = direction.e_.z_ = 0;	}
				else{																													direction.e_.y_	= direction.e_.z_ = 0;											}
				direction.mul(slope);
				direction.normalize();

				value.mul(slope);
			}
		}
	}
};

}	// namespace impl

class GenerateShape_Cube{

public:
	template<typename ResourceType>
	void BM3_FORCEINLINE Initialize(const ResourceType &res, ml::random &random ,ml::vector3d &position ,ml::vector3d &direction){

		impl::GenerateShape_Cube::Initialize(res, random, position, direction);
	}

private:

};

}	// namespace policy
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_GENERATESHAPE_CUBE_H
