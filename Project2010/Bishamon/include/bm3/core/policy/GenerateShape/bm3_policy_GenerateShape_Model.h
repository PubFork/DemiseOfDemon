#ifndef BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_GENERATESHAPE_MODEL_H
#define BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_GENERATESHAPE_MODEL_H

#include "../../bm3_Const.h"
#include <algorithm>
#include "../bm3_policy_PositionMaker.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

namespace policy{

namespace impl{

/// @brief GenerateShape_ModelƒNƒ‰ƒX
class GenerateShape_Model{

public:
	template<typename ResourceType>
	static BM3_FORCEINLINE void Initialize(const ResourceType &res, ml::random &random, ml::vector3d &value, ml::vector3d &direction){

//		if(res.GenerateShape_Basic_GenerateShapeType() == GenerateShapeTypeConst_Model)
		{
			PositionMaker::Model(
				value, direction, random, res.GenerateShape_Basic_ModelName(), res.GenerateShape_Area_Scale()
			);

		}
	}
};

}	// namespace impl

class GenerateShape_Model{

public:
	template<typename ResourceType>
	void BM3_FORCEINLINE Initialize(const ResourceType &res, ml::random &random ,ml::vector3d &position ,ml::vector3d &direction){

		impl::GenerateShape_Model::Initialize(res, random, position, direction);
	}

private:
};

}	// namespace policy
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_GENERATESHAPE_MODEL_H
