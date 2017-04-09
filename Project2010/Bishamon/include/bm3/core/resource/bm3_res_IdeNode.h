// Ç∑Ç◊ÇƒÇì‡ïÔÇ∑ÇÈIDEópÉmÅ[Éh

#ifndef BM3_SDK_INC_BM3_CORE_RESOURCE_BM3_RES_IDENODE_H
#define BM3_SDK_INC_BM3_CORE_RESOURCE_BM3_RES_IDENODE_H

#include "bm3/system/bm3_Config.h"
#include "bm3/core/type/bm3_Curve.h"
#include "bm3/core/type/bm3_Texture.h"
#include "bm3/core/resource/policy/bm3_res_policy_DynamicChildArray.h"
#include "bm3/core/bm3_Const.h"
#include "bm3/core/bm3_CoreTypeSwitch.h"
#include <ml/type/ml_basic.h>
#include <ml/type/ml_vector3d.h>
#include <ml/type/ml_color.h>

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE
namespace res{

#if defined(BM3_TARGET_IDE)

class IdeNode{

public:
	typedef	DynamicChildArray<IdeNode>	ChildArrayType_;

	#include "../../utility/internal/bm3_MacroBegin.h"
		#include "bm3_res_IdeNode_DECLARE_SETTER_GETTER.inc"
		BM3_DECLARE_SETTER_GETTER_REF(ChildArrayType_,	ChildArray);
	#include "../../utility/internal/bm3_MacroEnd.h"

	IdeNode() :
		is_selected_(false){;}

	bool	IsSelected() const{		return is_selected_;	}
	void	SetIsSelected(bool i){	is_selected_ = i;		}

private:
	bool	is_selected_;
};

#endif	// #if defined(BM3_TARGET_IDE)

}	// namespace res
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_RESOURCE_BM3_RES_IDENODE_H
