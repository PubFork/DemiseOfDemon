#ifndef BM3_SDK_INC_BM3_CORE_PRIMITIVE_BM3_PRIM_COMMONMANIPULATOR_H
#define BM3_SDK_INC_BM3_CORE_PRIMITIVE_BM3_PRIM_COMMONMANIPULATOR_H

#include "bm3_Primitive.h"
#include "../draw/policy/bm3_draw_policy_Common.h"
#include "../resource/bm3_res_MassParticle.h"
#include "../bm3_Debug.h"
#include "../bm3_ModelRenderer.h"
#include <ml/utility/ml_simpletimer.h>

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE


namespace prim{


#if defined(BM3_TARGET_IDE)
	#define	BM3_FIELD_RESOURCE_TYPE(NODE_TYPE)	res::IdeNode
#endif
#if defined(BM3_TARGET_EMBEDDED)
	#define	BM3_FIELD_RESOURCE_TYPE(NODE_TYPE)	res::NODE_TYPE
#endif

/// @brief CommonPrimitiveManipulatorクラス
class CommonPrimitiveManipulator{

public:
	/*
	*/
	template<typename PrimitiveType>
	static void SetAllDriveParentMatrix(PrimitiveType *prim_begin, PrimitiveType *prim_end, const ml::matrix44 &parent_matrix, const ml::vector3d &parent_scale){

		for(PrimitiveType *p = prim_begin ;p != prim_end ; ++p){

			p->SetParentMatrix(parent_matrix);
			p->SetParentScale(parent_scale);
		}
	}

	/*
	*/
	template<typename PrimitiveType>
	static void SetAllDriveGenerateTimeParentMatrix(PrimitiveType *prim_begin, PrimitiveType *prim_end, const ml::matrix44 &parent_matrix, const ml::vector3d &parent_scale){

		for(PrimitiveType *p = prim_begin ;p != prim_end ; ++p){

			p->SetGenerateTimeParentMatrix(parent_matrix);
			p->SetGenerateTimeParentScale(parent_scale);
		}
	}
	
