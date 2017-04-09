#ifndef BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_GRAVITY_MAGNITUDE_CONSTANT_H
#define BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_GRAVITY_MAGNITUDE_CONSTANT_H

#include "../../bm3_Const.h"
#include "../../bm3_DrawInfo.h"
#include <ml/type/ml_vector3d.h>

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

namespace policy{

namespace impl{

/// @brief Gravity_Magnitude_ConstantƒNƒ‰ƒX
class Gravity_Magnitude_Constant{

public:
	template<typename ResourceType>
	static BM3_FORCEINLINE void	Initialize(const ResourceType & /*res*/, ml::random &/*random*/, float &value, float &speed){

//		if(res.Field_Magnitude_FieldMagnitudeType() == FieldMagnitudeTypeConst_Constant)
		{
			value = speed = 0.0f;
		}
	}

	template<typename ResourceType, typename UpdateInfo>
	static BM3_FORCEINLINE void	Update(UpdateInfo &update_info, const ResourceType &res, float &value, float &speed){
//		if(res.Field_Magnitude_FieldMagnitudeType() == FieldMagnitudeTypeConst_Constant)
		{
			speed += res.Field_Magnitude_Initial()*update_info.GetUpdateFrame();
			value += speed;
		}
	}

	template<typename ResourceType ,typename FieldType>
	static void	Prepare(ml::vector3d &dst, DrawInfo &draw_info, const FieldType &field, const ResourceType &res, const ml::matrix44 &m){

	}
	
	template<typename ResourceType ,typename FieldType>
	static BM3_FORCEINLINE void	SwitchResource(const ResourceType &res, const ResourceType &old_res, ml::random &/*random*/, float &value, float &speed){
//		if(old_res.Field_Magnitude_FieldMagnitudeType() == FieldMagnitudeTypeConst_Constant){
//			switch(res.Field_Magnitude_FieldMagnitudeType()){
//				case FieldMagnitudeTypeConst_Constant: Gravity_Magnitude_Constant::Initialize(res, random, value, speed);	break;
//				case FieldMagnitudeTypeConst_Curve:																			break;
//			}
//		}
	}	
};

}	// namespace impl

}	// namespace policy
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_GRAVITY_MAGNITUDE_CONSTANT_H
