#ifndef BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_RADIALPOWER_CONSTANT_H
#define BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_RADIALPOWER_CONSTANT_H

#include "../../bm3_Const.h"
#include "../../bm3_DrawInfo.h"
#include <ml/type/ml_vector3d.h>

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

namespace policy{

namespace impl{

/// @brief Radialpower_ConstantƒNƒ‰ƒX
class Radialpower_Constant{

public:
	template<typename ResourceType>
	static BM3_FORCEINLINE void	Initialize(const ResourceType &res, ml::random &random, ml::vector3d &value){

//		if(res.Field_Basic_FieldRadialType() == FieldRadialTypeConst_Constant){
			value.set_unit();
//		}
	}

	template<typename ResourceType, typename UpdateInfo>
	static BM3_FORCEINLINE void	Update(UpdateInfo &update_info, const ResourceType &res, ml::vector3d &value){

	}

	template<
		typename FieldType,
		typename PrimitiveType,
		typename ResourceType
	>
	static void	Prepare(ml::vector3d &dst, DrawInfo &draw_info, FieldType &field, const ResourceType &res, const ml::matrix44 &m, const PrimitiveType &prim){

//		if(res.Field_Basic_FieldRadialType() == FieldRadialTypeConst_Constant){
/*
			{
				ml::ni::vector3d	translate;
				translate.mul(prim.GetTranslate(), m);
				translate.add(field.value_);
				translate.add(dst);
				translate.sub(res.Field_CenterPosition_Initial());

				float	mag = res.Field_Magnitude_Initial();
				float	len = translate.length();
				if((len != 0.0f) && (res.Field_Option_Damp() != 0)){
					mag *= ml::inverse(res.Field_Option_Damp() * translate.length());
				}
				if(len < ml::abs(mag)) mag = (mag<0) ? -len : len;

				translate.normalize().mul(mag);
				field.value_.add(translate);
			}
			dst.add(field.value_);
*///		}
	}
};

}	// namespace impl

}	// namespace policy
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_RADIALPOWER_CONSTANT_H
