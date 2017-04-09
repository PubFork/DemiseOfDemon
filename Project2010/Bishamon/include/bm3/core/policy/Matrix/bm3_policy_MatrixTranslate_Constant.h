#ifndef BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_MATRIXTRASNRATE_CONSTANT_H
#define BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_MATRIXTRASNRATE_CONSTANT_H

#include "../../bm3_Const.h"
#include <ml/type/ml_vector3d.h>

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

namespace policy{

namespace impl{

/// @brief MatrixTranslate_ConstantƒNƒ‰ƒX
class MatrixTranslate_Constant{

public:
	template<typename ResourceType>
	static BM3_FORCEINLINE void Initialize(const ResourceType &res, ml::random &random, ml::vector3d &value){

//		if(res.Translate_Basic_TranslateType() == TranslateTypeConst_Constant)
		{
			value.random_range(random, res.Translate_Value_Initial(), res.Translate_Value_Range());
		}
	}

	template<typename ResourceType>
	static BM3_FORCEINLINE void Initialize(const ResourceType &res, ml::random &random, ml::vector3d &value, ml::vector3d &position){

//		if(res.Translate_Basic_TranslateType() == TranslateTypeConst_Constant)
		{
			value.random_range(random, res.Translate_Value_Initial(), res.Translate_Value_Range());
			value.add(position);
		}
	}

	template<typename ResourceType, typename UpdateInfo>
	static BM3_FORCEINLINE void Update(UpdateInfo &/*update_info*/, const ResourceType &/*res*/, ml::vector3d &/*value*/){

		// ‰½‚à‚µ‚È‚¢
		;
	}

	template<typename ResourceType>
	static BM3_FORCEINLINE void Prepare(const ResourceType & /*res*/, const ml::vector3d &value, ml::matrix44 &m){

//		if(res.Translate_Basic_TranslateType() == TranslateTypeConst_Constant)
		{
			m.mul_translate(value.e_.x_, value.e_.y_, value.e_.z_);
		}
	}
};

}	// namespace impl

class MatrixTranslate_Constant{

public:
	template<typename ResourceType>
	void BM3_FORCEINLINE Initialize(const ResourceType &res, ml::random &random, ml::vector3d &value){

		impl::MatrixTranslate_Constant::Initialize(res, random, value);
	}

	template<typename ResourceType>
	void BM3_FORCEINLINE Initialize(const ResourceType &res, ml::random &random, ml::vector3d &value ,ml::vector3d &position){

		impl::MatrixTranslate_Constant::Initialize(res, random, value ,position);
	}

	template<typename ResourceType, typename UpdateInfo>
	void BM3_FORCEINLINE Update(UpdateInfo &update_info, const ResourceType &res, ml::vector3d &value){

		impl::MatrixTranslate_Constant::Update(update_info, res, value);
	}

	template<typename ResourceType>
	void BM3_FORCEINLINE Prepare(const ResourceType &res, const ml::vector3d &value, ml::matrix44 &m){

		impl::MatrixTranslate_Constant::Prepare(res, value, m);
	}
};

}	// namespace policy
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_MATRIXTRASNRATE_CONSTANT_H
