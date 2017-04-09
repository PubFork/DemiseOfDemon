#ifndef BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_MATRIXROTATE_SIWTCHRESOURCE_H
#define BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_MATRIXROTATE_SIWTCHRESOURCE_H

#include "../../bm3_Const.h"
#include <ml/type/ml_vector3d.h>

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

namespace policy{

namespace impl{

/// @brief MatrixRotate_SwitchResourceƒNƒ‰ƒX
class MatrixRotate_SwitchResource{

public:
	template<typename ResourceType>
	static BM3_FORCEINLINE void Switch(const ResourceType &res, const ResourceType &old_res, ml::random &random, ml::vector3d &value, ml::vector3d &speed, ml::vector3d &accel, const PrimitiveInitializeInfo & /*pii*/){
		switch(old_res.Rotate_Basic_RotateType()){
			case RotateTypeConst_Constant:
				switch(res.Rotate_Basic_RotateType()){
				case RotateTypeConst_Constant:			MatrixRotate_Constant::Initialize(			res, random, value						);	break;
				case RotateTypeConst_NewtonDynamics:
					speed.random_range(			random, res.Rotate_Speed_Initial(),			res.Rotate_Speed_Range());
					accel.random_range(	random, res.Rotate_Acceleration_Initial(),	res.Rotate_Acceleration_Range());
					speed	= speed.to_radian();
					accel	= accel.to_radian();
					break;
				case RotateTypeConst_CurveValue:
				case RotateTypeConst_CurveSpeed:
				case RotateTypeConst_MAX:
				case RotateTypeConst_FORCE32:
					break;
	//			case RotateTypeConst_NewtonDynamics:	MatrixRotate_Newtondynamics::Initialize(	res, random, value,	speed,	accel		);	break;
	//			case RotateTypeConst_CurveValue:		MatrixRotate_CurveValue::Initialize(		res, random, value,					pii	);	break;
	//			case RotateTypeConst_CurveSpeed:		MatrixRotate_CurveSpeed::Initialize(		res, random, value,	speed,  		pii	);	break;
				}
				break;
			case RotateTypeConst_NewtonDynamics:
			case RotateTypeConst_CurveValue:
			case RotateTypeConst_CurveSpeed:
				switch(res.Rotate_Basic_RotateType()){
	//			case RotateTypeConst_Constant:			MatrixRotate_Constant::Initialize(			res, random, value						);	break;
				case RotateTypeConst_NewtonDynamics:
					speed.random_range(			random, res.Rotate_Speed_Initial(),			res.Rotate_Speed_Range());
					accel.random_range(	random, res.Rotate_Acceleration_Initial(),	res.Rotate_Acceleration_Range());
					speed	= speed.to_radian();
					accel	= accel.to_radian();
					break;
				case RotateTypeConst_Constant:
				case RotateTypeConst_CurveValue:
				case RotateTypeConst_CurveSpeed:
				case RotateTypeConst_MAX:
				case RotateTypeConst_FORCE32:
					break;
	//			case RotateTypeConst_NewtonDynamics:	MatrixRotate_Newtondynamics::Initialize(	res, random, value,	speed,	accel		);	break;
	//			case RotateTypeConst_CurveValue:		MatrixRotate_CurveValue::Initialize(		res, random, value,					pii	);	break;
	//			case RotateTypeConst_CurveSpeed:		MatrixRotate_CurveSpeed::Initialize(		res, random, value,	speed,  		pii	);	break;
				}
				break;
			case RotateTypeConst_MAX:
			case RotateTypeConst_FORCE32:
				break;
		}
	}	
};

}	// namespace impl


}	// namespace policy
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_MATRIXROTATE_SIWTCHRESOURCE_H
