#ifndef BM3_SDK_INC_BM3_CORE_PRIMITIVE_BM3_PRIM_QUAD_H
#define BM3_SDK_INC_BM3_CORE_PRIMITIVE_BM3_PRIM_QUAD_H

#include "bm3_Primitive.h"
#include "../bm3_UpdateInfo.h"
#include "../field/bm3_field_Gravity.h"
#include "../field/bm3_field_Wind.h"
#include "../field/bm3_field_Vortex.h"
#include "../field/bm3_field_Radialpower.h"
#include "../field/bm3_field_Deceleration.h"
#include "../field/bm3_field_Collision.h"
#include "../field/bm3_field_Goal.h"
#include "../resource/bm3_res_Gravity.h"
#include "../resource/bm3_res_Wind.h"
#include "../resource/bm3_res_Vortex.h"
#include "../resource/bm3_res_Radialpower.h"
#include "../resource/bm3_res_Deceleration.h"
#include "../resource/bm3_res_Collision.h"
#include "../resource/bm3_res_Goal.h"
#include "../policy/Field/bm3_policy_Bounding_Sphere.h"
#include "../draw/policy/bm3_draw_policy_Common.h"
#include <ml/utility/ml_simpletimer.h>

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

class DrawInfo;

namespace prim{
namespace quad{

#if defined(BM3_TARGET_IDE)
	#define	BM3_FIELD_RESOURCE_TYPE(NODE_TYPE)	res::IdeNode
#endif
#if defined(BM3_TARGET_EMBEDDED)
	#define	BM3_FIELD_RESOURCE_TYPE(NODE_TYPE)	res::NODE_TYPE
#endif


/*!
	@brief	クアッドエミッタ一つ一つのパラメータの処理
	@note	このエミッターに関連するフィールドの処理も行っている
			関数は他のエミッタと同じインターフェイスになっている
*/
class PrimitiveManipulator{

public:
	/*!
		@brief	各種パラメータの初期化
		@param	prim_begin	エミッターの先頭ポインタ
		@param	prim_end	エミッターの終了ポインタ
		@param	res			エミッターリソースクラスの参照
		@param	random		ランダムクラス
		@param	random_seed	デフォルトのランダムシード（同期用）
		@return なし
	*/
	template<
		typename PrimitiveType,
		typename ResourceType
	>
	static void Initialize(PrimitiveType *prim_begin, PrimitiveType *prim_end, const ResourceType &res, ml::random random, unsigned int /*random_seed*/){

		ML_INITIALIZE_TIMER;
		ML_PTR_ASSERT(prim_begin);
		ML_PTR_ASSERT(prim_end);

		{
			int tex_ref_count = TextureReferenceCount(res.Texture_Basic_TextureDrawType());

			unsigned int pixel_type = res.Blend_Basic_BlendAlphaType()	<< 0 | res.Color_Basic_ColorType()	<< 2 | res.Color_Basic_Flat() << 4;

			#define INIT_PIXLE_CASE(BLEND, COLOR, IS_FLAT)	(BLEND << 0 | COLOR << 2 | IS_FLAT << 4)
			#define FLAT_COLOR													\
				p->color_right_top_.value_		= p->color_left_top_.value_;	\
				p->color_left_bottom_.value_	= p->color_left_top_.value_;	\
				p->color_right_bottom_.value_	= p->color_left_top_.value_;

			#define CONST_COLOR	\
				policy::impl::Color_Constant::Initialize(	res, random, p->color_right_top_.value_,	res.Color_VertexRightTop_Value(),		res.Color_VertexRightTop_Range(), res.Color_VertexRightTop_RandomChannelSync(),			p->alpha_);	\
				policy::impl::Color_Constant::Initialize(	res, random, p->color_left_bottom_.value_,	res.Color_VertexLeftBottom_Value(),		res.Color_VertexLeftBottom_Range(), res.Color_VertexLeftBottom_RandomChannelSync(),		p->alpha_);	\
				policy::impl::Color_Constant::Initialize(	res, random, p->color_right_bottom_.value_,	res.Color_VertexRightBottom_Value(),	res.Color_VertexRightBottom_Range(), res.Color_VertexRightBottom_RandomChannelSync(),	p->alpha_);

			#define CURVE_COLOR	\
				policy::impl::Color_Curve::Initialize(		res, random, p->color_right_top_.value_,	res.Color_VertexRightTop_Value(),		res.Color_VertexRightTop_Range(),														p->alpha_);	\
				policy::impl::Color_Curve::Initialize(		res, random, p->color_left_bottom_.value_,	res.Color_VertexLeftBottom_Value(),		res.Color_VertexLeftBottom_Range(),														p->alpha_);	\
				policy::impl::Color_Curve::Initialize(		res, random, p->color_right_bottom_.value_,	res.Color_VertexRightBottom_Value(),	res.Color_VertexRightBottom_Range(),													p->alpha_);

			const TextureAnimationTypeConst tat[TextureLayer::Max] = {
				res.Texture_Layer1_TextureAnimationType(),
				res.Texture_Layer2_TextureAnimationType()
			};


			for(PrimitiveType *p = prim_begin ;p != prim_end ;++ p){
				PrimitiveInitializeInfo	pii;
				{
					pii.random_seed_	= ml::ui32(p);
				}

				p->Initialize(res, random);

				// Matrix関係
				{
					switch(res.Scale_Basic_ScaleType()){
						case ScaleTypeConst_Constant:		policy::impl::MatrixScale_Constant<>::Initialize(			res, random, p->scale_.value_															);		break;
						case ScaleTypeConst_NewtonDynamics:	policy::impl::MatrixScale_Newtondynamics<>::Initialize(		res, random, p->scale_.value_,		p->scale_.speed_,		p->scale_.acceleration_		);		break;
						case ScaleTypeConst_CurveValue:		policy::impl::MatrixScale_CurveValue<>::Initialize(			res, random, p->scale_.value_															, pii);	break;
						case ScaleTypeConst_CurveSpeed:		policy::impl::MatrixScale_CurveSpeed<>::Initialize(			res, random, p->scale_.value_,		p->scale_.speed_									, pii);	break;
						default:	ML_ASSERT(false);		break;
					}

					switch(res.Rotate_Basic_RotateType()){
						case RotateTypeConst_Constant:			policy::impl::MatrixRotate_Constant::Initialize(			res, random, p->rotate_.value_															);		break;
						case RotateTypeConst_NewtonDynamics:	policy::impl::MatrixRotate_Newtondynamics::Initialize(		res, random, p->rotate_.value_,		p->rotate_.speed_,		p->rotate_.acceleration_	);		break;
						case RotateTypeConst_CurveValue:		policy::impl::MatrixRotate_CurveValue::Initialize(			res, random, p->rotate_.value_															, pii);	break;
						case RotateTypeConst_CurveSpeed:		policy::impl::MatrixRotate_CurveSpeed::Initialize(			res, random, p->rotate_.value_,		p->rotate_.speed_									, pii);	break;
						default:	ML_ASSERT(false);			break;
					}

					switch(res.GenerateShape_Basic_GenerateShapeType()){
						case GenerateShapeTypeConst_Point:		policy::impl::GenerateShape_Point::Initialize(			res, random, p->generate_shape_.position_ ,p->generate_shape_.direction_	);	break;
						case GenerateShapeTypeConst_Sphere:		policy::impl::GenerateShape_Sphere::Initialize(			res, random, p->generate_shape_.position_ ,p->generate_shape_.direction_	);	break;
						case GenerateShapeTypeConst_Cylinder:	policy::impl::GenerateShape_Cylinder::Initialize(		res, random, p->generate_shape_.position_ ,p->generate_shape_.direction_	);	break;
						case GenerateShapeTypeConst_Cube:		policy::impl::GenerateShape_Cube::Initialize(			res, random, p->generate_shape_.position_ ,p->generate_shape_.direction_	);	break;
						case GenerateShapeTypeConst_Model:		policy::impl::GenerateShape_Model::Initialize(			res, random, p->generate_shape_.position_ ,p->generate_shape_.direction_	);	break;
						default:	ML_ASSERT(false);			break;
					}

					switch(res.Translate_Basic_TranslateType()){
						case TranslateTypeConst_Constant:		policy::impl::MatrixTranslate_Constant::Initialize(			res, random, p->translate_.value_																,p->generate_shape_.position_									);	break;
						case TranslateTypeConst_NewtonDynamics:	policy::impl::MatrixTranslate_Newtondynamics::Initialize(	res, random, p->translate_.value_,	p->translate_.speed_,	p->translate_.acceleration_			,p->generate_shape_.position_	,p->generate_shape_.direction_	);	break;
						case TranslateTypeConst_CurveValue:		policy::impl::MatrixTranslate_CurveValue::Initialize(		res, random, p->translate_.value_														, pii	,p->generate_shape_.position_									);	break;
						case TranslateTypeConst_CurveSpeed:		policy::impl::MatrixTranslate_CurveSpeed::Initialize(		res, random, p->translate_.value_,	p->translate_.speed_								, pii	,p->generate_shape_.position_									);	break;
						default:	ML_ASSERT(false);			break;
					}
				}

				// Color関係
				{
					switch(pixel_type){
						case INIT_PIXLE_CASE(BlendAlphaTypeConst_Constant,	ColorTypeConst_Constant, 0):
							policy::impl::Blend_Constant::Initialize(	res, random, p->alpha_);
							policy::impl::Color_Constant::Initialize(	res, random, p->color_left_top_.value_,	res.Color_VertexLeftTop_Value(),	res.Color_VertexLeftTop_Range(), res.Color_VertexLeftTop_RandomChannelSync(),	p->alpha_);
							CONST_COLOR;
							break;
						case INIT_PIXLE_CASE(BlendAlphaTypeConst_Constant,	ColorTypeConst_Curve, 0):
							policy::impl::Blend_Constant::Initialize(	res, random, p->alpha_);
							policy::impl::Color_Curve::Initialize(		res, random, p->color_left_top_.value_,	res.Color_VertexLeftTop_Value(),	res.Color_VertexLeftTop_Range(),												p->alpha_);
							CURVE_COLOR;
							break;
						case INIT_PIXLE_CASE(BlendAlphaTypeConst_Curve,		ColorTypeConst_Constant, 0):
							policy::impl::Blend_Curve::Initialize(		res, random, p->alpha_);
							policy::impl::Color_Constant::Initialize(	res, random, p->color_left_top_.value_,	res.Color_VertexLeftTop_Value(),	res.Color_VertexLeftTop_Range(), res.Color_VertexLeftTop_RandomChannelSync(),	p->alpha_);
							CONST_COLOR;
							break;
						case INIT_PIXLE_CASE(BlendAlphaTypeConst_Curve,		ColorTypeConst_Curve, 0):
							policy::impl::Blend_Curve::Initialize(		res, random, p->alpha_);
							policy::impl::Color_Curve::Initialize(		res, random, p->color_left_top_.value_,	res.Color_VertexLeftTop_Value(),	res.Color_VertexLeftTop_Range(),												p->alpha_);
							CURVE_COLOR;
							break;
						case INIT_PIXLE_CASE(BlendAlphaTypeConst_Constant,	ColorTypeConst_Constant, 1):
							policy::impl::Blend_Constant::Initialize(	res, random, p->alpha_);
							policy::impl::Color_Constant::Initialize(	res, random, p->color_left_top_.value_,	res.Color_VertexLeftTop_Value(),	res.Color_VertexLeftTop_Range(), res.Color_VertexLeftTop_RandomChannelSync(),	p->alpha_);
							FLAT_COLOR;
							break;
						case INIT_PIXLE_CASE(BlendAlphaTypeConst_Constant,	ColorTypeConst_Curve, 1):
							policy::impl::Blend_Constant::Initialize(	res, random, p->alpha_);
							policy::impl::Color_Curve::Initialize(		res, random, p->color_left_top_.value_,	res.Color_VertexLeftTop_Value(),	res.Color_VertexLeftTop_Range(),												p->alpha_);
							FLAT_COLOR;
							break;
						case INIT_PIXLE_CASE(BlendAlphaTypeConst_Curve,		ColorTypeConst_Constant, 1):
							policy::impl::Blend_Curve::Initialize(		res, random, p->alpha_);
							policy::impl::Color_Constant::Initialize(	res, random, p->color_left_top_.value_,	res.Color_VertexLeftTop_Value(),	res.Color_VertexLeftTop_Range(), res.Color_VertexLeftTop_RandomChannelSync(),	p->alpha_);
							FLAT_COLOR;
							break;
						case INIT_PIXLE_CASE(BlendAlphaTypeConst_Curve,		ColorTypeConst_Curve, 1):
							policy::impl::Blend_Curve::Initialize(		res, random, p->alpha_);
							policy::impl::Color_Curve::Initialize(		res, random, p->color_left_top_.value_,	res.Color_VertexLeftTop_Value(),	res.Color_VertexLeftTop_Range(),												p->alpha_);
							FLAT_COLOR;
							break;
						default: ML_ASSERT(false);	break;
					}

/*
					policy::impl::Blend_Constant::Initialize(	res, random, p->alpha_);
					policy::impl::Blend_Curve::Initialize(		res, random, p->alpha_);

					policy::impl::Color_Constant::Initialize(	res, random, p->color_left_top_.value_,	res.Color_VertexLeftTop_Value(),	res.Color_VertexLeftTop_Range(), res.Color_VertexLeftTop_RandomChannelSync(),	p->alpha_);
					policy::impl::Color_Curve::Initialize(		res, random, p->color_left_top_.value_,	res.Color_VertexLeftTop_Value(),	res.Color_VertexLeftTop_Range(),												p->alpha_);

					if(res.Color_Basic_Flat()){
						p->color_right_top_.value_		= p->color_left_top_.value_;
						p->color_left_bottom_.value_	= p->color_left_top_.value_;
						p->color_right_bottom_.value_	= p->color_left_top_.value_;
					}else{
						policy::impl::Color_Constant::Initialize(	res, random, p->color_right_top_.value_,	res.Color_VertexRightTop_Value(),		res.Color_VertexRightTop_Range(), res.Color_VertexRightTop_RandomChannelSync(),			p->alpha_);
						policy::impl::Color_Curve::Initialize(		res, random, p->color_right_top_.value_,	res.Color_VertexRightTop_Value(),		res.Color_VertexRightTop_Range(),														p->alpha_);
						policy::impl::Color_Constant::Initialize(	res, random, p->color_left_bottom_.value_,	res.Color_VertexLeftBottom_Value(),		res.Color_VertexLeftBottom_Range(), res.Color_VertexLeftBottom_RandomChannelSync(),		p->alpha_);
						policy::impl::Color_Curve::Initialize(		res, random, p->color_left_bottom_.value_,	res.Color_VertexLeftBottom_Value(),		res.Color_VertexLeftBottom_Range(),														p->alpha_);
						policy::impl::Color_Constant::Initialize(	res, random, p->color_right_bottom_.value_,	res.Color_VertexRightBottom_Value(),	res.Color_VertexRightBottom_Range(), res.Color_VertexRightBottom_RandomChannelSync(),	p->alpha_);
						policy::impl::Color_Curve::Initialize(		res, random, p->color_right_bottom_.value_,	res.Color_VertexRightBottom_Value(),	res.Color_VertexRightBottom_Range(),													p->alpha_);
					}
*/
				}

				// Texture関係
				{
					for(int j = 0;j != tex_ref_count;++ j){
#ifdef BM3_TARGET_IDE
						// 途中でタイプを変えると強制終了してしまうので関係ないパラメータも初期化しておく。
						p->texture_[j].frame_	= 0;
						p->texture_[j].step_	= 0;
						p->texture_[j].v_scroll_= 0;
						p->texture_[j].v_scroll_= 0;
#endif
						switch(tat[j]){
							case TextureAnimationTypeConst_Nothing:		policy::impl::Texture_Nothing::Initialize(	res, random, p->texture_[j].value_,															TextureLayer::Type(j));	break;
							case TextureAnimationTypeConst_Crop:		policy::impl::Texture_Crop::Initialize(		res, random, p->texture_[j].value_,	p->texture_[j].frame_,		p->texture_[j].step_,		TextureLayer::Type(j));	break;
							case TextureAnimationTypeConst_Scroll:		policy::impl::Texture_Scroll::Initialize(	res, random, p->texture_[j].value_,	p->texture_[j].h_scroll_,	p->texture_[j].v_scroll_,	TextureLayer::Type(j));	break;
							case TextureAnimationTypeConst_Pattern:		policy::impl::Texture_Pattern::Initialize(	res, random, p->texture_[j].value_,	p->texture_[j].frame_,		p->texture_[j].step_,		TextureLayer::Type(j));	break;
							case TextureAnimationTypeConst_Transform:	policy::impl::Texture_Transform::Initialize(res, random, p->texture_[j].value_,															TextureLayer::Type(j));	break;
							default:	ML_ASSERT(false);	break;		
						}
					}
				}

				// Transform関係
				{
					policy::impl::Transform_Constant::Initialize(	res, random, p->transform_.left_top_, p->transform_.right_top_, p->transform_.left_bottom_, p->transform_.right_bottom_);
					policy::impl::Transform_Curve::Initialize(		res, random, p->transform_.left_top_, p->transform_.right_top_, p->transform_.left_bottom_, p->transform_.right_bottom_);
					policy::impl::Transform_Vertex::Initialize(		res, random, p->transform_.left_top_, p->transform_.right_top_, p->transform_.left_bottom_, p->transform_.right_bottom_);
				}

				// 初期化
				{
					p->previous_position_.set_unit();
					p->directional_.set_unit();
				}

				// フィールド
				p->field_info_.Reset();
			}
			#undef FLAT_COLOR
			#undef CONST_COLOR
			#undef CURVE_COLOR
			#undef INIT_PIXLE_CASE
		}
	}