	/*
	*/
	template<typename PrimitiveType, typename ResourceType>
	static void SetDriveMatrix(prim::Packet *packet, PrimitiveType **active_begin, PrimitiveType **active_end, const ResourceType &res, const ml::matrix44 &parent_matrix, const ml::vector3d &parent_scale){

		ML_STATIC_ASSERT(TranslateParentNodeInfluenceTypeConst_On				== 0);
		ML_STATIC_ASSERT(TranslateParentNodeInfluenceTypeConst_OnGenerateTime	== 1);
		ML_STATIC_ASSERT(TranslateParentNodeInfluenceTypeConst_MAX				== 2);
		ML_STATIC_ASSERT(RotateParentNodeInfluenceTypeConst_Off					== 0);
		ML_STATIC_ASSERT(RotateParentNodeInfluenceTypeConst_On					== 1);
		ML_STATIC_ASSERT(RotateParentNodeInfluenceTypeConst_OnGenerateTime		== 2);
		ML_STATIC_ASSERT(RotateParentNodeInfluenceTypeConst_MAX					== 3);
		ML_STATIC_ASSERT(ScaleParentNodeInfluenceTypeConst_Off					== 0);
		ML_STATIC_ASSERT(ScaleParentNodeInfluenceTypeConst_On					== 1);
		ML_STATIC_ASSERT(ScaleParentNodeInfluenceTypeConst_OnGenerateTime		== 2);
		ML_STATIC_ASSERT(ScaleParentNodeInfluenceTypeConst_MAX					== 3);

		TranslateParentNodeInfluenceTypeConst	t	= res.Basic_ParentNodeInfluence_TranslateParentNodeInfluenceType();
		RotateParentNodeInfluenceTypeConst		r	= res.Basic_ParentNodeInfluence_RotateParentNodeInfluenceType();
		ScaleParentNodeInfluenceTypeConst		s	= res.Basic_ParentNodeInfluence_ScaleParentNodeInfluenceType();

		PrimitiveType	**read			= active_begin;
		for(;read != active_end;++ read, ++packet){
			PrimitiveType &p = **read;

//			PrefetchData<sizeof(PrimitiveType)>(*(read + 1));
			PrefetchData<16>(*(read + 1));					// ここでは先頭から１６バイト分のみで足りる

//			PrefetchData<sizeof(*packet)>(packet + 1);

			ml::matrix44	&m = packet->generate_time_hierarchy_parent_matrix_;
			ml::vector3d	&v = packet->generate_time_hierarchy_parent_matrix_row_length_;

			PrefetchData<sizeof(ml::matrix44)>(&((packet + 1)->generate_time_hierarchy_parent_matrix_));

			switch(r){			// 回転
				case RotateParentNodeInfluenceTypeConst_Off:{					m.set_unit();							break;	}
//				case RotateParentNodeInfluenceTypeConst_On:{					m = parent_matrix;						break;	}
				case RotateParentNodeInfluenceTypeConst_On:{
					PrefetchData<sizeof(ml::matrix44)>(&parent_matrix);
//					PrefetchData<sizeof(ml::matrix44)>(&((packet + 1)->generate_time_hierarchy_parent_matrix_));

					m.e_.m00_ = parent_matrix.e_.m00_;
					m.e_.m01_ = parent_matrix.e_.m01_;
					m.e_.m02_ = parent_matrix.e_.m02_;
					m.e_.m03_ = parent_matrix.e_.m03_;
					m.e_.m10_ = parent_matrix.e_.m10_;
					m.e_.m11_ = parent_matrix.e_.m11_;
					m.e_.m12_ = parent_matrix.e_.m12_;
					m.e_.m13_ = parent_matrix.e_.m13_;
					m.e_.m20_ = parent_matrix.e_.m20_;
					m.e_.m21_ = parent_matrix.e_.m21_;
					m.e_.m22_ = parent_matrix.e_.m22_;
					m.e_.m23_ = parent_matrix.e_.m23_;
					m.e_.m30_ = parent_matrix.e_.m30_;
					m.e_.m31_ = parent_matrix.e_.m31_;
					m.e_.m32_ = parent_matrix.e_.m32_;
					m.e_.m33_ = parent_matrix.e_.m33_;
					break;
				}
				case RotateParentNodeInfluenceTypeConst_OnGenerateTime:{		m = p.GetGenerateTimeParentMatrix();	break;	}
				default:{														ML_ASSERT(false);								}
			}

			switch(s){			// 拡縮
				case ScaleParentNodeInfluenceTypeConst_Off:{
					if(parent_scale.is_unit()){
						m.set_scale(1.0f, 1.0f, 1.0f);
					}
					else{
						m.row(ml::matrix44::row::x).normalize();
						m.row(ml::matrix44::row::y).normalize();
						m.row(ml::matrix44::row::z).normalize();
					}
					v.set_unit();
					break;
				}
				case ScaleParentNodeInfluenceTypeConst_On:{
					m.row(ml::matrix44::row::x).set_length(parent_scale.e_.x_);
					m.row(ml::matrix44::row::y).set_length(parent_scale.e_.y_);
					m.row(ml::matrix44::row::z).set_length(parent_scale.e_.z_);
					v = parent_scale;
					break;
				}
				case ScaleParentNodeInfluenceTypeConst_OnGenerateTime:{
					m.row(ml::matrix44::row::x).set_length(p.GetGenerateTimeParentScale().e_.x_);
					m.row(ml::matrix44::row::y).set_length(p.GetGenerateTimeParentScale().e_.y_);
					m.row(ml::matrix44::row::z).set_length(p.GetGenerateTimeParentScale().e_.z_);
					v = p.GetGenerateTimeParentScale();
					break;
				}
				default:{
					ML_ASSERT(false);
				}
			}

			{
				PrimitiveType *next_p = (read == (active_end - 1)) ? NULL : *(read + 1);
				if(next_p){
					PrefetchData<sizeof(ml::matrix44)>(next_p->generate_time_parent_matrix_);
					PrefetchData<sizeof(ml::vector3d)>(next_p->generate_time_parent_scale_);
				}
			}

			switch(t){			// 平行移動
				case TranslateParentNodeInfluenceTypeConst_On:{					m.set_translate(parent_matrix.translate());						break;	}
				case TranslateParentNodeInfluenceTypeConst_OnGenerateTime:{		m.set_translate(p.GetGenerateTimeParentMatrix().translate());	break;	}
				default:{														ML_ASSERT(false);														}
			}

			p.SetParentMatrix(m);
			p.SetParentScale(v);
		}
	}
	
