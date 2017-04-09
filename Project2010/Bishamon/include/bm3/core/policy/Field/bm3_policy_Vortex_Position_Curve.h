#ifndef BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_VORTEX_POSITION_CURVE_H
#define BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_VORTEX_POSITION_CURVE_H

#include "../../bm3_Const.h"
#include "../../bm3_DrawInfo.h"
#include <ml/type/ml_vector3d.h>

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

namespace policy{

namespace impl{

/// Vortex_Position_CurveƒNƒ‰ƒX
class Vortex_Position_Curve{

public:
	template<typename ResourceType>
	static BM3_FORCEINLINE void	Initialize(const ResourceType &res, ml::random & /*random*/, ml::vector3d &value){

		if(res.Field_Position_FieldPosType() == FieldPosTypeConst_Curve){
			value	= res.Field_Position_Curve().Value(0.0f);
		}
	}

	template<typename ResourceType>
	static BM3_FORCEINLINE void	Update(const ResourceType &res, ml::vector3d &value, const PrimitiveUpdateInfo &primitive_update_info){

		if(res.Field_Position_FieldPosType() == FieldPosTypeConst_Curve){
			value = res.Field_Position_Curve().Value(primitive_update_info.life_rate_);
		}
	}

	template<typename ResourceType>
	static BM3_FORCEINLINE void Prepare(const ResourceType &res, ml::vector3d &value, const PrimitiveDrawInfo &pdi){

	}
};

}	// namespace impl

}	// namespace policy
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_VORTEX_POSITION_CURVE_H
