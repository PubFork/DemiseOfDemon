#ifndef BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_GRAVITY_DIRECTION_CONSTANT_H
#define BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_GRAVITY_DIRECTION_CONSTANT_H

#include "../../bm3_Const.h"
#include "../../bm3_DrawInfo.h"
#include <ml/type/ml_vector3d.h>

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

namespace policy{

namespace impl{

/// @brief Gravity_Direction_ConstantƒNƒ‰ƒX
class Gravity_Direction_Constant{

public:
	template<typename ResourceType>
	static BM3_FORCEINLINE void	Initialize(const ResourceType &res, ml::random &/*random*/, ml::vector3d &direction){

//		if(res.Field_Direction_FieldDirectionType() == FieldDirectionTypeConst_Constant)
		{
			direction = res.Field_Direction_Initial();
			direction.normalize();
		}
	}

	template<typename ResourceType, typename UpdateInfo>
	static BM3_FORCEINLINE void	Update(UpdateInfo &/*update_info*/, const ResourceType &/*res*/, ml::vector3d &/*direction*/){

	}

	template<typename ResourceType ,typename FieldType>
	static void	Prepare(ml::vector3d &/*dst*/, DrawInfo &/*draw_info*/, const FieldType &/*field*/, const ResourceType &/*res*/, const ml::matrix44 &/*m*/){
	}
	
	template<typename ResourceType>
	static BM3_FORCEINLINE void	SwitchResource(const ResourceType &res, const ResourceType &old_res, ml::random &random, ml::vector3d &direction){
		if(old_res.Field_Direction_FieldDirectionType() == FieldDirectionTypeConst_Constant){
			switch(res.Field_Direction_FieldDirectionType()){
				case FieldDirectionTypeConst_Constant: Gravity_Direction_Constant::Initialize(res, random, direction);	break;
				case FieldDirectionTypeConst_Curve:																		break;
				case FieldDirectionTypeConst_MAX:																		break;
				case FieldDirectionTypeConst_FORCE32:																		break;
			}
		}
	}	
};

}	// namespace impl

}	// namespace policy
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_GRAVITY_DIRECTION_CONSTANT_H
