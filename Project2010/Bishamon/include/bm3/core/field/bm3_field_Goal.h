#ifndef BM3_SDK_INC_BM3_CORE_PRIMITIVE_BM3_FIELD_GOAL_H
#define BM3_SDK_INC_BM3_CORE_PRIMITIVE_BM3_FIELD_GOAL_H

#include "bm3_field_Common.h"
#include "bm3/core/object/bm3_obj_RootNodeBase.h"
#include "bm3/core/policy/Field/bm3_policy_Goal_Position_Constant.h"
#include "bm3/core/policy/Field/bm3_policy_Goal_Position_Curve.h"
#include "bm3/core/policy/Field/bm3_policy_Goal_ResourceSwitch.h"
#include "bm3/core/policy/bm3_policy_PositionMaker.h"
#if defined(BM3_TARGET_IDE)
#include "../bm3_Debug.h"
#endif


namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

class DrawInfo;

namespace field{
namespace goal{

/// @brief Fieldクラス
struct Field{
	ml::vector3d	position_;
	ml::vector3d	offset_position_;
};

/// @brief FieldManipulatorクラス
class FieldManipulator{

public:
	template<
		typename FieldType,
		typename PrimitiveType,
		typename ResourceType
	>
	static void Initialize(FieldType &field, const ResourceType &res, ml::random &random, PrimitiveType & /*prim*/){

		ml::matrix44		slope;

		switch(res.Field_Basic_FieldGoalShapeType()){
			case FieldGoalShapeTypeConst_Point:
				policy::impl::PositionMaker::Point(field.offset_position_, random);
				break;
			case FieldGoalShapeTypeConst_Sphere:
				policy::impl::PositionMaker::Sphere(
					field.offset_position_, random,
					res.Field_Area_Inside() ,res.Field_Area_Outside(),
					res.Field_HorizontalArea_StartAngle(), res.Field_HorizontalArea_EndAngle(),
					res.Field_VerticalArea_StartAngle(), res.Field_VerticalArea_EndAngle()
				);
				slope.mul_rot_xyz(res.Field_Area_Slope());
				field.offset_position_.mul(slope);
				break;
			case FieldGoalShapeTypeConst_Cylinder:
				policy::impl::PositionMaker::Cylinder(
					field.offset_position_, random,
					res.Field_Area_Inside() ,res.Field_Area_Outside(),
					res.Field_HorizontalArea_StartAngle(), res.Field_HorizontalArea_EndAngle()
				);
				slope.mul_rot_xyz(res.Field_Area_Slope());
				field.offset_position_.mul(slope);
				break;
			case FieldGoalShapeTypeConst_Cube:
				policy::impl::PositionMaker::Cube(
					field.offset_position_, random,
					res.Field_Area_Inside() ,res.Field_Area_Outside()
				);
				slope.mul_rot_xyz(res.Field_Area_Slope());
				field.offset_position_.mul(slope);
				break;
			case FieldGoalShapeTypeConst_Model:
				{
					ml::vector3d dummy;
					policy::impl::PositionMaker::Model(
						field.offset_position_, dummy, random, res.Field_Basic_ModelName(), res.Field_Area_Scale()
					);
				}
				break;
			default:
				ML_ASSERT(0);
				break;
		}

		switch(res.Field_Position_FieldPosType()){
			case FieldPosTypeConst_Constant:	policy::impl::Goal_Position_Constant::Initialize(	res, random ,field.position_, field.offset_position_);	break;
			case FieldPosTypeConst_Curve:		policy::impl::Goal_Position_Curve::Initialize(		res, random ,field.position_, field.offset_position_);	break;
			default:	ML_ASSERT(false);	break;
		}
	}

	template<
		typename UpdateInfo,
		typename FieldType,
		typename PrimitiveType,
		typename ResourceType
	>
	static void Update(UpdateInfo &update_info, FieldType &field, const ResourceType &res, const PrimitiveType & /*prim*/, const PrimitiveUpdateInfo &primitive_update_info){

		if(res.Field_Position_FieldPosType()==FieldPosTypeConst_Curve){
//			policy::impl::Goal_Position_Constant::Update(	update_info, res, field.position_ , field.offset_position_							);
			policy::impl::Goal_Position_Curve::Update(		update_info, res, field.position_ , field.offset_position_,	primitive_update_info	);
		}
	}