	template<typename PrimitiveType, typename ResourceType>
	static void InitializeField(PrimitiveType *prim_begin, PrimitiveType *prim_end, ml::ui8* field_begin, const ResourceType &res, ml::random &random){

		CommonPrimitiveManipulator::InitializeField(prim_begin, prim_end, field_begin, res, random);
	}


#define	BM3_MAKE_CASE(TRANSFORM, BLEND, COLOR)	((TRANSFORM << 2) | (BLEND << 1) | (COLOR << 0))

	template<typename PrimitiveType, typename ResourceType, typename UpdateInfo>
	static int Update(PrimitiveType *prim_begin, PrimitiveType *prim_end, int start_prim_index, int **active_node_index, UpdateInfo &update_info, const ResourceType &res, ml::random &random){

		ML_UPDATE_TIMER;
		ML_PTR_ASSERT(prim_begin);
		ML_PTR_ASSERT(prim_end);

#if defined(BM3_TARGET_EMBEDDED)
		PrimitiveInfo uipi(
			&res.node_header_,
			reinterpret_cast<const void *>(&res),
			prim_begin,
			prim_end);
		update_info.OnPrimitiveUpdateBegin(uipi);
#endif

		int tex_ref_count = TextureReferenceCount(res.Texture_Basic_TextureDrawType());

		const TextureAnimationTypeConst tat[TextureLayer::Max] = {
			res.Texture_Layer1_TextureAnimationType(),
			res.Texture_Layer2_TextureAnimationType()
		};

		unsigned int make_type = BM3_MAKE_CASE(res.Deformation_Basic_TransformType(), res.Blend_Basic_BlendAlphaType(), res.Color_Basic_ColorType());

		int active_count			= 0;
		int	*temp_active_node_index	= *active_node_index;
		for(PrimitiveType *prim = prim_begin ; prim != prim_end ; ++prim, ++start_prim_index){

			PrimitiveType &p = *prim;

			PrefetchData<sizeof(PrimitiveType)>(prim + 1);

			bool	is_first_frame	= p.IsFirstFrame();
			p.life_.Update(update_info);

			if(p.IsActive()){
				*temp_active_node_index = start_prim_index;

				ml::ui32	random_seed	= ml::ui32(&p);
				if(is_first_frame == false){
					
					Update(p, update_info, res, tat, tex_ref_count, random_seed, random);
				}
				UpdatePixel(&p, update_info, res, make_type, random_seed);
				temp_active_node_index++;
				active_count++;
//				PrefetchData<sizeof(int)>(temp_active_node_index);
			}
		}

#if defined(BM3_TARGET_EMBEDDED)
		update_info.OnPrimitiveUpdateEnd(uipi);
#endif

		return active_count;
	}	

