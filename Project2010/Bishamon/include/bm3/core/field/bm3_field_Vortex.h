#ifndef BM3_SDK_INC_BM3_CORE_PRIMITIVE_BM3_FIELD_VORTEX_H
#define BM3_SDK_INC_BM3_CORE_PRIMITIVE_BM3_FIELD_VORTEX_H

#include <ml/math/ml_math_const.h>
#include "bm3_field_Common.h"
#include "bm3/core/policy/Field/bm3_policy_Vortex_Axis_Constant.h"
#include "bm3/core/policy/Field/bm3_policy_Vortex_Axis_Curve.h"
#include "bm3/core/policy/Field/bm3_policy_Vortex_Magnitude_Constant.h"
#include "bm3/core/policy/Field/bm3_policy_Vortex_Magnitude_Curve.h"
#include "bm3/core/policy/Field/bm3_policy_Vortex_Area_Constant.h"
#include "bm3/core/policy/Field/bm3_policy_Vortex_Area_Curve.h"
#include "bm3/core/policy/Field/bm3_policy_Vortex_Position_Constant.h"
#include "bm3/core/policy/Field/bm3_policy_Vortex_Position_Curve.h"
#include "bm3/core/policy/Field/bm3_policy_Vortex_ResourceSwitch.h"
#if defined(BM3_TARGET_IDE)
#include "../bm3_Debug.h"
#endif

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

class DrawInfo;

namespace field{
namespace vortex{

/*
	@brief	�Q�t�B�[���h�Ǘ��I�u�W�F�N�g
*/
struct Field{
	float			magnitude_;	// �e���͈͓��ɓ��������Ɍv�Z����������
	float			noise_;		// ��]���x�̂΂��
	float			area_;		// �Q�̑傫��
	ml::vector3d	axis_;		// ��
	ml::vector3d	position_;	// �t�B�[���h�̎��̈ʒu

	ml::vector3d	prim_pos_diff_addition;	// �t�B�[���h�ɂ�����e�������ׂăL���b�V��
	ml::vector3d	field_direction_;		// �t�B�[���h�̊|���Ă�������Ƒ傫��
};

/*
	@brief	�Q�t�B�[���h�����I�u�W�F�N�g
*/
class FieldManipulator{

public:
	/*
		@brief	�Q�t�B�[���h������
		@param	field	�t�B�[���h�I�u�W�F�N�g
		@param	res		�t�B�[���h���\�[�X�I�u�W�F�N�g
		@param	random	�����_��
		@param	prim	�֘A����v���~�e�B�u�I�u�W�F�N�g
		@return	�Ȃ�
	*/
	template<typename FieldType, typename PrimitiveType, typename ResourceType>
	static void Initialize(FieldType &field, const ResourceType &res, ml::random &random, PrimitiveType & /*prim*/){

		field.prim_pos_diff_addition.set_unit();
		field.field_direction_.set_unit();
		// ����
		policy::impl::Vortex_Magnitude_Constant::Initialize(	res, random, field.magnitude_,	field.noise_	);
		policy::impl::Vortex_Magnitude_Curve::Initialize(		res, random, field.magnitude_,	field.noise_	);

		// �͈�
		policy::impl::Vortex_Area_Constant::Initialize(	res, random, field.area_	);
		policy::impl::Vortex_Area_Curve::Initialize(	res, random, field.area_	);

		// �ʒu
		field.position_.set_unit();
		policy::impl::Vortex_Position_Constant::Initialize(	res, random, field.position_	);
		policy::impl::Vortex_Position_Curve::Initialize(	res, random, field.position_	);

		// ��]��������
		policy::impl::Vortex_Axis_Constant::Initialize(	res, random, field.axis_	);
		policy::impl::Vortex_Axis_Curve::Initialize(		res, random, field.axis_	);
		field.axis_.normalize();

	}

	/*
		@brief	�Q�t�B�[���h�X�V
		@param	update_info	UpdateInfo
		@param	field		�t�B�[���h�I�u�W�F�N�g
		@param	res			�t�B�[���h���\�[�X�I�u�W�F�N�g
		@param	prim		�֘A����v���~�e�B�u�I�u�W�F�N�g
		@param	primitive_update_info	PrimitiveUpdateInfo
		@return	�Ȃ�
	*/
	template<typename UpdateInfo, typename FieldType, typename PrimitiveType, typename ResourceType>
	static void Update(UpdateInfo &update_info, FieldType &field, const ResourceType &res, const PrimitiveType & /*prim*/, const PrimitiveUpdateInfo &primitive_update_info){

//		policy::impl::Vortex_Magnitude_Constant::Update(	update_info, res, field.magnitude_);
//		policy::impl::Vortex_Magnitude_Curve::Update(		update_info, res, field.magnitude_,	primitive_update_info);

		policy::impl::Vortex_Position_Constant::Update(	res, field.position_);
		policy::impl::Vortex_Position_Curve::Update(	res, field.position_,	primitive_update_info);

		policy::impl::Vortex_Area_Constant::Update(	update_info, res, field.area_);
		policy::impl::Vortex_Area_Curve::Update(	update_info, res, field.area_,	primitive_update_info);

//		policy::impl::Vortex_Axis_Constant::Update(	update_info, res, field.axis_);
//		policy::impl::Vortex_Axis_Curve::Update(		update_info, res, field.axis_,	primitive_update_info);
	}

