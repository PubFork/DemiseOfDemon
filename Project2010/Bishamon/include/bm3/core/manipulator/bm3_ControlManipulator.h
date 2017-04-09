#ifndef BM3_SDK_INC_BM3_CORE_MANIPULATOR_BM3_CONTROLMANIPULATOR_H
#define BM3_SDK_INC_BM3_CORE_MANIPULATOR_BM3_CONTROLMANIPULATOR_H

#include "../bm3_CoreType.h"	// PrimitiveUpdateInfo, PrimitivePrepareInfo
#include "../primitive/bm3_Primitive_Common.h"
#include "../policy/Matrix/bm3_policy_MatrixScale_Constant.h"
#include "../policy/Matrix/bm3_policy_MatrixScale_Newtondynamics.h"
#include "../policy/Matrix/bm3_policy_MatrixScale_CurveSpeed.h"
#include "../policy/Matrix/bm3_policy_MatrixScale_CurveValue.h"
#include "../policy/Matrix/bm3_policy_MatrixRotate_Constant.h"
#include "../policy/Matrix/bm3_policy_MatrixRotate_Newtondynamics.h"
#include "../policy/Matrix/bm3_policy_MatrixRotate_CurveSpeed.h"
#include "../policy/Matrix/bm3_policy_MatrixRotate_CurveValue.h"
#include "../policy/Matrix/bm3_policy_MatrixTranslate_Constant.h"
#include "../policy/Matrix/bm3_policy_MatrixTranslate_Newtondynamics.h"
#include "../policy/Matrix/bm3_policy_MatrixTranslate_CurveSpeed.h"
#include "../policy/Matrix/bm3_policy_MatrixTranslate_CurveValue.h"
#include "../policy/GenerateShape/bm3_policy_GenerateShape_Point.h"
#include "../policy/GenerateShape/bm3_policy_GenerateShape_Cube.h"
#include "../policy/GenerateShape/bm3_policy_GenerateShape_Sphere.h"
#include "../policy/GenerateShape/bm3_policy_GenerateShape_Cylinder.h"
#include "../policy/GenerateShape/bm3_policy_GenerateShape_Model.h"
#include "../policy/Matrix/bm3_policy_MatrixScale_SwitchResource.h"
#include "../policy/Matrix/bm3_policy_MatrixRotate_SwitchResource.h"
#include "../policy/Matrix/bm3_policy_MatrixTranslate_SwitchResource.h"
#include "../policy/Color/bm3_policy_Blend_SwitchResource.h"
#include "../policy/Color/bm3_policy_Blend_Constant.h"
#include "../policy/Color/bm3_policy_Blend_Curve.h"
#include "../policy/Color/bm3_policy_Color_Constant.h"
#include "../policy/Color/bm3_policy_Color_Curve.h"

#include <ml/type/ml_matrix44.h>

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

/// @brief ControlParamƒNƒ‰ƒX
template<typename LifePolicy>
struct ControlParam{
	LifePolicy			life_;
	prim::MatrixParam	scale_;
	prim::MatrixParam	rotate_;
	prim::MatrixParam	translate_;
	prim::GenerateShape	generate_shape_;
	prim::ColorParam	color_;	// color + alpha
	ml::vector3d		previous_position_;
	field::FieldInfo	field_info_;

	template<typename ResourceType>
	BM3_FORCEINLINE void	Initialize(const ResourceType &res, ml::random &random){
		life_.Initialize(res, random);
		life_.ForceActive();
		previous_position_.set_unit();
		field_info_.position_.set_unit();
	}

	template<typename UpdateInfo>
	BM3_FORCEINLINE void	UpdateLife(UpdateInfo &info){
		life_.Update(info);
	}

	template<typename ResourceType>
	BM3_FORCEINLINE void	SwitchResource(const ResourceType &res, ml::random &random){
		life_.SwitchResource(res, random);
	}

