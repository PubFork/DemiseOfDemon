#ifndef BM3_SDK_INC_BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_MATRIX1_H
#define BM3_SDK_INC_BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_MATRIX1_H

#include "bm3/system/bm3_Config.h"
#include "bm3_draw_policy_Common.h"
#include "../../bm3_DrawInfo.h"
#include "../../bm3_CoreType.h"
#include "bm3_draw_policy_Common.h"
#include <ml/type/ml_matrix44.h>
#include <ml/math/ml_triangular.h>

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE
namespace draw{
namespace policy{

template<
	DrawScale::Type		DrawScaleType,
	DrawRotate::Type	DrawRotateType
>
struct Matrix1{
	template<typename PrimitiveType, typename ResourceType>
	BM3_FORCEINLINE void Push(ml::matrix44 *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res ,const ml::matrix44 &matrix, const ml::vector3d & /*matrix_row_length*/, const PacketDrawInfo & /*pdi*/);
};

template<>
struct Matrix1<DrawScale::Nothing, DrawRotate::Nothing>{

	template<typename PrimitiveType, typename ResourceType>
	BM3_FORCEINLINE void Push(ml::matrix44 *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res ,const ml::matrix44 &matrix, const ml::vector3d & /*matrix_row_length*/, const PacketDrawInfo & /*pdi*/){
		ML_PTR_ASSERT(dst);

		dst->set_unit();
		MakeMatrixTranslate(*dst, prim ,matrix);
	}

	// BM2互換
	template<typename PrimitiveType, typename ResourceType>
	BM3_FORCEINLINE void PushBm2(ml::matrix44 *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res ,const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const PacketDrawInfo &pdi){
		Push(dst, prim, draw_info, res , matrix, matrix_row_length, pdi);
	}

};

template<>
struct Matrix1<DrawScale::XYZ, DrawRotate::Nothing>{

	template<typename PrimitiveType, typename ResourceType>
	BM3_FORCEINLINE void Push(ml::matrix44 *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res ,const ml::matrix44 &matrix, const ml::vector3d & /*matrix_row_length*/, const PacketDrawInfo & /*pdi*/){
		ML_PTR_ASSERT(dst);

		dst->set_scale(prim.scale_.value_);
		MakeMatrixTranslate(*dst, prim ,matrix);
	}

	// BM2互換
	template<typename PrimitiveType, typename ResourceType>
	BM3_FORCEINLINE void PushBm2(ml::matrix44 *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res ,const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const PacketDrawInfo &pdi){
		Push(dst, prim, draw_info, res , matrix, matrix_row_length, pdi);
	}
};

// -------------------------------------------------------------------------------
// xyz

template<>
struct Matrix1<DrawScale::Nothing, DrawRotate::XYZ>{

	template<typename PrimitiveType, typename ResourceType>
	BM3_FORCEINLINE void Push(ml::matrix44 *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res ,const ml::matrix44 &matrix, const ml::vector3d & /*matrix_row_length*/, const PacketDrawInfo & /*pdi*/){
		ML_PTR_ASSERT(dst);

		dst->set_rot_x(prim.rotate_.value_.x_);
		dst->mul_rot_y(prim.rotate_.value_.y_);
		dst->mul_rot_z(prim.rotate_.value_.z_);
		MakeMatrixTranslate(*dst, prim ,matrix);
	}

	// BM2互換
	template<typename PrimitiveType, typename ResourceType>
	BM3_FORCEINLINE void PushBm2(ml::matrix44 *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res ,const ml::matrix44 &matrix, const ml::vector3d & /*matrix_row_length*/, const PacketDrawInfo & /*pdi*/){
		ML_PTR_ASSERT(dst);

		dst->set_rot_z(prim.rotate_.value_.z_);
		dst->mul_rot_x(prim.rotate_.value_.x_);
		dst->mul_rot_y(prim.rotate_.value_.y_);
		MakeMatrixTranslate(*dst, prim ,matrix);
	}
};

template<>
struct Matrix1<DrawScale::XYZ, DrawRotate::XYZ>{