	template<
		typename FieldType,
		typename PrimitiveType,
		typename ResourceType
	>
	static void Prepare(FieldInfo &dst, FieldType &field, const ResourceType &res, const ml::matrix44 &matrix, UpdateInfo & /*update_info*/, const obj::RootNodeBase &root, PrimitiveType &prim){

		if(res.Basic_Meta_Enable()){

			float	life_rate = prim.LifeRate();
			if(res.Field_Term_Start()<=life_rate){

				// 現在の位置を算出
				ml::ni::vector3d	pos, field_pos;

				pos.mul(prim.translate_.value_ ,matrix);
				pos.add(dst.position_);
				field_pos.mul(field.position_, root.HierarchyMatrix());

				// 有効期間内にゴールへたどり着く
				if(life_rate<=res.Field_Term_End()){

					ml::ni::vector3d	dir = field_pos - pos;
					float				rate = (life_rate - res.Field_Term_Start()) * ml::inverse(res.Field_Term_End() - res.Field_Term_Start());
					dir.mul(rate);
					dst.position_.add(dir);
				}
				else{
					// 期間を過ぎたらその位置に留まるようにしている
					// ここは選択できた方がいいかも
					dst.position_.add(field_pos - pos);
				}
			}
		}
	}
	
	template<
		typename FieldType,
		typename ResourceType
	>
	static void SwitchResource(FieldType &field, const ResourceType &res, const ResourceType &old_res, ml::random &random){
		
		policy::impl::Goal_Position_ResourceSwitch::Switch(	res, old_res, random ,field.position_ ,field.offset_position_);
	}

	template<
		typename ResourceType,
		typename LifePolicy
	>
	static void Draw(DrawInfo &draw_info, const ResourceType &res, const ml::matrix44 &matrix, LifePolicy &life){
#if defined(BM3_TARGET_IDE)
		if(res.Basic_Meta_Enable() && res.Basic_Meta_Display() && res.Basic_Basic_AreaDisplay()){

			ml::vector3d	position;
			ml::matrix44	slope;
			ml::vector3d	inner;
			debug::SphereArea		angle(
					res.Field_HorizontalArea_StartAngle(), res.Field_HorizontalArea_EndAngle(),
					res.Field_VerticalArea_StartAngle(), res.Field_VerticalArea_EndAngle()
				);

			switch(res.Field_Position_FieldPosType()){
				case FieldPosTypeConst_Constant:	position	= res.Field_Position_Initial();							break;
				case FieldPosTypeConst_Curve:		position	= res.Field_Position_Curve().Value(life.LifeRate());	break;
				default:	ML_ASSERT(false);	break;
			}
			
			slope.mul_rot_xyz(res.Field_Area_Slope());
			slope.mul(matrix);
			inner.mul(res.Field_Area_Outside(), res.Field_Area_Inside());

			switch(res.Field_Basic_FieldGoalShapeType()){
				case FieldGoalShapeTypeConst_Point:
					debug::DrawPoint(draw_info,		slope, position);
					break;
				case FieldGoalShapeTypeConst_Sphere:
					debug::DrawSphere(draw_info,	slope, position, inner, angle);
					debug::DrawSphere(draw_info,	slope, position, res.Field_Area_Outside(), angle);
					break;
				case FieldGoalShapeTypeConst_Cylinder:
					debug::DrawCylinder(draw_info,	slope, position, inner, angle);
					debug::DrawCylinder(draw_info,	slope, position, res.Field_Area_Outside(), angle);
					break;
				case FieldGoalShapeTypeConst_Cube:
					debug::DrawCube(draw_info, slope, position, inner);
					debug::DrawCube(draw_info, slope, position, res.Field_Area_Outside());
					break;
				case FieldGoalShapeTypeConst_Model:
					{
						ml::matrix44	m;
						m.set_unit();
						m.set_scale(res.Field_Area_Scale());
						m.set_translate(position);
						ModelRenderer::instance().DebugDraw(res.Field_Basic_ModelName(), m, draw_info);
					}
				break;
			}
		}
#endif
	}

};

}	// namespace gravity
}	// namespace field
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_PRIMITIVE_BM3_FIELD_GOAL_H