	BM3_FORCEINLINE bool	IsActive() const{		return life_.IsActive();		}
	BM3_FORCEINLINE bool	IsFirstFrame() const{	return life_.IsFirstFrame();	}
	BM3_FORCEINLINE bool	IsExpired() const{		return life_.IsExpired();		}
	BM3_FORCEINLINE float	LifeRate() const{		return life_.LifeRate();		}

	BM3_FORCEINLINE const ml::vector3d&	GetTranslate()	const{	return translate_.value_;	}
	BM3_FORCEINLINE const ml::vector3d&	GetRotate()		const{	return rotate_.value_;	}
	BM3_FORCEINLINE const ml::vector3d&	GetScale()		const{	return scale_.value_;	}
	BM3_FORCEINLINE const ColorType&	GetColor()		const{	return color_.value_;	}

	const field::FieldInfo	&GetFieldInfo() const { return field_info_; }

	BM3_FORCEINLINE const ml::vector3d&	GetPreviousPosition()	const{	return previous_position_;	}
	BM3_FORCEINLINE void SetPreviousPosition(const ml::matrix44 &matrix){
		ml::vector3d	v;
		v.add(translate_.value_, field_info_.position_);
		v.mul(matrix);
		if(v.is_equal(previous_position_) == false){
			previous_position_	= v;
		}
	}
#ifdef BM3_PLATFORM_playstation3
} __attribute__ ((aligned(128)));
#else
};	
#endif

