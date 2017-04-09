#ifndef BM3_SDK_INC_BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_TANGENTSPACE2_H
#define BM3_SDK_INC_BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_TANGENTSPACE2_H

#include "bm3/system/bm3_Config.h"
#include "bm3_draw_policy_Common.h"
#include "../../bm3_DrawInfo.h"
#include "../../bm3_CoreType.h"
#include <ml/type/ml_vector3d.h>
#include <ml/math/ml_triangular.h>
#include <ml/math/ml_math_const.h>

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE
namespace draw{
namespace policy{

#define	BM3_DST(INDEX)	(reinterpret_cast<DstType *>(int(dst) + stride * (INDEX)))

template<
	DrawRotate::Type	DrawRotateType
>
struct TangentSpace2{
	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void Push(DstType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, const ml::matrix44 &matrix, int stride);
};

// DrawRotate::Billboard_Z
// DrawRotate::Polyline
// DrawRotate::XYZ

template<>
struct TangentSpace2<DrawRotate::Billboard_Z>{

	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void Push(DstType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &/*res*/, const ml::matrix44 &/*matrix*/, int stride){
		ML_PTR_ASSERT(dst);

		float sv0, cv0;
		float sv1, cv1;
		ml::fast::sin_cos(prim.rotate_.value_.e_.z_, sv0, cv0);
		sv1 = -cv0;
		cv1 = +sv0;

		ml::ni::vector3d v0(sv0, cv0, 0.0f, 1.0f);
		ml::ni::vector3d v1(sv1, cv1, 0.0f, 1.0f);

		v0.mul33(draw_info.InverseViewMatrix());
		v1.mul33(draw_info.InverseViewMatrix());

		BM3_DST(0)->tangent_.x_	 	= -v0.e_.x_;
		BM3_DST(0)->tangent_.y_	 	= -v0.e_.y_;
		BM3_DST(0)->tangent_.z_	 	= -v0.e_.z_;
		BM3_DST(0)->binormal_.x_	= -v1.e_.x_;
		BM3_DST(0)->binormal_.y_	= -v1.e_.y_;
		BM3_DST(0)->binormal_.z_	= -v1.e_.z_;
		BM3_DST(0)->normal_.x_		= -draw_info.InverseViewMatrix().e_.m20_;
		BM3_DST(0)->normal_.y_		= -draw_info.InverseViewMatrix().e_.m21_;
		BM3_DST(0)->normal_.z_		= -draw_info.InverseViewMatrix().e_.m22_;

		*BM3_DST(1) = *BM3_DST(0);
	}
};

template<>
struct TangentSpace2<DrawRotate::Polyline>{

	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void Push(DstType *dst, const PrimitiveType &prim, DrawInfo &/*draw_info*/, const ResourceType &/*res*/, const ml::matrix44 &matrix, int stride){

		// ‰¼

		ml::matrix44 m;
		{
			m.mul_rot_xyz(prim.rotate_.value_);
			m.mul33(matrix);
		}

		BM3_DST(0)->tangent_.x_	 	= m.e_.m00_;
		BM3_DST(0)->tangent_.y_	 	= m.e_.m01_;
		BM3_DST(0)->tangent_.z_	 	= m.e_.m02_;
		BM3_DST(0)->binormal_.x_	= m.e_.m10_;
		BM3_DST(0)->binormal_.y_	= m.e_.m11_;
		BM3_DST(0)->binormal_.z_	= m.e_.m12_;
		BM3_DST(0)->normal_.x_		= m.e_.m20_;
		BM3_DST(0)->normal_.y_		= m.e_.m21_;
		BM3_DST(0)->normal_.z_		= m.e_.m22_;

		*BM3_DST(1) = *BM3_DST(0);
	}
};

template<>
struct TangentSpace2<DrawRotate::XYZ>{

	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void Push(DstType *dst, const PrimitiveType &prim, DrawInfo &/*draw_info*/, const ResourceType &/*res*/, const ml::matrix44 &matrix, int stride){

		ml::matrix44 m;
		{
			m.mul_rot_xyz(prim.rotate_.value_);
			m.mul33(matrix);
		}

		BM3_DST(0)->tangent_.x_	 	= m.e_.m00_;
		BM3_DST(0)->tangent_.y_	 	= m.e_.m01_;
		BM3_DST(0)->tangent_.z_	 	= m.e_.m02_;
		BM3_DST(0)->binormal_.x_	= m.e_.m10_;
		BM3_DST(0)->binormal_.y_	= m.e_.m11_;
		BM3_DST(0)->binormal_.z_	= m.e_.m12_;
		BM3_DST(0)->normal_.x_		= m.e_.m20_;
		BM3_DST(0)->normal_.y_		= m.e_.m21_;
		BM3_DST(0)->normal_.z_		= m.e_.m22_;

		*BM3_DST(1) = *BM3_DST(0);
	}
};

template<>
struct TangentSpace2<DrawRotate::Directional>{

	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void Push(DstType *dst, const PrimitiveType &prim, DrawInfo &/*draw_info*/, const ResourceType &/*res*/, const ml::matrix44 &matrix, int stride){

		// ‰¼

		ml::matrix44 m;
		{
			m.mul_rot_xyz(prim.rotate_.value_);
			m.mul33(matrix);
		}

		BM3_DST(0)->tangent_.x_	 	= matrix.e_.m00_;
		BM3_DST(0)->tangent_.y_	 	= matrix.e_.m01_;
		BM3_DST(0)->tangent_.z_	 	= matrix.e_.m02_;
		BM3_DST(0)->binormal_.x_	= matrix.e_.m10_;
		BM3_DST(0)->binormal_.y_	= matrix.e_.m11_;
		BM3_DST(0)->binormal_.z_	= matrix.e_.m12_;
		BM3_DST(0)->normal_.x_		= matrix.e_.m20_;
		BM3_DST(0)->normal_.y_		= matrix.e_.m21_;
		BM3_DST(0)->normal_.z_		= matrix.e_.m22_;

		*BM3_DST(1) = *BM3_DST(0);
	}
};

template<>
struct TangentSpace2<DrawRotate::Facing>{

	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void Push(DstType *dst, const PrimitiveType &prim, DrawInfo &/*draw_info*/, const ResourceType &/*res*/, const ml::matrix44 &matrix, int stride){

		// ‰¼

		ml::matrix44 m;
		{
			m.mul_rot_xyz(prim.rotate_.value_);
			m.mul33(matrix);
		}

		BM3_DST(0)->tangent_.x_	 	= matrix.e_.m00_;
		BM3_DST(0)->tangent_.y_	 	= matrix.e_.m01_;
		BM3_DST(0)->tangent_.z_	 	= matrix.e_.m02_;
		BM3_DST(0)->binormal_.x_	= matrix.e_.m10_;
		BM3_DST(0)->binormal_.y_	= matrix.e_.m11_;
		BM3_DST(0)->binormal_.z_	= matrix.e_.m12_;
		BM3_DST(0)->normal_.x_		= matrix.e_.m20_;
		BM3_DST(0)->normal_.y_		= matrix.e_.m21_;
		BM3_DST(0)->normal_.z_		= matrix.e_.m22_;

		*BM3_DST(1) = *BM3_DST(0);
	}
};
	
#undef BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_TangentSpace2_MakeQuadEdgePos

#undef BM3_DST

}	// namespace policy
}	// namespace draw
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_TANGENTSPACE2_H
