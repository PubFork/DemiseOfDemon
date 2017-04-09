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
	@brief	渦フィールド管理オブジェクト
*/
struct Field{
	float			magnitude_;	// 影響範囲内に入った時に計算が加えられる
	float			noise_;		// 回転速度のばらつき
	float			area_;		// 渦の大きさ
	ml::vector3d	axis_;		// 軸
	ml::vector3d	position_;	// フィールドの軸の位置

	ml::vector3d	prim_pos_diff_addition;	// フィールドにおける影響をすべてキャッシュ
	ml::vector3d	field_direction_;		// フィールドの掛っている方向と大きさ
};

/*
	@brief	渦フィールド処理オブジェクト
*/
class FieldManipulator{

public:
	/*
		@brief	渦フィールド初期化
		@param	field	フィールドオブジェクト
		@param	res		フィールドリソースオブジェクト
		@param	random	ランダム
		@param	prim	関連するプリミティブオブジェクト
		@return	なし
	*/
	template<typename FieldType, typename PrimitiveType, typename ResourceType>
	static void Initialize(FieldType &field, const ResourceType &res, ml::random &random, PrimitiveType & /*prim*/){

		field.prim_pos_diff_addition.set_unit();
		field.field_direction_.set_unit();
		// 強さ
		policy::impl::Vortex_Magnitude_Constant::Initialize(	res, random, field.magnitude_,	field.noise_	);
		policy::impl::Vortex_Magnitude_Curve::Initialize(		res, random, field.magnitude_,	field.noise_	);

		// 範囲
		policy::impl::Vortex_Area_Constant::Initialize(	res, random, field.area_	);
		policy::impl::Vortex_Area_Curve::Initialize(	res, random, field.area_	);

		// 位置
		field.position_.set_unit();
		policy::impl::Vortex_Position_Constant::Initialize(	res, random, field.position_	);
		policy::impl::Vortex_Position_Curve::Initialize(	res, random, field.position_	);

		// 回転軸初期化
		policy::impl::Vortex_Axis_Constant::Initialize(	res, random, field.axis_	);
		policy::impl::Vortex_Axis_Curve::Initialize(		res, random, field.axis_	);
		field.axis_.normalize();

	}

