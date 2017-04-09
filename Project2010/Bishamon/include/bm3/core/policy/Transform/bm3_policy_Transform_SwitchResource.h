#ifndef BM3_SDK_INC_BM3_CORE_TRANSFORM_BM3_POLICY_TRANSFORM_SWITCHRESOURCE_H
#define BM3_SDK_INC_BM3_CORE_TRANSFORM_BM3_POLICY_TRANSFORM_SWITCHRESOURCE_H

#include "../../bm3_Const.h"
#include "../../bm3_CoreType.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

class DrawInfo;

namespace policy{

namespace impl{

/// @brief Transform_SwitchResourceƒNƒ‰ƒX
class Transform_SwitchResource{

public:
	template<typename ResourceType>
	static BM3_FORCEINLINE void Switch(const ResourceType &res, const ResourceType & /*old_res*/, ml::random &random, ml::vector3d &left_top, ml::vector3d &right_top, ml::vector3d &left_bottom, ml::vector3d &right_bottom){
//		switch(res.Deformation_Basic_TransformType()){
//			case TransformTypeConst_Constant:
				switch(res.Deformation_Basic_TransformType()){
				case TransformTypeConst_Constant:	Transform_Constant::Initialize(	res, random, left_top, right_top, left_bottom, right_bottom);	break;
				case TransformTypeConst_Curve:		Transform_Curve::Initialize(	res, random, left_top, right_top, left_bottom, right_bottom);	break;
				case TransformTypeConst_Vertex:		Transform_Vertex::Initialize(	res, random, left_top, right_top, left_bottom, right_bottom);	break;
				case TransformTypeConst_MAX:
				case TransformTypeConst_FORCE32:
					break;
				}
/*				break;
			case TransformTypeConst_Curve:
				switch(res.Deformation_Basic_TransformType()){
				case TransformTypeConst_Constant:	Transform_Constant::Initialize(	res, random, left_top, right_top, left_bottom, right_bottom);	break;
				case TransformTypeConst_Curve:		Transform_Curve::Initialize(	res, random, left_top, right_top, left_bottom, right_bottom);	break;
				case TransformTypeConst_Vertex:		Transform_Vertex::Initialize(	res, random, left_top, right_top, left_bottom, right_bottom);	break;
				}
				break;
			case TransformTypeConst_Vertex:
				switch(res.Deformation_Basic_TransformType()){
				case TransformTypeConst_Constant:	Transform_Constant::Initialize(	res, random, left_top, right_top, left_bottom, right_bottom);	break;
				case TransformTypeConst_Curve:		Transform_Curve::Initialize(	res, random, left_top, right_top, left_bottom, right_bottom);	break;
				case TransformTypeConst_Vertex:		Transform_Vertex::Initialize(	res, random, left_top, right_top, left_bottom, right_bottom);	break;
				}
				break;
*/	
//		}
	}
};

}	// namespace impl


}	// namespace policy
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_TRANSFORM_BM3_POLICY_TRANSFORM_SWITCHRESOURCE_H
