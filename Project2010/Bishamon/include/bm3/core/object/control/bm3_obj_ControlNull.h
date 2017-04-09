#ifndef BM3_SDK_INC_BM3_CORE_OBJECT_CONTROL_BM3_OBJ_CONTROLNULL_H
#define BM3_SDK_INC_BM3_CORE_OBJECT_CONTROL_BM3_OBJ_CONTROLNULL_H

#include "bm3_obj_ControlBase.h"
#include "../../resource/bm3_res_Null.h"
#include <ml/type/ml_basic.h>
#include <ml/math/ml_random.h>

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

namespace obj{
namespace control{

template<typename ResourceType = res::Null>
class Null : public Base<ResourceType>{

public:
	typedef	Null			ThisType_;
	typedef	ResourceType	ResourceType_;
};

}	// namespace control
}	// namespace obj
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#include "bm3_obj_ControlNull_inl.h"

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_OBJECT_CONTROL_BM3_OBJ_CONTROLNULL_H
