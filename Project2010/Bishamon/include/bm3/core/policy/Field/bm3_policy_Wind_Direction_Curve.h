#ifndef BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_WIND_DIRECTION_CURVE_H
#define BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_WIND_DIRECTION_CURVE_H

#include "../../bm3_Const.h"
#include "../../bm3_DrawInfo.h"
#include <ml/type/ml_vector3d.h>

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

namespace policy{

namespace impl{

/// @brief Wind_Direction_Curve�N���X
class Wind_Direction_Curve{

public:
	template<typename ResourceType>
	static BM3_FORCEINLINE void	Initialize(const ResourceType &res, ml::random &/*random*/, ml::vector3d &direction){

		if(res.Field_Direction_FieldDirectionType() == FieldDirectionTypeConst_Curve){
			direction = res.Field_Direction_Curve().Value(0.0f);
		}
	}

	template<typename ResourceType, typename UpdateInfo>
	static BM3_FORCEINLINE void	Update(UpdateInfo &/*update_info*/, const ResourceType &res, ml::vector3d &direction, const PrimitiveUpdateInfo &primitive_update_info){

		if(res.Field_Direction_FieldDirectionType() == FieldDirectionTypeConst_Curve){
//			direction = res.Field_Direction_Curve().Value(primitive_update_info.life_rate_, primitive_update_info.random_seed_, res.Field_Direction_Range());
			direction = res.Field_Direction_Curve().Value(primitive_update_info.life_rate_);
		}
	}

	template<typename ResourceType, typename UpdateInfo>
	static BM3_FORCEINLINE void	Update(UpdateInfo &/*update_info*/, const ResourceType &res, float &magnitude, ml::vector3d &direction, ml::vector3d &pre_direction, const PrimitiveUpdateInfo &primitive_update_info){

		if(res.Field_Direction_FieldDirectionType() == FieldDirectionTypeConst_Curve){
			ml::vector3d	temp	= res.Field_Direction_Curve().Value(primitive_update_info.life_rate_, primitive_update_info.random_seed_, res.Field_Direction_Range());
			ml::vector3d	temp2	= temp;
			temp.sub(pre_direction);
			direction.add(temp.mul(ml::inverse(magnitude)));
			pre_direction	= temp2;
		}
	}

	template<typename ResourceType ,typename FieldType>
	static void	Prepare(ml::vector3d &dst, DrawInfo &draw_info, const FieldType &field, const ResourceType &res, const ml::matrix44 &m){

	}
};

}	// namespace impl

}	// namespace policy
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_WIND_DIRECTION_CURVE_H