	template<typename PrimitiveType, typename ResourceType>
	BM3_FORCEINLINE void Push(ml::matrix44 *dst, const PrimitiveType &prim, DrawInfo &/*draw_info*/, const ResourceType &/*res */,const ml::matrix44 &matrix, const ml::vector3d & /*matrix_row_length*/, const PacketDrawInfo & /*pdi*/){
		ML_PTR_ASSERT(dst);

		dst->set_scale(prim.scale_.value_);
		dst->mul_rot_x(prim.rotate_.value_.e_.x_);
		dst->mul_rot_y(prim.rotate_.value_.e_.y_);
		dst->mul_rot_z(prim.rotate_.value_.e_.z_);
		MakeMatrixTranslate(*dst, prim ,matrix);
	}

	// BM2互換
	template<typename PrimitiveType, typename ResourceType>
	BM3_FORCEINLINE void PushBm2(ml::matrix44 *dst, const PrimitiveType &prim, DrawInfo &/*draw_info*/, const ResourceType &/*res */,const ml::matrix44 &matrix, const ml::vector3d & /*matrix_row_length*/, const PacketDrawInfo & /*pdi*/){
		ML_PTR_ASSERT(dst);

		dst->set_scale(prim.scale_.value_);
		dst->mul_rot_z(prim.rotate_.value_.e_.z_);
		dst->mul_rot_x(prim.rotate_.value_.e_.x_);
		dst->mul_rot_y(prim.rotate_.value_.e_.y_);
		MakeMatrixTranslate(*dst, prim ,matrix);
	}
};

// -------------------------------------------------------------------------------
// billboard

template<>
struct Matrix1<DrawScale::Nothing, DrawRotate::Billboard_Z>{

	template<typename PrimitiveType, typename ResourceType>
	BM3_FORCEINLINE void Push(ml::matrix44 *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res ,const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const PacketDrawInfo & /*pdi*/){
		ML_PTR_ASSERT(dst);

		dst->set_rot_z(prim.rotate_.value_.e_.z_);
//		MakeMatrixTranslate(*dst, prim ,matrix);
		
		ml::vector3d	p;
		p.mul(prim.translate_.value_, matrix);
		p.add(prim.field_info_.position_);
		
		{
			ml::matrix44	m;
			m.set_unit();
			m.set_scale(matrix_row_length);
			dst->mul33(m);
		}

//		ml::vector3d translate = dst->row(ml::matrix44::row::translate);
		dst->mul(draw_info.InverseViewMatrix());
//		dst->row(ml::matrix44::row::translate) = translate;
		dst->row(ml::matrix44::row::translate) = p;
	}

	// BM2互換
	template<typename PrimitiveType, typename ResourceType>
	BM3_FORCEINLINE void PushBm2(ml::matrix44 *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res ,const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const PacketDrawInfo &pdi){
		Push(dst, prim, draw_info, res , matrix, matrix_row_length, pdi);
	}

};

template<>
struct Matrix1<DrawScale::XYZ, DrawRotate::Billboard_Z>{

	template<typename PrimitiveType, typename ResourceType>
	BM3_FORCEINLINE void Push(ml::matrix44 *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &/*res */,const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const PacketDrawInfo & /*pdi*/){
		ML_PTR_ASSERT(dst);

		dst->set_scale(prim.scale_.value_);
		dst->mul_rot_z(prim.rotate_.value_.e_.z_);
//		MakeMatrixTranslate(*dst, prim ,matrix);

		ml::vector3d	p;
		p.mul(prim.translate_.value_, matrix);
		p.add(prim.field_info_.position_);
		
		{
			ml::matrix44	m;
			m.set_unit();
			m.set_scale(matrix_row_length);
			dst->mul33(m);
		}
		
//		ml::vector3d translate = dst->row(ml::matrix44::row::translate);
		dst->mul(draw_info.InverseViewMatrix());
//		dst->row(ml::matrix44::row::translate) = translate;
		dst->row(ml::matrix44::row::translate) = p;
	}

	// BM2互換
	template<typename PrimitiveType, typename ResourceType>
	BM3_FORCEINLINE void PushBm2(ml::matrix44 *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const PacketDrawInfo &pdi){
		Push(dst, prim, draw_info, res , matrix, matrix_row_length, pdi);
	}
};

// -------------------------------------------------------------------------------
// directional

template<>
struct Matrix1<DrawScale::Nothing, DrawRotate::Directional>{

