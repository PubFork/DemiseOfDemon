#ifndef BM3_SDK_INC_BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_POSITION4_H
#define BM3_SDK_INC_BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_POSITION4_H

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
	DrawScale::Type		DrawScaleType,
	DrawRotate::Type	DrawRotateType
>
struct Position4{
	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void Push(DstType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const PacketDrawInfo & /*pdi*/, int stride = sizeof(DstType));
};

// DrawScale::Nothing, DrawRotate::Billboard_Z => パーティクル
// DrawScale::XY,      DrawRotate::Billboard_Z => クアッド
// DrawScale::XY,      DrawRotate::Polyline    => クアッド
// DrawScale::XY,      DrawRotate::XYZ         => クアッド

template<>
struct Position4<DrawScale::Nothing, DrawRotate::Billboard_Z>{

	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void Push(DstType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &/*res*/, const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const PacketDrawInfo & /*pdi*/, int stride = sizeof(DstType)){
		ML_PTR_ASSERT(dst);

		ML_PTR_ASSERT(dst);

		ml::ni::vector3d v0, v1, p;
		MakeVertex(v0, v1, draw_info, matrix_row_length, prim);
		MakeCenterPosition(p, prim, matrix);

		Set(dst, p, v0, v1, v0, v1, stride);
	}

	//BM2互換
	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void PushBm2(DstType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const PacketDrawInfo &pdi, int stride = sizeof(DstType)){
		Push(dst, prim, draw_info, res, matrix, matrix_row_length, pdi, stride);
	}

	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void PushFixRotate(DstType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &/*res*/, const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const ml::vector3d &vv0, const ml::vector3d &vv1, const PacketDrawInfo & /*pdi*/, int stride = sizeof(DstType)){

		ML_PTR_ASSERT(dst);

		ml::ni::vector3d v0, v1, p;
		MakeVertex(v0, v1, draw_info, matrix_row_length, prim, vv0, vv1);
		MakeCenterPosition(p, prim, matrix);

		Set(dst, p, v0, v1, v0, v1, stride);
	}

	//BM2互換
	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void PushFixRotateBm2(DstType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const ml::vector3d &vv0, const ml::vector3d &vv1, const PacketDrawInfo &pdi, int stride = sizeof(DstType)){
		PushFixRotate(dst, prim, draw_info, res, matrix, matrix_row_length, vv0, vv1, pdi, stride);
	}


	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void PushNoField(DstType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &/*res*/, const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const PacketDrawInfo & /*pdi*/, int stride = sizeof(DstType)){

		ML_PTR_ASSERT(dst);

		ml::ni::vector3d v0, v1, p;
		MakeVertex(v0, v1, draw_info, matrix_row_length, prim);
		MakeCenterPositionNoField(p, prim, matrix);

		Set(dst, p, v0, v1, v0, v1, stride);
	}

	//BM2互換
	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void PushNoFieldBm2(DstType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const PacketDrawInfo &pdi, int stride = sizeof(DstType)){
		PushNoField(dst, prim, draw_info, res, matrix, matrix_row_length, pdi, stride);
	}

	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void PushFixRotateNoField(DstType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &/*res*/, const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const ml::vector3d &vv0, const ml::vector3d &vv1, const PacketDrawInfo & /*pdi*/, int stride = sizeof(DstType)){

		ML_PTR_ASSERT(dst);

		ml::ni::vector3d v0, v1, p;
		MakeVertex(v0, v1, draw_info, matrix_row_length, prim, vv0, vv1);
		MakeCenterPositionNoField(p, prim, matrix);

		Set(dst, p, v0, v1, v0, v1, stride);
	}

	//BM2互換
	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void PushFixRotateNoFieldBm2(DstType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const ml::vector3d &vv0, const ml::vector3d &vv1, const PacketDrawInfo &pdi, int stride = sizeof(DstType)){
		PushFixRotateNoField(dst, prim, draw_info, res, matrix, matrix_row_length, vv0, vv1, pdi, stride);
	}

private:
	template<typename PrimitiveType>
	BM3_FORCEINLINE void MakeVertex(ml::vector3d &v0, ml::vector3d &v1, DrawInfo &draw_info, const ml::vector3d &matrix_row_length, const PrimitiveType &prim){
		float sv0, cv0;
		float sv1, cv1;

		ml::fast::sin_cos( - prim.rotate_.ValueZ() - ml::degree45, sv0, cv0);
		sv1 = -cv0;	cv1 = +sv0;

		float	s = matrix_row_length.e_.x_ * prim.scale_.ValueX();
		v0.e_.x_	= sv0 * s;		v0.e_.y_	= cv0 * s;	v0.e_.z_	= 0.0f;
		v1.e_.x_	= sv1 * s;		v1.e_.y_	= cv1 * s;	v1.e_.z_	= 0.0f;
		v0.mul33xy(draw_info.InverseViewMatrix());
		v1.mul33xy(draw_info.InverseViewMatrix());
	}

	template<typename PrimitiveType>
	BM3_FORCEINLINE void MakeVertex(ml::vector3d &v0, ml::vector3d &v1, DrawInfo & /*draw_info*/, const ml::vector3d &matrix_row_length, const PrimitiveType &prim, const ml::vector3d &vv0, const ml::vector3d &vv1){

		float	s = matrix_row_length.e_.x_ * prim.scale_.ValueX();
		v0.e_.x_	= vv0.e_.x_ * s;		v0.e_.y_	= vv0.e_.y_ * s;	v0.e_.z_	= vv0.e_.z_ * s;
		v1.e_.x_	= vv1.e_.x_ * s;		v1.e_.y_	= vv1.e_.y_ * s;	v1.e_.z_	= vv1.e_.z_ * s;
//		v0.mul33xy(draw_info.InverseViewMatrix());
//		v1.mul33xy(draw_info.InverseViewMatrix());
	}

	template<typename DstType>
	BM3_FORCEINLINE void Set(DstType *dst, ml::ni::vector3d &base, ml::ni::vector3d &left_top, ml::ni::vector3d &left_bottom, ml::ni::vector3d &right_bottom, ml::ni::vector3d &right_top, int stride){

		BM3_DST(VertexElement::PC0)->x_	= base.e_.x_ + left_top.e_.x_;
		BM3_DST(VertexElement::PC0)->y_	= base.e_.y_ + left_top.e_.y_;
		BM3_DST(VertexElement::PC0)->z_	= base.e_.z_ + left_top.e_.z_;
		BM3_DST(VertexElement::PC1)->x_	= base.e_.x_ + left_bottom.e_.x_;
		BM3_DST(VertexElement::PC1)->y_	= base.e_.y_ + left_bottom.e_.y_;
		BM3_DST(VertexElement::PC1)->z_	= base.e_.z_ + left_bottom.e_.z_;
		BM3_DST(VertexElement::PC2)->x_	= base.e_.x_ - right_bottom.e_.x_;
		BM3_DST(VertexElement::PC2)->y_	= base.e_.y_ - right_bottom.e_.y_;
		BM3_DST(VertexElement::PC2)->z_	= base.e_.z_ - right_bottom.e_.z_;
		BM3_DST(VertexElement::PC3)->x_	= base.e_.x_ - right_top.e_.x_;
		BM3_DST(VertexElement::PC3)->y_	= base.e_.y_ - right_top.e_.y_;
		BM3_DST(VertexElement::PC3)->z_	= base.e_.z_ - right_top.e_.z_;
	}
};

// ４角形の1辺のそれぞれの頂点を求める。４角形中心からの位置
#define	BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_POSITION4_MakeQuadEdgePos(LOCAL_MATRIX, PARENT_SCALE)	\
	ml::ni::vector3d left_top;																		\
	ml::ni::vector3d right_top;																		\
	ml::ni::vector3d left_bottom;																	\
	ml::ni::vector3d right_bottom;																	\
	{																								\
		ML_STATIC_ASSERT(ScaleParentNodeInfluenceTypeConst_Off				== 0);					\
		ML_STATIC_ASSERT(ScaleParentNodeInfluenceTypeConst_On				== 1);					\
		ML_STATIC_ASSERT(ScaleParentNodeInfluenceTypeConst_OnGenerateTime	== 2);					\
		ML_STATIC_ASSERT(ScaleParentNodeInfluenceTypeConst_MAX				== 3);					\
																									\
		switch(res.Basic_ParentNodeInfluence_ScaleParentNodeInfluenceType()){						\
			case ScaleParentNodeInfluenceTypeConst_Off:{											\
				left_top		= prim.transform_.left_top_;										\
				right_top		= prim.transform_.right_top_;										\
				left_bottom		= prim.transform_.left_bottom_;										\
				right_bottom	= prim.transform_.right_bottom_;									\
				if(false==res.Deformation_Basic_CenterPosition().is_unit()){						\
					left_top.add(res.Deformation_Basic_CenterPosition());							\
					right_top.add(res.Deformation_Basic_CenterPosition());							\
					left_bottom.add(res.Deformation_Basic_CenterPosition());						\
					right_bottom.add(res.Deformation_Basic_CenterPosition());						\
				}																					\
				break;																				\
			}																						\
																									\
			case ScaleParentNodeInfluenceTypeConst_On:												\
			case ScaleParentNodeInfluenceTypeConst_OnGenerateTime:{									\
				left_top.mul(		prim.transform_.left_top_,		PARENT_SCALE);					\
				right_top.mul(		prim.transform_.right_top_,		PARENT_SCALE);					\
				left_bottom.mul(	prim.transform_.left_bottom_,	PARENT_SCALE);					\
				right_bottom.mul(	prim.transform_.right_bottom_,	PARENT_SCALE);					\
				if(false==res.Deformation_Basic_CenterPosition().is_unit()){						\
					ml::vector3d	pivot;															\
					pivot.mul(res.Deformation_Basic_CenterPosition(), PARENT_SCALE);				\
					left_top.add(pivot);															\
					right_top.add(pivot);															\
					left_bottom.add(pivot);															\
					right_bottom.add(pivot);														\
				}																					\
				break;																				\
			}																						\
																									\
			default:{																				\
				ML_ASSERT(false);																	\
				BM3_ASSUME(0);																		\
				break;																				\
			}																						\
		}																							\
																									\
																									\
		left_top.mul33(LOCAL_MATRIX);																\
		right_top.mul33(LOCAL_MATRIX);																\
		left_bottom.mul33(LOCAL_MATRIX);															\
		right_bottom.mul33(LOCAL_MATRIX);															\
	}

// ４角形の1辺のそれぞれの頂点を求める。４角形中心からの位置
// BM2互換版
#define	BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_POSITION4_MakeQuadEdgePos_ForBM2(LOCAL_MATRIX, PARENT_MATRIX)	\
	ml::ni::vector3d left_top;																		\
	ml::ni::vector3d right_top;																		\
	ml::ni::vector3d left_bottom;																	\
	ml::ni::vector3d right_bottom;																	\
	{																								\
		ML_STATIC_ASSERT(ScaleParentNodeInfluenceTypeConst_Off				== 0);					\
		ML_STATIC_ASSERT(ScaleParentNodeInfluenceTypeConst_On				== 1);					\
		ML_STATIC_ASSERT(ScaleParentNodeInfluenceTypeConst_OnGenerateTime	== 2);					\
		ML_STATIC_ASSERT(ScaleParentNodeInfluenceTypeConst_MAX				== 3);					\
																									\
		switch(res.Basic_ParentNodeInfluence_ScaleParentNodeInfluenceType()){						\
			case ScaleParentNodeInfluenceTypeConst_Off:{											\
				left_top		= prim.transform_.left_top_;										\
				right_top		= prim.transform_.right_top_;										\
				left_bottom		= prim.transform_.left_bottom_;										\
				right_bottom	= prim.transform_.right_bottom_;									\
				if(false==res.Deformation_Basic_CenterPosition().is_unit()){						\
					left_top.add(res.Deformation_Basic_CenterPosition());							\
					right_top.add(res.Deformation_Basic_CenterPosition());							\
					left_bottom.add(res.Deformation_Basic_CenterPosition());						\
					right_bottom.add(res.Deformation_Basic_CenterPosition());						\
				}																					\
				left_top.mul33(LOCAL_MATRIX);														\
				right_top.mul33(LOCAL_MATRIX);														\
				left_bottom.mul33(LOCAL_MATRIX);													\
				right_bottom.mul33(LOCAL_MATRIX);													\
				break;																				\
			}																						\
																									\
			case ScaleParentNodeInfluenceTypeConst_On:												\
			case ScaleParentNodeInfluenceTypeConst_OnGenerateTime:{									\
				left_top		= prim.transform_.left_top_;										\
				right_top		= prim.transform_.right_top_;										\
				left_bottom		= prim.transform_.left_bottom_;										\
				right_bottom	= prim.transform_.right_bottom_;									\
				if(false==res.Deformation_Basic_CenterPosition().is_unit()){						\
					left_top.add(res.Deformation_Basic_CenterPosition());							\
					right_top.add(res.Deformation_Basic_CenterPosition());							\
					left_bottom.add(res.Deformation_Basic_CenterPosition());						\
					right_bottom.add(res.Deformation_Basic_CenterPosition());						\
				}																					\
				left_top.mul33(LOCAL_MATRIX);														\
				right_top.mul33(LOCAL_MATRIX);														\
				left_bottom.mul33(LOCAL_MATRIX);													\
				right_bottom.mul33(LOCAL_MATRIX);													\
				left_top.mul33(PARENT_MATRIX);														\
				right_top.mul33(PARENT_MATRIX);														\
				left_bottom.mul33(PARENT_MATRIX);													\
				right_bottom.mul33(PARENT_MATRIX);													\
																									\
				break;																				\
			}																						\
																									\
			default:{																				\
				ML_ASSERT(false);																	\
				BM3_ASSUME(0);																		\
				break;																				\
			}																						\
		}																							\
																									\
																									\
	}