	/*
		@brief	���_�����O�̏���
		@param	dst			FieldInfo.�ŏI�I�ȃt�B�[���h�ɂ�����ω������Z���Ă���
		@param	field		�t�B�[���h�I�u�W�F�N�g
		@param	res			�t�B�[���h���\�[�X�I�u�W�F�N�g
		@param	matrix		�e�̃}�g���b�N�X
		@param	update_info	UpdateInfo
		@param	root		���[�g�m�[�h�I�u�W�F�N�g
		@param	prim		�֘A����v���~�e�B�u�I�u�W�F�N�g
		@return	�Ȃ�
	*/
	template<typename FieldType, typename PrimitiveType, typename ResourceType>
	static void Prepare(FieldInfo &dst, FieldType &field, const ResourceType &res, const ml::matrix44 &matrix, UpdateInfo &update_info, const obj::RootNodeBase &root, PrimitiveType &prim){

		if(res.Basic_Meta_Enable()){

			ml::vector3d	field_pos;
			ml::vector3d	prim_pos;

			// �O��̃t�B�[���h�̈ʒu���܂߂����݂̈ʒu�������
			{
				prim_pos.mul(prim.translate_.value_, matrix);
				prim_pos.add(dst.position_);
				prim_pos.e_.w_	= 1.0f;

				field_pos.mul(field.position_, root.GenerateTimeHierarchyParentMatrix());
				prim_pos.add(field.prim_pos_diff_addition);
			}

			float dist = DistanceSqOfPointRay(prim_pos, field_pos, field.axis_);
			// �Q�̒��S�ɂ���Ƃ��͉������Ȃ�
			if(dist){
				if((res.Field_Area_FieldAreaType() == FieldAreaTypeConst_Infinit) || (dist < field.area_*field.area_)){

					// �p�����[�^�̍X�V
					{
						PrimitiveDrawInfo	pdi;
						{
							pdi.life_rate_		= prim.LifeRate();
							pdi.random_seed_	= ml::ui32(&field);
						}
						if(res.Field_Magnitude_CentripetalForce()){
							policy::impl::Vortex_Magnitude_Constant::PrepareAddition(	res, field.magnitude_, field.noise_);
							policy::impl::Vortex_Magnitude_Curve::PrepareAddition(		res, field.magnitude_, field.noise_,	pdi);
						}
						else{
							policy::impl::Vortex_Magnitude_Constant::Prepare(	res, field.magnitude_, field.noise_);
							policy::impl::Vortex_Magnitude_Curve::Prepare(		res, field.magnitude_, field.noise_,	pdi);
						}
		//				policy::impl::Vortex_Area_Constant::Update(	update_info, res, field.area_);
		//				policy::impl::Vortex_Area_Curve::Update(	update_info, res, field.area_,	primitive_update_info);
						policy::impl::Vortex_Axis_Constant::Prepare(	res, field.axis_);
						policy::impl::Vortex_Axis_Curve::Prepare(		res, field.axis_,	pdi);
					}

					if(!field.axis_.is_unit()){

						// �������ɂ�鋭���̍Čv�Z
						if(res.Field_Magnitude_Damp()!=0.0f){
							field.magnitude_ = (0<field.magnitude_) ? 
								ml::minimum((float)FLT_MAX, field.magnitude_ * ml::inverse(res.Field_Magnitude_Damp() * ml::sqrtf(dist))):
								ml::maximum(-(float)FLT_MAX, field.magnitude_ * ml::inverse(res.Field_Magnitude_Damp() * ml::sqrtf(dist)));
						}

						// �v���~�e�B�u�̃X�P�[�����܂߂������̍Čv�Z
						float mag = 0.0f;
						{
							float scale	= 1.0f;
							if(res.Field_Magnitude_ScaleInfluence()){
								scale	= PrimitiveParam::ScaleValue(prim.scale_)!=0.0f ? ml::inverse(PrimitiveParam::ScaleValue(prim.scale_)) : 0;
							}
							mag = field.magnitude_ * scale * update_info.GetUpdateFrame();
						}

						// �ŏI�I�Ȉʒu�̌v�Z
						{
							ml::matrix44	rot;
							ml::vector3d	prim_pos_org	= prim_pos;
							// ��U���_�Ōv�Z���Č��ɖ߂�
							rot.set_rot_axis(mag, field.axis_);
							prim_pos.sub(field_pos);
							prim_pos.mul(rot);
							prim_pos.add(field_pos);

							// �t�B�[���h�ɂ��ω��ʂ��v�Z����
							field.field_direction_.sub(prim_pos, prim_pos_org);
							if(res.Field_Magnitude_CentripetalForce()){
								field.field_direction_.normalize();
								field.field_direction_.mul(mag);
							}
/*
								field.field_direction_.sub(prim_pos, prim_pos_org);
								float	length	= field.field_direction_.length();
								float	rate	= length * (1.0f - res.Field_Option_FocusRate());
								field.field_direction_.normalize();
								if(rate!=0.0f){
									field.field_direction_.mul(rate);
								}
*/
						}
					}
				}
			}

			field.prim_pos_diff_addition.add(field.field_direction_);	// �t�B�[���h���ɂ��Ȃ��Ă��ω��ʂ�^��������
			dst.position_.add(field.prim_pos_diff_addition);			// �t�B�[���h�ɂ�����ω��ʂ��t�B�[���h���Ƃ���
		}
	}

