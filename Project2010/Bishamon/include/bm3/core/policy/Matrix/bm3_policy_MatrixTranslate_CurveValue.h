#ifndef BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_MATRIXTRANSLATE_CURVEVALUE_H
#define BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_MATRIXTRANSLATE_CURVEVALUE_H

#include "../../bm3_Const.h"
#include "../../bm3_CoreType.h"
#include <ml/type/ml_vector3d.h>

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

namespace policy{

namespace impl{

/// @brief MatrixTranslate_CurveValueƒNƒ‰ƒX
class MatrixTranslate_CurveValue{

public:
	template<typename ResourceType>
	static BM3_FORCEINLINE void Initialize(const ResourceType &res, ml::random &/*random*/, ml::vector3d &value, const PrimitiveInitializeInfo &primitive_initialize_info){

//		if(res.Translate_Basic_TranslateType() == TranslateTypeConst_CurveValue)
		{
			value = res.Translate_Value_Curve().Value(0.0f, primitive_initialize_info.random_seed_, res.Translate_Value_Range());
		}
	}

	template<typename ResourceType>
	static BM3_FORCEINLINE void Initialize(const ResourceType &res, ml::random &/*random*/, ml::vector3d &value, const PrimitiveInitializeInfo &primitive_initialize_info, ml::vector3d &position){

//		if(res.Translate_Basic_TranslateType() == TranslateTypeConst_CurveValue)
		{
			value = res.Translate_Value_Curve().Value(0.0f, primitive_initialize_info.random_seed_, res.Translate_Value_Range());
			value.add(position);
		}
	}

	template<typename ResourceType, typename UpdateInfo>
	static BM3_FORCEINLINE void Update(UpdateInfo &update_info, const ResourceType &res, ml::vector3d &value, const PrimitiveUpdateInfo &primitive_update_info){

//		if(res.Translate_Basic_TranslateType() == TranslateTypeConst_CurveValue)
		{
			value = res.Translate_Value_Curve().Value(primitive_update_info.life_rate_, primitive_update_info.random_seed_, res.Translate_Value_Range());
		}
	}

	template<typename ResourceType, typename UpdateInfo>
	static BM3_FORCEINLINE void Update(UpdateInfo &/*update_info*/, const ResourceType &res, ml::vector3d &value, const PrimitiveUpdateInfo &primitive_update_info ,ml::vector3d &position){

//		if(res.Translate_Basic_TranslateType() == TranslateTypeConst_CurveValue)
		{
			value = res.Translate_Value_Curve().Value(primitive_update_info.life_rate_, primitive_update_info.random_seed_, res.Translate_Value_Range());
			value.add(position);
		}
	}

	template<typename ResourceType>
	static BM3_FORCEINLINE void Prepare(const ResourceType & /*res*/, const ml::vector3d &value, ml::matrix44 &m, const PrimitivePrepareInfo &/*primitive_prepare_info*/){

//		if(res.Translate_Basic_TranslateType() == TranslateTypeConst_CurveValue)
		{
			m.mul_translate(value.e_.x_, value.e_.y_, value.e_.z_);
		}
	}
};

}	// namespace impl

class MatrixTranslate_CurveValue{

public:
	template<typename ResourceType>
	void BM3_FORCEINLINE Initialize(const ResourceType &res, ml::random &random, ml::vector3d &value, const PrimitiveInitializeInfo &primitive_initialize_info){

		impl::MatrixTranslate_CurveValue::Initialize(res, random, value, primitive_initialize_info);
	}

	template<typename ResourceType>
	void BM3_FORCEINLINE Initialize(const ResourceType &res, ml::random &random, ml::vector3d &value, const PrimitiveInitializeInfo &primitive_initialize_info, ml::vector3d &position){

		impl::MatrixTranslate_CurveValue::Initialize(res, random, value, primitive_initialize_info, position);
	}

	template<typename ResourceType, typename UpdateInfo>
	void BM3_FORCEINLINE Update(UpdateInfo &update_info, const ResourceType &res, ml::vector3d &value, const PrimitiveUpdateInfo &primitive_update_info){

		impl::MatrixTranslate_CurveValue::Update(update_info, res, value, primitive_update_info);
	}

	template<typename ResourceType, typename UpdateInfo>
	void BM3_FORCEINLINE Update(UpdateInfo &update_info, const ResourceType &res, ml::vector3d &value, const PrimitiveUpdateInfo &primitive_update_info, ml::vector3d &position){

		impl::MatrixTranslate_CurveValue::Update(update_info, res, value, primitive_update_info ,position);
	}

	template<typename ResourceType>
	void BM3_FORCEINLINE Prepare(const ResourceType &res, const ml::vector3d &value, ml::matrix44 &m, const PrimitivePrepareInfo &primitive_prepare_info){

		impl::MatrixTranslate_CurveValue::Prepare(res, value, m, primitive_prepare_info);
	}
};

}	// namespace policy
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_MATRIX_BM3_POLICY_MATRIXTRANSLATE_CURVEVALUE_H