// ４角形の1辺のそれぞれの頂点を求める。４角形中心からの位置
// BM2互換版 親の回転は無視する
#define	BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_POSITION4_MakeQuadEdgePos_ForBM2_ParentScall(LOCAL_MATRIX, PARENT_SCALE)	\
	ml::ni::vector3d left_top;																		\
	ml::ni::vector3d right_top;																		\
	ml::ni::vector3d left_bottom;																	\
	ml::ni::vector3d right_bottom;																	\
	{																								\
		ML_STATIC_ASSERT(ScaleParentNodeInfluenceTypeConst_Off				== 0);					\
		ML_STATIC_ASSERT(ScaleParentNodeInfluenceTypeConst_On				== 1);					\
		ML_STATIC_ASSERT(ScaleParentNodeInfluenceTypeConst_OnGenerateTime	== 2);					\
		ML_STATIC_ASSERT(ScaleParentNodeInfluenceTypeConst_MAX				== 3);					\
																									\
		switch(res.Basic_ParentNodeInfluence_ScaleParentNodeInfluenceType()){						\
			case ScaleParentNodeInfluenceTypeConst_Off:{											\
				left_top		= prim.transform_.left_top_;										\
				right_top		= prim.transform_.right_top_;										\
				left_bottom		= prim.transform_.left_bottom_;										\
				right_bottom	= prim.transform_.right_bottom_;									\
				if(false==res.Deformation_Basic_CenterPosition().is_unit()){						\
					left_top.add(res.Deformation_Basic_CenterPosition());							\
					right_top.add(res.Deformation_Basic_CenterPosition());							\
					left_bottom.add(res.Deformation_Basic_CenterPosition());						\
					right_bottom.add(res.Deformation_Basic_CenterPosition());						\
				}																					\
				left_top.mul33(LOCAL_MATRIX);														\
				right_top.mul33(LOCAL_MATRIX);														\
				left_bottom.mul33(LOCAL_MATRIX);													\
				right_bottom.mul33(LOCAL_MATRIX);													\
				break;																				\
			}																						\
																									\
			case ScaleParentNodeInfluenceTypeConst_On:												\
			case ScaleParentNodeInfluenceTypeConst_OnGenerateTime:{									\
				left_top		= prim.transform_.left_top_;										\
				right_top		= prim.transform_.right_top_;										\
				left_bottom		= prim.transform_.left_bottom_;										\
				right_bottom	= prim.transform_.right_bottom_;									\
				if(false==res.Deformation_Basic_CenterPosition().is_unit()){						\
					left_top.add(res.Deformation_Basic_CenterPosition());							\
					right_top.add(res.Deformation_Basic_CenterPosition());							\
					left_bottom.add(res.Deformation_Basic_CenterPosition());						\
					right_bottom.add(res.Deformation_Basic_CenterPosition());						\
				}																					\
				left_top.mul33(LOCAL_MATRIX);														\
				right_top.mul33(LOCAL_MATRIX);														\
				left_bottom.mul33(LOCAL_MATRIX);													\
				right_bottom.mul33(LOCAL_MATRIX);													\
				left_top.mul(PARENT_SCALE);														\
				right_top.mul(PARENT_SCALE);														\
				left_bottom.mul(PARENT_SCALE);													\
				right_bottom.mul(PARENT_SCALE);													\
																									\
				break;																				\
			}																						\
																									\
			default:{																				\
				ML_ASSERT(false);																	\
				BM3_ASSUME(0);																		\
				break;																				\
			}																						\
		}																							\
																									\
																									\
	}

#define	BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_POSITION2_MakeQuadEdgePos(LOCAL_MATRIX, PARENT_SCALE)	\
	ml::ni::vector3d left_top;																		\
	ml::ni::vector3d right_top;																		\
	{																								\
		ML_STATIC_ASSERT(ScaleParentNodeInfluenceTypeConst_Off				== 0);					\
		ML_STATIC_ASSERT(ScaleParentNodeInfluenceTypeConst_On				== 1);					\
		ML_STATIC_ASSERT(ScaleParentNodeInfluenceTypeConst_OnGenerateTime	== 2);					\
		ML_STATIC_ASSERT(ScaleParentNodeInfluenceTypeConst_MAX				== 3);					\
																									\
		switch(res.Basic_ParentNodeInfluence_ScaleParentNodeInfluenceType()){						\
			case ScaleParentNodeInfluenceTypeConst_Off:{											\
				left_top		= prim.transform_.left_top_;										\
				right_top		= prim.transform_.right_top_;										\
				break;																				\
			}																						\
																									\
			case ScaleParentNodeInfluenceTypeConst_On:												\
			case ScaleParentNodeInfluenceTypeConst_OnGenerateTime:{									\
				left_top.mul(		prim.transform_.left_top_,		PARENT_SCALE);					\
				right_top.mul(		prim.transform_.right_top_,		PARENT_SCALE);					\
				break;																				\
			}																						\
																									\
			default:{																				\
				ML_ASSERT(false);																	\
				BM3_ASSUME(0);																		\
				break;																				\
			}																						\
		}																							\
																									\
		left_top.mul33(LOCAL_MATRIX);																\
		right_top.mul33(LOCAL_MATRIX);																\
	}

// BM2互換版
#define	BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_POSITION2_MakeQuadEdgePos_ForBM2(LOCAL_MATRIX, PARENT_MATRIX)	\
	ml::ni::vector3d left_top;																		\
	ml::ni::vector3d right_top;																		\
	{																								\
		ML_STATIC_ASSERT(ScaleParentNodeInfluenceTypeConst_Off				== 0);					\
		ML_STATIC_ASSERT(ScaleParentNodeInfluenceTypeConst_On				== 1);					\
		ML_STATIC_ASSERT(ScaleParentNodeInfluenceTypeConst_OnGenerateTime	== 2);					\
		ML_STATIC_ASSERT(ScaleParentNodeInfluenceTypeConst_MAX				== 3);					\
																									\
		switch(res.Basic_ParentNodeInfluence_ScaleParentNodeInfluenceType()){						\
			case ScaleParentNodeInfluenceTypeConst_Off:{											\
				left_top		= prim.transform_.left_top_;										\
				right_top		= prim.transform_.right_top_;										\
				left_top.mul33(LOCAL_MATRIX);														\
				right_top.mul33(LOCAL_MATRIX);														\
				break;																				\
			}																						\
																									\
			case ScaleParentNodeInfluenceTypeConst_On:												\
			case ScaleParentNodeInfluenceTypeConst_OnGenerateTime:{									\
				left_top		= prim.transform_.left_top_;										\
				right_top		= prim.transform_.right_top_;										\
				left_top.mul33(LOCAL_MATRIX);														\
				right_top.mul33(LOCAL_MATRIX);														\
				left_top.mul33(PARENT_MATRIX);														\
				right_top.mul33(PARENT_MATRIX);														\
				break;																				\
			}																						\
																									\
			default:{																				\
				ML_ASSERT(false);																	\
				BM3_ASSUME(0);																		\
				break;																				\
			}																						\
		}																							\
																									\
	}

// BM2互換版 親の回転は無視する
#define	BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_POSITION2_MakeQuadEdgePos_ForBM2_ParentScall(LOCAL_MATRIX, PARENT_SCALE)	\
	ml::ni::vector3d left_top;																		\
	ml::ni::vector3d right_top;																		\
	{																								\
		ML_STATIC_ASSERT(ScaleParentNodeInfluenceTypeConst_Off				== 0);					\
		ML_STATIC_ASSERT(ScaleParentNodeInfluenceTypeConst_On				== 1);					\
		ML_STATIC_ASSERT(ScaleParentNodeInfluenceTypeConst_OnGenerateTime	== 2);					\
		ML_STATIC_ASSERT(ScaleParentNodeInfluenceTypeConst_MAX				== 3);					\
																									\
		switch(res.Basic_ParentNodeInfluence_ScaleParentNodeInfluenceType()){						\
			case ScaleParentNodeInfluenceTypeConst_Off:{											\
				left_top		= prim.transform_.left_top_;										\
				right_top		= prim.transform_.right_top_;										\
				left_top.mul33(LOCAL_MATRIX);														\
				right_top.mul33(LOCAL_MATRIX);														\
				break;																				\
			}																						\
																									\
			case ScaleParentNodeInfluenceTypeConst_On:												\
			case ScaleParentNodeInfluenceTypeConst_OnGenerateTime:{									\
				left_top		= prim.transform_.left_top_;										\
				right_top		= prim.transform_.right_top_;										\
				left_top.mul33(LOCAL_MATRIX);														\
				right_top.mul33(LOCAL_MATRIX);														\
				left_top.mul(PARENT_SCALE);														\
				right_top.mul(PARENT_SCALE);														\
				break;																				\
			}																						\
																									\
			default:{																				\
				ML_ASSERT(false);																	\
				BM3_ASSUME(0);																		\
				break;																				\
			}																						\
		}																							\
																									\
	}

#define	BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_POSITION4_MakeQuadEdgePos_OPT(LOCAL_MATRIX, PARENT_SCALE)	\
	ml::ni::vector3d left_top;																		\
	ml::ni::vector3d right_top;																		\
	ml::ni::vector3d left_bottom;																	\
	ml::ni::vector3d right_bottom;																	\
	{																								\
		ML_STATIC_ASSERT(ScaleParentNodeInfluenceTypeConst_Off				== 0);					\
		ML_STATIC_ASSERT(ScaleParentNodeInfluenceTypeConst_On				== 1);					\
		ML_STATIC_ASSERT(ScaleParentNodeInfluenceTypeConst_OnGenerateTime	== 2);					\
		ML_STATIC_ASSERT(ScaleParentNodeInfluenceTypeConst_MAX				== 3);					\
																									\
		ml::vector3d	pivot;																		\
		switch(/*res.Basic_ParentNodeInfluence_ScaleParentNodeInfluenceType()*/ spnit){				\
			case ScaleParentNodeInfluenceTypeConst_Off:{											\
				left_top		= prim.transform_.left_top_;										\
				right_top		= prim.transform_.right_top_;										\
				left_bottom		= prim.transform_.left_bottom_;										\
				right_bottom	= prim.transform_.right_bottom_;									\
				pivot			= deformation_basic_centerposition;									\
				break;																				\
			}																						\
																									\
			case ScaleParentNodeInfluenceTypeConst_On:{												\
				left_top.mul(		prim.transform_.left_top_,		PARENT_SCALE);					\
				right_top.mul(		prim.transform_.right_top_,		PARENT_SCALE);					\
				left_bottom.mul(	prim.transform_.left_bottom_,	PARENT_SCALE);					\
				right_bottom.mul(	prim.transform_.right_bottom_,	PARENT_SCALE);					\
				pivot.mul(deformation_basic_centerposition, PARENT_SCALE);							\
				break;																				\
			}																						\
																									\
			case ScaleParentNodeInfluenceTypeConst_OnGenerateTime:{									\
				left_top.mul(		prim.transform_.left_top_,		PARENT_SCALE);					\
				right_top.mul(		prim.transform_.right_top_,		PARENT_SCALE);					\
				left_bottom.mul(	prim.transform_.left_bottom_,	PARENT_SCALE);					\
				right_bottom.mul(	prim.transform_.right_bottom_,	PARENT_SCALE);					\
				pivot.mul(deformation_basic_centerposition, PARENT_SCALE);							\
				break;																				\
			}																						\
																									\
			default:{																				\
				ML_ASSERT(false);																	\
				BM3_ASSUME(0);																		\
				break;																				\
			}																						\
		}																							\
																									\
		left_top.add(pivot);																		\
		right_top.add(pivot);																		\
		left_bottom.add(pivot);																		\
		right_bottom.add(pivot);																	\
																									\
		left_top.mul33(LOCAL_MATRIX);																\
		right_top.mul33(LOCAL_MATRIX);																\
		left_bottom.mul33(LOCAL_MATRIX);															\
		right_bottom.mul33(LOCAL_MATRIX);															\
	}

