#ifndef BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_GOAL_RESOURCESWITCH_H
#define BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_GOAL_RESOURCESWITCH_H

#include "../../bm3_Const.h"
#include "../../bm3_DrawInfo.h"
#include <ml/type/ml_vector3d.h>

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

namespace policy{

namespace impl{

/// @brief Goal_Position_ResourceSwitchƒNƒ‰ƒX
class Goal_Position_ResourceSwitch{
public:
	template<typename ResourceType>
	static BM3_FORCEINLINE void	Switch(const ResourceType &res, const ResourceType & /*old_res*/, ml::random &random, ml::vector3d &position, ml::vector3d &offset_position){
		switch(res.Field_Position_FieldPosType()){
			case FieldPosTypeConst_Constant: Goal_Position_Constant::Initialize(res, random, position, offset_position);	break;
			case FieldPosTypeConst_Curve:																break;
			case FieldPosTypeConst_MAX:																break;
			case FieldPosTypeConst_FORCE32:																break;
		}
	}
};

}	// namespace impl

}	// namespace policy
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_GOAL_RESOURCESWITCH_H