	template<typename PrimitiveType, typename ResourceType, typename UpdateInfo>
	static int Update(UpdateInfo &update_info, const ResourceType &res, PrimitiveType *prim_begin, int *active_node_index, int active_node_count, const PrimitiveUpdateInfo & /*pui*/, int begin_count){
		ML_UPDATE_TIMER;
		ML_PTR_ASSERT(prim_begin);

		int tex_ref_count = TextureReferenceCount(res.Texture_Basic_TextureDrawType());

		const TextureAnimationTypeConst tat[TextureLayer::Max] = {
			res.Texture_Layer1_TextureAnimationType(),
			res.Texture_Layer2_TextureAnimationType()
		};
		unsigned int make_type = BM3_MAKE_CASE(res.Deformation_Basic_TransformType(), res.Blend_Basic_BlendAlphaType(), res.Color_Basic_ColorType());

		for(int i=0 ; i<active_node_count ; ++i){
			int index = active_node_index[i] - begin_count;
			if(active_node_count<index)	return i;

//			PrefetchData<sizeof(PrimitiveType)>(prim + 1);
			ml::ui32	random_seed	= ml::ui32(&prim_begin[index]);
			Update(prim_begin[index], update_info, res, tat, tex_ref_count, random_seed);
			UpdatePixel(&prim_begin[index], update_info, res, make_type, random_seed);
		}
		return active_node_count;
	}