// BM2互換版
#define	BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_POSITION4_MakeQuadEdgePos_OPT_ForBM2(LOCAL_MATRIX, PARENT_MATRIX)	\
	ml::ni::vector3d left_top;																		\
	ml::ni::vector3d right_top;																		\
	ml::ni::vector3d left_bottom;																	\
	ml::ni::vector3d right_bottom;																	\
	{																								\
		ML_STATIC_ASSERT(ScaleParentNodeInfluenceTypeConst_Off				== 0);					\
		ML_STATIC_ASSERT(ScaleParentNodeInfluenceTypeConst_On				== 1);					\
		ML_STATIC_ASSERT(ScaleParentNodeInfluenceTypeConst_OnGenerateTime	== 2);					\
		ML_STATIC_ASSERT(ScaleParentNodeInfluenceTypeConst_MAX				== 3);					\
																									\
		ml::vector3d	pivot;																		\
		switch(/*res.Basic_ParentNodeInfluence_ScaleParentNodeInfluenceType()*/ spnit){				\
			case ScaleParentNodeInfluenceTypeConst_Off:{											\
				left_top		= prim.transform_.left_top_;										\
				right_top		= prim.transform_.right_top_;										\
				left_bottom		= prim.transform_.left_bottom_;										\
				right_bottom	= prim.transform_.right_bottom_;									\
				pivot			= deformation_basic_centerposition;									\
				left_top.add(pivot);																\
				right_top.add(pivot);																\
				left_bottom.add(pivot);																\
				right_bottom.add(pivot);															\
																									\
				left_top.mul33(LOCAL_MATRIX);														\
				right_top.mul33(LOCAL_MATRIX);														\
				left_bottom.mul33(LOCAL_MATRIX);													\
				right_bottom.mul33(LOCAL_MATRIX);													\
				break;																				\
			}																						\
																									\
			case ScaleParentNodeInfluenceTypeConst_On:												\
			case ScaleParentNodeInfluenceTypeConst_OnGenerateTime:{									\
				left_top		= prim.transform_.left_top_;										\
				right_top		= prim.transform_.right_top_;										\
				left_bottom		= prim.transform_.left_bottom_;										\
				right_bottom	= prim.transform_.right_bottom_;									\
				pivot			= deformation_basic_centerposition;									\
				left_top.add(pivot);																\
				right_top.add(pivot);																\
				left_bottom.add(pivot);																\
				right_bottom.add(pivot);															\
				left_top.mul33(LOCAL_MATRIX);														\
				right_top.mul33(LOCAL_MATRIX);														\
				left_bottom.mul33(LOCAL_MATRIX);													\
				right_bottom.mul33(LOCAL_MATRIX);													\
				left_top.mul33(PARENT_MATRIX);														\
				right_top.mul33(PARENT_MATRIX);														\
				left_bottom.mul33(PARENT_MATRIX);													\
				right_bottom.mul33(PARENT_MATRIX);													\
																									\
				break;																				\
			}																						\
																									\
			default:{																				\
				ML_ASSERT(false);																	\
				BM3_ASSUME(0);																		\
				break;																				\
			}																						\
		}																							\
																									\
	}
// BM2互換版 親の回転は無視する
#define	BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_POSITION4_MakeQuadEdgePos_OPT_ForBM2_ParentScall(LOCAL_MATRIX, PARENT_SCALE)	\
	ml::ni::vector3d left_top;																		\
	ml::ni::vector3d right_top;																		\
	ml::ni::vector3d left_bottom;																	\
	ml::ni::vector3d right_bottom;																	\
	{																								\
		ML_STATIC_ASSERT(ScaleParentNodeInfluenceTypeConst_Off				== 0);					\
		ML_STATIC_ASSERT(ScaleParentNodeInfluenceTypeConst_On				== 1);					\
		ML_STATIC_ASSERT(ScaleParentNodeInfluenceTypeConst_OnGenerateTime	== 2);					\
		ML_STATIC_ASSERT(ScaleParentNodeInfluenceTypeConst_MAX				== 3);					\
																									\
		ml::vector3d	pivot;																		\
		switch(/*res.Basic_ParentNodeInfluence_ScaleParentNodeInfluenceType()*/ spnit){				\
			case ScaleParentNodeInfluenceTypeConst_Off:{											\
				left_top		= prim.transform_.left_top_;										\
				right_top		= prim.transform_.right_top_;										\
				left_bottom		= prim.transform_.left_bottom_;										\
				right_bottom	= prim.transform_.right_bottom_;									\
				pivot			= deformation_basic_centerposition;									\
				left_top.add(pivot);																\
				right_top.add(pivot);																\
				left_bottom.add(pivot);																\
				right_bottom.add(pivot);															\
																									\
				left_top.mul33(LOCAL_MATRIX);														\
				right_top.mul33(LOCAL_MATRIX);														\
				left_bottom.mul33(LOCAL_MATRIX);													\
				right_bottom.mul33(LOCAL_MATRIX);													\
				break;																				\
			}																						\
																									\
			case ScaleParentNodeInfluenceTypeConst_On:												\
			case ScaleParentNodeInfluenceTypeConst_OnGenerateTime:{									\
				left_top		= prim.transform_.left_top_;										\
				right_top		= prim.transform_.right_top_;										\
				left_bottom		= prim.transform_.left_bottom_;										\
				right_bottom	= prim.transform_.right_bottom_;									\
				pivot			= deformation_basic_centerposition;									\
				left_top.add(pivot);																\
				right_top.add(pivot);																\
				left_bottom.add(pivot);																\
				right_bottom.add(pivot);															\
				left_top.mul33(LOCAL_MATRIX);														\
				right_top.mul33(LOCAL_MATRIX);														\
				left_bottom.mul33(LOCAL_MATRIX);													\
				right_bottom.mul33(LOCAL_MATRIX);													\
				left_top.mul(PARENT_SCALE);														\
				right_top.mul(PARENT_SCALE);														\
				left_bottom.mul(PARENT_SCALE);													\
				right_bottom.mul(PARENT_SCALE);													\
																									\
				break;																				\
			}																						\
																									\
			default:{																				\
				ML_ASSERT(false);																	\
				BM3_ASSUME(0);																		\
				break;																				\
			}																						\
		}																							\
																									\
	}


template<>
struct Position4<DrawScale::XY, DrawRotate::Billboard_Z>{

	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void Push(DstType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const PacketDrawInfo & /*pdi*/, int stride = sizeof(DstType)){
		ML_PTR_ASSERT(dst);

		ml::ni::fast::matrix44 m;
		MakeMatrix(m, prim, draw_info);

		BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_POSITION4_MakeQuadEdgePos(m, matrix_row_length);

		ml::ni::vector3d p;
		MakeCenterPosition(p, prim, matrix);

		Set(dst, p, left_top, left_bottom, right_bottom, right_top, stride);
	}

	// BM2互換
	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void PushBm2(DstType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const PacketDrawInfo & /*pdi*/, int stride = sizeof(DstType)){
		ML_PTR_ASSERT(dst);

		ml::ni::fast::matrix44 m;
		MakeMatrixBm2(m, prim, draw_info);

		//BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_POSITION4_MakeQuadEdgePos_ForBM2(m, matrix);
		BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_POSITION4_MakeQuadEdgePos_ForBM2_ParentScall(m, matrix_row_length);

		ml::ni::vector3d p;
		MakeCenterPosition(p, prim, matrix);

		Set(dst, p, left_top, left_bottom, right_bottom, right_top, stride);
	}

	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void PushFixQuad(DstType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const PacketDrawInfo & /*pdi*/, int stride = sizeof(DstType)){
		ML_PTR_ASSERT(dst);

		ml::ni::fast::matrix44 m;
		MakeMatrix(m, prim, draw_info);

		BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_POSITION2_MakeQuadEdgePos(m, matrix_row_length);

		ml::ni::vector3d p;
		MakeCenterPosition(p, prim, matrix);

		if(res.Basic_Meta_OptimizeBit() & OptimizeBit::FixQuadPivot){
			ml::vector3d	pivot;
			pivot.mul(res.Deformation_Basic_CenterPosition(), matrix_row_length);
			pivot.mul33(m);
			p.add(pivot);
		}
		Set(dst, p, left_top, right_top, stride);
	}

	// BM2互換
	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void PushFixQuadBm2(DstType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const PacketDrawInfo & /*pdi*/, int stride = sizeof(DstType)){
		ML_PTR_ASSERT(dst);

		ml::ni::fast::matrix44 m;
		MakeMatrixBm2(m, prim, draw_info);

		//BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_POSITION2_MakeQuadEdgePos_ForBM2(m, matrix);
		BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_POSITION2_MakeQuadEdgePos_ForBM2_ParentScall(m, matrix_row_length);

		ml::ni::vector3d p;
		MakeCenterPosition(p, prim, matrix);

		if(res.Basic_Meta_OptimizeBit() & OptimizeBit::FixQuadPivot){
			ml::vector3d	pivot;
			pivot.mul33(res.Deformation_Basic_CenterPosition(), m);
			pivot.mul33(matrix);
			p.add(pivot);
		}
		Set(dst, p, left_top, right_top, stride);
	}

	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void PushNoField(DstType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const PacketDrawInfo & /*pdi*/, int stride = sizeof(DstType)){
		ML_PTR_ASSERT(dst);

		ml::ni::fast::matrix44 m;
		MakeMatrix(m, prim, draw_info);

		BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_POSITION4_MakeQuadEdgePos(m, matrix_row_length);

		ml::ni::vector3d p;
		MakeCenterPositionNoField(p, prim, matrix);

		Set(dst, p, left_top, left_bottom, right_bottom, right_top, stride);
	}

	// BM2互換
	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void PushNoFieldBm2(DstType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const PacketDrawInfo & /*pdi*/, int stride = sizeof(DstType)){
		ML_PTR_ASSERT(dst);

		ml::ni::fast::matrix44 m;
		MakeMatrixBm2(m, prim, draw_info);

		//BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_POSITION4_MakeQuadEdgePos_ForBM2(m, matrix);
		BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_POSITION4_MakeQuadEdgePos_ForBM2_ParentScall(m, matrix_row_length);

		ml::ni::vector3d p;
		MakeCenterPositionNoField(p, prim, matrix);

		Set(dst, p, left_top, left_bottom, right_bottom, right_top, stride);
	}

	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void PushFixQuadNoField(DstType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const PacketDrawInfo & /*pdi*/, int stride = sizeof(DstType)){
		ML_PTR_ASSERT(dst);

		ml::ni::fast::matrix44 m;
		MakeMatrix(m, prim, draw_info);

		BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_POSITION2_MakeQuadEdgePos(m, matrix_row_length);

		ml::ni::vector3d p;
		MakeCenterPositionNoField(p, prim, matrix);

		if(res.Basic_Meta_OptimizeBit() & OptimizeBit::FixQuadPivot){
			ml::vector3d	pivot;
			pivot.mul(res.Deformation_Basic_CenterPosition(), matrix_row_length);
			pivot.mul33(m);
			p.add(pivot);
		}
		Set(dst, p, left_top, right_top, stride);
	}

