#ifndef BM3_SDK_INC_BM3_CORE_PRIMITIVE_BM3_FIELD_COMMON_H
#define BM3_SDK_INC_BM3_CORE_PRIMITIVE_BM3_FIELD_COMMON_H

#include "../bm3_CoreType.h"
#include "../primitive/bm3_Primitive_Common.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

namespace field{

/// @brief PrimitiveParamクラス
struct PrimitiveParam{
	static float ScaleValue(prim::MatrixParam &param){	return param.value_.e_.x_;	}
	static float ScaleValue(prim::ScalarParam &param){	return param.value_;	}
};

/// @brief FieldInfoクラス
struct FieldInfo{
	ml::vector3d	position_;

	void Reset(){
		position_.set_unit();
	}

	void Derive(const FieldInfo &info){
		position_ = info.position_;
	}

	const ml::vector3d	Position() const{ return position_; }
	
};


}	// namespace field
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_PRIMITIVE_BM3_FIELD_COMMON_H
