#ifndef BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_MATRIXSCALE_SWITCHRESOURCE_H
#define BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_MATRIXSCALE_SWITCHRESOURCE_H

#include "../../bm3_Const.h"
#include <ml/type/ml_vector3d.h>

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

namespace policy{

namespace impl{

/// MatrixScale_SwitchResourceƒNƒ‰ƒX
template<bool AllowFor_Scale_Basic_Square = true>
class MatrixScale_SwitchResource{

public:
	template<typename ResourceType>
	static BM3_FORCEINLINE void Switch(const ResourceType &res, const ResourceType &old_res, ml::random &random, ml::vector3d &value, ml::vector3d &speed, ml::vector3d &accel, const PrimitiveInitializeInfo & /*pii*/);
};

template<>
class MatrixScale_SwitchResource<true>{

public:
	template<typename ResourceType>
	static BM3_FORCEINLINE void Switch(const ResourceType &res, const ResourceType &old_res, ml::random &random, ml::vector3d &value, ml::vector3d &speed, ml::vector3d &accel, const PrimitiveInitializeInfo & /*pii*/){
		switch(old_res.Scale_Basic_ScaleType()){
			case ScaleTypeConst_Constant:
				switch(res.Scale_Basic_ScaleType()){
				case ScaleTypeConst_Constant:			MatrixScale_Constant<true>::Initialize(			res, random, value						);	break;
				case ScaleTypeConst_NewtonDynamics:
					speed.random_range(	res.Scale_Basic_Square(), random, res.Scale_Speed_Initial(),		res.Scale_Speed_Range());
					accel.random_range(	res.Scale_Basic_Square(), random, res.Scale_Acceleration_Initial(),	res.Scale_Acceleration_Range());
					break;
				case ScaleTypeConst_CurveValue:
				case ScaleTypeConst_CurveSpeed:
				case ScaleTypeConst_MAX:
				case ScaleTypeConst_FORCE32:
					break;
	//			case ScaleTypeConst_NewtonDynamics:		MatrixScale_Newtondynamics<true>::Initialize(	res, random, value,	speed,	accel		);	break;
	//			case ScaleTypeConst_CurveValue:			MatrixScale_CurveValue<true>::Initialize(		res, random, value,					pii	);	break;
	//			case ScaleTypeConst_CurveSpeed:			MatrixScale_CurveSpeed<true>::Initialize(		res, random, value,	speed,  		pii	);	break;
				}
				break;
			case ScaleTypeConst_NewtonDynamics:
			case ScaleTypeConst_CurveValue:
			case ScaleTypeConst_CurveSpeed:
				switch(res.Scale_Basic_ScaleType()){
	//			case ScaleTypeConst_Constant:			MatrixScale_Constant<true>::Initialize(			res, random, value						);	break;
				case ScaleTypeConst_NewtonDynamics:
					speed.random_range(	res.Scale_Basic_Square(), random, res.Scale_Speed_Initial(),		res.Scale_Speed_Range());
					accel.random_range(	res.Scale_Basic_Square(), random, res.Scale_Acceleration_Initial(),	res.Scale_Acceleration_Range());
					break;
				case ScaleTypeConst_Constant:
				case ScaleTypeConst_CurveValue:
				case ScaleTypeConst_CurveSpeed:
				case ScaleTypeConst_MAX:
				case ScaleTypeConst_FORCE32:
					break;
	//			case ScaleTypeConst_NewtonDynamics:		MatrixScale_Newtondynamics<true>::Initialize(	res, random, value,	speed,	accel		);	break;
	//			case ScaleTypeConst_CurveValue:			MatrixScale_CurveValue<true>::Initialize(		res, random, value,					pii	);	break;
	//			case ScaleTypeConst_CurveSpeed:			MatrixScale_CurveSpeed<true>::Initialize(		res, random, value,	speed,  		pii	);	break;
				}
				break;
			case ScaleTypeConst_MAX:
			case ScaleTypeConst_FORCE32:
				break;
		}
	}
	