	// BM2互換
	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void PushFixQuadNoFieldBm2(DstType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const PacketDrawInfo & /*pdi*/, int stride = sizeof(DstType)){
		ML_PTR_ASSERT(dst);

		ml::ni::fast::matrix44 m;
		MakeMatrixBm2(m, prim, draw_info);

		//BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_POSITION2_MakeQuadEdgePos_ForBM2(m, matrix);
		BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_POSITION2_MakeQuadEdgePos_ForBM2_ParentScall(m, matrix_row_length);

		ml::ni::vector3d p;
		MakeCenterPositionNoField(p, prim, matrix);

		if(res.Basic_Meta_OptimizeBit() & OptimizeBit::FixQuadPivot){
			ml::vector3d	pivot;
			pivot.mul33(res.Deformation_Basic_CenterPosition(), m);
			pivot.mul33(matrix);
			p.add(pivot);
		}
		Set(dst, p, left_top, right_top, stride);
	}

private:
	template<typename PrimitiveType>
	BM3_FORCEINLINE void MakeMatrix(ml::matrix44 &dst, const PrimitiveType &prim, DrawInfo &draw_info){
		dst.set_scale(prim.scale_.value_);
		dst.mul_rot_z(prim.rotate_.value_.e_.z_);
		dst.mul33(draw_info.InverseViewMatrix());
	}

	// BM2互換
	template<typename PrimitiveType>
	BM3_FORCEINLINE void MakeMatrixBm2(ml::matrix44 &dst, const PrimitiveType &prim, DrawInfo &draw_info){
		dst.set_scale(prim.scale_.value_);
		dst.mul_rot_z(prim.rotate_.value_.e_.z_);
		dst.mul33(draw_info.InverseViewMatrix());
	}

	template<typename DstType>
	BM3_FORCEINLINE void Set(DstType *dst, ml::ni::vector3d &base, ml::ni::vector3d &left_top, ml::ni::vector3d &left_bottom, ml::ni::vector3d &right_bottom, ml::ni::vector3d &right_top, int stride){
		BM3_DST(VertexElement::PC0)->x_	= base.e_.x_ + left_top.e_.x_;
		BM3_DST(VertexElement::PC0)->y_	= base.e_.y_ + left_top.e_.y_;
		BM3_DST(VertexElement::PC0)->z_	= base.e_.z_ + left_top.e_.z_;
		BM3_DST(VertexElement::PC1)->x_	= base.e_.x_ + left_bottom.e_.x_;
		BM3_DST(VertexElement::PC1)->y_	= base.e_.y_ + left_bottom.e_.y_;
		BM3_DST(VertexElement::PC1)->z_	= base.e_.z_ + left_bottom.e_.z_;
		BM3_DST(VertexElement::PC2)->x_	= base.e_.x_ + right_bottom.e_.x_;
		BM3_DST(VertexElement::PC2)->y_	= base.e_.y_ + right_bottom.e_.y_;
		BM3_DST(VertexElement::PC2)->z_	= base.e_.z_ + right_bottom.e_.z_;
		BM3_DST(VertexElement::PC3)->x_	= base.e_.x_ + right_top.e_.x_;
		BM3_DST(VertexElement::PC3)->y_	= base.e_.y_ + right_top.e_.y_;
		BM3_DST(VertexElement::PC3)->z_	= base.e_.z_ + right_top.e_.z_;
	}

	template<typename DstType>
	BM3_FORCEINLINE void Set(DstType *dst, ml::ni::vector3d &base, ml::ni::vector3d &left_top, ml::ni::vector3d &right_top, int stride){
		BM3_DST(VertexElement::PC0)->x_	= base.e_.x_ + left_top.e_.x_;
		BM3_DST(VertexElement::PC0)->y_	= base.e_.y_ + left_top.e_.y_;
		BM3_DST(VertexElement::PC0)->z_	= base.e_.z_ + left_top.e_.z_;
		BM3_DST(VertexElement::PC1)->x_	= base.e_.x_ - right_top.e_.x_;
		BM3_DST(VertexElement::PC1)->y_	= base.e_.y_ - right_top.e_.y_;
		BM3_DST(VertexElement::PC1)->z_	= base.e_.z_ - right_top.e_.z_;
		BM3_DST(VertexElement::PC2)->x_	= base.e_.x_ - left_top.e_.x_;
		BM3_DST(VertexElement::PC2)->y_	= base.e_.y_ - left_top.e_.y_;
		BM3_DST(VertexElement::PC2)->z_	= base.e_.z_ - left_top.e_.z_;
		BM3_DST(VertexElement::PC3)->x_	= base.e_.x_ + right_top.e_.x_;
		BM3_DST(VertexElement::PC3)->y_	= base.e_.y_ + right_top.e_.y_;
		BM3_DST(VertexElement::PC3)->z_	= base.e_.z_ + right_top.e_.z_;
	}
};

template<>
struct Position4<DrawScale::XY, DrawRotate::Polyline>{

	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void Push(DstType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const PacketDrawInfo & /*pdi*/, int stride = sizeof(DstType)){
		ML_PTR_ASSERT(dst);

		ml::ni::vector3d p;
		ml::ni::vector3d x, y, z;
		MakeCenterPosition(p, prim, matrix);
		MakeXYZ(x, y, z, prim, draw_info, res, p, matrix_row_length);
		{
			ml::ni::fast::matrix44 temp;
			MakeMatrix(temp, prim, x, y, z);
			BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_POSITION4_MakeQuadEdgePos(temp, matrix_row_length);
			Set(dst, p, left_top, left_bottom, right_bottom, right_top, stride);
		}
	}

	// BM2互換
	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void PushBm2(DstType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const PacketDrawInfo & /*pdi*/, int stride = sizeof(DstType)){
		ML_PTR_ASSERT(dst);

		ml::ni::vector3d p;
		ml::ni::vector3d x, y, z;
		MakeCenterPosition(p, prim, matrix);
		MakeZXY(x, y, z, prim, draw_info, res, p, matrix);
		{
			ml::ni::fast::matrix44 temp;
			MakeMatrixBm2(temp, prim, x, y, z);
			//BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_POSITION4_MakeQuadEdgePos_ForBM2(temp, matrix);
			BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_POSITION4_MakeQuadEdgePos_ForBM2_ParentScall(temp, matrix_row_length);
			Set(dst, p, left_top, left_bottom, right_bottom, right_top, stride);
		}
	}

	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void PushFixQuad(DstType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const PacketDrawInfo & /*pdi*/, int stride = sizeof(DstType)){
		ML_PTR_ASSERT(dst);

		ml::ni::vector3d p;
		ml::ni::vector3d x, y, z;
		MakeCenterPosition(p, prim, matrix);
		MakeXYZ(x, y, z, prim, draw_info, res, p, matrix_row_length);
		{
			ml::ni::fast::matrix44 m;
			MakeMatrix(m, prim, x, y, z);
			BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_POSITION2_MakeQuadEdgePos(m, matrix_row_length);
			if(res.Basic_Meta_OptimizeBit() & OptimizeBit::FixQuadPivot){
				ml::vector3d	pivot;
				pivot.mul(res.Deformation_Basic_CenterPosition(), matrix_row_length);
				pivot.mul33(m);
				p.add(pivot);
			}
			Set(dst, p, left_top, right_top, stride);
		}
	}

	// BM2互換
	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void PushFixQuadBm2(DstType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const PacketDrawInfo & /*pdi*/, int stride = sizeof(DstType)){
		ML_PTR_ASSERT(dst);

		ml::ni::vector3d p;
		ml::ni::vector3d x, y, z;
		MakeCenterPosition(p, prim, matrix);
		MakeZXY(x, y, z, prim, draw_info, res, p, matrix);
		{
			ml::ni::fast::matrix44 m;
			MakeMatrixBm2(m, prim, x, y, z);
			//BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_POSITION2_MakeQuadEdgePos_ForBM2(m, matrix);
			BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_POSITION2_MakeQuadEdgePos_ForBM2_ParentScall(m, matrix_row_length);
			if(res.Basic_Meta_OptimizeBit() & OptimizeBit::FixQuadPivot){
				ml::vector3d	pivot;
				pivot.mul33(res.Deformation_Basic_CenterPosition(), m);
				pivot.mul33(matrix);
				p.add(pivot);
			}
			Set(dst, p, left_top, right_top, stride);
		}
	}

	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void PushNoField(DstType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const PacketDrawInfo & /*pdi*/, int stride = sizeof(DstType)){
		ML_PTR_ASSERT(dst);

		ml::ni::vector3d p;
		ml::ni::vector3d x, y, z;
		MakeCenterPositionNoField(p, prim, matrix);
		MakeXYZ(x, y, z, prim, draw_info, res, p, matrix_row_length);
		{
			ml::ni::fast::matrix44 temp;
			MakeMatrix(temp, prim, x, y, z);
			BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_POSITION4_MakeQuadEdgePos(temp, matrix_row_length);
			Set(dst, p, left_top, left_bottom, right_bottom, right_top, stride);
		}
	}

	// BM2互換
	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void PushNoFieldBm2(DstType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const PacketDrawInfo & /*pdi*/, int stride = sizeof(DstType)){
		ML_PTR_ASSERT(dst);

		ml::ni::vector3d p;
		ml::ni::vector3d x, y, z;
		MakeCenterPositionNoField(p, prim, matrix);
		MakeZXY(x, y, z, prim, draw_info, res, p, matrix);
		{
			ml::ni::fast::matrix44 temp;
			MakeMatrixBm2(temp, prim, x, y, z);
			//BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_POSITION4_MakeQuadEdgePos_ForBM2(temp, matrix);
			BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_POSITION4_MakeQuadEdgePos_ForBM2_ParentScall(temp, matrix_row_length);
			Set(dst, p, left_top, left_bottom, right_bottom, right_top, stride);
		}
	}

	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void PushFixQuadNoField(DstType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const PacketDrawInfo & /*pdi*/, int stride = sizeof(DstType)){
		ML_PTR_ASSERT(dst);

		ml::ni::vector3d p;
		ml::ni::vector3d x, y, z;
		MakeCenterPositionNoField(p, prim, matrix);
		MakeXYZ(x, y, z, prim, draw_info, res, p, matrix_row_length);
		{
			ml::ni::fast::matrix44 m;
			MakeMatrix(m, prim, x, y, z);
			BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_POSITION2_MakeQuadEdgePos(m, matrix_row_length);
			if(res.Basic_Meta_OptimizeBit() & OptimizeBit::FixQuadPivot){
				ml::vector3d	pivot;
				pivot.mul(res.Deformation_Basic_CenterPosition(), matrix_row_length);
				pivot.mul33(m);
				p.add(pivot);
			}
			Set(dst, p, left_top, right_top, stride);
		}
	}

	// BM2互換
	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void PushFixQuadNoFieldBm2(DstType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const PacketDrawInfo & /*pdi*/, int stride = sizeof(DstType)){
		ML_PTR_ASSERT(dst);

		ml::ni::vector3d p;
		ml::ni::vector3d x, y, z;
		MakeCenterPositionNoField(p, prim, matrix);
		MakeZXY(x, y, z, prim, draw_info, res, p, matrix);
		{
			ml::ni::fast::matrix44 m;
			MakeMatrixBm2(m, prim, x, y, z);
			//BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_POSITION2_MakeQuadEdgePos_ForBM2(m, matrix);
			BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_POSITION2_MakeQuadEdgePos_ForBM2_ParentScall(m, matrix_row_length);
			if(res.Basic_Meta_OptimizeBit() & OptimizeBit::FixQuadPivot){
				ml::vector3d	pivot;
				pivot.mul33(res.Deformation_Basic_CenterPosition(), m);
				pivot.mul33(matrix);
				p.add(pivot);
			}
			Set(dst, p, left_top, right_top, stride);
		}
	}
private:
	template<typename PrimitiveType, typename ResourceType>
	BM3_FORCEINLINE void MakeXYZ(ml::vector3d &x, ml::vector3d &y, ml::vector3d &z, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, const ml::vector3d &p, const ml::vector3d &matrix_row_length){

		ml::ni::fast::matrix44 temp;
		{
			temp.set_scale(prim.scale_.value_);
			temp.mul_rot_xyz(prim.rotate_.value_);
		}

		BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_POSITION4_MakeQuadEdgePos(temp, matrix_row_length);

		{
			ml::ni::vector3d top, bottom;
			{
				top.add(left_top, right_top).mul(0.5f);
				bottom.add(left_bottom, right_bottom).mul(0.5f);

				top.e_.w_ = 1.0f;
				bottom.e_.w_ = 1.0f;
			}

			top.add(p);
			bottom.add(p);

			y.sub(top, bottom);
			y.e_.w_ = 1.0f;
			y.normalize();
		}

		{
			ml::ni::vector3d to_front;
#ifdef BM3_TARGET_IDE
			ml::vector3d	eye = draw_info.ViewMatrix().translate();
			eye.mul(draw_info.InverseViewMatrix());
			eye.mul(-1.0f);	// これを入れないと向きが合わない
			to_front.sub(p, eye);
			to_front.normalize();
#else
			to_front.sub(p, draw_info.InverseViewMatrix().translate());
			to_front.normalize();
#endif
			x = to_front.cross(y).normalize();
			z = x.cross(y).normalize();
		}
		x.e_.w_ = 0.0f;
		y.e_.w_ = 0.0f;
		z.e_.w_ = 0.0f;
	}