	template<typename PrimitiveType, typename ResourceType, typename UpdateInfo>
	static BM3_FORCEINLINE void Update(PrimitiveType &p, UpdateInfo &update_info, const ResourceType &res, const TextureAnimationTypeConst *tat, int tex_ref_count, const unsigned int &random_seed, ml::random &random){

		#define	BM3_PM_AllowFor_Scale_Basic_Square	true
#ifdef BM3_PLATFORM_psp
		#include "bm3_prim_update_matrix_psp.inc"
#else
		#include "bm3_prim_update_matrix.inc"
#endif
		#undef	BM3_PM_AllowFor_Scale_Basic_Square

		// Texture関係
		{
			for(int j = 0;j != tex_ref_count;++ j){
				switch(tat[j]){
					case TextureAnimationTypeConst_Nothing:		policy::impl::Texture_Nothing::Update(	update_info, res, p.texture_[j].value_,																	tat[j], TextureLayer::Type(j));	break;
					case TextureAnimationTypeConst_Crop:		policy::impl::Texture_Crop::Update(		update_info, res, p.texture_[j].value_,	p.texture_[j].frame_,		p.texture_[j].step_,		random,	tat[j], TextureLayer::Type(j));	break;
					case TextureAnimationTypeConst_Scroll:		policy::impl::Texture_Scroll::Update(	update_info, res, p.texture_[j].value_,	p.texture_[j].h_scroll_,	p.texture_[j].v_scroll_,			tat[j], TextureLayer::Type(j));	break;
					case TextureAnimationTypeConst_Pattern:		policy::impl::Texture_Pattern::Update(	update_info, res, p.texture_[j].value_,	p.texture_[j].frame_,		p.texture_[j].step_,		random,	tat[j], TextureLayer::Type(j));	break;
					case TextureAnimationTypeConst_Transform:	policy::impl::Texture_Transform::Update(update_info, res, p.texture_[j].value_,																	tat[j], TextureLayer::Type(j));	break;
					default:	ML_ASSERT(false);	break;
				}
			}
		}
	}

	template<typename PrimitiveType, typename ResourceType>
	static void ZSort(PrimitiveType *prim_begin, int active_node_count, int *active_node_index_array, DrawInfo &draw_info, const ResourceType &res, WholeZInfo &z_info, PrimZList &z_list){
		ML_MAKEVERTEX_TIMER;
		if(z_info.is_enabled_){
			CommonPrimitiveManipulator::ZSort(prim_begin, active_node_count, active_node_index_array, draw_info);
			CommonPrimitiveManipulator::MakeZInfoList(prim_begin, active_node_count, active_node_index_array, z_list, z_info);
		}
		else if(res.Basic_Basic_ZSort()){
			CommonPrimitiveManipulator::ZSort(prim_begin, active_node_count, active_node_index_array, draw_info);
		}
	}

#define COLOR_CONST()																																													\
	float blend_value	= p->alpha_;																																							\
	policy::impl::Color_Constant::Prepare(	res, p->color_left_top_.value_,				blend_value);	\
	if(res.Color_Basic_Flat()){																																											\
		p->color_right_top_.value_		= p->color_left_top_.value_;																																	\
		p->color_left_bottom_.value_	= p->color_left_top_.value_;																																	\
		p->color_right_bottom_.value_	= p->color_left_top_.value_;																																	\
	}else{																																																\
		policy::impl::Color_Constant::Prepare(	res, p->color_right_top_.value_,		blend_value);	\
		policy::impl::Color_Constant::Prepare(	res, p->color_left_bottom_.value_,		blend_value);	\
		policy::impl::Color_Constant::Prepare(	res, p->color_right_bottom_.value_,		blend_value);	\
	}

#define COLOR_CURVE()																																																					\
	float blend_value	= p->alpha_;																																															\
	policy::impl::Color_Curve::Prepare(	res, p->color_left_top_.value_,		res.Color_VertexLeftTop_Curve(),		res.Color_VertexLeftTop_Range(),			res.Color_VertexLeftTop_RandomChannelSync(),	p->LifeRate(), random_seed, blend_value);		\
	if(res.Color_Basic_Flat()){																																																			\
		p->color_right_top_.value_		= p->color_left_top_.value_;																																									\
		p->color_left_bottom_.value_	= p->color_left_top_.value_;																																								\
		p->color_right_bottom_.value_	= p->color_left_top_.value_;																																									\
	}else{																																																								\
		policy::impl::Color_Curve::Prepare(	res, p->color_right_top_.value_,	res.Color_VertexRightTop_Curve(),		res.Color_VertexRightTop_Range(),		res.Color_VertexRightTop_RandomChannelSync(),	p->LifeRate(), random_seed, blend_value);	\
		policy::impl::Color_Curve::Prepare(	res, p->color_left_bottom_.value_,	res.Color_VertexLeftBottom_Curve(),		res.Color_VertexLeftBottom_Range(),		res.Color_VertexLeftBottom_RandomChannelSync(),	p->LifeRate(), random_seed, blend_value);	\
		policy::impl::Color_Curve::Prepare(	res, p->color_right_bottom_.value_,	res.Color_VertexRightBottom_Curve(),	res.Color_VertexRightBottom_Range(),	res.Color_VertexRightBottom_RandomChannelSync(),p->LifeRate(), random_seed, blend_value);	\
	}

