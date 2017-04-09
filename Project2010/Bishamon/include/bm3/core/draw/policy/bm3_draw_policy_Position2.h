#ifndef BM3_SDK_INC_BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_POSITION2_H
#define BM3_SDK_INC_BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_POSITION2_H

#include "bm3/system/bm3_Config.h"
#include "bm3_draw_policy_Common.h"
#include "../../bm3_DrawInfo.h"
#include "../../bm3_CoreType.h"
#include <ml/type/ml_vector3d.h>
#include <ml/math/ml_triangular.h>

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE
namespace draw{
namespace policy{

#define	BM3_DST(INDEX)	(reinterpret_cast<DstPositionType *>(int(dst) + stride * (INDEX)))

namespace {
	template<typename DstPositionType>
	void SetVertex(DstPositionType *dst, ml::vector3d &base, ml::vector3d &offset, int stride){
		BM3_DST(0)->x_	= base.e_.x_ - offset.e_.x_;
		BM3_DST(0)->y_	= base.e_.y_ - offset.e_.y_;
		BM3_DST(0)->z_	= base.e_.z_ - offset.e_.z_;
		BM3_DST(1)->x_	= base.e_.x_ + offset.e_.x_;
		BM3_DST(1)->y_	= base.e_.y_ + offset.e_.y_;
		BM3_DST(1)->z_	= base.e_.z_ + offset.e_.z_;
	}
}

template<
	DrawScale::Type		DrawScaleType,
	DrawRotate::Type	DrawRotateType
>
struct Position2{
	template<typename PrimitiveType, typename ResourceType>
	BM3_FORCEINLINE void Push(ml::vector3d *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const PacketDrawInfo &pdi);

	template<typename PrimitiveType, typename ResourceType, typename DstPositionType>
	BM3_FORCEINLINE void Push(DstPositionType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, int stride);
};

template<>
struct Position2<DrawScale::X, DrawRotate::Billboard_Z>{

	template<typename PrimitiveType, typename ResourceType, typename DstPositionType>
	BM3_FORCEINLINE void Push(DstPositionType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType & /*res*/, const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const PacketDrawInfo & /*pdi*/, int stride){
		ML_PTR_ASSERT(dst);

		ml::ni::vector3d ax;
		ax.mul(draw_info.InverseViewMatrix().row(ml::matrix44::row::x), prim.scale_.value_.e_.x_ * matrix_row_length.e_.x_);
//		ax.mul(matrix);

		ml::ni::vector3d p;
		MakeCenterPosition(p, prim, matrix);

		SetVertex(dst, p, ax, stride);
	}

	//BM2å›ä∑
	template<typename PrimitiveType, typename ResourceType, typename DstPositionType>
	BM3_FORCEINLINE void PushBm2(DstPositionType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const PacketDrawInfo &pdi, int stride){
		Push(dst, prim, draw_info, res, matrix, matrix_row_length, pdi, stride);
	}

};

template<>
struct Position2<DrawScale::X, DrawRotate::XYZ>{

	template<typename PrimitiveType, typename ResourceType, typename DstPositionType>
	BM3_FORCEINLINE void Push(DstPositionType *dst, const PrimitiveType &prim, DrawInfo & /*draw_info*/, const ResourceType &res, const ml::matrix44 &matrix, const ml::vector3d & /*matrix_row_length*/, const PacketDrawInfo & /*pdi*/, int stride){
		ML_PTR_ASSERT(dst);

		ml::fast::matrix44	m;
		{
			m.set_scale(prim.scale_.value_);
			m.mul_rot_xyz(prim.rotate_.value_);
			m.mul33(matrix);
		}

		ml::ni::vector3d p;
		MakeCenterPosition(p, prim, matrix);

		// âÒì]ÇÃâeãøÇ≈éŒÇﬂÇ…èoÇƒÇµÇ‹Ç§ÇÃÇ≈ëºÇÃóvëfÇÕ0
//		ml::ni::vector3d ax(res.Deformation_Vertex_Value() ,0.0f ,0.0f ,1.0f);
//		ax.mul33(m);
		ml::ni::vector3d ax;
		ax.e_.x_	= res.Deformation_Vertex_Value() * m.m_[0][0];
		ax.e_.y_	= res.Deformation_Vertex_Value() * m.m_[0][1];
		ax.e_.z_	= res.Deformation_Vertex_Value() * m.m_[0][2];

		SetVertex(dst, p, ax, stride);
	}

	//BM2å›ä∑
	template<typename PrimitiveType, typename ResourceType, typename DstPositionType>
	BM3_FORCEINLINE void PushBm2(DstPositionType *dst, const PrimitiveType &prim, DrawInfo & /*draw_info*/, const ResourceType &res, const ml::matrix44 &matrix, const ml::vector3d & /*matrix_row_length*/, const PacketDrawInfo & /*pdi*/, int stride){
		ML_PTR_ASSERT(dst);

		ml::fast::matrix44	m;
		{
			m.set_scale(prim.scale_.value_);
			m.mul_rot_zxy(prim.rotate_.value_);
			m.mul33(matrix);
		}

		ml::ni::vector3d p;
		MakeCenterPosition(p, prim, matrix);

		// âÒì]ÇÃâeãøÇ≈éŒÇﬂÇ…èoÇƒÇµÇ‹Ç§ÇÃÇ≈ëºÇÃóvëfÇÕ0
//		ml::ni::vector3d ax(res.Deformation_Vertex_Value() ,0.0f ,0.0f ,1.0f);
//		ax.mul33(m);
		ml::ni::vector3d ax;
		ax.e_.x_	= res.Deformation_Vertex_Value() * m.m_[0][0];
		ax.e_.y_	= res.Deformation_Vertex_Value() * m.m_[0][1];
		ax.e_.z_	= res.Deformation_Vertex_Value() * m.m_[0][2];

		SetVertex(dst, p, ax, stride);
	}

};

template<>
struct Position2<DrawScale::X, DrawRotate::Facing>{

	template<typename PrimitiveType, typename ResourceType, typename DstPositionType>
	BM3_FORCEINLINE void Push(DstPositionType *dst, const PrimitiveType &prim, DrawInfo & /*draw_info*/, const ResourceType & /*res*/, const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const PacketDrawInfo & pdi, int stride){
		ML_PTR_ASSERT(dst);

		ml::ni::vector3d p;
		MakeCenterPosition(p, prim, matrix);
		
		ml::matrix44	m;
		{
#ifdef BM3_TARGET_IDE
			m.lookat_r(p, pdi.facing_position_, ml::vector3d(0,1,0,0));
#else
			m.lookat_r(pdi.facing_position_,	p, ml::vector3d(0,1,0,0));
#endif
			m.inverse();
			m.set_translate(0,0,0);
		}

		ml::ni::vector3d ax;
		ax.mul(m.row(ml::matrix44::row::x), prim.scale_.value_.e_.x_ * matrix_row_length.e_.x_);
//		ax.mul(matrix);
		
		SetVertex(dst, p, ax, stride);
	}

	//BM2å›ä∑
	template<typename PrimitiveType, typename ResourceType, typename DstPositionType>
	BM3_FORCEINLINE void PushBm2(DstPositionType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const PacketDrawInfo &pdi, int stride){
		Push(dst, prim, draw_info, res, matrix, matrix_row_length, pdi, stride);
	}
};	

#undef	BM3_Position2_Store
#undef	BM3_Position2_POSITION
#undef BM3_DST

}	// namespace policy
}	// namespace draw
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_POSITION2_H