	// BM2互換
	template<typename PrimitiveType, typename ResourceType>
	BM3_FORCEINLINE void MakeZXY(ml::vector3d &x, ml::vector3d &y, ml::vector3d &z, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, const ml::vector3d &p, const ml::matrix44 &matrix){

		ml::ni::fast::matrix44 temp;
		{
			temp.set_scale(prim.scale_.value_);
			temp.mul_rot_zxy(prim.rotate_.value_);
		}

		BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_POSITION4_MakeQuadEdgePos_ForBM2(temp, matrix);

		{
			ml::ni::vector3d top, bottom;
			{
				top.add(left_top, right_top).mul(0.5f);
				bottom.add(left_bottom, right_bottom).mul(0.5f);

				top.e_.w_ = 1.0f;
				bottom.e_.w_ = 1.0f;
			}

			top.add(p);
			bottom.add(p);

			y.sub(top, bottom);
			y.e_.w_ = 1.0f;
			y.normalize();
		}

		{
			ml::ni::vector3d to_front;
#ifdef BM3_TARGET_IDE
			ml::vector3d	eye = draw_info.ViewMatrix().translate();
			eye.mul(draw_info.InverseViewMatrix());
			eye.mul(-1.0f);	// これを入れないと向きが合わない
			to_front.sub(p, eye);
			to_front.normalize();
#else
			to_front.sub(p, draw_info.InverseViewMatrix().translate());
			to_front.normalize();
#endif
			x = to_front.cross(y).normalize();
			z = x.cross(y).normalize();
		}
		x.e_.w_ = 0.0f;
		y.e_.w_ = 0.0f;
		z.e_.w_ = 0.0f;
	}

	template<typename PrimitiveType>
	BM3_FORCEINLINE void MakeMatrix(ml::matrix44 &dst, const PrimitiveType &prim, const ml::vector3d &x, const ml::vector3d &y, const ml::vector3d &z){
		ml::matrix44 m;
		dst.set_scale(prim.scale_.value_);

		m.row(ml::matrix44::row::x) = x;
		m.row(ml::matrix44::row::y) = y;
		m.row(ml::matrix44::row::z) = z;
		dst.mul33(m);
	}

	// BM2互換
	template<typename PrimitiveType>
	BM3_FORCEINLINE void MakeMatrixBm2(ml::matrix44 &dst, const PrimitiveType &prim, const ml::vector3d &x, const ml::vector3d &y, const ml::vector3d &z){
		ml::matrix44 m;
		dst.set_scale(prim.scale_.value_);

		m.row(ml::matrix44::row::z) = z;
		m.row(ml::matrix44::row::x) = x;
		m.row(ml::matrix44::row::y) = y;
		dst.mul33(m);
	}

	template<typename DstType>
	BM3_FORCEINLINE void Set(DstType *dst, ml::ni::vector3d &base, ml::ni::vector3d &left_top, ml::ni::vector3d &left_bottom, ml::ni::vector3d &right_bottom, ml::ni::vector3d &right_top, int stride){
		BM3_DST(VertexElement::PC0)->x_	= base.e_.x_ + left_top.e_.x_;
		BM3_DST(VertexElement::PC0)->y_	= base.e_.y_ + left_top.e_.y_;
		BM3_DST(VertexElement::PC0)->z_	= base.e_.z_ + left_top.e_.z_;
		BM3_DST(VertexElement::PC1)->x_	= base.e_.x_ + left_bottom.e_.x_;
		BM3_DST(VertexElement::PC1)->y_	= base.e_.y_ + left_bottom.e_.y_;
		BM3_DST(VertexElement::PC1)->z_	= base.e_.z_ + left_bottom.e_.z_;
		BM3_DST(VertexElement::PC2)->x_	= base.e_.x_ + right_bottom.e_.x_;
		BM3_DST(VertexElement::PC2)->y_	= base.e_.y_ + right_bottom.e_.y_;
		BM3_DST(VertexElement::PC2)->z_	= base.e_.z_ + right_bottom.e_.z_;
		BM3_DST(VertexElement::PC3)->x_	= base.e_.x_ + right_top.e_.x_;
		BM3_DST(VertexElement::PC3)->y_	= base.e_.y_ + right_top.e_.y_;
		BM3_DST(VertexElement::PC3)->z_	= base.e_.z_ + right_top.e_.z_;
	}

	template<typename DstType>
	BM3_FORCEINLINE void Set(DstType *dst, ml::ni::vector3d &base, ml::ni::vector3d &left_top, ml::ni::vector3d &right_top, int stride){
		BM3_DST(VertexElement::PC0)->x_	= base.e_.x_ + left_top.e_.x_;
		BM3_DST(VertexElement::PC0)->y_	= base.e_.y_ + left_top.e_.y_;
		BM3_DST(VertexElement::PC0)->z_	= base.e_.z_ + left_top.e_.z_;
		BM3_DST(VertexElement::PC1)->x_	= base.e_.x_ - right_top.e_.x_;
		BM3_DST(VertexElement::PC1)->y_	= base.e_.y_ - right_top.e_.y_;
		BM3_DST(VertexElement::PC1)->z_	= base.e_.z_ - right_top.e_.z_;
		BM3_DST(VertexElement::PC2)->x_	= base.e_.x_ - left_top.e_.x_;
		BM3_DST(VertexElement::PC2)->y_	= base.e_.y_ - left_top.e_.y_;
		BM3_DST(VertexElement::PC2)->z_	= base.e_.z_ - left_top.e_.z_;
		BM3_DST(VertexElement::PC3)->x_	= base.e_.x_ + right_top.e_.x_;
		BM3_DST(VertexElement::PC3)->y_	= base.e_.y_ + right_top.e_.y_;
		BM3_DST(VertexElement::PC3)->z_	= base.e_.z_ + right_top.e_.z_;
	}
};

template<>
struct Position4<DrawScale::XY, DrawRotate::XYZ>{

	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void Push(DstType *dst, const PrimitiveType &prim, DrawInfo &/*draw_info*/, const ResourceType &res, const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const PacketDrawInfo & /*pdi*/, int stride = sizeof(DstType)){
		ML_PTR_ASSERT(dst);

		ml::matrix44 m;
		MakeMatrix(m, prim, matrix);
		BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_POSITION4_MakeQuadEdgePos(m, matrix_row_length);

		ml::ni::vector3d p;
		MakeCenterPosition(p, prim, matrix);

		Set(dst, p, left_top, left_bottom, right_bottom, right_top, stride);
	}

	// BM2互換
	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void PushBm2(DstType *dst, const PrimitiveType &prim, DrawInfo &/*draw_info*/, const ResourceType &res, const ml::matrix44 &matrix, const ml::vector3d &/*matrix_row_length*/, const PacketDrawInfo & /*pdi*/, int stride = sizeof(DstType)){
		ML_PTR_ASSERT(dst);

		ml::matrix44 m;
		MakeMatrixBm2(m, prim, matrix);
		BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_POSITION4_MakeQuadEdgePos_ForBM2(m, matrix);

		ml::ni::vector3d p;
		MakeCenterPosition(p, prim, matrix);

		Set(dst, p, left_top, left_bottom, right_bottom, right_top, stride);
	}

	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void PushFixQuad(DstType *dst, const PrimitiveType &prim, DrawInfo & /*draw_info*/, const ResourceType &res, const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const PacketDrawInfo & /*pdi*/, int stride = sizeof(DstType)){
		ML_PTR_ASSERT(dst);

		ml::matrix44 m;
		MakeMatrix(m, prim, matrix);
		BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_POSITION2_MakeQuadEdgePos(m, matrix_row_length);

		ml::ni::vector3d p;
		MakeCenterPosition(p, prim, matrix);

		if(res.Basic_Meta_OptimizeBit() & OptimizeBit::FixQuadPivot){
			ml::vector3d	pivot;
			pivot.mul(res.Deformation_Basic_CenterPosition(), matrix_row_length);
			pivot.mul33(m);
			p.add(pivot);
		}
		Set(dst, p, left_top, right_top, stride);
	}

	// BM2互換
	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void PushFixQuadBm2(DstType *dst, const PrimitiveType &prim, DrawInfo & /*draw_info*/, const ResourceType &res, const ml::matrix44 &matrix, const ml::vector3d &/*matrix_row_length*/, const PacketDrawInfo & /*pdi*/, int stride = sizeof(DstType)){
		ML_PTR_ASSERT(dst);

		ml::matrix44 m;
		MakeMatrixBm2(m, prim, matrix);
		BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_POSITION2_MakeQuadEdgePos_ForBM2(m, matrix);

		ml::ni::vector3d p;
		MakeCenterPosition(p, prim, matrix);

		if(res.Basic_Meta_OptimizeBit() & OptimizeBit::FixQuadPivot){
			ml::vector3d	pivot;
			pivot.mul33(res.Deformation_Basic_CenterPosition(), m);
			pivot.mul33(matrix);
			p.add(pivot);
		}
		Set(dst, p, left_top, right_top, stride);
	}

	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void PushNoField(DstType *dst, const PrimitiveType &prim, DrawInfo &/*draw_info*/, const ResourceType &res, const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const PacketDrawInfo & /*pdi*/, int stride = sizeof(DstType)){
		ML_PTR_ASSERT(dst);

		ml::matrix44 m;
		MakeMatrix(m, prim, matrix);
		BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_POSITION4_MakeQuadEdgePos(m, matrix_row_length);

		ml::ni::vector3d p;
		MakeCenterPositionNoField(p, prim, matrix);

		Set(dst, p, left_top, left_bottom, right_bottom, right_top, stride);
	}

	// BM2互換
	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void PushNoFieldBm2(DstType *dst, const PrimitiveType &prim, DrawInfo &/*draw_info*/, const ResourceType &res, const ml::matrix44 &matrix, const ml::vector3d &/*matrix_row_length*/, const PacketDrawInfo & /*pdi*/, int stride = sizeof(DstType)){
		ML_PTR_ASSERT(dst);

		ml::matrix44 m;
		MakeMatrixBm2(m, prim, matrix);
		BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_POSITION4_MakeQuadEdgePos_ForBM2(m, matrix);

		ml::ni::vector3d p;
		MakeCenterPositionNoField(p, prim, matrix);

		Set(dst, p, left_top, left_bottom, right_bottom, right_top, stride);
	}

	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void PushFixQuadNoField(DstType *dst, const PrimitiveType &prim, DrawInfo & /*draw_info*/, const ResourceType &res, const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const PacketDrawInfo & /*pdi*/, int stride = sizeof(DstType)){
		ML_PTR_ASSERT(dst);

		ml::matrix44 m;
		MakeMatrix(m, prim, matrix);
		BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_POSITION2_MakeQuadEdgePos(m, matrix_row_length);

		ml::ni::vector3d p;
		MakeCenterPositionNoField(p, prim, matrix);

		if(res.Basic_Meta_OptimizeBit() & OptimizeBit::FixQuadPivot){
			ml::vector3d	pivot;
			pivot.mul(res.Deformation_Basic_CenterPosition(), matrix_row_length);
			pivot.mul33(m);
			p.add(pivot);
		}
		Set(dst, p, left_top, right_top, stride);
	}