	template<typename PrimitiveType, typename ResourceType>
	static BM3_FORCEINLINE void UpdatePixel(PrimitiveType *p, UpdateInfo &/*update_info*/,const ResourceType &res, const unsigned int &make_type, const unsigned int &random_seed){

		switch(make_type){
			case BM3_MAKE_CASE(TransformTypeConst_Constant,	BlendAlphaTypeConst_Constant,	ColorTypeConst_Constant):{
					COLOR_CONST();
				}
				break;
			case BM3_MAKE_CASE(TransformTypeConst_Constant,	BlendAlphaTypeConst_Constant,	ColorTypeConst_Curve):{
					policy::impl::Blend_Constant::Prepare(	res, p->alpha_);
					COLOR_CURVE();
				}
				break;
			case BM3_MAKE_CASE(TransformTypeConst_Constant,	BlendAlphaTypeConst_Curve,		ColorTypeConst_Constant):{
					policy::impl::Blend_Curve::Prepare(		res, p->alpha_,	p->LifeRate(), random_seed);
					COLOR_CONST();
				}
				break;
			case BM3_MAKE_CASE(TransformTypeConst_Constant,	BlendAlphaTypeConst_Curve,		ColorTypeConst_Curve):{
					policy::impl::Blend_Curve::Prepare(	res, p->alpha_,	p->LifeRate(), random_seed);
					COLOR_CURVE();
				}
				break;
			case BM3_MAKE_CASE(TransformTypeConst_Curve,	BlendAlphaTypeConst_Constant,	ColorTypeConst_Constant):{
					policy::impl::Blend_Constant::Prepare(	res, p->alpha_);
					COLOR_CONST();
					policy::impl::Transform_Curve::Prepare(	res, p->transform_.left_top_, p->transform_.right_top_, p->transform_.left_bottom_, p->transform_.right_bottom_, res.Deformation_Size_Curve(), p->LifeRate(), random_seed);
				}
				break;
			case BM3_MAKE_CASE(TransformTypeConst_Curve,	BlendAlphaTypeConst_Constant,	ColorTypeConst_Curve):{
					policy::impl::Blend_Constant::Prepare(	res, p->alpha_);
					COLOR_CURVE();
					policy::impl::Transform_Curve::Prepare(	res, p->transform_.left_top_, p->transform_.right_top_, p->transform_.left_bottom_, p->transform_.right_bottom_, res.Deformation_Size_Curve(), p->LifeRate(), random_seed);
				}
				break;
			case BM3_MAKE_CASE(TransformTypeConst_Curve,	BlendAlphaTypeConst_Curve,		ColorTypeConst_Constant):{
					policy::impl::Blend_Curve::Prepare(	res, p->alpha_,	p->LifeRate(), random_seed);
					COLOR_CONST();
					policy::impl::Transform_Curve::Prepare(	res, p->transform_.left_top_, p->transform_.right_top_, p->transform_.left_bottom_, p->transform_.right_bottom_, res.Deformation_Size_Curve(), p->LifeRate(), random_seed);
				}
				break;
			case BM3_MAKE_CASE(TransformTypeConst_Curve,	BlendAlphaTypeConst_Curve,		ColorTypeConst_Curve):{
					policy::impl::Blend_Curve::Prepare(	res, p->alpha_,	p->LifeRate(), random_seed);
					COLOR_CURVE();
					policy::impl::Transform_Curve::Prepare(	res, p->transform_.left_top_, p->transform_.right_top_, p->transform_.left_bottom_, p->transform_.right_bottom_, res.Deformation_Size_Curve(), p->LifeRate(), random_seed);
				}
				break;
			case BM3_MAKE_CASE(TransformTypeConst_Vertex,	BlendAlphaTypeConst_Constant,	ColorTypeConst_Constant):{
					policy::impl::Blend_Constant::Prepare(	res, p->alpha_);
					COLOR_CONST();
				}
				break;
			case BM3_MAKE_CASE(TransformTypeConst_Vertex,	BlendAlphaTypeConst_Constant,	ColorTypeConst_Curve):{
					policy::impl::Blend_Constant::Prepare(	res, p->alpha_);
					COLOR_CURVE();
				}
				break;
			case BM3_MAKE_CASE(TransformTypeConst_Vertex,	BlendAlphaTypeConst_Curve,		ColorTypeConst_Constant):{
					policy::impl::Blend_Curve::Prepare(	res, p->alpha_,	p->LifeRate(), random_seed);
					COLOR_CONST();
				}
				break;
			case BM3_MAKE_CASE(TransformTypeConst_Vertex,	BlendAlphaTypeConst_Curve,		ColorTypeConst_Curve):{
					policy::impl::Blend_Curve::Prepare(	res, p->alpha_,	p->LifeRate(), random_seed);
					COLOR_CURVE();
				}
				break;
			default: ML_ASSERT(false);	break;
		}
	}

#if 0
	template<typename PrimitiveType, typename ResourceType>
	static void Prepare(PrimitiveType *prim, int active_node_count, int *active_node_index_array, UpdateInfo & /*update_info*/, const ResourceType &res, const obj::RootNodeBase & /*root*/){

		ML_MAKEVERTEX_TIMER;
		ML_PTR_ASSERT(prim);
		ML_PTR_ASSERT(active_node_index_array);
		{
			#define PREFECH_DEF_PREPAREINFO		PrefetchData<sizeof(PrimitiveType)>(&prim[active_node_index_array[k+1]]);

			unsigned int make_type = BM3_MAKE_CASE(res.Deformation_Basic_TransformType(), res.Blend_Basic_BlendAlphaType(), res.Color_Basic_ColorType());

			switch(make_type){
				case BM3_MAKE_CASE(TransformTypeConst_Constant,	BlendAlphaTypeConst_Constant,	ColorTypeConst_Constant):
					for(int k=0 ;k < active_node_count; ++ k){
						int index	= active_node_index_array[k];
						PrimitiveType	*p		= &prim[index];
						
						PrefetchData<sizeof(PrimitiveType)>(&prim[active_node_index_array[k+1]]);

						COLOR_CONST();
					}
					break;
				case BM3_MAKE_CASE(TransformTypeConst_Constant,	BlendAlphaTypeConst_Constant,	ColorTypeConst_Curve):
					for(int k=0 ;k < active_node_count; ++ k){
						int index	= active_node_index_array[k];
						PrimitiveType	*p		= &prim[index];
						unsigned int random_seed = ml::ui32(p);
						
						PREFECH_DEF_PREPAREINFO

						policy::impl::Blend_Constant::Prepare(	res, p->alpha_);
						COLOR_CURVE();
					}
					break;
				case BM3_MAKE_CASE(TransformTypeConst_Constant,	BlendAlphaTypeConst_Curve,		ColorTypeConst_Constant):
					for(int k=0 ;k < active_node_count; ++ k){
						int index	= active_node_index_array[k];
						PrimitiveType	*p		= &prim[index];
						unsigned int random_seed = ml::ui32(p);
						
						PREFECH_DEF_PREPAREINFO

						policy::impl::Blend_Curve::Prepare(		res, p->alpha_,	p->LifeRate(), random_seed);
						COLOR_CONST();
					}
					break;
				case BM3_MAKE_CASE(TransformTypeConst_Constant,	BlendAlphaTypeConst_Curve,		ColorTypeConst_Curve):
					for(int k=0 ;k < active_node_count; ++ k){
						int index	= active_node_index_array[k];
						PrimitiveType	*p		= &prim[index];
						unsigned int random_seed = ml::ui32(p);
						
						PREFECH_DEF_PREPAREINFO

						policy::impl::Blend_Curve::Prepare(	res, p->alpha_,	p->LifeRate(), random_seed);
						COLOR_CURVE();
					}
					break;
				case BM3_MAKE_CASE(TransformTypeConst_Curve,	BlendAlphaTypeConst_Constant,	ColorTypeConst_Constant):
					for(int k=0 ;k < active_node_count; ++ k){
						int index	= active_node_index_array[k];
						PrimitiveType	*p		= &prim[index];
						unsigned int random_seed = ml::ui32(p);
						
						PREFECH_DEF_PREPAREINFO

						policy::impl::Blend_Constant::Prepare(	res, p->alpha_);
						COLOR_CONST();
						policy::impl::Transform_Curve::Prepare(	res, p->transform_.left_top_, p->transform_.right_top_, p->transform_.left_bottom_, p->transform_.right_bottom_, res.Deformation_Size_Curve(), p->LifeRate(), random_seed);
					}
					break;
				case BM3_MAKE_CASE(TransformTypeConst_Curve,	BlendAlphaTypeConst_Constant,	ColorTypeConst_Curve):
					for(int k=0 ;k < active_node_count; ++ k){
						int index	= active_node_index_array[k];
						PrimitiveType	*p		= &prim[index];
						unsigned int random_seed = ml::ui32(p);

						PREFECH_DEF_PREPAREINFO

						policy::impl::Blend_Constant::Prepare(	res, p->alpha_);
						COLOR_CURVE();
						policy::impl::Transform_Curve::Prepare(	res, p->transform_.left_top_, p->transform_.right_top_, p->transform_.left_bottom_, p->transform_.right_bottom_, res.Deformation_Size_Curve(), p->LifeRate(), random_seed);
					}
					break;
				case BM3_MAKE_CASE(TransformTypeConst_Curve,	BlendAlphaTypeConst_Curve,		ColorTypeConst_Constant):
					for(int k=0 ;k < active_node_count; ++ k){
						int index	= active_node_index_array[k];
						PrimitiveType	*p		= &prim[index];
						unsigned int random_seed = ml::ui32(p);

						PREFECH_DEF_PREPAREINFO

						policy::impl::Blend_Curve::Prepare(	res, p->alpha_,	p->LifeRate(), random_seed);
						COLOR_CONST();
						policy::impl::Transform_Curve::Prepare(	res, p->transform_.left_top_, p->transform_.right_top_, p->transform_.left_bottom_, p->transform_.right_bottom_, res.Deformation_Size_Curve(), p->LifeRate(), random_seed);
					}
					break;
				case BM3_MAKE_CASE(TransformTypeConst_Curve,	BlendAlphaTypeConst_Curve,		ColorTypeConst_Curve):
					for(int k=0 ;k < active_node_count; ++ k){
						int index	= active_node_index_array[k];
						PrimitiveType	*p		= &prim[index];
						unsigned int random_seed = ml::ui32(p);

						PREFECH_DEF_PREPAREINFO

						policy::impl::Blend_Curve::Prepare(	res, p->alpha_,	p->LifeRate(), random_seed);
						COLOR_CURVE();
						policy::impl::Transform_Curve::Prepare(	res, p->transform_.left_top_, p->transform_.right_top_, p->transform_.left_bottom_, p->transform_.right_bottom_, res.Deformation_Size_Curve(), p->LifeRate(), random_seed);
					}
					break;
				case BM3_MAKE_CASE(TransformTypeConst_Vertex,	BlendAlphaTypeConst_Constant,	ColorTypeConst_Constant):

					for(int k=0 ;k < active_node_count; ++ k){
						int index	= active_node_index_array[k];
						PrimitiveType	*p		= &prim[index];

						PrefetchData<sizeof(PrimitiveType)>(&prim[active_node_index_array[k+1]]);

						policy::impl::Blend_Constant::Prepare(	res, p->alpha_);
						COLOR_CONST();
					}
					break;
				case BM3_MAKE_CASE(TransformTypeConst_Vertex,	BlendAlphaTypeConst_Constant,	ColorTypeConst_Curve):
					for(int k=0 ;k < active_node_count; ++ k){
						int index	= active_node_index_array[k];
						PrimitiveType	*p		= &prim[index];
						unsigned int random_seed = ml::ui32(p);

						PREFECH_DEF_PREPAREINFO

						policy::impl::Blend_Constant::Prepare(	res, p->alpha_);
						COLOR_CURVE();
					}
					break;
				case BM3_MAKE_CASE(TransformTypeConst_Vertex,	BlendAlphaTypeConst_Curve,		ColorTypeConst_Constant):
					for(int k=0 ;k < active_node_count; ++ k){
						int index	= active_node_index_array[k];
						PrimitiveType	*p		= &prim[index];
						unsigned int random_seed = ml::ui32(p);

						PREFECH_DEF_PREPAREINFO

						policy::impl::Blend_Curve::Prepare(	res, p->alpha_,	p->LifeRate(), random_seed);
						COLOR_CONST();
					}
					break;
				case BM3_MAKE_CASE(TransformTypeConst_Vertex,	BlendAlphaTypeConst_Curve,		ColorTypeConst_Curve):
					for(int k=0 ;k < active_node_count; ++ k){
						int index	= active_node_index_array[k];
						PrimitiveType	*p		= &prim[index];
						unsigned int random_seed = ml::ui32(p);

						PREFECH_DEF_PREPAREINFO

						policy::impl::Blend_Curve::Prepare(	res, p->alpha_,	p->LifeRate(), random_seed);
						COLOR_CURVE();
					}
					break;
				default: ML_ASSERT(false);	break;
			}

			#undef PREFECH_DEF_PREPAREINFO
		}
	}
#endif
#undef COLOR_CONST
#undef COLOR_CURVE
#undef BM3_MAKE_CASE
	
