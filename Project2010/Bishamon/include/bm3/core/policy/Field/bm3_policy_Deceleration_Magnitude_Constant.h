#ifndef BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_DECELERATION_MAGNITUDE_CONSTANT_H
#define BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_DECELERATION_MAGNITUDE_CONSTANT_H

#include "../../bm3_Const.h"
#include "../../bm3_DrawInfo.h"
#include <ml/type/ml_vector3d.h>

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

namespace policy{

namespace impl{

/// @brief Deceleration_Magnitude_Constant�N���X
class Deceleration_Magnitude_Constant{

public:
	template<typename ResourceType>
	static BM3_FORCEINLINE void	Initialize(const ResourceType &res, ml::random &/*random*/, float &value){

//		if(res.Field_Magnitude_FieldMagnitudeType() == FieldMagnitudeTypeConst_Constant)
		{
			value = res.Field_Magnitude_Initial();
		}
	}

	template<typename ResourceType, typename UpdateInfo>
	static BM3_FORCEINLINE void	Update(UpdateInfo &update_info, const ResourceType &res, float &value){
//		if(res.Field_Magnitude_FieldMagnitudeType() == FieldMagnitudeTypeConst_Constant)
		{
			value += res.Field_Magnitude_Initial() * (1.0f - value) * update_info.GetUpdateFrame();
			value	= ml::minimum(value, 1.0f);
		}
	}

	template<typename ResourceType ,typename FieldType>
	static void	Prepare(ml::vector3d &/*dst*/, DrawInfo &/*draw_info*/, const FieldType &/*field*/, const ResourceType &/*res*/, const ml::matrix44 &/*m*/){

	}
	
	template<typename ResourceType>
	static BM3_FORCEINLINE void	SwitchResource(const ResourceType &res, const ResourceType &old_res, ml::random &random, float &value){
		if(old_res.Field_Magnitude_FieldMagnitudeType() == FieldMagnitudeTypeConst_Constant){
			switch(res.Field_Magnitude_FieldMagnitudeType()){
				case FieldMagnitudeTypeConst_Constant: Deceleration_Magnitude_Constant::Initialize(res, random, value);	break;
				case FieldMagnitudeTypeConst_Curve:																		break;
				case FieldMagnitudeTypeConst_MAX:																		break;
				case FieldMagnitudeTypeConst_FORCE32:																		break;
			}
		}
	}	
};

}	// namespace impl

}	// namespace policy
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_DECELERATION_MAGNITUDE_CONSTANT_H