	/*
	*/
	template<typename PrimitiveType>
	struct CompZ{
		PrimitiveType	*prim_begin_;
	public:
		void	SetPrimitiveBegin(PrimitiveType	*prim_begin){	prim_begin_ = prim_begin;	}
		bool operator ()(int l, int r) const {	return (prim_begin_[l].GetZ() < prim_begin_[r].GetZ());		}
	};
	
	/*
	*/
	template<typename PrimitiveType>
	static void ZSort(PrimitiveType *prim_begin, int active_node_count, int *active_node_index_array, DrawInfo &draw_info){
		ML_PTR_ASSERT(prim_begin);
		ML_PTR_ASSERT(active_node_index_array);
		
		ml::vector3d	pos;
		
#ifdef BM3_TARGET_IDE
		ml::vector3d	view_pos	= draw_info.ViewMatrix().translate();
		for(int i=0 ; i<active_node_count ; ++i){
			int	index = active_node_index_array[i];
			PrimitiveType	&prim	= prim_begin[index];

			draw::policy::MakeCenterPosition(pos, prim, prim.GetParentMatrix());
			pos.mul(draw_info.IdeEffectMatrix());
			
#else
		ml::vector3d	view_pos	= draw_info.InverseViewMatrix().translate();
		for(int i=0 ; i<active_node_count ; ++i){
			int	index = active_node_index_array[i];
			PrimitiveType	&prim	= prim_begin[index];
			
//			PrefetchData<sizeof(PrimitiveType)>(&prim_begin[active_node_index_array[i+1]]);	// 速度未調査
			draw::policy::MakeCenterPosition(pos, prim, prim.GetParentMatrix());
#endif
			pos.mul(draw_info.ViewMatrix());
			prim.SetZ(pos.e_.z_ - view_pos.e_.z_ );
		}
		
		CompZ<PrimitiveType>	compare;
			compare.SetPrimitiveBegin(prim_begin);
		std::sort(&active_node_index_array[0], &active_node_index_array[active_node_count], compare);
	}

	/*
	*/
	template<typename PrimitiveType>
	static void MakeZInfoList(PrimitiveType *prim_begin, int active_node_count, int *active_node_index_array, PrimZList &z_list, WholeZInfo &z_info){
		ML_PTR_ASSERT(prim_begin);
		ML_PTR_ASSERT(active_node_index_array);

		for(int i=0 ; i<active_node_count ; ++i){
			int	index = active_node_index_array[i];
			PrimitiveType	&prim	= prim_begin[index];

			PrimZInfo &pzi		= z_list.Push();
			pzi.emitter_index_	= z_info.emitter_index_;
#ifdef BM3_PLATFORM_psvita
			//j 描画するときは0からのインデックスで良いためヘッダーのポインタを与えておく
			//e set the index header pointer because it use index from zero at rendering.
			pzi.index_buffer_	= z_info.index_head_;//ml::offset_pointer<void*>(z_info.index_head_, i*z_info.prim_index_size_);
#endif
			pzi.vertex_buffer_	= ml::offset_pointer<void*>(z_info.vertex_head_, i*z_info.prim_vtx_size_);
			pzi.draw_count_		= 1;
			pzi.z_value_		= prim.GetZ();
		}
	}

	/*
	*/
	template<typename PrimitiveType, typename ResourceType, typename UpdateInfo>
	static int UpdateLife(PrimitiveType *prim_begin, PrimitiveType *prim_end, int start_prim_index, int **active_node_index, UpdateInfo &update_info, const ResourceType & /*res*/){
		ML_UPDATE_TIMER;
		ML_PTR_ASSERT(prim_begin);
		ML_PTR_ASSERT(prim_end);

		int active_count			= 0;
		int	*temp_active_node_index	= *active_node_index;
		for(PrimitiveType *prim = prim_begin ; prim != prim_end ; ++prim, ++start_prim_index){

			PrimitiveType &p = *prim;

			PrefetchData<sizeof(PrimitiveType)>(prim + 1);

			p.life_.Update(update_info);

			if(p.life_.IsActive()){
				*temp_active_node_index = start_prim_index;
				
				temp_active_node_index++;
				active_count++;
			}
		}

		return active_count;
	}		

	/*
	*/
	template<typename ResourceType, typename UpdateInfo, typename LifePolicy>
	static void	Update(UpdateInfo &update_info, const ResourceType &res, ControlParam<LifePolicy> *control_begin, int *active_index_array, int node_count, const PrimitiveUpdateInfo &pui, int /*begin_count*/, const float /*parent_alpha*/){
		for(int i=0 ; i<node_count ; ++i){
			int index = active_index_array[i];
			Update(update_info, res, control_begin[index], pui);
		}
	}

	/*
	*/
	template<typename PrimitiveType, typename ResourceType>
	static void DrawGenerateShape(const PrimitiveType *prim, int active_node_count, int *active_node_index_array, DrawInfo &draw_info, const ResourceType &res){
		
		const ml::matrix44	*current_matrix	= NULL;
		switch(res.GenerateShape_Basic_GenerateShapeType()){
			case GenerateShapeTypeConst_Point:
				{
					for(int i=0 ; i<active_node_count ; ++i){
						int				index	= active_node_index_array[i];
						PrimitiveType	&p		= *const_cast<PrimitiveType*>(&prim[index]);
						
						if(current_matrix!=&p.GetParentMatrix()){
							current_matrix	= &p.GetParentMatrix();
							debug::DrawPoint(draw_info,	p.GetParentMatrix(), res.Translate_Value_Initial());
						}
					}
				}
				break;
			case GenerateShapeTypeConst_Sphere:
				{
					debug::SphereArea		angle(
						res.GenerateShape_HorizontalArea_StartAngle(), res.GenerateShape_HorizontalArea_EndAngle(),
						res.GenerateShape_VerticalArea_StartAngle(), res.GenerateShape_VerticalArea_EndAngle()
					);
					for(int i=0 ; i<active_node_count ; ++i){
						int				index	= active_node_index_array[i];
						PrimitiveType	&p		= *const_cast<PrimitiveType*>(&prim[index]);
						ml::matrix44	m;
						ml::vector3d	inner;

						m.mul_rot_xyz(res.Field_Area_Slope());
						m.mul(p.GetParentMatrix());
						inner.mul(res.GenerateShape_Area_Outside(), res.GenerateShape_Area_Inside());

						if(current_matrix!=&p.GetParentMatrix()){
							current_matrix	= &p.GetParentMatrix();
							debug::DrawSphere(draw_info,	m, res.Translate_Value_Initial(), inner, angle);
							debug::DrawSphere(draw_info,	m, res.Translate_Value_Initial(), res.GenerateShape_Area_Outside(), angle);
						}
					}
				}
				break;
			case GenerateShapeTypeConst_Cylinder:
				{
					debug::SphereArea		angle(
						res.GenerateShape_HorizontalArea_StartAngle(), res.GenerateShape_HorizontalArea_EndAngle(), 0,0
					);
					for(int i=0 ; i<active_node_count ; ++i){
						int				index	= active_node_index_array[i];
						PrimitiveType	&p		= *const_cast<PrimitiveType*>(&prim[index]);
						ml::matrix44	m;
						ml::vector3d	inner;
						m.mul_rot_xyz(res.Field_Area_Slope());
						m.mul(p.GetParentMatrix());
						inner.mul(res.GenerateShape_Area_Outside(), res.GenerateShape_Area_Inside());

						if(current_matrix!=&p.GetParentMatrix()){
							current_matrix	= &p.GetParentMatrix();
							debug::DrawCylinder(draw_info,	m, res.Translate_Value_Initial(), inner, angle);
							debug::DrawCylinder(draw_info,	m, res.Translate_Value_Initial(), res.GenerateShape_Area_Outside(), angle);
						}
					}
				}
				break;
			case GenerateShapeTypeConst_Cube:
				{
					for(int i=0 ; i<active_node_count ; ++i){
						int				index	= active_node_index_array[i];
						PrimitiveType	&p		= *const_cast<PrimitiveType*>(&prim[index]);
						ml::matrix44	m;
						ml::vector3d	inner;
						m.mul_rot_xyz(res.Field_Area_Slope());
						m.mul(p.GetParentMatrix());
						inner.mul(res.GenerateShape_Area_Outside(), res.GenerateShape_Area_Inside());

						if(current_matrix!=&p.GetParentMatrix()){
							current_matrix	= &p.GetParentMatrix();
							debug::DrawCube(draw_info,	m, res.Translate_Value_Initial(), inner);
							debug::DrawCube(draw_info,	m, res.Translate_Value_Initial(), res.GenerateShape_Area_Outside());
						}
					}
				}
				break;
			case GenerateShapeTypeConst_Model:
				{
					for(int i=0 ; i<active_node_count ; ++i){
						int				index	= active_node_index_array[i];
						PrimitiveType	&p		= *const_cast<PrimitiveType*>(&prim[index]);
						ml::matrix44	m;
						m.set_unit();
						m.set_scale(res.GenerateShape_Area_Scale());
						m.set_translate(res.Translate_Value_Initial());
						m.mul(p.GetParentMatrix());
						ModelRenderer::instance().DebugDraw(res.GenerateShape_Basic_ModelName(), m, draw_info);
					}
				}
			
				break;
		}
	}

	/*
	*/
	template<typename PrimitiveType, typename ResourceType>
	static void InitializeField(PrimitiveType *prim_begin, PrimitiveType *prim_end, ml::ui8* field_begin, const ResourceType &res, ml::random &random){

		ML_INITIALIZE_TIMER;
		const typename ResourceType::ChildArrayType_	&child_array	= res.ChildArray();
		int												child_count		= ml::minimum(res.ChildArray().Count(), BM3_MAX_CHILD_NODE_COUNT);
	
		ml::ui8* field = field_begin;
		for(PrimitiveType *p = prim_begin ;p != prim_end ;++ p){

			#define	BM3_INITIALIZE_FIELD(NAMESPACE, NODE_TYPE)												\
				new(field) NAMESPACE::Field;																\
				NAMESPACE::FieldManipulator::Initialize(													\
					*reinterpret_cast<NAMESPACE::Field*>(field),											\
					reinterpret_cast<const BM3_FIELD_RESOURCE_TYPE(NODE_TYPE) &>(child_array.Child(index)),	\
					random,																					\
					*p);

			#define	BM3_FIELD_NEXT_BUFFER(NAMESPACE)	field += sizeof(NAMESPACE::Field);

			for(int index = 0;index != child_count;++ index){
				switch(child_array.Child(index).Basic_Meta_NodeType()){
					case NodeTypeConst_ControlNull:																			break;
					case NodeTypeConst_EmitterQuad:																			break;
					case NodeTypeConst_EmitterParticle:																		break;
					case NodeTypeConst_EmitterSimpleParticle:																break;
					case NodeTypeConst_EmitterMassParticle:																	break;
					case NodeTypeConst_EmitterModel:																		break;
					case NodeTypeConst_EmitterStripe:																		break;
					case NodeTypeConst_FieldGravity:		{	BM3_INITIALIZE_FIELD(field::gravity		,Gravity);		BM3_FIELD_NEXT_BUFFER(field::gravity);		}	break;		
					case NodeTypeConst_FieldWind:			{	BM3_INITIALIZE_FIELD(field::wind		,Wind);			BM3_FIELD_NEXT_BUFFER(field::wind);			}	break;
					case NodeTypeConst_FieldVortex:			{	BM3_INITIALIZE_FIELD(field::vortex		,Vortex);		BM3_FIELD_NEXT_BUFFER(field::vortex);		}	break;
					case NodeTypeConst_FieldRadialpower:	{	BM3_INITIALIZE_FIELD(field::radialpower	,Radialpower);	BM3_FIELD_NEXT_BUFFER(field::radialpower);	}	break;
					case NodeTypeConst_FieldDeceleration:	{	BM3_INITIALIZE_FIELD(field::deceleration,Deceleration);	BM3_FIELD_NEXT_BUFFER(field::deceleration);	}	break;
					case NodeTypeConst_FieldCollision:{
							BM3_INITIALIZE_FIELD(field::collision	,Collision);
							policy::impl::Bounding_Sphere::Initialize(	res, random, *reinterpret_cast<field::collision::Field*>(field), p->translate_, p->scale_);
							BM3_FIELD_NEXT_BUFFER(field::collision);	
						}
						break;
					case NodeTypeConst_FieldGoal:			{	BM3_INITIALIZE_FIELD(field::goal		,Goal);			BM3_FIELD_NEXT_BUFFER(field::goal);			}	break;
					case NodeTypeConst_ObjectReference:			break;
					default:							ML_ASSERT(false);BM3_ASSUME(0);										break;
				}
			}
			#undef BM3_FIELD_NEXT_BUFFER
			#undef BM3_INITIALIZE_FIELD
		}
	}

	/*
	*/
	template<typename PrimitiveType, typename ResourceType>
	static void PrepareField(PrimitiveType *prim, int active_node_count, int *active_node_index_array, ml::ui8* field_buffer, int field_all_size, UpdateInfo &update_info, const ResourceType &res, const obj::RootNodeBase &root){
		
		ML_MAKEVERTEX_TIMER;
		const typename ResourceType::ChildArrayType_	&child_array	= res.ChildArray();
		int												child_count		= ml::minimum(res.ChildArray().Count(), BM3_MAX_CHILD_NODE_COUNT);

		for(int i=0 ; i<active_node_count ; ++i){
			int			index	= active_node_index_array[i];
			PrimitiveType	&p	= prim[index];
			ml::ui8		*f	= ml::offset_pointer<ml::ui8*>(field_buffer, index * field_all_size);
			
			PrefetchData<sizeof(PrimitiveType)>(&prim[active_node_index_array[i+1]]);

			#define BM3_PREPARE_FIELDOBJ(NAMESPACE)	NAMESPACE::Field &field = *reinterpret_cast<NAMESPACE::Field*>(f)
			#define	BM3_PREPARE_FIELD(NAMESPACE, NODE_TYPE)									\
				NAMESPACE::FieldManipulator::Prepare(p.field_info_ ,field ,					\
					reinterpret_cast<const BM3_FIELD_RESOURCE_TYPE(NODE_TYPE) &>(child_array.Child(index)), p.GetParentMatrix(), update_info, root, p);	\
				f += sizeof(NAMESPACE::Field);

			p.field_info_.Reset();
			for(int index = 0;index != child_count;++ index){
				switch(child_array.Child(index).Basic_Meta_NodeType()){
					case NodeTypeConst_ControlNull:																														break;
					case NodeTypeConst_EmitterQuad:																														break;
					case NodeTypeConst_EmitterParticle:																													break;
					case NodeTypeConst_EmitterSimpleParticle:																											break;
					case NodeTypeConst_EmitterMassParticle:																												break;
					case NodeTypeConst_EmitterModel:																													break;
					case NodeTypeConst_EmitterStripe:																													break;
					case NodeTypeConst_FieldGravity:		{	BM3_PREPARE_FIELDOBJ(field::gravity);		BM3_PREPARE_FIELD(field::gravity		,Gravity);		}	break;		
					case NodeTypeConst_FieldWind:			{	BM3_PREPARE_FIELDOBJ(field::wind);			BM3_PREPARE_FIELD(field::wind			,Wind);			}	break;
					case NodeTypeConst_FieldVortex:			{	BM3_PREPARE_FIELDOBJ(field::vortex);		BM3_PREPARE_FIELD(field::vortex			,Vortex);		}	break;
					case NodeTypeConst_FieldRadialpower:	{	BM3_PREPARE_FIELDOBJ(field::radialpower);	BM3_PREPARE_FIELD(field::radialpower	,Radialpower);	}	break;
					case NodeTypeConst_FieldDeceleration:	{	BM3_PREPARE_FIELDOBJ(field::deceleration);	BM3_PREPARE_FIELD(field::deceleration	,Deceleration);	}	break;
					case NodeTypeConst_FieldCollision:		{	
							BM3_PREPARE_FIELDOBJ(field::collision);
							policy::impl::Bounding_Sphere::Prepare(	res, field, p.scale_, p.GetParentScale());
							BM3_PREPARE_FIELD(field::collision		,Collision);
						}
						break;
					case NodeTypeConst_FieldGoal:			{	BM3_PREPARE_FIELDOBJ(field::goal);			BM3_PREPARE_FIELD(field::goal			,Goal);			}	break;
					case NodeTypeConst_ObjectReference:			break;
					default:							ML_ASSERT(false);BM3_ASSUME(0);								break;
				}
			}
			#undef BM3_PREPARE_FIELD
			#undef BM3_PREPARE_FIELDOBJ
		}
	}

	/*
	*/
	template<typename PrimitiveType, typename ResourceType>
	static void SwitchResourceField(PrimitiveType * /*prim_begin*/, int active_node_count, int *active_node_index_array, ml::ui8* field_buffer, int field_all_size, const ResourceType &res, const ResourceType &old_res, ml::random &random){

		ML_PTR_ASSERT(active_node_index_array);
		ML_PTR_ASSERT(field_buffer);

		const typename ResourceType::ChildArrayType_	&child_array		= res.ChildArray();
		const typename ResourceType::ChildArrayType_	&old_child_array	= old_res.ChildArray();
		int												child_count		= ml::minimum(res.ChildArray().Count(), BM3_MAX_CHILD_NODE_COUNT);
		
		for(int i=0 ; i<active_node_count ; ++i){
			int			index	= active_node_index_array[i];
			ml::ui8		*f	= ml::offset_pointer<ml::ui8*>(field_buffer, index * field_all_size);

//			PrefetchData<sizeof(PrimitiveType)>(&prim[active_node_index_array[i+1]]);

			#define	BM3_SWITCHRESOURCE_FIELD(NAMESPACE, NODE_TYPE)	\
				new(f) NAMESPACE::Field;							\
				NAMESPACE::FieldManipulator::SwitchResource(		\
					*reinterpret_cast<NAMESPACE::Field*>(f) ,		\
					reinterpret_cast<const BM3_FIELD_RESOURCE_TYPE(NODE_TYPE) &>(child_array.Child(index)),		\
					reinterpret_cast<const BM3_FIELD_RESOURCE_TYPE(NODE_TYPE) &>(old_child_array.Child(index)),	\
					random);										\
				f += sizeof(NAMESPACE::Field);

			for(int index = 0;index != child_count;++ index){
				switch(child_array.Child(index).Basic_Meta_NodeType()){
					case NodeTypeConst_ControlNull:																		break;
					case NodeTypeConst_EmitterQuad:																		break;
					case NodeTypeConst_EmitterParticle:																	break;
					case NodeTypeConst_EmitterModel:																	break;
					case NodeTypeConst_EmitterStripe:																	break;
					case NodeTypeConst_FieldGravity:		BM3_SWITCHRESOURCE_FIELD(field::gravity		,Gravity);		break;		
					case NodeTypeConst_FieldWind:			BM3_SWITCHRESOURCE_FIELD(field::wind		,Wind);			break;
					case NodeTypeConst_FieldVortex:			BM3_SWITCHRESOURCE_FIELD(field::vortex		,Vortex);		break;
					case NodeTypeConst_FieldRadialpower:	BM3_SWITCHRESOURCE_FIELD(field::radialpower	,Radialpower);	break;
					case NodeTypeConst_FieldDeceleration:	BM3_SWITCHRESOURCE_FIELD(field::deceleration,Deceleration);	break;
					case NodeTypeConst_FieldCollision:		BM3_SWITCHRESOURCE_FIELD(field::collision	,Collision);	break;
					case NodeTypeConst_FieldGoal:			BM3_SWITCHRESOURCE_FIELD(field::goal		,Goal);			break;
					case NodeTypeConst_ObjectReference:			break;
					default:							ML_ASSERT(false);BM3_ASSUME(0);									break;
				}
			}
			#undef BM3_SWITCHRESOURCE_FIELD
		}
	}


};

#undef BM3_FIELD_RESOURCE_TYPE

}	// namespace prim
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_PRIMITIVE_BM3_PRIM_COMMONMANIPULATOR_H
