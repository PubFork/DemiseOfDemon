#ifndef BM3_SDK_INC_BM3_CORE_PRIMITIVE_BM3_FIELD_WIND_H
#define BM3_SDK_INC_BM3_CORE_PRIMITIVE_BM3_FIELD_WIND_H

#include <ml/math/ml_math_const.h>
#include "bm3_field_Common.h"
#include "bm3/core/policy/Field/bm3_policy_Wind_Magnitude_Constant.h"
#include "bm3/core/policy/Field/bm3_policy_Wind_Magnitude_Curve.h"
#include "bm3/core/policy/Field/bm3_policy_Wind_Direction_Constant.h"
#include "bm3/core/policy/Field/bm3_policy_Wind_Direction_Curve.h"
#include "bm3/core/policy/Field/bm3_policy_Wind_Area_Constant.h"
#include "bm3/core/policy/Field/bm3_policy_Wind_Area_Curve.h"
#include "bm3/core/policy/Field/bm3_policy_Wind_Position_Constant.h"
#include "bm3/core/policy/Field/bm3_policy_Wind_Position_Curve.h"
#include "bm3/core/policy/Field/bm3_policy_Wind_ResourceSwitch.h"
#if defined(BM3_TARGET_IDE)
#include "../bm3_Debug.h"
#endif

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

class UpdateInfo;
class DrawInfo;

namespace field{
namespace wind{

/// @brief Fieldクラス
struct Field{
	ml::vector3d	value_;	// 風による位置の変化量
	ml::vector3d	speed_;	// 風による速度の変化量
	ml::vector3d	direction_;	// 風の方向
	ml::vector3d	area_;		// 風の範囲
	ml::vector3d	position_;	// 風の中心位置
	float			magnitude_;	// 風の強さ
	float			noise_;		// 
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
		policy::impl::Wind_Magnitude_Constant::Initialize(	res, random, field.magnitude_	, field.noise_);
		policy::impl::Wind_Magnitude_Curve::Initialize(		res, random, field.magnitude_	, field.noise_);

		{
//			ml::random	rand;
//			ml::random::seed_info	si;
//			si.set1(0);
//			rand.set_seed(si);
//			policy::impl::Wind_Direction_Constant::Initialize(	res, rand, field.direction_	);
//			policy::impl::Wind_Direction_Curve::Initialize(		res, rand, field.direction_	);
			policy::impl::Wind_Direction_Constant::Initialize(	res, random, field.direction_	);
			policy::impl::Wind_Direction_Curve::Initialize(		res, random, field.direction_	);
		}

		policy::impl::Wind_Area_Constant::Initialize(	res, random, field.area_	);
		policy::impl::Wind_Area_Curve::Initialize(		res, random, field.area_	);

		policy::impl::Wind_Position_Constant::Initialize(	res, random, field.position_	);
		policy::impl::Wind_Position_Curve::Initialize(		res, random, field.position_	);

	}

	template<
		typename UpdateInfo,
		typename FieldType,
		typename PrimitiveType,
		typename ResourceType
	>
	static void Update(UpdateInfo &update_info, FieldType &field, const ResourceType &res, const PrimitiveType & /*prim*/, const PrimitiveUpdateInfo &primitive_update_info){

		policy::impl::Wind_Magnitude_Constant::Update(	update_info, res, field.magnitude_, field.noise_							);
		policy::impl::Wind_Magnitude_Curve::Update(		update_info, res, field.magnitude_, field.noise_,	primitive_update_info	);

		{
//			PrimitiveUpdateInfo	pui;
//			pui.life_rate_		= primitive_update_info.life_rate_;
//			pui.random_seed_	= 0;
//			policy::impl::Wind_Direction_Constant::Update(	update_info, res, field.direction_							);
//			policy::impl::Wind_Direction_Curve::Update(		update_info, res, field.direction_,	pui	);
			policy::impl::Wind_Direction_Constant::Update(	update_info, res, field.direction_							);
			policy::impl::Wind_Direction_Curve::Update(		update_info, res, field.direction_,	primitive_update_info	);
		}

		policy::impl::Wind_Area_Constant::Update(	update_info, res, field.area_	);
		policy::impl::Wind_Area_Curve::Update(		update_info, res, field.area_	,primitive_update_info	);

		policy::impl::Wind_Position_Constant::Update(	update_info, res, field.position_	);
		policy::impl::Wind_Position_Curve::Update(		update_info, res, field.position_	,primitive_update_info	);
	}