	/*
		@brief	���\�[�X��u��������
		@param	field	�t�B�[���h�Ǘ��N���X
		@param	res		���\�[�X�I�u�W�F�N�g
		@param	old_res	�Â����\�[�X�I�u�W�F�N�g
		@param	random	�����_���I�u�W�F�N�g
		@return	�Ȃ�
	*/
	template<typename FieldType, typename ResourceType>
	static void SwitchResource(FieldType &field, const ResourceType &res, const ResourceType &old_res, ml::random &random){
		policy::impl::Vortex_Area_ResourceSwitch::Switch(		res, old_res, random, field.area_	);
		policy::impl::Vortex_Axis_ResourceSwitch::Switch(		res, old_res, random, field.axis_	);
		policy::impl::Vortex_Magnitude_ResourceSwitch::Switch(	res, old_res, random, field.magnitude_,	field.noise_	);
		policy::impl::Vortex_Position_ResourceSwitch::Switch(	res, old_res, random, field.position_	);
	}

	/*
		@brief	�t�B�[���h���̈ʒu��`��i�f�o�b�O�p�j
		@param	draw_info	DrawInfo
		@param	res			�t�B�[���h���\�[�X�I�u�W�F�N�g
		@param	matrix		�e�̃}�g���b�N�X
		@param	life		�v���~�e�B�u�̃��C�t�I�u�W�F�N�g
	*/
	template<typename ResourceType,	typename LifePolicy>
	static void Draw(DrawInfo &draw_info, const ResourceType &res, const ml::matrix44 &matrix, LifePolicy &life){
#if defined(BM3_TARGET_IDE)
		if(res.Basic_Meta_Enable() && res.Basic_Meta_Display() && res.Basic_Basic_AreaDisplay()){


			ml::matrix44	slope;
			slope.set_unit();
			// �X���̌v�Z
			{
				ml::vector3d	axis;
				switch(res.Field_Axis_FieldAxisType()){
					case FieldAxisTypeConst_Constant:	axis = res.Field_Axis_Initial();						break;
					case FieldAxisTypeConst_Curve:		axis = res.Field_Axis_Curve().Value(life.LifeRate());	break;
					default:	ML_ASSERT(false);	break;
				}
				{
					if(axis.is_unit()==false){
						ml::vector3d	c;
						ml::vector3d	y = ml::vector3d(0,1,0,0);

						axis.normalize();
						axis.e_.w_	= 0;
						if(axis.is_equal(y)==false){
							float cos = ml::acosf(y.dot(axis));
							c = y.cross(axis).normalize();
							slope.set_rot_axis(cos, c);
						}
					}
				}
				slope.mul(matrix);
			}

			// �ʒu�̌v�Z
			ml::vector3d	position;
			switch(res.Field_Position_FieldPosType()){
				case FieldPosTypeConst_Constant:	position	= res.Field_Position_Initial();							break;
				case FieldPosTypeConst_Curve:		position	= res.Field_Position_Curve().Value(life.LifeRate());	break;
				default:	ML_ASSERT(false);	break;
			}

			// �͈͂̌v�Z
			ml::vector3d	area;
			switch(res.Field_Area_FieldAreaType()){
				case FieldAreaTypeConst_Constant: 	area	= ml::vector3d(res.Field_Area_Initial(), 1000000.0f, res.Field_Area_Initial(), 1.0f);	break;
				case FieldAreaTypeConst_Curve:
					{
						float a = res.Field_Area_Curve().Value(life.LifeRate());
						area	= ml::vector3d(a,a,a,0);	
					}
					break;
				case FieldAreaTypeConst_Infinit:	area.set_unit();	break;
				default:	break;
			}

			// �`��
			debug::SphereArea	angle(res.Field_HorizontalArea_StartAngle(), res.Field_HorizontalArea_EndAngle(),0,0);
			debug::DrawCylinder(draw_info,	slope, position, area, angle);
		}
#endif
	}

private:
	/*
		@brief	�_�ƒ����̋������v�Z����
		@param	point		�_�̈ʒu
		@param	ray_start	�����̎n�_
		@param	dir			�����̕���
		@return	�_�ƒ����̋���
	*/
	static float DistanceSqOfPointRay(const ml::vector3d &point, const ml::vector3d &ray_start, ml::vector3d &dir){
		ml::vector3d	vec, line;

		vec.sub(point ,ray_start);
//		line.add(ray_start ,dir);

		float proj	= vec.dot(dir);

		return vec.length_sq() - (proj*proj)/dir.length_sq();
	}
};

}	// namespace vortex
}	// namespace field
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_PRIMITIVE_BM3_FIELD_VORTEX_H