	template<typename PrimitiveType, typename ResourceType>
	static void PrepareField(PrimitiveType *prim, int active_node_count, int *active_node_index_array, ml::ui8* field_buffer, int field_all_size, UpdateInfo &update_info, const ResourceType &res, const obj::RootNodeBase &root){
		
		CommonPrimitiveManipulator::PrepareField(prim, active_node_count, active_node_index_array, field_buffer, field_all_size, update_info, res, root);
	}


	template<typename ResourceType>
	static void SetFacingSetting(PacketDrawInfo &pdi, DrawInfo &draw_info, const ResourceType &res){
		if(res.Detail_Basic_QuadType()==QuadTypeConst_Facing){
			switch(res.Detail_Basic_FacingType()){
				case FacingTypeConst_Root:		break;
				case FacingTypeConst_Camera:
#ifdef BM3_TARGET_IDE
					pdi.facing_position_ = draw_info.ViewMatrix().translate();
					pdi.facing_position_.mul(draw_info.InverseViewMatrix());
					pdi.facing_position_.mul(-1.0f);	// これを入れないと向きが合わない
#else
					pdi.facing_position_ = draw_info.InverseViewMatrix().translate();
#endif
					break;
				case FacingTypeConst_User:
					pdi.facing_position_ = draw_info.FacingPosition();
					break;
				default:	ML_ASSERT(false);	break;
			}
		}
	}
	