	// BM2互換
	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void PushFixQuadNoFieldBm2(DstType *dst, const PrimitiveType &prim, DrawInfo & /*draw_info*/, const ResourceType &res, const ml::matrix44 &matrix, const ml::vector3d &/*matrix_row_length*/, const PacketDrawInfo & /*pdi*/, int stride = sizeof(DstType)){
		ML_PTR_ASSERT(dst);

		ml::matrix44 m;
		MakeMatrixBm2(m, prim, matrix);
		BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_POSITION2_MakeQuadEdgePos_ForBM2(m, matrix);

		ml::ni::vector3d p;
		MakeCenterPositionNoField(p, prim, matrix);

		if(res.Basic_Meta_OptimizeBit() & OptimizeBit::FixQuadPivot){
			ml::vector3d	pivot;
			pivot.mul33(res.Deformation_Basic_CenterPosition(), m);
			pivot.mul33(matrix);
			p.add(pivot);
		}
		Set(dst, p, left_top, right_top, stride);
	}

private:
	template<typename PrimitiveType>
	BM3_FORCEINLINE void MakeMatrix(ml::matrix44 &dst, const PrimitiveType &prim, const ml::matrix44 &matrix){
		dst.set_scale(prim.scale_.value_);
		dst.mul_rot_xyz(prim.rotate_.value_);

		ml::matrix44	mm = matrix;
		mm.row(ml::matrix44::row::x).normalize();
		mm.row(ml::matrix44::row::y).normalize();
		mm.row(ml::matrix44::row::z).normalize();

		dst.mul33(mm);
	}

	// BM2互換
	template<typename PrimitiveType>
	BM3_FORCEINLINE void MakeMatrixBm2(ml::matrix44 &dst, const PrimitiveType &prim, const ml::matrix44 &/*matrix*/){
		dst.set_scale(prim.scale_.value_);
		dst.mul_rot_zxy(prim.rotate_.value_);

//		ml::matrix44	mm = matrix;
//		mm.row(ml::matrix44::row::x).normalize();
//		mm.row(ml::matrix44::row::y).normalize();
//		mm.row(ml::matrix44::row::z).normalize();

//		dst.mul33(mm);
	}

	template<typename DstType>
	BM3_FORCEINLINE void Set(DstType *dst, ml::ni::vector3d &base, ml::ni::vector3d &left_top, ml::ni::vector3d &left_bottom, ml::ni::vector3d &right_bottom, ml::ni::vector3d &right_top, int stride){
		BM3_DST(VertexElement::PC0)->x_	= base.e_.x_ + left_top.e_.x_;
		BM3_DST(VertexElement::PC0)->y_	= base.e_.y_ + left_top.e_.y_;
		BM3_DST(VertexElement::PC0)->z_	= base.e_.z_ + left_top.e_.z_;
		BM3_DST(VertexElement::PC1)->x_	= base.e_.x_ + left_bottom.e_.x_;
		BM3_DST(VertexElement::PC1)->y_	= base.e_.y_ + left_bottom.e_.y_;
		BM3_DST(VertexElement::PC1)->z_	= base.e_.z_ + left_bottom.e_.z_;
		BM3_DST(VertexElement::PC2)->x_	= base.e_.x_ + right_bottom.e_.x_;
		BM3_DST(VertexElement::PC2)->y_	= base.e_.y_ + right_bottom.e_.y_;
		BM3_DST(VertexElement::PC2)->z_	= base.e_.z_ + right_bottom.e_.z_;
		BM3_DST(VertexElement::PC3)->x_	= base.e_.x_ + right_top.e_.x_;
		BM3_DST(VertexElement::PC3)->y_	= base.e_.y_ + right_top.e_.y_;
		BM3_DST(VertexElement::PC3)->z_	= base.e_.z_ + right_top.e_.z_;
	}
	template<typename DstType>
	BM3_FORCEINLINE void Set(DstType *dst, ml::ni::vector3d &base, ml::ni::vector3d &left_top, ml::ni::vector3d &right_top, int stride){
		BM3_DST(VertexElement::PC0)->x_	= base.e_.x_ + left_top.e_.x_;
		BM3_DST(VertexElement::PC0)->y_	= base.e_.y_ + left_top.e_.y_;
		BM3_DST(VertexElement::PC0)->z_	= base.e_.z_ + left_top.e_.z_;
		BM3_DST(VertexElement::PC1)->x_	= base.e_.x_ - right_top.e_.x_;
		BM3_DST(VertexElement::PC1)->y_	= base.e_.y_ - right_top.e_.y_;
		BM3_DST(VertexElement::PC1)->z_	= base.e_.z_ - right_top.e_.z_;
		BM3_DST(VertexElement::PC2)->x_	= base.e_.x_ - left_top.e_.x_;
		BM3_DST(VertexElement::PC2)->y_	= base.e_.y_ - left_top.e_.y_;
		BM3_DST(VertexElement::PC2)->z_	= base.e_.z_ - left_top.e_.z_;
		BM3_DST(VertexElement::PC3)->x_	= base.e_.x_ + right_top.e_.x_;
		BM3_DST(VertexElement::PC3)->y_	= base.e_.y_ + right_top.e_.y_;
		BM3_DST(VertexElement::PC3)->z_	= base.e_.z_ + right_top.e_.z_;
	}
};

template<>
struct Position4<DrawScale::XY, DrawRotate::Directional>{

	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void Push(DstType *dst, const PrimitiveType &prim, DrawInfo &/*draw_info*/, const ResourceType &res, const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const PacketDrawInfo & /*pdi*/, int stride = sizeof(DstType)){
		ML_PTR_ASSERT(dst);

		ml::ni::vector3d p;
		MakeCenterPosition(p, prim, matrix);

		ml::matrix44 m;
		MakeMatrix(m, prim, matrix, p);
		BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_POSITION4_MakeQuadEdgePos(m, matrix_row_length);

		Set(dst, p, left_top, left_bottom, right_bottom, right_top, stride);
	}

	// BM2互換
	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void PushBm2(DstType *dst, const PrimitiveType &prim, DrawInfo & /*draw_info*/, const ResourceType &res, const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const PacketDrawInfo & /*pdi*/, int stride = sizeof(DstType)){
		ML_PTR_ASSERT(dst);

		ml::ni::vector3d p;
		MakeCenterPosition(p, prim, matrix);

		ml::matrix44 m;
		MakeMatrixBm2(m, prim, matrix, p);
		BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_POSITION4_MakeQuadEdgePos_ForBM2_ParentScall(m, matrix_row_length);


		Set(dst, p, left_top, left_bottom, right_bottom, right_top, stride);
	}

	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void PushNoField(DstType *dst, const PrimitiveType &prim, DrawInfo &/*draw_info*/, const ResourceType &res, const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const PacketDrawInfo & /*pdi*/, int stride = sizeof(DstType)){
		ML_PTR_ASSERT(dst);

		ml::ni::vector3d p;
		MakeCenterPositionNoField(p, prim, matrix);

		ml::matrix44 m;
		MakeMatrix(m, prim, matrix, p);
		BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_POSITION4_MakeQuadEdgePos(m, matrix_row_length);

		Set(dst, p, left_top, left_bottom, right_bottom, right_top, stride);
	}

	// BM2互換
	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void PushNoFieldBm2(DstType *dst, const PrimitiveType &prim, DrawInfo &/*draw_info*/, const ResourceType &res, const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const PacketDrawInfo & /*pdi*/, int stride = sizeof(DstType)){
		ML_PTR_ASSERT(dst);

		ml::ni::vector3d p;
		MakeCenterPositionNoField(p, prim, matrix);

		ml::matrix44 m;
		MakeMatrixBm2(m, prim, matrix, p);
		BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_POSITION4_MakeQuadEdgePos_ForBM2_ParentScall(m, matrix_row_length);

		Set(dst, p, left_top, left_bottom, right_bottom, right_top, stride);
	}

	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void PushFixQuad(DstType *dst, const PrimitiveType &prim, DrawInfo & /*draw_info*/, const ResourceType &res, const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const PacketDrawInfo & /*pdi*/, int stride = sizeof(DstType)){
		ML_PTR_ASSERT(dst);

		ml::ni::vector3d p;
		MakeCenterPosition(p, prim, matrix);

		ml::matrix44 m;
		MakeMatrix(m, prim, matrix, p);
		BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_POSITION2_MakeQuadEdgePos(m, matrix_row_length);

		if(res.Basic_Meta_OptimizeBit() & OptimizeBit::FixQuadPivot){
			ml::vector3d	pivot;
			pivot.mul(res.Deformation_Basic_CenterPosition(), matrix_row_length);
			pivot.mul33(m);
			p.add(pivot);
		}
		Set(dst, p, left_top, right_top, stride);
	}

	// BM2互換
	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void PushFixQuadBm2(DstType *dst, const PrimitiveType &prim, DrawInfo & /*draw_info*/, const ResourceType &res, const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const PacketDrawInfo & /*pdi*/, int stride = sizeof(DstType)){
		ML_PTR_ASSERT(dst);

		ml::ni::vector3d p;
		MakeCenterPosition(p, prim, matrix);

		ml::matrix44 m;
		MakeMatrixBm2(m, prim, matrix, p);
		BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_POSITION2_MakeQuadEdgePos_ForBM2_ParentScall(m, matrix_row_length);

		if(res.Basic_Meta_OptimizeBit() & OptimizeBit::FixQuadPivot){
			ml::vector3d	pivot;
			pivot.mul33(res.Deformation_Basic_CenterPosition(), m);
			pivot.mul33(matrix);
			p.add(pivot);
		}
		Set(dst, p, left_top, right_top, stride);
	}

	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void PushFixQuadNoField(DstType *dst, const PrimitiveType &prim, DrawInfo & /*draw_info*/, const ResourceType &res, const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const PacketDrawInfo & /*pdi*/, int stride = sizeof(DstType)){
		ML_PTR_ASSERT(dst);

		ml::ni::vector3d p;
		MakeCenterPositionNoField(p, prim, matrix);

		ml::matrix44 m;
		MakeMatrix(m, prim, matrix, p);
		BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_POSITION2_MakeQuadEdgePos(m, matrix_row_length);

		if(res.Basic_Meta_OptimizeBit() & OptimizeBit::FixQuadPivot){
			ml::vector3d	pivot;
			pivot.mul(res.Deformation_Basic_CenterPosition(), matrix_row_length);
			pivot.mul33(m);
			p.add(pivot);
		}
		Set(dst, p, left_top, right_top, stride);
	}

	// BM2互換
	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void PushFixQuadNoFieldBm2(DstType *dst, const PrimitiveType &prim, DrawInfo & /*draw_info*/, const ResourceType &res, const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const PacketDrawInfo & /*pdi*/, int stride = sizeof(DstType)){
		ML_PTR_ASSERT(dst);

		ml::ni::vector3d p;
		MakeCenterPositionNoField(p, prim, matrix);

		ml::matrix44 m;
		MakeMatrixBm2(m, prim, matrix, p);
		BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_POSITION2_MakeQuadEdgePos_ForBM2_ParentScall(m, matrix_row_length);

		if(res.Basic_Meta_OptimizeBit() & OptimizeBit::FixQuadPivot){
			ml::vector3d	pivot;
			pivot.mul33(res.Deformation_Basic_CenterPosition(), m);
			pivot.mul33(matrix);
			p.add(pivot);
		}
		Set(dst, p, left_top, right_top, stride);
	}
private:
	template<typename PrimitiveType>
	BM3_FORCEINLINE void MakeMatrix(ml::matrix44 &dst, const PrimitiveType &prim, const ml::matrix44 & /*matrix*/, const ml::vector3d &position){

		// directionがゼロの場合は一度も動いたことがないということ、それは最初だけ。		
		if(prim.GetDirection().is_unit()){
			dst.set_scale(0.0f,0.0f,0.0f);		// 見えないようにする
		}
		else{
			dst.set_scale(prim.scale_.value_);
			dst.mul_rot_xyz(prim.rotate_.value_);

			MakeDirectionalMatrix(dst, prim, position);
		}
	}