	template<typename ResourceType>
	static BM3_FORCEINLINE void Switch(const ResourceType &res, const ResourceType &old_res, ml::random &random, float &value, float &speed, float &accel, const PrimitiveInitializeInfo & /*pii*/){

		switch(old_res.Scale_Basic_ScaleType()){
			case ScaleTypeConst_Constant:
				switch(res.Scale_Basic_ScaleType()){
				case ScaleTypeConst_Constant:			MatrixScale_Constant<true>::Initialize(			res, random, value						);	break;
				case ScaleTypeConst_NewtonDynamics:
					speed = random.f32_range(			res.Scale_Speed_InitialX(),			res.Scale_Speed_RangeX());
					accel = random.f32_range(	res.Scale_Acceleration_InitialX(),	res.Scale_Acceleration_RangeX());
					value = ml::maximum(value, 0.0f);
					break;
				case ScaleTypeConst_CurveValue:
				case ScaleTypeConst_CurveSpeed:
				case ScaleTypeConst_MAX:
				case ScaleTypeConst_FORCE32:
					break;
	//			case ScaleTypeConst_NewtonDynamics:		MatrixScale_Newtondynamics<true>::Initialize(	res, random, value,	speed,	accel		);	break;
	//			case ScaleTypeConst_CurveValue:			MatrixScale_CurveValue<true>::Initialize(		res, random, value,					pii	);	break;
	//			case ScaleTypeConst_CurveSpeed:			MatrixScale_CurveSpeed<true>::Initialize(		res, random, value,	speed,  		pii	);	break;
				}
				break;
			case ScaleTypeConst_NewtonDynamics:
			case ScaleTypeConst_CurveValue:
			case ScaleTypeConst_CurveSpeed:
				switch(res.Scale_Basic_ScaleType()){
	//			case ScaleTypeConst_Constant:			MatrixScale_Constant<true>::Initialize(			res, random, value						);	break;
				case ScaleTypeConst_NewtonDynamics:
					speed = random.f32_range(			res.Scale_Speed_InitialX(),			res.Scale_Speed_RangeX());
					accel = random.f32_range(	res.Scale_Acceleration_InitialX(),	res.Scale_Acceleration_RangeX());
					value = ml::maximum(value, 0.0f);
					break;
				case ScaleTypeConst_Constant:
				case ScaleTypeConst_CurveValue:
				case ScaleTypeConst_CurveSpeed:
				case ScaleTypeConst_MAX:
				case ScaleTypeConst_FORCE32:
					break;
	//			case ScaleTypeConst_NewtonDynamics:		MatrixScale_Newtondynamics<true>::Initialize(	res, random, value,	speed,	accel		);	break;
	//			case ScaleTypeConst_CurveValue:			MatrixScale_CurveValue<true>::Initialize(		res, random, value,					pii	);	break;
	//			case ScaleTypeConst_CurveSpeed:			MatrixScale_CurveSpeed<true>::Initialize(		res, random, value,	speed,  		pii	);	break;
				}
				break;
			case ScaleTypeConst_MAX:
			case ScaleTypeConst_FORCE32:
				break;
		}
	}
	
};

template<>
class MatrixScale_SwitchResource<false>{

public:
	template<typename ResourceType>
	static BM3_FORCEINLINE void Switch(const ResourceType &res, const ResourceType &old_res, ml::random &random, ml::vector3d &value, ml::vector3d &speed, ml::vector3d &accel, const PrimitiveInitializeInfo & /*pii*/){
		switch(old_res.Scale_Basic_ScaleType()){
			case ScaleTypeConst_Constant:
				switch(res.Scale_Basic_ScaleType()){
				case ScaleTypeConst_Constant:			MatrixScale_Constant<false>::Initialize(		res, random, value						);	break;
				case ScaleTypeConst_NewtonDynamics:
					speed.random_range(	random, res.Scale_Speed_Initial(),			res.Scale_Speed_Range());
					accel.random_range(	random, res.Scale_Acceleration_Initial(),	res.Scale_Acceleration_Range());
					break;
				case ScaleTypeConst_CurveValue:
				case ScaleTypeConst_CurveSpeed:
				case ScaleTypeConst_MAX:
				case ScaleTypeConst_FORCE32:
					break;
	//			case ScaleTypeConst_NewtonDynamics:		MatrixScale_Newtondynamics<false>::Initialize(	res, random, value,	speed,	accel		);	break;
	//			case ScaleTypeConst_CurveValue:			MatrixScale_CurveValue<false>::Initialize(		res, random, value,					pii	);	break;
	//			case ScaleTypeConst_CurveSpeed:			MatrixScale_CurveSpeed<false>::Initialize(		res, random, value,	speed,  		pii	);	break;
				}
				break;
			case ScaleTypeConst_NewtonDynamics:
			case ScaleTypeConst_CurveValue:
			case ScaleTypeConst_CurveSpeed:
				switch(res.Scale_Basic_ScaleType()){
	//			case ScaleTypeConst_Constant:			MatrixScale_Constant<false>::Initialize(		res, random, value						);	break;
				case ScaleTypeConst_NewtonDynamics:
					speed.random_range(			random, res.Scale_Speed_Initial(),			res.Scale_Speed_Range());
					accel.random_range(	random, res.Scale_Acceleration_Initial(),	res.Scale_Acceleration_Range());
					break;
				case ScaleTypeConst_Constant:
				case ScaleTypeConst_CurveValue:
				case ScaleTypeConst_CurveSpeed:
				case ScaleTypeConst_MAX:
				case ScaleTypeConst_FORCE32:
					break;
	//			case ScaleTypeConst_NewtonDynamics:		MatrixScale_Newtondynamics<false>::Initialize(	res, random, value,	speed,	accel		);	break;
	//			case ScaleTypeConst_CurveValue:			MatrixScale_CurveValue<false>::Initialize(		res, random, value,					pii	);	break;
	//			case ScaleTypeConst_CurveSpeed:			MatrixScale_CurveSpeed<false>::Initialize(		res, random, value,	speed,  		pii	);	break;
				}
				break;
			case ScaleTypeConst_MAX:
			case ScaleTypeConst_FORCE32:
				break;
		}
	}	
};

}	// namespace impl

}	// namespace policy
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_MATRIXSCALE_SWITCHRESOURCE_H
