#ifndef BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_VORTEX_MAGNITUDE_CURVE_H
#define BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_VORTEX_MAGNITUDE_CURVE_H

#include "../../bm3_Const.h"
#include "../../bm3_DrawInfo.h"
#include <ml/type/ml_vector3d.h>

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

namespace policy{

namespace impl{

/// @brief Vortex_Magnitude_CurveƒNƒ‰ƒX
class Vortex_Magnitude_Curve{

public:
	template<typename ResourceType>
	static BM3_FORCEINLINE void	Initialize(const ResourceType &res, ml::random &random, float &value, float &noise){

		if(res.Field_Magnitude_FieldMagnitudeType() == FieldMagnitudeTypeConst_Curve){
//			value	= res.Field_Magnitude_Curve().Value(0.0f);
			value	= 0.0f;
			noise	= random.f32(-res.Field_Option_Noise(), res.Field_Option_Noise());
		}
	}

	template<typename ResourceType, typename UpdateInfo>
	static BM3_FORCEINLINE void	Update(UpdateInfo &/*update_info*/, const ResourceType &res, float &value, const PrimitiveUpdateInfo &primitive_update_info){

//		if(res.Field_Magnitude_FieldMagnitudeType() == FieldMagnitudeTypeConst_Curve){
//		}
	}

	template<typename ResourceType>
	static BM3_FORCEINLINE void PrepareAddition(const ResourceType &res, float &value, float &noise, const PrimitiveDrawInfo &pdi){

		if(res.Field_Magnitude_FieldMagnitudeType() == FieldMagnitudeTypeConst_Curve){
			value += res.Field_Magnitude_Curve().Value(pdi.life_rate_) + noise;
		}
	}
	template<typename ResourceType>
	static BM3_FORCEINLINE void Prepare(const ResourceType &res, float &value, float &noise, const PrimitiveDrawInfo &pdi){

		if(res.Field_Magnitude_FieldMagnitudeType() == FieldMagnitudeTypeConst_Curve){
			value = res.Field_Magnitude_Curve().Value(pdi.life_rate_) + noise;
		}
	}
};

}	// namespace impl

}	// namespace policy
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_VORTEX_MAGNITUDE_CURVE_H