	template<
		typename FieldType,
		typename PrimitiveType,
		typename ResourceType
	>
	static void Prepare(FieldInfo &dst, FieldType &field, const ResourceType &res, const ml::matrix44 &matrix, UpdateInfo &update_info, const obj::RootNodeBase &root, PrimitiveType &prim){


		if(res.Basic_Meta_Enable()){

			ml::ni::vector3d	field_pos;
			ml::ni::vector3d	prim_pos;

			prim_pos.mul(prim.translate_.value_, matrix);
			prim_pos.add(field.value_);
			prim_pos.add(dst.position_);
			field_pos.mul(field.position_, root.GenerateTimeHierarchyParentMatrix());

			if(IsInfluence(res, prim_pos, field_pos, field.area_)){
				ml::ni::vector3d	wind;
				ml::ni::vector3d	dir;

				dir	= field.direction_;
				dir.normalize();
/*				if(res.Field_Option_Noise()!=0.0f){
					ml::ni::vector3d	temp_dir;
					ml::random			random;
					{
						ml::random::seed_info	si;
						si.set1((int)(prim.LifeRate() * 100.0f) + ml::ui32(&field));
						random.set_seed(si);
					}
					temp_dir.e_.x_	= random.f32(-res.Field_Option_Noise(), res.Field_Option_Noise());
					temp_dir.e_.y_	= random.f32(-res.Field_Option_Noise(), res.Field_Option_Noise());
					temp_dir.e_.z_	= random.f32(-res.Field_Option_Noise(), res.Field_Option_Noise());
					dir.add(temp_dir);
				}
				*/		
				wind.mul(dir ,res.Field_Magnitude_ScaleInfluence() ? field.magnitude_ / PrimitiveParam::ScaleValue(prim.scale_) * update_info.GetUpdateFrame() : field.magnitude_ * update_info.GetUpdateFrame());
				field.speed_.add(wind);
			}
			field.value_.add(field.speed_);
			dst.position_.add(field.value_);
		}
	}

	template<
		typename FieldType,
		typename ResourceType
	>
	static void SwitchResource(FieldType &field, const ResourceType &res, const ResourceType &old_res, ml::random &random){
		policy::impl::Wind_Magnitude_ResourceSwitch::Switch(	res, old_res, random, field.magnitude_	, field.noise_);
		policy::impl::Wind_Direction_ResourceSwitch::Switch(	res, old_res, random, field.direction_	);
		policy::impl::Wind_Area_ResourceSwitch::Switch(			res, old_res, random, field.area_		);
		policy::impl::Wind_Position_ResourceSwitch::Switch(		res, old_res, random, field.position_	);
	}

	template<typename ResourceType,	typename LifePolicy>
	static void Draw(DrawInfo &draw_info, const ResourceType &res, const ml::matrix44 &matrix, LifePolicy &life){
#if defined(BM3_TARGET_IDE)
		if(res.Basic_Meta_Enable() && res.Basic_Meta_Display() && res.Basic_Basic_AreaDisplay()){

			ml::vector3d	position;
			switch(res.Field_Position_FieldPosType()){
				case FieldPosTypeConst_Constant:	position	= res.Field_Position_Initial();							break;
				case FieldPosTypeConst_Curve:		position	= res.Field_Position_Curve().Value(life.LifeRate());	break;
				default:	ML_ASSERT(false);	break;
			}

			ml::vector3d	area;
			switch(res.Field_Area_FieldAreaType()){
				case FieldAreaTypeConst_Constant: 	area	= res.Field_Area_Initial3d();						break;
				case FieldAreaTypeConst_Curve:		area	= res.Field_Area_Curve3d().Value(life.LifeRate());	break;
				case FieldAreaTypeConst_Infinit:	area.set_unit();	break;
				default:	break;
			}

			debug::DrawPoint(draw_info,	matrix, position);
			debug::DrawCube(draw_info, matrix, position, area);
		}
#endif
	}

private:

	template<typename ResourceType>
	static bool IsInfluence(const ResourceType &res, ml::vector3d &prim_position, ml::vector3d &field_position, ml::vector3d &area){
		if(res.Field_Area_FieldAreaType() != FieldAreaTypeConst_Infinit){
			
			ml::vector3d	min;
			min.sub(field_position, area);
			if(prim_position.e_.x_ < min.e_.x_)	return false;
			if(prim_position.e_.y_ < min.e_.y_)	return false;
			if(prim_position.e_.z_ < min.e_.z_)	return false;

			ml::vector3d	max;
			max.add(field_position, area);
			if(prim_position.e_.x_ > max.e_.x_)	return false;
			if(prim_position.e_.y_ > max.e_.y_)	return false;
			if(prim_position.e_.z_ > max.e_.z_)	return false;
		}
		return true;
	}
};

}	// namespace wind
}	// namespace field
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_PRIMITIVE_BM3_FIELD_WIND_H
