#ifndef BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_VORTEX_AXIS_CURVE_H
#define BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_VORTEX_AXIS_CURVE_H

#include "../../bm3_Const.h"
#include "../../bm3_DrawInfo.h"
#include <ml/type/ml_vector3d.h>

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

namespace policy{

namespace impl{

/// @brief Vortex_Axis_CurveƒNƒ‰ƒX
class Vortex_Axis_Curve{

public:
	template<typename ResourceType>
	static BM3_FORCEINLINE void	Initialize(const ResourceType &res, ml::random & /*random*/, ml::vector3d &value){

		if(res.Field_Axis_FieldAxisType() == FieldAxisTypeConst_Curve){
			value = res.Field_Axis_Curve().Value(0.0f);
		}
	}

	template<typename ResourceType, typename UpdateInfo>
	static BM3_FORCEINLINE void	Update(UpdateInfo &/*update_info*/, const ResourceType &res, ml::vector3d &value, const PrimitiveUpdateInfo &primitive_update_info){

	}

	template<typename ResourceType>
	static void Prepare(const ResourceType &res, ml::vector3d &value, PrimitiveDrawInfo &pdi){
		if(res.Field_Axis_FieldAxisType() == FieldAxisTypeConst_Curve){
			value = res.Field_Axis_Curve().Value(pdi.life_rate_);
		}
	}

};

}	// namespace impl

}	// namespace policy
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_VORTEX_AXIS_CURVE_H