	template<typename PrimitiveType, typename ResourceType, typename DrawType>
	static void MakeVertex(DrawType &dst, PrimitiveType *prim, int active_node_count, int *active_node_index_array, DrawInfo &draw_info, const PacketDrawInfo &packet_draw_info, const ResourceType &res, void* vertex_buffer, bool is_interchanged_bm2){

		ML_MAKEVERTEX_TIMER;
		ML_PTR_ASSERT(prim);
		ML_PTR_ASSERT(active_node_index_array);

		if(is_interchanged_bm2==true)
		{
			switch(res.Detail_Basic_QuadType()){
				case QuadTypeConst_Billboard:{
					dst.template PushRangeBm2<DrawScale::XY, DrawRotate::Billboard_Z, DrawColor::_4>(		prim, active_node_count, active_node_index_array, draw_info, packet_draw_info, res, vertex_buffer);
					break;
				}
				case QuadTypeConst_Polygon:{
					dst.template PushRangeBm2<DrawScale::XY, DrawRotate::XYZ, DrawColor::_4>(				prim, active_node_count, active_node_index_array, draw_info, packet_draw_info, res, vertex_buffer);
					break;
				}
				case QuadTypeConst_Facing:{
					dst.template PushRangeBm2<DrawScale::XY, DrawRotate::Facing, DrawColor::_4>(			prim, active_node_count, active_node_index_array, draw_info, packet_draw_info, res, vertex_buffer);
					break;
				}
				case QuadTypeConst_Polyline:{
					dst.template PushRangeBm2<DrawScale::XY, DrawRotate::Polyline, DrawColor::_4>(			prim, active_node_count, active_node_index_array, draw_info, packet_draw_info, res, vertex_buffer);
					break;
				}
				case QuadTypeConst_Directional:{
					dst.template PushRangeBm2<DrawScale::XY, DrawRotate::Directional, DrawColor::_4>(		prim, active_node_count, active_node_index_array, draw_info, packet_draw_info, res, vertex_buffer);
					break;
				}
				case QuadTypeConst_DirectionalPolyline:{
					dst.template PushRangeBm2<DrawScale::XY, DrawRotate::DirectionalPolyline, DrawColor::_4>(prim, active_node_count, active_node_index_array, draw_info, packet_draw_info, res, vertex_buffer);
					break;
				}
				default:{
					ML_ASSERT(false);
					break;
				}
			}
		}
		else
		{
			switch(res.Detail_Basic_QuadType()){
				case QuadTypeConst_Billboard:{
					dst.template PushRange<DrawScale::XY, DrawRotate::Billboard_Z, DrawColor::_4>(			prim, active_node_count, active_node_index_array, draw_info, packet_draw_info, res, vertex_buffer);
					break;
				}
				case QuadTypeConst_Polygon:{
					dst.template PushRange<DrawScale::XY, DrawRotate::XYZ, DrawColor::_4>(					prim, active_node_count, active_node_index_array, draw_info, packet_draw_info, res, vertex_buffer);
					break;
				}
				case QuadTypeConst_Facing:{
					dst.template PushRange<DrawScale::XY, DrawRotate::Facing, DrawColor::_4>(				prim, active_node_count, active_node_index_array, draw_info, packet_draw_info, res, vertex_buffer);
					break;
				}
				case QuadTypeConst_Polyline:{
					dst.template PushRange<DrawScale::XY, DrawRotate::Polyline, DrawColor::_4>(				prim, active_node_count, active_node_index_array, draw_info, packet_draw_info, res, vertex_buffer);
					break;
				}
				case QuadTypeConst_Directional:{
					dst.template PushRange<DrawScale::XY, DrawRotate::Directional, DrawColor::_4>(			prim, active_node_count, active_node_index_array, draw_info, packet_draw_info, res, vertex_buffer);
					break;
				}
				case QuadTypeConst_DirectionalPolyline:{
					dst.template PushRange<DrawScale::XY, DrawRotate::DirectionalPolyline, DrawColor::_4>(	prim, active_node_count, active_node_index_array, draw_info, packet_draw_info, res, vertex_buffer);
					break;
				}
				default:{
					ML_ASSERT(false);
					break;
				}
			}
		}
#ifndef BM3_PLATFORM_xbox360
		// 最終位置の更新
		{
			bool	is_update_position	= false;

			if(	(res.Detail_Basic_QuadType() == QuadTypeConst_Directional) ||
				(res.Detail_Basic_QuadType() == QuadTypeConst_DirectionalPolyline)){
				is_update_position	= true;
			}
			else{
				const typename ResourceType::ChildArrayType_	&child_array	= res.ChildArray();
				int												child_count		= ml::minimum(res.ChildArray().Count(), BM3_MAX_CHILD_NODE_COUNT);
				for(int index = 0;index != child_count;++ index){
					if(child_array.Child(index).Basic_Meta_NodeType() == NodeTypeConst_FieldCollision){
						is_update_position	= true;
					}
				}
			}

			if(is_update_position){

				for(int i=0 ; i<active_node_count ; ++i){
					int			index	= active_node_index_array[i];
					Primitive	&p		= prim[index];
					p.SetPreviousPosition(p.GetParentMatrix());
				}
			}
		}
#endif
	}
	
