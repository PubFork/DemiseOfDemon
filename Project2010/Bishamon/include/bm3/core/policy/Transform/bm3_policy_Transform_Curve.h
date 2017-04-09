#ifndef BM3_SDK_INC_BM3_CORE_TRANSFORM_BM3_POLICY_TRANSFORM_CURVE_H
#define BM3_SDK_INC_BM3_CORE_TRANSFORM_BM3_POLICY_TRANSFORM_CURVE_H

#include "../../bm3_Const.h"
#include "../../bm3_CoreType.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

class DrawInfo;

namespace policy{

namespace impl{

/// @brief Transform_CurveÉNÉâÉX
class Transform_Curve{

public:
	template<typename ResourceType>
	static BM3_FORCEINLINE void	Initialize(const ResourceType &/*res*/, ml::random &/*random*/, ml::vector3d &/*left_top*/, ml::vector3d &/*right_top*/, ml::vector3d &/*left_bottom*/, ml::vector3d &/*right_bottom*/){

		// âΩÇ‡ÇµÇ»Ç¢
		;
	}

	template<typename ResourceType, typename UpdateInfo>
	static BM3_FORCEINLINE void	Update(UpdateInfo &/*update_info*/, const ResourceType &/*res*/, ml::vector3d &/*left_top*/, ml::vector3d &/*right_top*/, ml::vector3d &/*left_bottom*/, ml::vector3d &/*right_bottom*/){

		// âΩÇ‡ÇµÇ»Ç¢
		;
	}

	template<typename ResourceType>
	static BM3_FORCEINLINE void	Prepare(const ResourceType &res, ml::vector3d &left_top, ml::vector3d &right_top, ml::vector3d &left_bottom, ml::vector3d &right_bottom, const TransformCurveType &curve, const float life_rate, const unsigned int /*random_seed*/){

		if(res.Deformation_Basic_TransformType() == TransformTypeConst_Curve)	// è´óàìIÇ…Ç±ÇÃifï∂ÇÕÇ»Ç≠Ç»ÇÈ
		{
			ml::vector3d size = curve.Value(life_rate);
			float x = size.e_.x_;
			float y = size.e_.y_;

			left_top.set(		-x, +y, 0.0f, 1.0f);
			right_top.set(		+x, +y, 0.0f, 1.0f);
			left_bottom.set(	-x, -y, 0.0f, 1.0f);
			right_bottom.set(	+x, -y, 0.0f, 1.0f);
		}
	}
};

}	// namespace impl

class Transform_Curve{

public:
	template<typename ResourceType>
	BM3_FORCEINLINE void	Initialize(const ResourceType &res, ml::random &random, ml::vector3d &left_top, ml::vector3d &right_top, ml::vector3d &left_bottom, ml::vector3d &right_bottom){

		impl::Transform_Curve::Initialize(res, random, left_top, right_top, left_bottom, right_bottom);
	}

	template<typename ResourceType, typename UpdateInfo>
	BM3_FORCEINLINE void	Update(UpdateInfo &update_info, const ResourceType &res, ml::vector3d &left_top, ml::vector3d &right_top, ml::vector3d &left_bottom, ml::vector3d &right_bottom){

		impl::Transform_Curve::Update(update_info, res, left_top, right_top, left_bottom, right_bottom);
	}
};

}	// namespace policy
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_TRANSFORM_BM3_POLICY_TRANSFORM_CURVE_H
