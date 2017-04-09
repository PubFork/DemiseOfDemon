#ifndef BM3_SDK_INC_BM3_CORE_PRIMITIVE_BM3_FIELD_DECELERATION_H
#define BM3_SDK_INC_BM3_CORE_PRIMITIVE_BM3_FIELD_DECELERATION_H

#include "bm3_field_Common.h"
#include "bm3/core/policy/Field/bm3_policy_Deceleration_Magnitude_Constant.h"
#include "bm3/core/policy/Field/bm3_policy_Deceleration_Magnitude_Curve.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

class DrawInfo;

namespace field{
namespace deceleration{

///@ brief Fieldクラス
struct Field{
	float			magnitude_;
	ml::vector3d	total_dec_value_;
};

///@ brief FieldManipulatorクラス
class FieldManipulator{

public:
	template<
		typename FieldType,
		typename PrimitiveType,
		typename ResourceType
	>
	static void Initialize(FieldType &field, const ResourceType &res, ml::random &random, PrimitiveType &/*prim*/){

		field.total_dec_value_.set_unit();
		switch(res.Field_Magnitude_FieldMagnitudeType()){
			case FieldMagnitudeTypeConst_Constant:	policy::impl::Deceleration_Magnitude_Constant::Initialize(	res, random, field.magnitude_	);	break;
			case FieldMagnitudeTypeConst_Curve:		policy::impl::Deceleration_Magnitude_Curve::Initialize(		res, random, field.magnitude_	);	break;
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

		switch(res.Field_Magnitude_FieldMagnitudeType()){
			case FieldMagnitudeTypeConst_Constant:	policy::impl::Deceleration_Magnitude_Constant::Update(	update_info, res, field.magnitude_							);	break;
			case FieldMagnitudeTypeConst_Curve:		policy::impl::Deceleration_Magnitude_Curve::Update(		update_info, res, field.magnitude_,	primitive_update_info	);	break;
			default:	ML_ASSERT(false);	break;
		}
#if 0
		{
			ml::ni::vector3d	now_speed;
			now_speed.mul(prim.translate_.speed_, field.magnitude_);
			{
				field.total_dec_value_.add(now_speed);
			}
		}
#endif		
	}

	template<
		typename FieldType,
		typename PrimitiveType,
		typename ResourceType
	>
	static void Prepare(FieldInfo &dst, FieldType &field, const ResourceType &res, const ml::matrix44 &matrix, UpdateInfo &update_info, const obj::RootNodeBase &/*root*/, PrimitiveType &prim){

		if(res.Basic_Meta_Enable()){
#if 1
			ml::ni::vector3d	now_speed;
			now_speed.mul(prim.translate_.speed_, field.magnitude_ *update_info.GetUpdateFrame());
			{
				ml::matrix44	mm = matrix;
				mm.row(ml::matrix44::row::x).normalize();
				mm.row(ml::matrix44::row::y).normalize();
				mm.row(ml::matrix44::row::z).normalize();
		
				now_speed	= field.total_dec_value_.add(now_speed);
				now_speed.mul33(mm);
			}
			dst.position_.sub(now_speed);
#else
			ml::vector3d	pos = field.total_dec_value_;
			{
				ml::matrix44	mm = matrix;
				mm.row(ml::matrix44::row::x).normalize();
				mm.row(ml::matrix44::row::y).normalize();
				mm.row(ml::matrix44::row::z).normalize();

				pos.mul33(mm);
			}
			dst.position_.sub(pos);
#endif
		}
	}
	
	template<
		typename FieldType,
		typename ResourceType
	>
	static void SwitchResource(FieldType &field, const ResourceType &res, const ResourceType &old_res, ml::random &random){
		policy::impl::Deceleration_Magnitude_Constant::SwitchResource(	res, old_res, random, field.magnitude_	);
		policy::impl::Deceleration_Magnitude_Curve::SwitchResource(		res, old_res, random, field.magnitude_	);
	}	

	template<
		typename ResourceType,
		typename LifePolicy
	>
	static void Draw(DrawInfo &draw_info, const ResourceType &res, const ml::matrix44 &matrix, LifePolicy &life){
	}

};

}	// namespace deceleration
}	// namespace field
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_PRIMITIVE_BM3_FIELD_DECELERATION_H