	template<typename PrimitiveType, typename ResourceType>
	static void SwitchResource(PrimitiveType *prim_begin, int active_node_count, int *active_node_index_array, const ResourceType &res, const ResourceType &old_res, ml::random &random){

		ML_PTR_ASSERT(prim_begin);
		ML_PTR_ASSERT(active_node_index_array);

		int tex_ref_count = TextureReferenceCount(res.Texture_Basic_TextureDrawType());

		const TextureAnimationTypeConst tat[TextureLayer::Max] = {
			res.Texture_Layer1_TextureAnimationType(),
			res.Texture_Layer2_TextureAnimationType()
		};
		const TextureAnimationTypeConst old_tat[TextureLayer::Max] = {
			old_res.Texture_Layer1_TextureAnimationType(),
			old_res.Texture_Layer2_TextureAnimationType()
		};

		for(int i=0 ; i<active_node_count ; ++i){
			int			index	= active_node_index_array[i];
			Primitive	&p		= prim_begin[index];

//			PrefetchData<sizeof(PrimitiveType)>(*(read + 1));

			{
				PrimitiveInitializeInfo	pii;
				{
					pii.random_seed_	= ml::ui32(&p);
				}

				// ライフ
				p.SwitchResource(res, random);

				// Matrix関係
				{
					policy::impl::MatrixScale_SwitchResource<>::Switch(			res, old_res, random, p.scale_.value_, p.scale_.speed_, p.scale_.acceleration_ ,pii);
					policy::impl::MatrixRotate_SwitchResource::Switch(			res, old_res, random, p.rotate_.value_, p.rotate_.speed_, p.rotate_.acceleration_ ,pii);
					policy::impl::MatrixTranslate_SwitchResource::Switch(		res, old_res, random, p.translate_.value_, p.translate_.speed_, p.translate_.acceleration_ ,pii, p.generate_shape_.position_ ,p.generate_shape_.direction_);
				}
				// Color関係
				{
					float	blend_value	= 0.0f;
					policy::impl::Blend_SwitchResource::Switch(	res, old_res, random, blend_value, pii);
					policy::impl::Color_SwitchResource::Switch(	res, old_res, random, p.color_left_top_.value_,	res.Color_VertexLeftTop_Value(),	res.Color_VertexLeftTop_Range(), res.Color_VertexLeftTop_RandomChannelSync(),	blend_value, pii);

					if(res.Color_Basic_Flat()){
						p.color_right_top_.value_		= p.color_left_top_.value_;
						p.color_left_bottom_.value_		= p.color_left_top_.value_;
						p.color_right_bottom_.value_	= p.color_left_top_.value_;
					}else{
						policy::impl::Color_SwitchResource::Switch(	res, old_res, random, p.color_right_top_.value_,	res.Color_VertexRightTop_Value(),		res.Color_VertexRightTop_Range(), res.Color_VertexRightTop_RandomChannelSync(),			blend_value, pii);
						policy::impl::Color_SwitchResource::Switch(	res, old_res, random, p.color_left_bottom_.value_,	res.Color_VertexLeftBottom_Value(),		res.Color_VertexLeftBottom_Range(), res.Color_VertexLeftBottom_RandomChannelSync(),		blend_value, pii);
						policy::impl::Color_SwitchResource::Switch(	res, old_res, random, p.color_right_bottom_.value_,	res.Color_VertexRightBottom_Value(),	res.Color_VertexRightBottom_Range(), res.Color_VertexRightBottom_RandomChannelSync(),	blend_value, pii);
					}
				}
				// Texture関係
				{
					for(int j = 0;j != tex_ref_count;++ j){
						policy::impl::Texture_SwitchResource::Switch(	res, old_res, random, p.texture_[j].value_,	p.texture_[j].frame_, p.texture_[j].step_, p.texture_[j].h_scroll_,	p.texture_[j].v_scroll_, tat[j], old_tat[j], TextureLayer::Type(j));
					}
				}

				// Transform関係
				{
					policy::impl::Transform_SwitchResource::Switch(	res, old_res, random, p.transform_.left_top_, p.transform_.right_top_, p.transform_.left_bottom_, p.transform_.right_bottom_);
				}
			}

		}
	}
	
	template<typename PrimitiveType, typename ResourceType>
	static void SwitchResourceField(PrimitiveType *prim_begin, int active_node_count, int *active_node_index_array, ml::ui8* field_buffer, int field_all_size, const ResourceType &res, const ResourceType &old_res, ml::random &random){

		CommonPrimitiveManipulator::SwitchResourceField(prim_begin, active_node_count, active_node_index_array, field_buffer, field_all_size, res, old_res, random);
	}


	template<typename PrimitiveType, typename ResourceType>
	static void SetDriveMatrix(prim::Packet *packet_array, PrimitiveType *prim_begin, int active_node_count, int *active_node_index_array, const ResourceType &res, ParentParam *parent_param){

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

		for(int i=0 ; i<active_node_count ; ++i){
			int				index	= active_node_index_array[i];
			PrimitiveType	&p		= prim_begin[index];
			prim::Packet	*packet	= &packet_array[index];
			prim::ParentParam	&parent	= parent_param[index];

//			PrefetchData<sizeof(PrimitiveType)>(*(read + 1));
//			PrefetchData<16>(prim_begin[active_node_index_array[i+1]]);					// ここでは先頭から１６バイト分のみで足りる

//			PrefetchData<sizeof(*packet)>(packet + 1);

			ml::matrix44	&m = packet->generate_time_hierarchy_parent_matrix_;
			ml::vector3d	&v = packet->generate_time_hierarchy_parent_matrix_row_length_;
			const ml::matrix44 &parent_matrix	= parent.Base().HierarchyMatrix();
			const ml::vector3d &parent_scale	= parent.Base().HierarchyMatrixRowLength();
			

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
					v.set(1,1,1,1);
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
/*
			{
				PrimitiveType *next_p = (read == (active_end - 1)) ? NULL : *(read + 1);
				if(next_p){
					PrefetchData<sizeof(ml::matrix44)>(next_p->generate_time_parent_matrix_);
					PrefetchData<sizeof(ml::vector3d)>(next_p->generate_time_parent_scale_);
				}
			}
*/
			switch(t){			// 平行移動
				case TranslateParentNodeInfluenceTypeConst_On:{					m.set_translate(parent_matrix.translate());						break;	}
				case TranslateParentNodeInfluenceTypeConst_OnGenerateTime:{		m.set_translate(p.GetGenerateTimeParentMatrix().translate());	break;	}
				default:{														ML_ASSERT(false);														}
			}

			p.SetParentMatrix(m);
			p.SetParentScale(v);
		}
	}
	
};

}	// namespace quad
}	// namespace prim
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_PRIMITIVE_BM3_PRIM_QUAD_H
