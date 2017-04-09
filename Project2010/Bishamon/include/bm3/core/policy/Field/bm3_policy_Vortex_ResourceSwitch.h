#ifndef BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_VORTEX_RESOURCESWITCH_H
#define BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_VORTEX_RESOURCESWITCH_H

#include "../../bm3_Const.h"
#include "../../bm3_DrawInfo.h"
#include <ml/type/ml_vector3d.h>

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

namespace policy{

namespace impl{

/// @brief Vortex_Area_ResourceSwitchƒNƒ‰ƒX
class Vortex_Area_ResourceSwitch{
public:
	template<typename ResourceType>
	static BM3_FORCEINLINE void	Switch(const ResourceType &res, const ResourceType & /*old_res*/, ml::random &random, float &area){
		switch(res.Field_Area_FieldAreaType()){
			case FieldAreaTypeConst_Infinit:															break;
			case FieldAreaTypeConst_Constant: Vortex_Area_Constant::Initialize(res, random, area);	break;
			case FieldAreaTypeConst_Curve:															break;
			case FieldAreaTypeConst_MAX:															break;
			case FieldAreaTypeConst_FORCE32:															break;
		}
	}
};

class Vortex_Axis_ResourceSwitch{
public:
	template<typename ResourceType>
	static BM3_FORCEINLINE void	Switch(const ResourceType &res, const ResourceType & /*old_res*/, ml::random &random, ml::vector3d &axis){
		switch(res.Field_Axis_FieldAxisType()){
			case FieldAxisTypeConst_Constant: Vortex_Axis_Constant::Initialize(res, random, axis);	break;
			case FieldAxisTypeConst_Curve:															break;
			case FieldAxisTypeConst_MAX:															break;
			case FieldAxisTypeConst_FORCE32:															break;
		}
	}
};

class Vortex_Magnitude_ResourceSwitch{
public:
	template<typename ResourceType>
	static BM3_FORCEINLINE void	Switch(const ResourceType &res, const ResourceType & /*old_res*/, ml::random &random, float &magnitude, float &noise){
		switch(res.Field_Magnitude_FieldMagnitudeType()){
			case FieldMagnitudeTypeConst_Constant: Vortex_Magnitude_Constant::Initialize(res, random, magnitude, noise);	break;
			case FieldMagnitudeTypeConst_Curve:																				break;
			case FieldMagnitudeTypeConst_MAX:																				break;
			case FieldMagnitudeTypeConst_FORCE32:																				break;
		}
	}
};

class Vortex_Position_ResourceSwitch{
public:
	template<typename ResourceType>
	static BM3_FORCEINLINE void	Switch(const ResourceType &res, const ResourceType & /*old_res*/, ml::random &random, ml::vector3d &position){
		switch(res.Field_Position_FieldPosType()){
			case FieldPosTypeConst_Constant: Vortex_Position_Constant::Initialize(res, random, position);	break;
			case FieldPosTypeConst_Curve:																	break;
			case FieldPosTypeConst_MAX:																	break;
			case FieldPosTypeConst_FORCE32:																	break;
		}
	}
};

}	// namespace impl

}	// namespace policy
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_VORTEX_RESOURCESWITCH_H