class ControlManipulator{

public:
	template<typename ResourceType, typename LifePolicy>
	static void	Initialize(const ResourceType &res, ml::random &random, ControlParam<LifePolicy> &p, const PrimitiveInitializeInfo &pii){

		p.Initialize(res, random);

		switch(res.Scale_Basic_ScaleType()){
			case ScaleTypeConst_Constant:		policy::impl::MatrixScale_Constant<>::Initialize(			res, random, p.scale_.value_															);		break;
			case ScaleTypeConst_NewtonDynamics:	policy::impl::MatrixScale_Newtondynamics<>::Initialize(		res, random, p.scale_.value_,		p.scale_.speed_,		p.scale_.acceleration_		);		break;
			case ScaleTypeConst_CurveValue:		policy::impl::MatrixScale_CurveValue<>::Initialize(			res, random, p.scale_.value_															, pii);	break;
			case ScaleTypeConst_CurveSpeed:		policy::impl::MatrixScale_CurveSpeed<>::Initialize(			res, random, p.scale_.value_,		p.scale_.speed_									, pii);	break;
			default:	ML_ASSERT(false);		break;
		}

		switch(res.Rotate_Basic_RotateType()){
			case RotateTypeConst_Constant:			policy::impl::MatrixRotate_Constant::Initialize(			res, random, p.rotate_.value_															);		break;
			case RotateTypeConst_NewtonDynamics:	policy::impl::MatrixRotate_Newtondynamics::Initialize(		res, random, p.rotate_.value_,		p.rotate_.speed_,		p.rotate_.acceleration_	);		break;
			case RotateTypeConst_CurveValue:		policy::impl::MatrixRotate_CurveValue::Initialize(			res, random, p.rotate_.value_															, pii);	break;
			case RotateTypeConst_CurveSpeed:		policy::impl::MatrixRotate_CurveSpeed::Initialize(			res, random, p.rotate_.value_,		p.rotate_.speed_									, pii);	break;
			default:	ML_ASSERT(false);			break;
		}

		switch(res.GenerateShape_Basic_GenerateShapeType()){
			case GenerateShapeTypeConst_Point:		policy::impl::GenerateShape_Point::Initialize(			res, random, p.generate_shape_.position_ ,p.generate_shape_.direction_	);	break;
			case GenerateShapeTypeConst_Sphere:		policy::impl::GenerateShape_Sphere::Initialize(			res, random, p.generate_shape_.position_ ,p.generate_shape_.direction_	);	break;
			case GenerateShapeTypeConst_Cylinder:	policy::impl::GenerateShape_Cylinder::Initialize(		res, random, p.generate_shape_.position_ ,p.generate_shape_.direction_	);	break;
			case GenerateShapeTypeConst_Cube:		policy::impl::GenerateShape_Cube::Initialize(			res, random, p.generate_shape_.position_ ,p.generate_shape_.direction_	);	break;
			case GenerateShapeTypeConst_Model:		policy::impl::GenerateShape_Model::Initialize(			res, random, p.generate_shape_.position_ ,p.generate_shape_.direction_	);	break;
			default:	ML_ASSERT(false);			break;
		}

		switch(res.Translate_Basic_TranslateType()){
			case TranslateTypeConst_Constant:		policy::impl::MatrixTranslate_Constant::Initialize(			res, random, p.translate_.value_																,p.generate_shape_.position_									);	break;
			case TranslateTypeConst_NewtonDynamics:	policy::impl::MatrixTranslate_Newtondynamics::Initialize(	res, random, p.translate_.value_,	p.translate_.speed_,	p.translate_.acceleration_			,p.generate_shape_.position_	,p.generate_shape_.direction_	);	break;
			case TranslateTypeConst_CurveValue:		policy::impl::MatrixTranslate_CurveValue::Initialize(		res, random, p.translate_.value_														, pii	,p.generate_shape_.position_									);	break;
			case TranslateTypeConst_CurveSpeed:		policy::impl::MatrixTranslate_CurveSpeed::Initialize(		res, random, p.translate_.value_,	p.translate_.speed_									, pii	,p.generate_shape_.position_									);	break;
			default:	ML_ASSERT(false);			break;
		}

		switch(res.Blend_Basic_BlendAlphaType()){
			case BlendAlphaTypeConst_Constant:	policy::impl::Blend_Constant::Initialize(	res, random, p.color_.value_.e_.a_		);	break;
			case BlendAlphaTypeConst_Curve:		policy::impl::Blend_Curve::Initialize(		res, random, p.color_.value_.e_.a_		);	break;
			default:	ML_ASSERT(false);	break;
		}
		switch(res.Color_Basic_ColorType()){
			case ColorTypeConst_Constant:	policy::impl::Color_Constant::Initialize(	res, random, p.color_.value_, res.Color_Center_Value(), res.Color_Center_Range(), res.Color_Center_RandomChannelSync(), p.color_.value_.e_.a_);	break;
			case ColorTypeConst_Curve:		policy::impl::Color_Curve::Initialize(		res, random, p.color_.value_, res.Color_Center_Value(), res.Color_Center_Range(),										p.color_.value_.e_.a_);	break;
			default:	ML_ASSERT(false);	break;
		}
	}

	template<typename ResourceType, typename UpdateInfo, typename LifePolicy>
	static void	Update(UpdateInfo &update_info, const ResourceType &res, ControlParam<LifePolicy> &p, const PrimitiveUpdateInfo & /*primitive_update_info*/, ml::random & /*random*/){

		ml::ui32	random_seed	= ml::ui32(&p);

		#define	BM3_PM_AllowFor_Scale_Basic_Square	true
#ifdef BM3_PLATFORM_psp
		#include "../primitive/bm3_prim_update_matrix_psp.inc"
#else
		#include "../primitive/bm3_prim_update_matrix.inc"
#endif
		#undef	BM3_PM_AllowFor_Scale_Basic_Square

		
		if(res.Blend_Basic_BlendAlphaType() == BlendAlphaTypeConst_Curve){
			policy::impl::Blend_Curve::Prepare(			res, p.color_.value_.e_.a_, p.LifeRate(), random_seed);
		}
		switch(res.Color_Basic_ColorType()){
			case ColorTypeConst_Constant:	policy::impl::Color_Constant::Prepare(	res, p.color_.value_,																															p.color_.value_.e_.a_);	break;
			case ColorTypeConst_Curve:		policy::impl::Color_Curve::Prepare(		res, p.color_.value_, reinterpret_cast<const ColorCurveType&>(res.Color_Center_Curve()), res.Color_Center_Range(),	res.Color_Center_RandomChannelSync(),	p.LifeRate(), random_seed,	p.color_.value_.e_.a_);	break;
			default:	ML_ASSERT(false);	break;
		}
	}

