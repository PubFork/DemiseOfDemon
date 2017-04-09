#ifndef BM3_SDK_INC_BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_TANGENTSPACE6_H
#define BM3_SDK_INC_BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_TANGENTSPACE6_H

#include "bm3/system/bm3_Config.h"
#include "bm3_draw_policy_Common.h"
#include "../../bm3_DrawInfo.h"
#include "../../bm3_CoreType.h"
#include <ml/type/ml_vector3d.h>
#include <ml/math/ml_triangular.h>

#include "bm3_draw_policy_TangentSpace4.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE
namespace draw{
namespace policy{

#define	BM3_TangentSpace6_TANGENTSPACE(INDEX)	(reinterpret_cast<DstTangentSpaceType *>(int(dst) + stride * (INDEX)))
#define	BM3_TangentSpace6_Store()	{				\
	*BM3_TangentSpace6_TANGENTSPACE(0) = temp[0];	\
	*BM3_TangentSpace6_TANGENTSPACE(1) = temp[1];	\
	*BM3_TangentSpace6_TANGENTSPACE(2) = temp[2];	\
	*BM3_TangentSpace6_TANGENTSPACE(3) = temp[0];	\
	*BM3_TangentSpace6_TANGENTSPACE(4) = temp[2];	\
	*BM3_TangentSpace6_TANGENTSPACE(5) = temp[3];	\
}

template<
	DrawRotate::Type	DrawRotateType
>
struct TangentSpace6{
	template<typename PrimitiveType, typename ResourceType, typename DstTangentSpaceType>
	BM3_FORCEINLINE void Push(DstTangentSpaceType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, const ml::matrix44 &matrix, int stride){

		ML_PTR_ASSERT(dst);

		TangentSpaceType	temp[4];
		TangentSpace4<DrawRotateType>().Push(temp, prim, draw_info, res, matrix, sizeof(TangentSpaceType));

		BM3_TangentSpace6_Store();
	}
};

#undef	BM3_TangentSpace6_Store
#undef	BM3_TangentSpace6_TANGENTSPACE

}	// namespace policy
}	// namespace draw
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_TANGENTSPACE6_H
