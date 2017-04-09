#ifndef BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_MATRIXTRASNRATE_NEWTONDYNAMICS_H
#define BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_MATRIXTRASNRATE_NEWTONDYNAMICS_H

#include "../../bm3_Const.h"
#include <ml/type/ml_vector3d.h>

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

namespace policy{

namespace impl{

/// @brief MatrixTranslate_NewtondynamicsƒNƒ‰ƒX
class MatrixTranslate_Newtondynamics{

public:
	template<typename ResourceType>
	static BM3_FORCEINLINE void Initialize(const ResourceType &res, ml::random &random, ml::vector3d &value, ml::vector3d &speed, ml::vector3d &acceleration){

//		if(res.Translate_Basic_TranslateType() == TranslateTypeConst_NewtonDynamics)
		{
			value.random_range(random,			res.Translate_Value_Initial(),			res.Translate_Value_Range());
			speed.random_range(random,			res.Translate_Speed_Initial(),			res.Translate_Speed_Range());
			acceleration.random_range(random,	res.Translate_Acceleration_Initial(),	res.Translate_Acceleration_Range());
		}
	}

	template<typename ResourceType>
	static BM3_FORCEINLINE void Initialize(const ResourceType &res, ml::random &random, ml::vector3d &value, ml::vector3d &speed, ml::vector3d &acceleration, ml::vector3d &position, ml::vector3d &direction){

//		if(res.Translate_Basic_TranslateType() == TranslateTypeConst_NewtonDynamics)
		{
			value.random_range(random, res.Translate_Value_Initial(), res.Translate_Value_Range());
			value.add(position);
			if(res.GenerateShape_Basic_Normal()){
				speed.mul(direction	,res.Translate_Speed_Initial());
				speed.random_range(random, speed, res.Translate_Speed_Range());
				acceleration.mul(direction, res.Translate_Acceleration_Initial());
				acceleration.random_range(random, acceleration,	res.Translate_Acceleration_Range());
			}
			else{
				speed.random_range(random,			res.Translate_Speed_Initial(),			res.Translate_Speed_Range());
				acceleration.random_range(random,	res.Translate_Acceleration_Initial(),	res.Translate_Acceleration_Range());
			}
		}
	}

	template<typename ResourceType, typename UpdateInfo>
	static BM3_FORCEINLINE void Update(UpdateInfo &/*update_info*/, const ResourceType &res, ml::vector3d &value, ml::vector3d &speed, ml::vector3d &acceleration){

//		if(res.Translate_Basic_TranslateType() == TranslateTypeConst_NewtonDynamics)
		{
			speed.add(acceleration);
			value.add(speed);
		}
	}

	template<typename ResourceType>
	static BM3_FORCEINLINE void Prepare(const ResourceType & /*res*/, const ml::vector3d &value, ml::matrix44 &m){

//		if(res.Translate_Basic_TranslateType() == TranslateTypeConst_NewtonDynamics)
		{
			m.mul_translate(value.e_.x_, value.e_.y_, value.e_.z_);
		}
	}
};

}	// namespace impl

class MatrixTranslate_Newtondynamics{

public:
	template<typename ResourceType>
	void BM3_FORCEINLINE Initialize(const ResourceType &res, ml::random &random, ml::vector3d &value){

		impl::MatrixTranslate_Newtondynamics::Initialize(res, random, value, speed_, acceleration_);
	}

	template<typename ResourceType>
	void BM3_FORCEINLINE Initialize(const ResourceType &res, ml::random &random, ml::vector3d &value, ml::vector3d &position, ml::vector3d &direction){

		impl::MatrixTranslate_Newtondynamics::Initialize(res, random, value, speed_, acceleration_ ,position ,direction);
	}

	template<typename ResourceType, typename UpdateInfo>
	void BM3_FORCEINLINE Update(UpdateInfo &update_info, const ResourceType &res, ml::vector3d &value){

		impl::MatrixTranslate_Newtondynamics::Update(update_info, res, value, speed_, acceleration_);
	}

	template<typename ResourceType>
	void BM3_FORCEINLINE Prepare(const ResourceType &res, const ml::vector3d &value, ml::matrix44 &m){

		impl::MatrixTranslate_Newtondynamics::Prepare(res, value, m);
	}

private:
	ml::ni::vector3d	speed_;
	ml::ni::vector3d	acceleration_;
};

}	// namespace policy
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_MATRIXTRASNRATE_NEWTONDYNAMICS_H