	template<typename ResourceType, typename LifePolicy>
	static void	Prepare(const ResourceType &res, ControlParam<LifePolicy> &p, ml::matrix44 &m, const PrimitivePrepareInfo &primitive_Prepare_info){

		switch(res.Scale_Basic_ScaleType()){
			case ScaleTypeConst_Constant:		policy::impl::MatrixScale_Constant<>::Prepare(			res, p.scale_.value_, m);							break;
			case ScaleTypeConst_NewtonDynamics:	policy::impl::MatrixScale_Newtondynamics<>::Prepare(	res, p.scale_.value_, m);							break;
			case ScaleTypeConst_CurveValue:		policy::impl::MatrixScale_CurveValue<>::Prepare(		res, p.scale_.value_, m, primitive_Prepare_info);	break;
			case ScaleTypeConst_CurveSpeed:		policy::impl::MatrixScale_CurveSpeed<>::Prepare(		res, p.scale_.value_, m, primitive_Prepare_info);	break;
			default:	ML_ASSERT(false);		break;
		}

		switch(res.Rotate_Basic_RotateType()){
			case RotateTypeConst_Constant:			policy::impl::MatrixRotate_Constant::Prepare(			res, p.rotate_.value_, m);							break;
			case RotateTypeConst_NewtonDynamics:	policy::impl::MatrixRotate_Newtondynamics::Prepare(		res, p.rotate_.value_, m);							break;
			case RotateTypeConst_CurveValue:		policy::impl::MatrixRotate_CurveValue::Prepare(			res, p.rotate_.value_, m, primitive_Prepare_info);	break;
			case RotateTypeConst_CurveSpeed:		policy::impl::MatrixRotate_CurveSpeed::Prepare(			res, p.rotate_.value_, m, primitive_Prepare_info);	break;
			default:	ML_ASSERT(false);			break;
		}

		switch(res.Translate_Basic_TranslateType()){
			case TranslateTypeConst_Constant:		policy::impl::MatrixTranslate_Constant::Prepare(		res, p.translate_.value_, m);							break;
			case TranslateTypeConst_NewtonDynamics:	policy::impl::MatrixTranslate_Newtondynamics::Prepare(	res, p.translate_.value_, m);							break;
			case TranslateTypeConst_CurveValue:		policy::impl::MatrixTranslate_CurveValue::Prepare(		res, p.translate_.value_, m, primitive_Prepare_info);	break;
			case TranslateTypeConst_CurveSpeed:		policy::impl::MatrixTranslate_CurveSpeed::Prepare(		res, p.translate_.value_, m, primitive_Prepare_info);	break;
			default:	ML_ASSERT(false);			
		}
	}

