#ifndef BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_MATRIXTRASNRATE_SWITCHRESOURCE_H
#define BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_MATRIXTRASNRATE_SWITCHRESOURCE_H

#include "../../bm3_Const.h"
#include <ml/type/ml_vector3d.h>

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

namespace policy{

namespace impl{

/// @brief MatrixTranslate_SwitchResourceƒNƒ‰ƒX
class MatrixTranslate_SwitchResource{

public:
	template<typename ResourceType>
	static BM3_FORCEINLINE void Switch(const ResourceType &res, const ResourceType &old_res, ml::random &random, ml::vector3d &value, ml::vector3d &speed, ml::vector3d &accel, const PrimitiveInitializeInfo & /*pii*/, ml::vector3d &position, ml::vector3d &direction){
		switch(old_res.Translate_Basic_TranslateType()){
			case TranslateTypeConst_Constant:
				switch(res.Translate_Basic_TranslateType()){
				case TranslateTypeConst_Constant:		MatrixTranslate_Constant::Initialize(		res, random, value						,position);	break;
				case TranslateTypeConst_NewtonDynamics:	
					if(res.GenerateShape_Basic_Normal()){
						speed.mul(direction	,res.Translate_Speed_Initial());
						speed.random_range(random, speed, res.Translate_Speed_Range());
						accel.mul(direction, res.Translate_Acceleration_Initial());
						accel.random_range(random, accel,	res.Translate_Acceleration_Range());
					}
					else{
						speed.random_range(random,			res.Translate_Speed_Initial(),			res.Translate_Speed_Range());
						accel.random_range(random,	res.Translate_Acceleration_Initial(),	res.Translate_Acceleration_Range());
					}
					break;
				case TranslateTypeConst_CurveValue:	
				case TranslateTypeConst_CurveSpeed:	
				case TranslateTypeConst_MAX:	
				case TranslateTypeConst_FORCE32:
					break;
	//			case TranslateTypeConst_NewtonDynamics:	MatrixTranslate_Newtondynamics::Initialize(	res, random, value,	speed,	accel		);	break;
	//			case TranslateTypeConst_CurveValue:		MatrixTranslate_CurveValue::Initialize(		res, random, value,					pii	);	break;
	//			case TranslateTypeConst_CurveSpeed:		MatrixTranslate_CurveSpeed::Initialize(		res, random, value,	speed,  		pii	);	break;
				}
				break;
			case TranslateTypeConst_NewtonDynamics:
			case TranslateTypeConst_CurveValue:
			case TranslateTypeConst_CurveSpeed:
				switch(res.Translate_Basic_TranslateType()){
	//			case TranslateTypeConst_Constant:		MatrixTranslate_Constant::Initialize(		res, random, value						,position);	break;
				case TranslateTypeConst_NewtonDynamics:	
					if(res.GenerateShape_Basic_Normal()){
						speed.mul(direction	,res.Translate_Speed_Initial());
						speed.random_range(random, speed, res.Translate_Speed_Range());
						accel.mul(direction, res.Translate_Acceleration_Initial());
						accel.random_range(random, accel,	res.Translate_Acceleration_Range());
					}
					else{
						speed.random_range(random,			res.Translate_Speed_Initial(),			res.Translate_Speed_Range());
						accel.random_range(random,	res.Translate_Acceleration_Initial(),	res.Translate_Acceleration_Range());
					}
					break;
				case TranslateTypeConst_Constant:	
				case TranslateTypeConst_CurveValue:	
				case TranslateTypeConst_CurveSpeed:	
				case TranslateTypeConst_MAX:	
				case TranslateTypeConst_FORCE32:
					break;
	//			case TranslateTypeConst_NewtonDynamics:	MatrixTranslate_Newtondynamics::Initialize(	res, random, value,	speed,	accel		);	break;
	//			case TranslateTypeConst_CurveValue:		MatrixTranslate_CurveValue::Initialize(		res, random, value,					pii	);	break;
	//			case TranslateTypeConst_CurveSpeed:		MatrixTranslate_CurveSpeed::Initialize(		res, random, value,	speed,  		pii	);	break;
				}
				break;
			case TranslateTypeConst_MAX:
			case TranslateTypeConst_FORCE32:
				break;
		}
	}
	
};

}	// namespace impl

}	// namespace policy
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_MATRIXTRASNRATE_SWITCHRESOURCE_H
