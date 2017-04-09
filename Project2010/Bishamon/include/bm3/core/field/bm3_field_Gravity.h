#ifndef BM3_SDK_INC_BM3_CORE_PRIMITIVE_BM3_FIELD_GRAVITY_H
#define BM3_SDK_INC_BM3_CORE_PRIMITIVE_BM3_FIELD_GRAVITY_H

#include "bm3_field_Common.h"
#include "bm3/core/object/bm3_obj_RootNodeBase.h"
#include "bm3/core/policy/Field/bm3_policy_Gravity_Magnitude_Constant.h"
#include "bm3/core/policy/Field/bm3_policy_Gravity_Magnitude_Curve.h"
#include "bm3/core/policy/Field/bm3_policy_Gravity_Direction_Constant.h"
#include "bm3/core/policy/Field/bm3_policy_Gravity_Direction_Curve.h"


namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

class DrawInfo;

namespace field{
namespace gravity{

/// @brief Fieldクラス
struct Field{
	float			magnitude_;
	float			speed_;
	ml::vector3d	direction_;
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
		
		unsigned int type = (res.Field_Magnitude_FieldMagnitudeType() << 0) | 
							(res.Field_Direction_FieldDirectionType() << 2);

#define BM3_GRAVITY_TYPE(MAG, DIR)	(MAG << 0) | (DIR << 2)
		switch(type){
			case BM3_GRAVITY_TYPE(FieldMagnitudeTypeConst_Constant, FieldDirectionTypeConst_Constant):
				policy::impl::Gravity_Magnitude_Constant::Initialize(	res, random ,field.magnitude_,	field.speed_	);
				policy::impl::Gravity_Direction_Constant::Initialize(	res, random ,field.direction_	);
				break;
			case BM3_GRAVITY_TYPE(FieldMagnitudeTypeConst_Constant, FieldDirectionTypeConst_Curve):
				policy::impl::Gravity_Magnitude_Constant::Initialize(	res, random ,field.magnitude_,	field.speed_	);
				policy::impl::Gravity_Direction_Curve::Initialize(		res, random ,field.direction_	);
				break;
			case BM3_GRAVITY_TYPE(FieldMagnitudeTypeConst_Curve, FieldDirectionTypeConst_Constant):
				policy::impl::Gravity_Magnitude_Curve::Initialize(		res, random ,field.magnitude_,	field.speed_	);
				policy::impl::Gravity_Direction_Constant::Initialize(	res, random ,field.direction_	);
				break;
			case BM3_GRAVITY_TYPE(FieldMagnitudeTypeConst_Curve, FieldDirectionTypeConst_Curve):
				policy::impl::Gravity_Magnitude_Curve::Initialize(		res, random ,field.magnitude_,	field.speed_	);
				policy::impl::Gravity_Direction_Curve::Initialize(		res, random ,field.direction_	);
				break;
		}

#undef	BM3_GRAVITY_TYPE
	}

	template<
		typename UpdateInfo,
		typename FieldType,
		typename PrimitiveType,
		typename ResourceType
	>
	static void Update(UpdateInfo &update_info, FieldType &field, const ResourceType &res, const PrimitiveType & /*prim*/, const PrimitiveUpdateInfo &primitive_update_info){

		unsigned int type = (res.Field_Magnitude_FieldMagnitudeType() << 0) | 
							(res.Field_Direction_FieldDirectionType() << 2);
#define BM3_GRAVITY_TYPE(MAG, DIR)	(MAG << 0) | (DIR << 2)
		switch(type){
			case BM3_GRAVITY_TYPE(FieldMagnitudeTypeConst_Constant, FieldDirectionTypeConst_Constant):
				policy::impl::Gravity_Magnitude_Constant::Update(	update_info, res, field.magnitude_,	field.speed_							);
				policy::impl::Gravity_Direction_Constant::Update(	update_info, res, field.direction_							);
				break;
			case BM3_GRAVITY_TYPE(FieldMagnitudeTypeConst_Constant, FieldDirectionTypeConst_Curve):
				policy::impl::Gravity_Magnitude_Constant::Update(	update_info, res, field.magnitude_,	field.speed_							);
				policy::impl::Gravity_Direction_Curve::Update(		update_info, res, field.direction_,	primitive_update_info	);
				break;
			case BM3_GRAVITY_TYPE(FieldMagnitudeTypeConst_Curve, FieldDirectionTypeConst_Constant):
				policy::impl::Gravity_Magnitude_Curve::Update(		update_info, res, field.magnitude_,	field.speed_,	primitive_update_info	);
				policy::impl::Gravity_Direction_Constant::Update(	update_info, res, field.direction_							);
				break;
			case BM3_GRAVITY_TYPE(FieldMagnitudeTypeConst_Curve, FieldDirectionTypeConst_Curve):
				policy::impl::Gravity_Magnitude_Curve::Update(		update_info, res, field.magnitude_,	field.speed_,	primitive_update_info	);
				policy::impl::Gravity_Direction_Curve::Update(		update_info, res, field.direction_,	primitive_update_info	);
				break;
		}
#undef	BM3_GRAVITY_TYPE
	}

	template<
		typename FieldType,
		typename PrimitiveType,
		typename ResourceType
	>
	static void Prepare(FieldInfo &dst, FieldType &field, const ResourceType &res, const ml::matrix44 &matrix, UpdateInfo & /*update_info*/, const obj::RootNodeBase &/*root*/, PrimitiveType &/*prim*/){

		if(res.Basic_Meta_Enable()){
#ifdef BM3_PLATFORM_psp
			ml::ni::vector3d scale = matrix.row_length();
			__asm__ (
				".set			push\n"				
				".set			noreorder\n"		
				"lv.t			c000, %2\n"			
				"lv.s			s010, %3\n"			
				"vscl.t			c100, c000, s010\n"	
				"lv.t			c020, %1\n"			
				"vmul.t			c110, c100, c020\n"	
				"lv.t			c030, %0\n"			
				"vadd.t			c120, c110, c030\n"	
				"sv.t			c120, %0\n"		
				".set			pop\n"				
				: "=m"(dst.position_.v_)
				: "m"(scale.v_), "m"(field.direction_.v_), "m"(&field.magnitude_)
			);
#else
			ml::ni::vector3d mag;
			ml::ni::vector3d scale = matrix.row_length();

			mag.mul(field.direction_, field.magnitude_);
			mag.mul(scale);
			dst.position_.add(mag);
#endif
		}
	}
	
	template<
		typename FieldType,
		typename ResourceType
	>
	static void SwitchResource(FieldType &field, const ResourceType &res, const ResourceType &old_res, ml::random &random){
//		policy::impl::Gravity_Magnitude_Constant::SwitchResource(	res, old_res, random ,field.magnitude_,	field.speed_	);
//		policy::impl::Gravity_Magnitude_Curve::SwitchResource(		res, old_res, random ,field.magnitude_,	field.speed_	);

		policy::impl::Gravity_Direction_Constant::SwitchResource(	res, old_res, random ,field.direction_	);
		policy::impl::Gravity_Direction_Curve::SwitchResource(		res, old_res, random ,field.direction_	);
	}	

	template<
		typename ResourceType,
		typename LifePolicy
	>
	static void Draw(DrawInfo &draw_info, const ResourceType &res, const ml::matrix44 &matrix, LifePolicy &life){
	}

};

}	// namespace gravity
}	// namespace field
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_PRIMITIVE_BM3_FIELD_GRAVITY_H
