#ifndef BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_RADIALPOWER_POSITION_CONSTANT_H
#define BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_RADIALPOWER_POSITION_CONSTANT_H

#include "../../bm3_Const.h"
#include "../../bm3_DrawInfo.h"
#include <ml/type/ml_vector3d.h>

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

namespace policy{

namespace impl{

/// @brief Radialpower_Position_ConstantƒNƒ‰ƒX
class Radialpower_Position_Constant{

public:
	template<typename ResourceType>
	static BM3_FORCEINLINE void	Initialize(const ResourceType &res, ml::random &/*random*/, ml::vector3d &position){

		if(res.Field_CenterPosition_FieldPositionType() == FieldPositionTypeConst_Constant){
			position = res.Field_CenterPosition_Initial();
		}
	}

	template<typename ResourceType, typename UpdateInfo>
	static BM3_FORCEINLINE void	Update(UpdateInfo &/*update_info*/, const ResourceType &/*res*/, ml::vector3d &/*position*/){

	}

	template<typename ResourceType ,typename FieldType>
	static void	Prepare(ml::vector3d &/*dst*/, DrawInfo &/*draw_info*/, const FieldType &/*field*/, const ResourceType &/*res*/, const ml::matrix44 &/*m*/){
	}
	
	template<typename ResourceType>
	static BM3_FORCEINLINE void	SwitchResource(const ResourceType &res, const ResourceType &old_res, ml::random &random, ml::vector3d &position){
		if(old_res.Field_CenterPosition_FieldPositionType() == FieldPositionTypeConst_Constant){
			switch(res.Field_CenterPosition_FieldPositionType()){
				case FieldPositionTypeConst_Constant: Radialpower_Position_Constant::Initialize(res, random, position);	break;
				case FieldPositionTypeConst_Curve: break;
				case FieldPositionTypeConst_MAX: break;
				case FieldPositionTypeConst_FORCE32: break;
			}
		}
	}	
};

}	// namespace impl

}	// namespace policy
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_RADIALPOWER_POSITION_CONSTANT_H
