#ifndef BM3_SDK_INC_BM3_CORE_TRANSFORM_BM3_POLICY_TRANSFORM_VERTEX_H
#define BM3_SDK_INC_BM3_CORE_TRANSFORM_BM3_POLICY_TRANSFORM_VERTEX_H

#include "../../bm3_Const.h"
#include "../../bm3_CoreType.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

class DrawInfo;

namespace policy{

namespace impl{

/// @brief Transform_VertexÉNÉâÉX
class Transform_Vertex{

public:
	template<typename ResourceType>
	static BM3_FORCEINLINE void	Initialize(const ResourceType &res, ml::random &random, ml::vector3d &left_top, ml::vector3d &right_top, ml::vector3d &left_bottom, ml::vector3d &right_bottom){

		if(res.Deformation_Basic_TransformType() == TransformTypeConst_Vertex)	// è´óàìIÇ…Ç±ÇÃifï∂ÇÕÇ»Ç≠Ç»ÇÈ
		{
			left_top.random_range(		random, res.Deformation_VertexLeftTop_Value(),		res.Deformation_VertexLeftTop_Range());
			right_top.random_range(		random, res.Deformation_VertexRightTop_Value(),		res.Deformation_VertexRightTop_Range());
			left_bottom.random_range(	random, res.Deformation_VertexLeftBottom_Value(),	res.Deformation_VertexLeftBottom_Range());
			right_bottom.random_range(	random, res.Deformation_VertexRightBottom_Value(),	res.Deformation_VertexRightBottom_Range());
		}
	}

	template<typename ResourceType, typename UpdateInfo>
	static BM3_FORCEINLINE void	Update(UpdateInfo &/*update_info*/, const ResourceType &/*res*/, ml::vector3d &/*left_top*/, ml::vector3d &/*right_top*/, ml::vector3d &/*left_bottom*/, ml::vector3d &/*right_bottom*/){

		// âΩÇ‡ÇµÇ»Ç¢
		;
	}

	template<typename ResourceType>
	static void	Prepare(const ResourceType &/*res*/, ml::vector3d &/*left_top*/, ml::vector3d &/*right_top*/, ml::vector3d &/*left_bottom*/, ml::vector3d &/*right_bottom*/, const TransformCurveType &/*curve*/, const PrimitivePrepareInfo &/*ppi*/){

		// âΩÇ‡ÇµÇ»Ç¢
		;
	}
};

}	// namespace impl

class Transform_Vertex{

public:
	template<typename ResourceType>
	BM3_FORCEINLINE void	Initialize(const ResourceType &res, ml::random &random, ml::vector3d &left_top, ml::vector3d &right_top, ml::vector3d &left_bottom, ml::vector3d &right_bottom){

		impl::Transform_Vertex::Initialize(res, random, left_top, right_top, left_bottom, right_bottom);
	}

	template<typename ResourceType, typename UpdateInfo>
	BM3_FORCEINLINE void	Update(UpdateInfo &update_info, const ResourceType &res, ml::vector3d &left_top, ml::vector3d &right_top, ml::vector3d &left_bottom, ml::vector3d &right_bottom){

		impl::Transform_Vertex::Update(update_info, res, left_top, right_top, left_bottom, right_bottom);
	}
};

}	// namespace policy
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_TRANSFORM_BM3_POLICY_TRANSFORM_VERTEX_H