	template<typename PrimitiveType, typename ResourceType>
	BM3_FORCEINLINE void Push(ml::matrix44 *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res ,const ml::matrix44 &matrix, const ml::vector3d & /*matrix_row_length*/, const PacketDrawInfo & /*pdi*/){
		ML_PTR_ASSERT(dst);

		// directionがゼロの場合は一度も動いたことがないということ、それは最初だけ。
		// この場合は見えないようにする
		if(prim.GetDirection().is_unit()){	
			dst->set_scale(0.0f, 0.0f, 0.0f);
		}
		else{
			dst->set_unit();
			dst->mul_rot_zxy(prim.rotate_.value_);

			MakeMatrix(*dst, prim, matrix);
		}
	}

	// BM2互換
	template<typename PrimitiveType, typename ResourceType>
	BM3_FORCEINLINE void PushBm2(ml::matrix44 *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res ,const ml::matrix44 &matrix, const ml::vector3d & /*matrix_row_length*/, const PacketDrawInfo & /*pdi*/){
		ML_PTR_ASSERT(dst);

		// directionがゼロの場合は一度も動いたことがないということ、それは最初だけ。
		// この場合は見えないようにする
		if(prim.GetDirection().is_unit()){	dst->set_scale(0.0f, 0.0f, 0.0f);			}
		else{								MakeMatrixTranslate(*dst, prim ,matrix);	}


		// directionがゼロの場合は一度も動いたことがないということ、それは最初だけ。
		// この場合は見えないようにする
		if(prim.GetDirection().is_unit()){	
			dst->set_scale(0.0f, 0.0f, 0.0f);
		}
		else{
			dst->set_unit();
			dst->mul_rot_zxy(prim.rotate_.value_);

			MakeMatrix(*dst, prim, matrix);
		}
	}

	template<typename PrimitiveType>
	BM3_FORCEINLINE void MakeMatrix(ml::matrix44 &dst, const PrimitiveType &prim, const ml::matrix44 &matrix){
		ml::vector3d	position;
		position.mul(prim.translate_.value_ ,matrix);
		position.add(prim.field_info_.position_);
		MakeDirectionalMatrix(dst, prim, position);
		MakeMatrixTranslate(dst, prim ,matrix);
	}

};

template<>
struct Matrix1<DrawScale::XYZ, DrawRotate::Directional>{

	template<typename PrimitiveType, typename ResourceType>
	BM3_FORCEINLINE void Push(ml::matrix44 *dst, const PrimitiveType &prim, DrawInfo & /*draw_info*/, const ResourceType & /*res*/, const ml::matrix44 &matrix, const ml::vector3d & /*matrix_row_length*/, const PacketDrawInfo & /*pdi*/){
		ML_PTR_ASSERT(dst);

		// directionがゼロの場合は一度も動いたことがないということ、それは最初だけ。
		// この場合は見えないようにする
		if(prim.GetDirection().is_unit()){	
			dst->set_scale(0.0f, 0.0f, 0.0f);
		}
		else{
			dst->set_unit();
			dst->set_scale(prim.scale_.value_);
			dst->mul_rot_xyz(prim.rotate_.value_);

			MakeMatrix(*dst, prim, matrix);
		}

	}

	// BM2互換
	template<typename PrimitiveType, typename ResourceType>
	BM3_FORCEINLINE void PushBm2(ml::matrix44 *dst, const PrimitiveType &prim, DrawInfo & /*draw_info*/, const ResourceType & /*res*/, const ml::matrix44 &matrix, const ml::vector3d & /*matrix_row_length*/, const PacketDrawInfo & /*pdi*/){
		ML_PTR_ASSERT(dst);

		// directionがゼロの場合は一度も動いたことがないということ、それは最初だけ。
		// この場合は見えないようにする
		if(prim.GetDirection().is_unit()){	
			dst->set_scale(0.0f, 0.0f, 0.0f);
		}
		else{
			dst->set_unit();
			dst->set_scale(prim.scale_.value_);
			dst->mul_rot_zxy(prim.rotate_.value_);

			MakeMatrix(*dst, prim, matrix);
		}
	}

