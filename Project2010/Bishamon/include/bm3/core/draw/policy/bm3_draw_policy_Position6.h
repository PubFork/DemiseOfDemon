#ifndef BM3_SDK_INC_BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_POSITION6_H
#define BM3_SDK_INC_BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_POSITION6_H

#include "bm3/system/bm3_Config.h"
#include "bm3_draw_policy_Common.h"
#include "../../bm3_DrawInfo.h"
#include "../../bm3_CoreType.h"
#include <ml/type/ml_vector3d.h>
#include <ml/math/ml_triangular.h>

#include "bm3_draw_policy_Position4.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE
namespace draw{
namespace policy{

#define	BM3_DST(INDEX)	(reinterpret_cast<DstPositionType *>(int(dst) + stride * (INDEX)))

template<
	DrawScale::Type		DrawScaleType,
	DrawRotate::Type	DrawRotateType
>
struct Position6{
	template<typename PrimitiveType, typename ResourceType, typename DstPositionType>
	BM3_FORCEINLINE void Push(DstPositionType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const PacketDrawInfo &pdi, int stride){

		ML_PTR_ASSERT(dst);

		Position4<DrawScaleType, DrawRotateType>().Push(dst, prim, draw_info, res, matrix, matrix_row_length, pdi, stride);
		Set(dst, stride);
	}

	// BM2ŒÝŠ·
	template<typename PrimitiveType, typename ResourceType, typename DstPositionType>
	BM3_FORCEINLINE void PushBm2(DstPositionType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const PacketDrawInfo &pdi, int stride){

		ML_PTR_ASSERT(dst);

		Position4<DrawScaleType, DrawRotateType>().PushBm2(dst, prim, draw_info, res, matrix, matrix_row_length, pdi, stride);
		Set(dst, stride);
	}

	template<typename PrimitiveType, typename ResourceType, typename DstPositionType>
	BM3_FORCEINLINE void PushFixRotate(DstPositionType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const ml::vector3d vv0, const ml::vector3d &vv1, const PacketDrawInfo &pdi, int stride){

		ML_PTR_ASSERT(dst);

		Position4<DrawScaleType, DrawRotateType>().PushFixRotate(dst, prim, draw_info, res, matrix, matrix_row_length, vv0, vv1, pdi, stride);
		Set(dst, stride);
	}

	//BM2ŒÝŠ·
	template<typename PrimitiveType, typename ResourceType, typename DstPositionType>
	BM3_FORCEINLINE void PushFixRotateBm2(DstPositionType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const ml::vector3d vv0, const ml::vector3d &vv1, const PacketDrawInfo &pdi, int stride){

		ML_PTR_ASSERT(dst);

		Position4<DrawScaleType, DrawRotateType>().PushFixRotateBm2(dst, prim, draw_info, res, matrix, matrix_row_length, vv0, vv1, pdi, stride);
		Set(dst, stride);
	}

	template<typename PrimitiveType, typename ResourceType, typename DstPositionType>
	BM3_FORCEINLINE void PushFixRotateNoField(DstPositionType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const ml::vector3d vv0, const ml::vector3d &vv1, const PacketDrawInfo &pdi, int stride){

		ML_PTR_ASSERT(dst);

		Position4<DrawScaleType, DrawRotateType>().PushFixRotateNoField(dst, prim, draw_info, res, matrix, matrix_row_length, vv0, vv1, pdi, stride);
		Set(dst, stride);
	}

	//BM2ŒÝŠ·
	template<typename PrimitiveType, typename ResourceType, typename DstPositionType>
	BM3_FORCEINLINE void PushFixRotateNoFieldBm2(DstPositionType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const ml::vector3d vv0, const ml::vector3d &vv1, const PacketDrawInfo &pdi, int stride){

		ML_PTR_ASSERT(dst);

		Position4<DrawScaleType, DrawRotateType>().PushFixRotateNoFieldBm2(dst, prim, draw_info, res, matrix, matrix_row_length, vv0, vv1, pdi, stride);
		Set(dst, stride);
	}

	template<typename PrimitiveType, typename ResourceType, typename DstPositionType>
	BM3_FORCEINLINE void PushFixQuad(DstPositionType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const PacketDrawInfo &pdi, int stride){

		ML_PTR_ASSERT(dst);

		Position4<DrawScaleType, DrawRotateType>().PushFixQuad(dst, prim, draw_info, res, matrix, matrix_row_length, pdi, stride);
		Set(dst, stride);
	}

	//BM2ŒÝŠ·
	template<typename PrimitiveType, typename ResourceType, typename DstPositionType>
	BM3_FORCEINLINE void PushFixQuadBm2(DstPositionType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const PacketDrawInfo &pdi, int stride){

		ML_PTR_ASSERT(dst);

		Position4<DrawScaleType, DrawRotateType>().PushFixQuadBm2(dst, prim, draw_info, res, matrix, matrix_row_length, pdi, stride);
		Set(dst, stride);
	}

	template<typename PrimitiveType, typename ResourceType, typename DstPositionType>
	BM3_FORCEINLINE void PushNoField(DstPositionType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const PacketDrawInfo &pdi, int stride){

		ML_PTR_ASSERT(dst);

		Position4<DrawScaleType, DrawRotateType>().PushNoField(dst, prim, draw_info, res, matrix, matrix_row_length, pdi, stride);
		Set(dst, stride);
	}

	//BM2ŒÝŠ·
	template<typename PrimitiveType, typename ResourceType, typename DstPositionType>
	BM3_FORCEINLINE void PushNoFieldBm2(DstPositionType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const PacketDrawInfo &pdi, int stride){

		ML_PTR_ASSERT(dst);

		Position4<DrawScaleType, DrawRotateType>().PushNoFieldBm2(dst, prim, draw_info, res, matrix, matrix_row_length, pdi, stride);
		Set(dst, stride);
	}

	template<typename PrimitiveType, typename ResourceType, typename DstPositionType>
	BM3_FORCEINLINE void PushFixQuadNoField(DstPositionType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const PacketDrawInfo &pdi, int stride){

		ML_PTR_ASSERT(dst);

		Position4<DrawScaleType, DrawRotateType>().PushFixQuadNoField(dst, prim, draw_info, res, matrix, matrix_row_length, pdi, stride);
		Set(dst, stride);
	}
	
	//BM2ŒÝŠ·
	template<typename PrimitiveType, typename ResourceType, typename DstPositionType>
	BM3_FORCEINLINE void PushFixQuadNoFieldBm2(DstPositionType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const PacketDrawInfo &pdi, int stride){

		ML_PTR_ASSERT(dst);

		Position4<DrawScaleType, DrawRotateType>().PushFixQuadNoFieldBm2(dst, prim, draw_info, res, matrix, matrix_row_length, pdi, stride);
		Set(dst, stride);
	}

	template<typename DstPositionType>
	BM3_FORCEINLINE void Set(DstPositionType *dst, int stride){
		BM3_DST(4)->x_	= BM3_DST(VertexElement::PC0)->x_;
		BM3_DST(4)->y_	= BM3_DST(VertexElement::PC0)->y_;
		BM3_DST(4)->z_	= BM3_DST(VertexElement::PC0)->z_;
		BM3_DST(5)->x_	= BM3_DST(VertexElement::PC2)->x_;
		BM3_DST(5)->y_	= BM3_DST(VertexElement::PC2)->y_;
		BM3_DST(5)->z_	= BM3_DST(VertexElement::PC2)->z_;
	}
};

#undef	BM3_DST

}	// namespace policy
}	// namespace draw
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_POSITION6_H