	template<typename ResourceType, typename LifePolicy>
	static void	PrepareBm2(const ResourceType &res, ControlParam<LifePolicy> &p, ml::matrix44 &m, const PrimitivePrepareInfo &primitive_Prepare_info){

		switch(res.Scale_Basic_ScaleType()){
			case ScaleTypeConst_Constant:		policy::impl::MatrixScale_Constant<>::Prepare(			res, p.scale_.value_, m);							break;
			case ScaleTypeConst_NewtonDynamics:	policy::impl::MatrixScale_Newtondynamics<>::Prepare(	res, p.scale_.value_, m);							break;
			case ScaleTypeConst_CurveValue:		policy::impl::MatrixScale_CurveValue<>::Prepare(		res, p.scale_.value_, m, primitive_Prepare_info);	break;
			case ScaleTypeConst_CurveSpeed:		policy::impl::MatrixScale_CurveSpeed<>::Prepare(		res, p.scale_.value_, m, primitive_Prepare_info);	break;
			default:	ML_ASSERT(false);		break;
		}

		switch(res.Rotate_Basic_RotateType()){
			case RotateTypeConst_Constant:			policy::impl::MatrixRotate_Constant::PrepareBm2(			res, p.rotate_.value_, m);							break;
			case RotateTypeConst_NewtonDynamics:	policy::impl::MatrixRotate_Newtondynamics::PrepareBm2(		res, p.rotate_.value_, m);							break;
			case RotateTypeConst_CurveValue:		policy::impl::MatrixRotate_CurveValue::PrepareBm2(			res, p.rotate_.value_, m, primitive_Prepare_info);	break;
			case RotateTypeConst_CurveSpeed:		policy::impl::MatrixRotate_CurveSpeed::PrepareBm2(			res, p.rotate_.value_, m, primitive_Prepare_info);	break;
			default:	ML_ASSERT(false);			break;
		}

		switch(res.Translate_Basic_TranslateType()){
			case TranslateTypeConst_Constant:		policy::impl::MatrixTranslate_Constant::Prepare(		res, p.translate_.value_, m);							break;
			case TranslateTypeConst_NewtonDynamics:	policy::impl::MatrixTranslate_Newtondynamics::Prepare(	res, p.translate_.value_, m);							break;
			case TranslateTypeConst_CurveValue:		policy::impl::MatrixTranslate_CurveValue::Prepare(		res, p.translate_.value_, m, primitive_Prepare_info);	break;
			case TranslateTypeConst_CurveSpeed:		policy::impl::MatrixTranslate_CurveSpeed::Prepare(		res, p.translate_.value_, m, primitive_Prepare_info);	break;
			default:	ML_ASSERT(false);			
		}
	}

	template<typename ResourceType, typename LifePolicy>
	static void	SwitchResource(const ResourceType &res,const ResourceType &old_res, ml::random &random, ControlParam<LifePolicy> &p, const PrimitiveInitializeInfo &pii){

		p.SwitchResource(res, random);
		policy::impl::MatrixScale_SwitchResource<>::Switch(		res, old_res, random, p.scale_.value_,		p.scale_.speed_,		p.scale_.acceleration_,		pii);
		policy::impl::MatrixRotate_SwitchResource::Switch(		res, old_res, random, p.rotate_.value_,		p.rotate_.speed_,		p.rotate_.acceleration_,	pii);
		policy::impl::MatrixTranslate_SwitchResource::Switch(	res, old_res, random, p.translate_.value_,	p.translate_.speed_,	p.translate_.acceleration_,	pii, p.generate_shape_.position_ ,p.generate_shape_.direction_);
		policy::impl::Blend_SwitchResource::Switch(				res, old_res, random, p.color_.value_.e_.a_,													pii);
		policy::impl::Color_SwitchResource::Switch(				res, old_res, random, p.color_.value_,		res.Color_Center_Value(),	res.Color_Center_Range(), res.Color_Center_RandomChannelSync(),	p.color_.value_.e_.a_, pii);
	}

	template<typename ResourceType, typename UpdateInfo, typename LifePolicy>
	static int	Update(UpdateInfo &update_info, const ResourceType &res, ControlParam<LifePolicy> *control_begin, int *active_index_array, int active_node_count, const PrimitiveUpdateInfo &pui, int begin_count){

		for(int i=0 ; i<active_node_count ; ++i){
			int index = active_index_array[i] - begin_count;
			if(active_node_count<index)	return i;
			Update(update_info, res, control_begin[index], pui);
		}
		return active_node_count;
	}

};

BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_MANIPULATOR_BM3_CONTROLMANIPULATOR_H