	template<typename PrimitiveType>
	BM3_FORCEINLINE void MakeMatrix(ml::matrix44 &dst, const PrimitiveType &prim, const ml::matrix44 &matrix){
		ml::vector3d	position;
		position.mul(prim.translate_.value_ ,matrix);
		position.add(prim.field_info_.position_);
		MakeDirectionalMatrix(dst, prim, position);
		MakeMatrixTranslate(dst, prim ,matrix);
	}
};

// -------------------------------------------------------------------------------
// directional polyline

template<>
struct Matrix1<DrawScale::Nothing, DrawRotate::DirectionalPolyline>{

	template<typename PrimitiveType, typename ResourceType>
	BM3_FORCEINLINE void Push(ml::matrix44 *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const PacketDrawInfo &pdi){
		ML_PTR_ASSERT(dst);

		// directionがゼロの場合は一度も動いたことがないということ、それは最初だけ。
		// この場合は見えないようにする
		if(prim.GetDirection().is_unit()){	
			dst->set_scale(0.0f, 0.0f, 0.0f);
		}
		else{
			dst->set_unit();
			dst->mul_rot_xyz(prim.rotate_.value_);

			MakeMatrix(*dst, prim ,matrix, draw_info);
		}
	}

	// BM2互換
	template<typename PrimitiveType, typename ResourceType>
	BM3_FORCEINLINE void PushBm2(ml::matrix44 *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res,const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const PacketDrawInfo &pdi){
		ML_PTR_ASSERT(dst);

		// directionがゼロの場合は一度も動いたことがないということ、それは最初だけ。
		// この場合は見えないようにする
		if(prim.GetDirection().is_unit()){	
			dst->set_scale(0.0f, 0.0f, 0.0f);
		}
		else{
			dst->set_unit();
			dst->mul_rot_zxy(prim.rotate_.value_);

			MakeMatrix(*dst, prim ,matrix, draw_info);
		}
	}

	template<typename PrimitiveType>
	BM3_FORCEINLINE void MakeMatrix(ml::matrix44 &dst, const PrimitiveType &prim, const ml::matrix44 &matrix, DrawInfo &draw_info){
		ml::vector3d	position;
		position.mul(prim.translate_.value_ ,matrix);
		position.add(prim.field_info_.position_);
		MakeDierctionalPolylineMatrix(dst, prim, position, draw_info);
		MakeMatrixTranslate(dst, prim ,matrix);
	}
};

template<>
struct Matrix1<DrawScale::XYZ, DrawRotate::DirectionalPolyline>{

	template<typename PrimitiveType, typename ResourceType>
	BM3_FORCEINLINE void Push(ml::matrix44 *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType & /*res*/, const ml::matrix44 &matrix, const ml::vector3d & /*matrix_row_length*/, const PacketDrawInfo & /*pdi*/){
		ML_PTR_ASSERT(dst);

		// directionがゼロの場合は一度も動いたことがないということ、それは最初だけ。
		// この場合は見えないようにする
		if(prim.GetDirection().is_unit()){	
			dst->set_scale(0.0f, 0.0f, 0.0f);
		}
		else{
			dst->set_unit();
			dst->set_scale(prim.scale_.value_);
			dst->mul_rot_xyz(prim.rotate_.value_);

			MakeMatrix(*dst, prim ,matrix, draw_info);
		}
	}

	// BM2互換
	template<typename PrimitiveType, typename ResourceType>
	BM3_FORCEINLINE void PushBm2(ml::matrix44 *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType & /*res*/,const ml::matrix44 &matrix, const ml::vector3d & /*matrix_row_length*/, const PacketDrawInfo & /*pdi*/){
		ML_PTR_ASSERT(dst);

		// directionがゼロの場合は一度も動いたことがないということ、それは最初だけ。
		// この場合は見えないようにする
		if(prim.GetDirection().is_unit()){	
			dst->set_scale(0.0f, 0.0f, 0.0f);
		}
		else{
			dst->set_unit();
			dst->set_scale(prim.scale_.value_);
			dst->mul_rot_zxy(prim.rotate_.value_);

			MakeMatrix(*dst, prim ,matrix, draw_info);
		}
	}

