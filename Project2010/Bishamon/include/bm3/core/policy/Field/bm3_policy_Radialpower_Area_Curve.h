#ifndef BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_RADIALPOWER_AREA_CURVE_H
#define BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_RADIALPOWER_AREA_CURVE_H

#include "../../bm3_Const.h"
#include "../../bm3_DrawInfo.h"
#include <ml/type/ml_vector3d.h>

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

namespace policy{

namespace impl{

/// @brief Radialpower_Area_CurveƒNƒ‰ƒX
class Radialpower_Area_Curve{

public:
	template<typename ResourceType>
	static BM3_FORCEINLINE void	Initialize(const ResourceType &res, ml::random &/*random*/, float &area){

		if(res.Field_Area_FieldAreaType() == FieldAreaTypeConst_Curve){
			area = res.Field_Area_Curve().Value(0.0f);
		}
	}

	template<typename ResourceType, typename UpdateInfo>
	static BM3_FORCEINLINE void	Update(UpdateInfo &/*update_info*/, const ResourceType &res, float &area, const PrimitiveUpdateInfo &primitive_update_info){

		if(res.Field_Area_FieldAreaType() == FieldAreaTypeConst_Curve){
			area = res.Field_Area_Curve().Value(primitive_update_info.life_rate_);
		}
	}

	template<typename ResourceType ,typename FieldType>
	static void	Prepare(ml::vector3d &dst, DrawInfo &draw_info, const FieldType &field, const ResourceType &res, const ml::matrix44 &m){

	}
	
	template<typename ResourceType>
	static BM3_FORCEINLINE void	SwitchResource(const ResourceType & /*res*/, const ResourceType & /*old_res*/, ml::random & /*random*/, float & /*area*/){
//		if(old_res.Field_Area_FieldAreaType() == FieldAreaTypeConst_Constant){
//			switch(res.Field_Area_FieldAreaType()){
//				case FieldAreaTypeConst_Constant: Radialpower_Area_Constant::Initialize(res, random, area);	break;
//				case FieldAreaTypeConst_Curve: break;
//			}
//		}
	}	
};

}	// namespace impl

}	// namespace policy
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_RADIALPOWER_AREA_CURVE_H