	// BM2互換
	template<typename PrimitiveType>
	BM3_FORCEINLINE void MakeMatrixBm2(ml::matrix44 &dst, const PrimitiveType &prim, const ml::matrix44 & /*matrix*/, const ml::vector3d &position){

		// directionがゼロの場合は一度も動いたことがないということ、それは最初だけ。		
		if(prim.GetDirection().is_unit()){
			dst.set_scale(0.0f,0.0f,0.0f);		// 見えないようにする
		}
		else{
			dst.set_scale(prim.scale_.value_);
			dst.mul_rot_zxy(prim.rotate_.value_);

			MakeDirectionalMatrix(dst, prim, position);
		}
	}

	template<typename DstType>
	BM3_FORCEINLINE void Set(DstType *dst, ml::ni::vector3d &base, ml::ni::vector3d &left_top, ml::ni::vector3d &left_bottom, ml::ni::vector3d &right_bottom, ml::ni::vector3d &right_top, int stride){

		BM3_DST(VertexElement::PC0)->x_	= base.e_.x_ + left_top.e_.x_;
		BM3_DST(VertexElement::PC0)->y_	= base.e_.y_ + left_top.e_.y_;
		BM3_DST(VertexElement::PC0)->z_	= base.e_.z_ + left_top.e_.z_;
		BM3_DST(VertexElement::PC1)->x_	= base.e_.x_ + left_bottom.e_.x_;
		BM3_DST(VertexElement::PC1)->y_	= base.e_.y_ + left_bottom.e_.y_;
		BM3_DST(VertexElement::PC1)->z_	= base.e_.z_ + left_bottom.e_.z_;
		BM3_DST(VertexElement::PC2)->x_	= base.e_.x_ + right_bottom.e_.x_;
		BM3_DST(VertexElement::PC2)->y_	= base.e_.y_ + right_bottom.e_.y_;
		BM3_DST(VertexElement::PC2)->z_	= base.e_.z_ + right_bottom.e_.z_;
		BM3_DST(VertexElement::PC3)->x_	= base.e_.x_ + right_top.e_.x_;
		BM3_DST(VertexElement::PC3)->y_	= base.e_.y_ + right_top.e_.y_;
		BM3_DST(VertexElement::PC3)->z_	= base.e_.z_ + right_top.e_.z_;
	}
	template<typename DstType>
	BM3_FORCEINLINE void Set(DstType *dst, ml::ni::vector3d &base, ml::ni::vector3d &left_top, ml::ni::vector3d &right_top, int stride){
		BM3_DST(VertexElement::PC0)->x_	= base.e_.x_ + left_top.e_.x_;
		BM3_DST(VertexElement::PC0)->y_	= base.e_.y_ + left_top.e_.y_;
		BM3_DST(VertexElement::PC0)->z_	= base.e_.z_ + left_top.e_.z_;
		BM3_DST(VertexElement::PC1)->x_	= base.e_.x_ - right_top.e_.x_;
		BM3_DST(VertexElement::PC1)->y_	= base.e_.y_ - right_top.e_.y_;
		BM3_DST(VertexElement::PC1)->z_	= base.e_.z_ - right_top.e_.z_;
		BM3_DST(VertexElement::PC2)->x_	= base.e_.x_ - left_top.e_.x_;
		BM3_DST(VertexElement::PC2)->y_	= base.e_.y_ - left_top.e_.y_;
		BM3_DST(VertexElement::PC2)->z_	= base.e_.z_ - left_top.e_.z_;
		BM3_DST(VertexElement::PC3)->x_	= base.e_.x_ + right_top.e_.x_;
		BM3_DST(VertexElement::PC3)->y_	= base.e_.y_ + right_top.e_.y_;
		BM3_DST(VertexElement::PC3)->z_	= base.e_.z_ + right_top.e_.z_;
	}
};

template<>
struct Position4<DrawScale::XY, DrawRotate::DirectionalPolyline>{

	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void Push(DstType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const PacketDrawInfo & /*pdi*/, int stride = sizeof(DstType)){
		ML_PTR_ASSERT(dst);

		ml::ni::vector3d p;
		MakeCenterPosition(p, prim, matrix);

		ml::matrix44 m;
		MakeMatrix(m, prim, matrix, draw_info, p);
		BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_POSITION4_MakeQuadEdgePos(m, matrix_row_length);

		Set(dst, p, left_top, left_bottom, right_bottom, right_top, stride);
	}

	// BM2互換
	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void PushBm2(DstType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const PacketDrawInfo & /*pdi*/, int stride = sizeof(DstType)){
		ML_PTR_ASSERT(dst);

		ml::ni::vector3d p;
		MakeCenterPosition(p, prim, matrix);

		ml::matrix44 m;
		MakeMatrixBm2(m, prim, matrix, draw_info, p);
		BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_POSITION4_MakeQuadEdgePos_ForBM2_ParentScall(m, matrix_row_length);


		Set(dst, p, left_top, left_bottom, right_bottom, right_top, stride);
	}

	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void PushNoField(DstType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const PacketDrawInfo & /*pdi*/, int stride = sizeof(DstType)){
		ML_PTR_ASSERT(dst);

		ml::ni::vector3d p;
		MakeCenterPositionNoField(p, prim, matrix);

		ml::matrix44 m;
		MakeMatrix(m, prim, matrix, draw_info, p);
		BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_POSITION4_MakeQuadEdgePos(m, matrix_row_length);

		Set(dst, p, left_top, left_bottom, right_bottom, right_top, stride);
	}

	// BM2互換
	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void PushNoFieldBm2(DstType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const PacketDrawInfo & /*pdi*/, int stride = sizeof(DstType)){
		ML_PTR_ASSERT(dst);

		ml::ni::vector3d p;
		MakeCenterPositionNoField(p, prim, matrix);

		ml::matrix44 m;
		MakeMatrixBm2(m, prim, matrix, draw_info, p);
		BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_POSITION4_MakeQuadEdgePos_ForBM2_ParentScall(m, matrix_row_length);

		Set(dst, p, left_top, left_bottom, right_bottom, right_top, stride);
	}

	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void PushFixQuad(DstType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const PacketDrawInfo & /*pdi*/, int stride = sizeof(DstType)){
		ML_PTR_ASSERT(dst);

		ml::ni::vector3d p;
		MakeCenterPosition(p, prim, matrix);

		ml::matrix44 m;
		MakeMatrix(m, prim, matrix, draw_info, p);
		BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_POSITION2_MakeQuadEdgePos(m, matrix_row_length);

		if(res.Basic_Meta_OptimizeBit() & OptimizeBit::FixQuadPivot){
			ml::vector3d	pivot;
			pivot.mul(res.Deformation_Basic_CenterPosition(), matrix_row_length);
			pivot.mul33(m);
			p.add(pivot);
		}
		Set(dst, p, left_top, right_top, stride);
	}

	// BM2互換
	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void PushFixQuadBm2(DstType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const PacketDrawInfo & /*pdi*/, int stride = sizeof(DstType)){
		ML_PTR_ASSERT(dst);

		ml::ni::vector3d p;
		MakeCenterPosition(p, prim, matrix);

		ml::matrix44 m;
		MakeMatrixBm2(m, prim, matrix, draw_info, p);
		BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_POSITION2_MakeQuadEdgePos_ForBM2_ParentScall(m, matrix_row_length);

		if(res.Basic_Meta_OptimizeBit() & OptimizeBit::FixQuadPivot){
			ml::vector3d	pivot;
			pivot.mul33(res.Deformation_Basic_CenterPosition(), m);
			pivot.mul33(matrix);
			p.add(pivot);
		}
		Set(dst, p, left_top, right_top, stride);
	}

	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void PushFixQuadNoField(DstType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const PacketDrawInfo & /*pdi*/, int stride = sizeof(DstType)){
		ML_PTR_ASSERT(dst);

		ml::ni::vector3d p;
		MakeCenterPositionNoField(p, prim, matrix);

		ml::matrix44 m;
		MakeMatrix(m, prim, matrix, draw_info, p);
		BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_POSITION2_MakeQuadEdgePos(m, matrix_row_length);

		if(res.Basic_Meta_OptimizeBit() & OptimizeBit::FixQuadPivot){
			ml::vector3d	pivot;
			pivot.mul(res.Deformation_Basic_CenterPosition(), matrix_row_length);
			pivot.mul33(m);
			p.add(pivot);
		}
		Set(dst, p, left_top, right_top, stride);
	}

	// BM2互換
	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void PushFixQuadNoFieldBm2(DstType *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const PacketDrawInfo & /*pdi*/, int stride = sizeof(DstType)){
		ML_PTR_ASSERT(dst);

		ml::ni::vector3d p;
		MakeCenterPositionNoField(p, prim, matrix);

		ml::matrix44 m;
		MakeMatrixBm2(m, prim, matrix, draw_info, p);
		BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_POSITION2_MakeQuadEdgePos_ForBM2_ParentScall(m, matrix_row_length);

		if(res.Basic_Meta_OptimizeBit() & OptimizeBit::FixQuadPivot){
			ml::vector3d	pivot;
			pivot.mul33(res.Deformation_Basic_CenterPosition(), m);
			pivot.mul33(matrix);
			p.add(pivot);
		}
		Set(dst, p, left_top, right_top, stride);
	}
private:
	template<typename PrimitiveType>
	BM3_FORCEINLINE void MakeMatrix(ml::matrix44 &dst, const PrimitiveType &prim, const ml::matrix44 & /*matrix*/, DrawInfo &info, ml::vector3d &position){

		// directionがゼロの場合は一度も動いたことがないということ、それは最初だけ。		
		if(prim.GetDirection().is_unit()){
			dst.set_scale(0.0f,0.0f,0.0f);	// 見えないようにする	
		}
		else{
			dst.set_scale(prim.scale_.value_);
			dst.mul_rot_xyz(prim.rotate_.value_);

			MakeDierctionalPolylineMatrix(dst, prim, position, info);
		}
	}

	// BM2互換
	template<typename PrimitiveType>
	BM3_FORCEINLINE void MakeMatrixBm2(ml::matrix44 &dst, const PrimitiveType &prim, const ml::matrix44 & /*matrix*/, DrawInfo &info, ml::vector3d &position){

		// directionがゼロの場合は一度も動いたことがないということ、それは最初だけ。		
		if(prim.GetDirection().is_unit()){	
			dst.set_scale(0.0f,0.0f,0.0f);// 見えないようにする
		}
		else{
			dst.set_scale(prim.scale_.value_);
			dst.mul_rot_zxy(prim.rotate_.value_);

			MakeDierctionalPolylineMatrix(dst, prim, position, info);
		}
	}

	template<typename DstType>
	BM3_FORCEINLINE void Set(DstType *dst, ml::ni::vector3d &base, ml::ni::vector3d &left_top, ml::ni::vector3d &left_bottom, ml::ni::vector3d &right_bottom, ml::ni::vector3d &right_top, int stride){

		BM3_DST(VertexElement::PC0)->x_	= base.e_.x_ + left_top.e_.x_;
		BM3_DST(VertexElement::PC0)->y_	= base.e_.y_ + left_top.e_.y_;
		BM3_DST(VertexElement::PC0)->z_	= base.e_.z_ + left_top.e_.z_;
		BM3_DST(VertexElement::PC1)->x_	= base.e_.x_ + left_bottom.e_.x_;
		BM3_DST(VertexElement::PC1)->y_	= base.e_.y_ + left_bottom.e_.y_;
		BM3_DST(VertexElement::PC1)->z_	= base.e_.z_ + left_bottom.e_.z_;
		BM3_DST(VertexElement::PC2)->x_	= base.e_.x_ + right_bottom.e_.x_;
		BM3_DST(VertexElement::PC2)->y_	= base.e_.y_ + right_bottom.e_.y_;
		BM3_DST(VertexElement::PC2)->z_	= base.e_.z_ + right_bottom.e_.z_;
		BM3_DST(VertexElement::PC3)->x_	= base.e_.x_ + right_top.e_.x_;
		BM3_DST(VertexElement::PC3)->y_	= base.e_.y_ + right_top.e_.y_;
		BM3_DST(VertexElement::PC3)->z_	= base.e_.z_ + right_top.e_.z_;
	}
	template<typename DstType>
	BM3_FORCEINLINE void Set(DstType *dst, ml::ni::vector3d &base, ml::ni::vector3d &left_top, ml::ni::vector3d &right_top, int stride){
		BM3_DST(VertexElement::PC0)->x_	= base.e_.x_ + left_top.e_.x_;
		BM3_DST(VertexElement::PC0)->y_	= base.e_.y_ + left_top.e_.y_;
		BM3_DST(VertexElement::PC0)->z_	= base.e_.z_ + left_top.e_.z_;
		BM3_DST(VertexElement::PC1)->x_	= base.e_.x_ - right_top.e_.x_;
		BM3_DST(VertexElement::PC1)->y_	= base.e_.y_ - right_top.e_.y_;
		BM3_DST(VertexElement::PC1)->z_	= base.e_.z_ - right_top.e_.z_;
		BM3_DST(VertexElement::PC2)->x_	= base.e_.x_ - left_top.e_.x_;
		BM3_DST(VertexElement::PC2)->y_	= base.e_.y_ - left_top.e_.y_;
		BM3_DST(VertexElement::PC2)->z_	= base.e_.z_ - left_top.e_.z_;
		BM3_DST(VertexElement::PC3)->x_	= base.e_.x_ + right_top.e_.x_;
		BM3_DST(VertexElement::PC3)->y_	= base.e_.y_ + right_top.e_.y_;
		BM3_DST(VertexElement::PC3)->z_	= base.e_.z_ + right_top.e_.z_;
	}
};