	/*
		@brief	渦フィールド更新
		@param	update_info	UpdateInfo
		@param	field		フィールドオブジェクト
		@param	res			フィールドリソースオブジェクト
		@param	prim		関連するプリミティブオブジェクト
		@param	primitive_update_info	PrimitiveUpdateInfo
		@return	なし
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
		@brief	頂点を作る前の処理
		@param	dst			FieldInfo.最終的なフィールドにおける変化を加算していく
		@param	field		フィールドオブジェクト
		@param	res			フィールドリソースオブジェクト
		@param	matrix		親のマトリックス
		@param	update_info	UpdateInfo
		@param	root		ルートノードオブジェクト
		@param	prim		関連するプリミティブオブジェクト
		@return	なし
	*/
	template<typename FieldType, typename PrimitiveType, typename ResourceType>
	static void Prepare(FieldInfo &dst, FieldType &field, const ResourceType &res, const ml::matrix44 &matrix, UpdateInfo &update_info, const obj::RootNodeBase &root, PrimitiveType &prim){

		if(res.Basic_Meta_Enable()){

			ml::vector3d	field_pos;
			ml::vector3d	prim_pos;

			// 前回のフィールドの位置を含めた現在の位置情報を作る
			{
				prim_pos.mul(prim.translate_.value_, matrix);
				prim_pos.add(dst.position_);
				prim_pos.e_.w_	= 1.0f;

				field_pos.mul(field.position_, root.GenerateTimeHierarchyParentMatrix());
				prim_pos.add(field.prim_pos_diff_addition);
			}

			float dist = DistanceSqOfPointRay(prim_pos, field_pos, field.axis_);
			// 渦の中心にいるときは何もしない
			if(dist){
				if((res.Field_Area_FieldAreaType() == FieldAreaTypeConst_Infinit) || (dist < field.area_*field.area_)){

					// パラメータの更新
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

						// 減衰率による強さの再計算
						if(res.Field_Magnitude_Damp()!=0.0f){
							field.magnitude_ = (0<field.magnitude_) ? 
								ml::minimum((float)FLT_MAX, field.magnitude_ * ml::inverse(res.Field_Magnitude_Damp() * ml::sqrtf(dist))):
								ml::maximum(-(float)FLT_MAX, field.magnitude_ * ml::inverse(res.Field_Magnitude_Damp() * ml::sqrtf(dist)));
						}

						// プリミティブのスケールを含めた強さの再計算
						float mag = 0.0f;
						{
							float scale	= 1.0f;
							if(res.Field_Magnitude_ScaleInfluence()){
								scale	= PrimitiveParam::ScaleValue(prim.scale_)!=0.0f ? ml::inverse(PrimitiveParam::ScaleValue(prim.scale_)) : 0;
							}
							mag = field.magnitude_ * scale * update_info.GetUpdateFrame();
						}

						// 最終的な位置の計算
						{
							ml::matrix44	rot;
							ml::vector3d	prim_pos_org	= prim_pos;
							// 一旦原点で計算して元に戻す
							rot.set_rot_axis(mag, field.axis_);
							prim_pos.sub(field_pos);
							prim_pos.mul(rot);
							prim_pos.add(field_pos);

							// フィールドによる変化量を計算する
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

			field.prim_pos_diff_addition.add(field.field_direction_);	// フィールド内にいなくても変化量を与え続ける
			dst.position_.add(field.prim_pos_diff_addition);			// フィールドにおける変化量をフィールド情報とする
		}
	}

	/*
		@brief	リソースを置き換える
		@param	field	フィールド管理クラス
		@param	res		リソースオブジェクト
		@param	old_res	古いリソースオブジェクト
		@param	random	ランダムオブジェクト
		@return	なし
	*/
	template<typename FieldType, typename ResourceType>
	static void SwitchResource(FieldType &field, const ResourceType &res, const ResourceType &old_res, ml::random &random){
		policy::impl::Vortex_Area_ResourceSwitch::Switch(		res, old_res, random, field.area_	);
		policy::impl::Vortex_Axis_ResourceSwitch::Switch(		res, old_res, random, field.axis_	);
		policy::impl::Vortex_Magnitude_ResourceSwitch::Switch(	res, old_res, random, field.magnitude_,	field.noise_	);
		policy::impl::Vortex_Position_ResourceSwitch::Switch(	res, old_res, random, field.position_	);
	}

	/*
		@brief	フィールドをの位置を描画（デバッグ用）
		@param	draw_info	DrawInfo
		@param	res			フィールドリソースオブジェクト
		@param	matrix		親のマトリックス
		@param	life		プリミティブのライフオブジェクト
	*/
	template<typename ResourceType,	typename LifePolicy>
	static void Draw(DrawInfo &draw_info, const ResourceType &res, const ml::matrix44 &matrix, LifePolicy &life){
#if defined(BM3_TARGET_IDE)
		if(res.Basic_Meta_Enable() && res.Basic_Meta_Display() && res.Basic_Basic_AreaDisplay()){


			ml::matrix44	slope;
			slope.set_unit();
			// 傾きの計算
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

			// 位置の計算
			ml::vector3d	position;
			switch(res.Field_Position_FieldPosType()){
				case FieldPosTypeConst_Constant:	position	= res.Field_Position_Initial();							break;
				case FieldPosTypeConst_Curve:		position	= res.Field_Position_Curve().Value(life.LifeRate());	break;
				default:	ML_ASSERT(false);	break;
			}

			// 範囲の計算
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

			// 描画
			debug::SphereArea	angle(res.Field_HorizontalArea_StartAngle(), res.Field_HorizontalArea_EndAngle(),0,0);
			debug::DrawCylinder(draw_info,	slope, position, area, angle);
		}
#endif
	}

private:
	/*
		@brief	点と直線の距離を計算する
		@param	point		点の位置
		@param	ray_start	直線の始点
		@param	dir			直線の方向
		@return	点と直線の距離
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
