#ifndef BM3_SDK_INC_BM3_CORE_PRIMITIVE_BM3_FIELD_RADIALPOWER_H
#define BM3_SDK_INC_BM3_CORE_PRIMITIVE_BM3_FIELD_RADIALPOWER_H

#include "bm3_field_Common.h"
#include "bm3/core/policy/Field/bm3_policy_Radialpower_Magnitude_Constant.h"
#include "bm3/core/policy/Field/bm3_policy_Radialpower_Magnitude_Curve.h"
#include "bm3/core/policy/Field/bm3_policy_Radialpower_Position_Constant.h"
#include "bm3/core/policy/Field/bm3_policy_Radialpower_Position_Curve.h"
#include "bm3/core/policy/Field/bm3_policy_Radialpower_Area_Constant.h"
#include "bm3/core/policy/Field/bm3_policy_Radialpower_Area_Curve.h"
#if defined(BM3_TARGET_IDE)
#include "../bm3_Debug.h"
#endif

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

class DrawInfo;

namespace field{
namespace radialpower{

/// @brief Fieldクラス
struct Field{
	ml::vector3d	value_;
	ml::vector3d	speed_;
	float			magnitude_;
	ml::vector3d	position_;
	float			area_;

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

		field.value_.set_unit();
		field.speed_.set_unit();
		policy::impl::Radialpower_Magnitude_Constant::Initialize(	res, random, field.magnitude_	);
		policy::impl::Radialpower_Magnitude_Curve::Initialize(		res, random, field.magnitude_	);

		policy::impl::Radialpower_Position_Constant::Initialize(	res, random, field.position_	);
		policy::impl::Radialpower_Position_Curve::Initialize(		res, random, field.position_	);

		policy::impl::Radialpower_Area_Constant::Initialize(		res, random, field.area_		);
		policy::impl::Radialpower_Area_Curve::Initialize(			res, random, field.area_		);

	}

	template<
		typename UpdateInfo,
		typename FieldType,
		typename PrimitiveType,
		typename ResourceType
	>
	static void Update(UpdateInfo &update_info, FieldType &field, const ResourceType &res, const PrimitiveType & /*prim*/, const PrimitiveUpdateInfo &primitive_update_info){

		policy::impl::Radialpower_Magnitude_Constant::Update(	update_info, res, field.magnitude_							);
		policy::impl::Radialpower_Magnitude_Curve::Update(		update_info, res, field.magnitude_,	primitive_update_info	);

		policy::impl::Radialpower_Position_Constant::Update(	update_info, res, field.position_							);
		policy::impl::Radialpower_Position_Curve::Update(		update_info, res, field.position_,	primitive_update_info	);

		policy::impl::Radialpower_Area_Constant::Update(		update_info, res, field.area_								);
		policy::impl::Radialpower_Area_Curve::Update(			update_info, res, field.area_,		primitive_update_info	);
	}

	template<
		typename FieldType,
		typename PrimitiveType,
		typename ResourceType
	>
	static void Prepare(FieldInfo &dst, FieldType &field, const ResourceType &res, const ml::matrix44 &matrix, UpdateInfo & /*update_info*/, const obj::RootNodeBase &root, PrimitiveType &prim){

		if(res.Basic_Meta_Enable()){

			ml::ni::vector3d	translate;
			translate.mul(prim.GetTranslate(), matrix);
			translate.add(field.value_);
			translate.add(dst.position_);
			{
				ml::ni::vector3d	pos;
				pos.mul(field.position_, root.GenerateTimeHierarchyParentMatrix());
				translate.sub(pos);
			}

			float	len = translate.length();
			if((res.Field_Area_FieldAreaType() == FieldAreaTypeConst_Infinit) || (len < field.area_)){

				float	mag = field.magnitude_;
				if((len != 0.0f) && (res.Field_Magnitude_Damp() != 0.0f)){
					mag = (0<mag) ? 
						ml::minimum((float)FLT_MAX, mag * ml::inverse(res.Field_Magnitude_Damp() * len)) :
						ml::maximum(-(float)FLT_MAX, mag * ml::inverse(res.Field_Magnitude_Damp() * len)) ;
//					mag *= ml::inverse(ml::abs(mag) * res.Field_Magnitude_Damp() * len * 0.06f);	// 引力-斥力の減衰の割合が強さが正のとき効いていないバグ解消（#780）（0.06は今までの過去のものとの整合性を合わす重み）
				}
				if(len < ml::abs(mag)) mag = (mag<0) ? -len : len;
				translate.normalize().mul(mag);

				if(res.Field_Influence_FieldInfluenceType()==FieldInfluenceTypeConst_Position){
					field.value_.add(translate);
					dst.position_.add(field.value_);
				}else
				if(res.Field_Influence_FieldInfluenceType()==FieldInfluenceTypeConst_Speed){
					field.speed_.add(translate);
					field.value_.add(field.speed_);
					dst.position_.add(field.value_);
				}
			}
			else{
				dst.position_.add(field.value_);
			}
		}
	}
	
	template<
		typename FieldType,
		typename ResourceType
	>
	static void SwitchResource(FieldType &field, const ResourceType &res, const ResourceType &old_res, ml::random &random){
		policy::impl::Radialpower_Magnitude_Constant::SwitchResource(	res, old_res, random, field.magnitude_	);
		policy::impl::Radialpower_Magnitude_Curve::SwitchResource(		res, old_res, random, field.magnitude_	);
																		 
		policy::impl::Radialpower_Position_Constant::SwitchResource(	res, old_res, random, field.position_	);
		policy::impl::Radialpower_Position_Curve::SwitchResource(		res, old_res, random, field.position_	);
																		 
		policy::impl::Radialpower_Area_Constant::SwitchResource(		res, old_res, random, field.area_		);
		policy::impl::Radialpower_Area_Curve::SwitchResource(			res, old_res, random, field.area_		);
	}

	template<typename ResourceType,	typename LifePolicy>
	static void Draw(DrawInfo &draw_info, const ResourceType &res, const ml::matrix44 &matrix, LifePolicy &life){
#if defined(BM3_TARGET_IDE)
		if(res.Basic_Meta_Enable() && res.Basic_Meta_Display() && res.Basic_Basic_AreaDisplay()){

			ml::vector3d	position;
			switch(res.Field_CenterPosition_FieldPositionType()){
				case FieldPositionTypeConst_Constant:	position	= res.Field_CenterPosition_Initial();						break;
				case FieldPositionTypeConst_Curve:		position	= res.Field_CenterPosition_Curve().Value(life.LifeRate());	break;
				default:	ML_ASSERT(false);	break;
			}

			ml::vector3d	area;
			switch(res.Field_Area_FieldAreaType()){
				case FieldAreaTypeConst_Constant: 	area	= ml::vector3d(res.Field_Area_Initial(), res.Field_Area_Initial(), res.Field_Area_Initial(), 1.0f);	break;
				case FieldAreaTypeConst_Curve:
					{
						float a = res.Field_Area_Curve().Value(life.LifeRate());
						area	= ml::vector3d(a,a,a,0);	
					}
					break;
				case FieldAreaTypeConst_Infinit:	area.set_unit();	break;
				default:	break;
			}

			debug::SphereArea angle(0.0f,360.0f, 0.0f, 180.0f);

			debug::DrawPoint(draw_info,		matrix, position);
			debug::DrawSphere(draw_info,	matrix, position, area, angle);
		}
#endif
	}

};

}	// namespace radialpower
}	// namespace field
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_PRIMITIVE_BM3_FIELD_RADIALPOWER_H