template<>
struct Position4<DrawScale::XY, DrawRotate::Facing>{

	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void Push(DstType *dst, const PrimitiveType &prim, DrawInfo & /*draw_info*/, const ResourceType &res, const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const PacketDrawInfo &pdi, int stride = sizeof(DstType)){
		ML_PTR_ASSERT(dst);

		ml::ni::vector3d p;
		MakeCenterPosition(p, prim, matrix);
		ml::ni::fast::matrix44 m;
		MakeMatrix(m, prim, p, pdi);

		BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_POSITION4_MakeQuadEdgePos(m, matrix_row_length);

		Set(dst, p, left_top, left_bottom, right_bottom, right_top, stride);
	}

	// BM2互換
	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void PushBm2(DstType *dst, const PrimitiveType &prim, DrawInfo & /*draw_info*/, const ResourceType &res, const ml::matrix44 &matrix, const ml::vector3d &/*matrix_row_length*/, const PacketDrawInfo &pdi, int stride = sizeof(DstType)){
		ML_PTR_ASSERT(dst);

		ml::ni::vector3d p;
		MakeCenterPosition(p, prim, matrix);
		ml::ni::fast::matrix44 m;
		MakeMatrixBm2(m, prim, p, pdi);

		BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_POSITION4_MakeQuadEdgePos_ForBM2(m, matrix);

		Set(dst, p, left_top, left_bottom, right_bottom, right_top, stride);
	}

	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void PushNoField(DstType *dst, const PrimitiveType &prim, DrawInfo & /*draw_info*/, const ResourceType &res, const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const PacketDrawInfo &pdi, int stride = sizeof(DstType)){
		ML_PTR_ASSERT(dst);

		ml::ni::vector3d p;
		MakeCenterPositionNoField(p, prim, matrix);
		ml::ni::fast::matrix44 m;
		MakeMatrix(m, prim, p, pdi);

		BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_POSITION4_MakeQuadEdgePos(m, matrix_row_length);

		Set(dst, p, left_top, left_bottom, right_bottom, right_top, stride);
	}

	// BM2互換
	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void PushNoFieldBm2(DstType *dst, const PrimitiveType &prim, DrawInfo & /*draw_info*/, const ResourceType &res, const ml::matrix44 &matrix, const ml::vector3d &/*matrix_row_length*/, const PacketDrawInfo &pdi, int stride = sizeof(DstType)){
		ML_PTR_ASSERT(dst);

		ml::ni::vector3d p;
		MakeCenterPositionNoField(p, prim, matrix);
		ml::ni::fast::matrix44 m;
		MakeMatrixBm2(m, prim, p, pdi);

		BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_POSITION4_MakeQuadEdgePos_ForBM2(m, matrix);

		Set(dst, p, left_top, left_bottom, right_bottom, right_top, stride);
	}

	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void PushFixQuad(DstType *dst, const PrimitiveType &prim, DrawInfo & /*draw_info*/, const ResourceType &res, const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const PacketDrawInfo &pdi, int stride = sizeof(DstType)){
		ML_PTR_ASSERT(dst);

		ml::ni::vector3d p;
		MakeCenterPosition(p, prim, matrix);
		ml::ni::fast::matrix44 m;
		MakeMatrix(m, prim, p, pdi);

		BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_POSITION2_MakeQuadEdgePos(m, matrix_row_length);

		if(res.Basic_Meta_OptimizeBit() & OptimizeBit::FixQuadPivot){
			ml::vector3d	pivot;
			pivot.mul(res.Deformation_Basic_CenterPosition(), matrix_row_length);
			pivot.mul33(m);
			p.add(pivot);
		}
		Set(dst, p, left_top, right_top, stride);
	}

	// BM2互換
	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void PushFixQuadBm2(DstType *dst, const PrimitiveType &prim, DrawInfo & /*draw_info*/, const ResourceType &res, const ml::matrix44 &matrix, const ml::vector3d &/*matrix_row_length*/, const PacketDrawInfo &pdi, int stride = sizeof(DstType)){
		ML_PTR_ASSERT(dst);

		ml::ni::vector3d p;
		MakeCenterPosition(p, prim, matrix);
		ml::ni::fast::matrix44 m;
		MakeMatrixBm2(m, prim, p, pdi);

		BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_POSITION2_MakeQuadEdgePos_ForBM2(m, matrix);

		if(res.Basic_Meta_OptimizeBit() & OptimizeBit::FixQuadPivot){
			ml::vector3d	pivot;
			pivot.mul33(res.Deformation_Basic_CenterPosition(), m);
			pivot.mul33(matrix);
			p.add(pivot);
		}
		Set(dst, p, left_top, right_top, stride);
	}

	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void PushFixQuadNoField(DstType *dst, const PrimitiveType &prim, DrawInfo & /*draw_info*/, const ResourceType &res, const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const PacketDrawInfo &pdi, int stride = sizeof(DstType)){
		ML_PTR_ASSERT(dst);

		ml::ni::vector3d p;
		MakeCenterPositionNoField(p, prim, matrix);
		ml::ni::fast::matrix44 m;
		MakeMatrix(m, prim, p, pdi);

		BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_POSITION2_MakeQuadEdgePos(m, matrix_row_length);

		if(res.Basic_Meta_OptimizeBit() & OptimizeBit::FixQuadPivot){
			ml::vector3d	pivot;
			pivot.mul(res.Deformation_Basic_CenterPosition(), matrix_row_length);
			pivot.mul33(m);
			p.add(pivot);
		}
		Set(dst, p, left_top, right_top, stride);
	}

	// BM2互換
	template<typename PrimitiveType, typename ResourceType, typename DstType>
	BM3_FORCEINLINE void PushFixQuadNoFieldBm2(DstType *dst, const PrimitiveType &prim, DrawInfo & /*draw_info*/, const ResourceType &res, const ml::matrix44 &matrix, const ml::vector3d &/*matrix_row_length*/, const PacketDrawInfo &pdi, int stride = sizeof(DstType)){
		ML_PTR_ASSERT(dst);

		ml::ni::vector3d p;
		MakeCenterPositionNoField(p, prim, matrix);
		ml::ni::fast::matrix44 m;
		MakeMatrixBm2(m, prim, p, pdi);

		BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_POSITION2_MakeQuadEdgePos_ForBM2(m, matrix);

		if(res.Basic_Meta_OptimizeBit() & OptimizeBit::FixQuadPivot){
			ml::vector3d	pivot;
			pivot.mul33(res.Deformation_Basic_CenterPosition(), m);
			pivot.mul33(matrix);
			p.add(pivot);
		}
		Set(dst, p, left_top, right_top, stride);
	}

private:
	template<typename PrimitiveType>
	BM3_FORCEINLINE void MakeMatrix(ml::matrix44 &dst, const PrimitiveType &prim, const ml::vector3d &p, const PacketDrawInfo &pdi){
		dst.set_scale(prim.scale_.value_);
		dst.mul_rot_z(prim.rotate_.value_.e_.z_);
		ml::matrix44	view;
#ifdef BM3_TARGET_IDE
		view.lookat_r(p, pdi.facing_position_, ml::vector3d(0,1,0,0));
#else
		view.lookat_r(pdi.facing_position_,	p, ml::vector3d(0,1,0,0));
#endif
		view.inverse();
		dst.mul33(view);
	}

	// BM2互換
	template<typename PrimitiveType>
	BM3_FORCEINLINE void MakeMatrixBm2(ml::matrix44 &dst, const PrimitiveType &prim, const ml::vector3d &p, const PacketDrawInfo &pdi){
		dst.set_scale(prim.scale_.value_);
		dst.mul_rot_z(prim.rotate_.value_.e_.z_);
		ml::matrix44	view;
#ifdef BM3_TARGET_IDE
		view.lookat_r(p, pdi.facing_position_, ml::vector3d(0,1,0,0));
#else
		view.lookat_r(pdi.facing_position_,	p, ml::vector3d(0,1,0,0));
#endif
		view.inverse();
		dst.mul33(view);
	}

	template<typename DstType>
	BM3_FORCEINLINE void Set(DstType *dst, ml::ni::vector3d &base, ml::ni::vector3d &left_top, ml::ni::vector3d &left_bottom, ml::ni::vector3d &right_bottom, ml::ni::vector3d &right_top, int stride){

		BM3_DST(VertexElement::PC0)->x_	= base.e_.x_ + left_top.e_.x_;
		BM3_DST(VertexElement::PC0)->y_	= base.e_.y_ + left_top.e_.y_;
		BM3_DST(VertexElement::PC0)->z_	= base.e_.z_ + left_top.e_.z_;
		BM3_DST(VertexElement::PC1)->x_	= base.e_.x_ + left_bottom.e_.x_;
		BM3_DST(VertexElement::PC1)->y_	= base.e_.y_ + left_bottom.e_.y_;
		BM3_DST(VertexElement::PC1)->z_	= base.e_.z_ + left_bottom.e_.z_;
		BM3_DST(VertexElement::PC2)->x_	= base.e_.x_ + right_bottom.e_.x_;
		BM3_DST(VertexElement::PC2)->y_	= base.e_.y_ + right_bottom.e_.y_;
		BM3_DST(VertexElement::PC2)->z_	= base.e_.z_ + right_bottom.e_.z_;
		BM3_DST(VertexElement::PC3)->x_	= base.e_.x_ + right_top.e_.x_;
		BM3_DST(VertexElement::PC3)->y_	= base.e_.y_ + right_top.e_.y_;
		BM3_DST(VertexElement::PC3)->z_	= base.e_.z_ + right_top.e_.z_;
	}
	template<typename DstType>
	BM3_FORCEINLINE void Set(DstType *dst, ml::ni::vector3d &base, ml::ni::vector3d &left_top, ml::ni::vector3d &right_top, int stride){
		BM3_DST(VertexElement::PC0)->x_	= base.e_.x_ + left_top.e_.x_;
		BM3_DST(VertexElement::PC0)->y_	= base.e_.y_ + left_top.e_.y_;
		BM3_DST(VertexElement::PC0)->z_	= base.e_.z_ + left_top.e_.z_;
		BM3_DST(VertexElement::PC1)->x_	= base.e_.x_ - right_top.e_.x_;
		BM3_DST(VertexElement::PC1)->y_	= base.e_.y_ - right_top.e_.y_;
		BM3_DST(VertexElement::PC1)->z_	= base.e_.z_ - right_top.e_.z_;
		BM3_DST(VertexElement::PC2)->x_	= base.e_.x_ - left_top.e_.x_;
		BM3_DST(VertexElement::PC2)->y_	= base.e_.y_ - left_top.e_.y_;
		BM3_DST(VertexElement::PC2)->z_	= base.e_.z_ - left_top.e_.z_;
		BM3_DST(VertexElement::PC3)->x_	= base.e_.x_ + right_top.e_.x_;
		BM3_DST(VertexElement::PC3)->y_	= base.e_.y_ + right_top.e_.y_;
		BM3_DST(VertexElement::PC3)->z_	= base.e_.z_ + right_top.e_.z_;
	}

};

//#undef BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_POSITION4_MakeQuadEdgePos

#undef BM3_DST

}	// namespace policy
}	// namespace draw
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_POSITION4_H
