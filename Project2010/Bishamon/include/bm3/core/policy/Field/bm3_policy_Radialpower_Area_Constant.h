#ifndef BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_RADIALPOWER_AREA_CONSTANT_H
#define BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_RADIALPOWER_AREA_CONSTANT_H

#include "../../bm3_Const.h"
#include "../../bm3_DrawInfo.h"
#include <ml/type/ml_vector3d.h>

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

namespace policy{

namespace impl{

/// @brief Radialpower_Area_ConstantƒNƒ‰ƒX
class Radialpower_Area_Constant{

public:
	template<typename ResourceType>
	static BM3_FORCEINLINE void	Initialize(const ResourceType &res, ml::random &/*random*/, float &area){

		if(res.Field_Area_FieldAreaType() == FieldAreaTypeConst_Constant){
			area = res.Field_Area_Initial();
		}
	}

	template<typename ResourceType, typename UpdateInfo>
	static BM3_FORCEINLINE void	Update(UpdateInfo &/*update_info*/, const ResourceType &/*res*/, float &/*area*/){

	}

	template<typename ResourceType ,typename FieldType>
	static void	Prepare(ml::vector3d &/*dst*/, DrawInfo &/*draw_info*/, const FieldType &/*field*/, const ResourceType &/*res*/, const ml::matrix44 &/*m*/){
	}
	
	template<typename ResourceType>
	static BM3_FORCEINLINE void	SwitchResource(const ResourceType &res, const ResourceType &old_res, ml::random &random, float &area){
		if(old_res.Field_Area_FieldAreaType() == FieldAreaTypeConst_Constant){
			switch(res.Field_Area_FieldAreaType()){
				case FieldAreaTypeConst_Infinit: break;
				case FieldAreaTypeConst_Constant: Radialpower_Area_Constant::Initialize(res, random, area);	break;
				case FieldAreaTypeConst_Curve: break;
				case FieldAreaTypeConst_MAX: break;
				case FieldAreaTypeConst_FORCE32: break;
			}
		}
	}	
};

}	// namespace impl

}	// namespace policy
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_RADIALPOWER_AREA_CONSTANT_H