	template<typename PrimitiveType>
	BM3_FORCEINLINE void MakeMatrix(ml::matrix44 &dst, const PrimitiveType &prim, const ml::matrix44 &matrix, DrawInfo &draw_info){
		ml::vector3d	position;
		position.mul(prim.translate_.value_ ,matrix);
		position.add(prim.field_info_.position_);
		MakeDierctionalPolylineMatrix(dst, prim, position, draw_info);
		MakeMatrixTranslate(dst, prim ,matrix);
	}
};
	
// -------------------------------------------------------------------------------
// facing

template<>
struct Matrix1<DrawScale::Nothing, DrawRotate::Facing>{

	template<typename PrimitiveType, typename ResourceType>
	BM3_FORCEINLINE void Push(ml::matrix44 *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res ,const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const PacketDrawInfo &pdi){
		ML_PTR_ASSERT(dst);

		dst->set_rot_z(prim.rotate_.value_.e_.z_);
#if 1
		ml::vector3d	p;
		p.mul(prim.translate_.value_, matrix);
		p.add(prim.field_info_.position_);
		
		{
			ml::matrix44	m;
			m.set_unit();
			m.set_scale(matrix_row_length);
			dst->mul33(m);
		}
		{
			ml::matrix44	view;
#ifdef BM3_TARGET_IDE
			view.lookat_r(p, pdi.facing_position_,	ml::vector3d(0,1,0,0));
#else
			view.lookat_r(pdi.facing_position_,	p,	ml::vector3d(0,1,0,0));
#endif
			view.inverse();
			dst->mul33(view);
		}
		dst->row(ml::matrix44::row::translate) = p;
	
#else
		
		MakeMatrixTranslate(*dst, prim ,matrix);
		ml::ni::vector3d	p	= dst->translate();
		{
			ml::matrix44	view;
#ifdef BM3_TARGET_IDE
			view.lookat_r(p, pdi.facing_position_,	ml::vector3d(0,1,0,0));
#else
			view.lookat_r(pdi.facing_position_,	p,	ml::vector3d(0,1,0,0));
#endif
			view.inverse();
			dst->mul33(view);
		}
		dst->row(ml::matrix44::row::translate) = p;
#endif		
	}

	// BM2互換
	template<typename PrimitiveType, typename ResourceType>
	BM3_FORCEINLINE void PushBm2(ml::matrix44 *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res ,const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const PacketDrawInfo &pdi){
		Push(dst, prim, draw_info, res , matrix, matrix_row_length, pdi);
	}

};

template<>
struct Matrix1<DrawScale::XYZ, DrawRotate::Facing>{

	template<typename PrimitiveType, typename ResourceType>
	BM3_FORCEINLINE void Push(ml::matrix44 *dst, const PrimitiveType &prim, DrawInfo & /*draw_info*/, const ResourceType &/*res */,const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const PacketDrawInfo &pdi){

		ML_PTR_ASSERT(dst);

		dst->set_scale(prim.scale_.value_);
		dst->mul_rot_z(prim.rotate_.value_.e_.z_);

#if 1
		ml::vector3d	p;
		p.mul(prim.translate_.value_, matrix);
		p.add(prim.field_info_.position_);
		
		{
			ml::matrix44	m;
			m.set_unit();
			m.set_scale(matrix_row_length);
			dst->mul33(m);
		}
		{
			ml::matrix44	view;
			view.lookat_r(pdi.facing_position_,	p,	ml::vector3d(0,1,0,0));
			view.inverse();
			dst->mul33(view);
		}
		dst->row(ml::matrix44::row::translate) = p;
		
#else
		MakeMatrixTranslate(*dst, prim ,matrix);

		ml::ni::vector3d	p	= dst->translate();

		{
			ml::matrix44	view;
			view.lookat_r(pdi.facing_position_,	p,	ml::vector3d(0,1,0,0));
			view.inverse();
			dst->mul33(view);
		}
		dst->row(ml::matrix44::row::translate) = p;
#endif
	}

	// BM2互換
	template<typename PrimitiveType, typename ResourceType>
	BM3_FORCEINLINE void PushBm2(ml::matrix44 *dst, const PrimitiveType &prim, DrawInfo &draw_info, const ResourceType &res, const ml::matrix44 &matrix, const ml::vector3d &matrix_row_length, const PacketDrawInfo &pdi){
		Push(dst, prim, draw_info, res , matrix, matrix_row_length, pdi);
	}

};	

}	// namespace policy
}	// namespace draw
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_MATRIX1_H
