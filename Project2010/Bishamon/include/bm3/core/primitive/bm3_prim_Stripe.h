#ifndef BM3_SDK_INC_BM3_CORE_PRIMITIVE_BM3_PRIM_STRIPE_H
#define BM3_SDK_INC_BM3_CORE_PRIMITIVE_BM3_PRIM_STRIPE_H

#include "bm3_Primitive.h"
#include "../bm3_UpdateInfo.h"
#include "../field/bm3_field_Gravity.h"
#include "../field/bm3_field_Wind.h"
#include "../field/bm3_field_Vortex.h"
#include "../field/bm3_field_Radialpower.h"
#include "../field/bm3_field_Deceleration.h"
#include "../field/bm3_field_Collision.h"
#include "../resource/bm3_res_Gravity.h"
#include "../resource/bm3_res_Wind.h"
#include "../resource/bm3_res_Vortex.h"
#include "../resource/bm3_res_Radialpower.h"
#include "../resource/bm3_res_Deceleration.h"
#include "../resource/bm3_res_Collision.h"
#include "../policy/Field/bm3_policy_Bounding_Sphere.h"
#include <ml/utility/ml_simpletimer.h>

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

class DrawInfo;

namespace prim{
namespace stripe{

#if defined(BM3_TARGET_IDE)
	#define	BM3_FIELD_RESOURCE_TYPE(NODE_TYPE)	res::IdeNode
#endif
#if defined(BM3_TARGET_EMBEDDED)
	#define	BM3_FIELD_RESOURCE_TYPE(NODE_TYPE)	res::NODE_TYPE
#endif

/*!
	@brief	ストライプエミッタ一つ一つのパラメータの処理
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
	static void Initialize(PrimitiveType *prim_begin, PrimitiveType *prim_end, const ResourceType &res, ml::random random, unsigned int random_seed){

		ML_INITIALIZE_TIMER;
		ML_PTR_ASSERT(prim_begin);
		ML_PTR_ASSERT(prim_end);

		{
			int tex_ref_count = TextureReferenceCount(res.Texture_Basic_TextureDrawType());

			unsigned int pixel_type = res.Blend_Basic_BlendAlphaType()	<< 0 | res.Color_Basic_ColorType()	<< 2 | res.Color_Basic_Flat() << 4;

			#define INIT_PIXLE_CASE(BLEND, COLOR, IS_FLAT)	(BLEND << 0 | COLOR << 2 | IS_FLAT << 4)

			for(PrimitiveType *p = prim_begin ;p != prim_end ;++ p){
				PrimitiveInitializeInfo	pii;
				pii.random_seed_	= ml::ui32(p);

				p->Initialize(res, random);

				// Matrix関係
				{
					switch(res.Scale_Basic_ScaleType()){
						case ScaleTypeConst_Constant:		policy::impl::MatrixScale_Constant<false>::Initialize(			res, random, p->scale_.value_															);		break;
						case ScaleTypeConst_NewtonDynamics:	policy::impl::MatrixScale_Newtondynamics<false>::Initialize(	res, random, p->scale_.value_,		p->scale_.speed_,		p->scale_.acceleration_		);		break;
						case ScaleTypeConst_CurveValue:		policy::impl::MatrixScale_CurveValue<false>::Initialize(		res, random, p->scale_.value_															, pii);	break;
						case ScaleTypeConst_CurveSpeed:		policy::impl::MatrixScale_CurveSpeed<false>::Initialize(		res, random, p->scale_.value_,		p->scale_.speed_									, pii);	break;
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
					ml::random* random_handle;
					ml::random	sync_random;
					if(res.Color_Basic_RandomSync())
					{
						ml::random::seed_info	rsi(random_seed);
						sync_random.set_seed(rsi);
						random_handle	= &sync_random;
					}
					else{
						random_handle	= &random;
					}
					switch(pixel_type){
						case INIT_PIXLE_CASE(BlendAlphaTypeConst_Constant,	ColorTypeConst_Constant, 0):
							policy::impl::Blend_Constant::Initialize(	res, random,	p->alpha_);
							policy::impl::Color_Constant::Initialize(	res, *random_handle, p->color_left_top_.value_,	res.Color_VertexLeftTop_Value(),	res.Color_VertexLeftTop_Range(), res.Color_VertexLeftTop_RandomChannelSync(),	p->alpha_);
							policy::impl::Color_Constant::Initialize(	res, *random_handle, p->color_right_top_.value_,res.Color_VertexRightTop_Value(),	res.Color_VertexRightTop_Range(), res.Color_VertexRightTop_RandomChannelSync(),	p->alpha_);
							break;
						case INIT_PIXLE_CASE(BlendAlphaTypeConst_Constant,	ColorTypeConst_Curve, 0):
							policy::impl::Blend_Constant::Initialize(	res, random,	p->alpha_);
							policy::impl::Color_Curve::Initialize(		res, *random_handle, p->color_left_top_.value_,	res.Color_VertexLeftTop_Value(),	res.Color_VertexLeftTop_Range(),												p->alpha_);
							policy::impl::Color_Curve::Initialize(		res, *random_handle, p->color_right_top_.value_,res.Color_VertexRightTop_Value(),	res.Color_VertexRightTop_Range(),												p->alpha_);
							break;
						case INIT_PIXLE_CASE(BlendAlphaTypeConst_Curve,		ColorTypeConst_Constant, 0):
							policy::impl::Blend_Curve::Initialize(		res, random,	p->alpha_);
							policy::impl::Color_Constant::Initialize(	res, *random_handle, p->color_left_top_.value_,	res.Color_VertexLeftTop_Value(),	res.Color_VertexLeftTop_Range(), res.Color_VertexLeftTop_RandomChannelSync(),	p->alpha_);
							policy::impl::Color_Constant::Initialize(	res, *random_handle, p->color_right_top_.value_,res.Color_VertexRightTop_Value(),	res.Color_VertexRightTop_Range(), res.Color_VertexRightTop_RandomChannelSync(),	p->alpha_);
							break;
						case INIT_PIXLE_CASE(BlendAlphaTypeConst_Curve,		ColorTypeConst_Curve, 0):
							policy::impl::Blend_Curve::Initialize(		res, random,	p->alpha_);
							policy::impl::Color_Curve::Initialize(		res, *random_handle, p->color_left_top_.value_,	res.Color_VertexLeftTop_Value(),	res.Color_VertexLeftTop_Range(),												p->alpha_);
							policy::impl::Color_Curve::Initialize(		res, *random_handle, p->color_right_top_.value_,res.Color_VertexRightTop_Value(),	res.Color_VertexRightTop_Range(),												p->alpha_);
							break;
						case INIT_PIXLE_CASE(BlendAlphaTypeConst_Constant,	ColorTypeConst_Constant, 1):
							policy::impl::Blend_Constant::Initialize(	res, random,	p->alpha_);
							policy::impl::Color_Constant::Initialize(	res, *random_handle, p->color_left_top_.value_,	res.Color_VertexLeftTop_Value(),	res.Color_VertexLeftTop_Range(), res.Color_VertexLeftTop_RandomChannelSync(),	p->alpha_);
							p->color_right_top_.value_		= p->color_left_top_.value_;
							break;
						case INIT_PIXLE_CASE(BlendAlphaTypeConst_Constant,	ColorTypeConst_Curve, 1):
							policy::impl::Blend_Constant::Initialize(	res, random,	p->alpha_);
							policy::impl::Color_Curve::Initialize(		res, *random_handle, p->color_left_top_.value_,	res.Color_VertexLeftTop_Value(),	res.Color_VertexLeftTop_Range(),												p->alpha_);
							p->color_right_top_.value_		= p->color_left_top_.value_;
							break;
						case INIT_PIXLE_CASE(BlendAlphaTypeConst_Curve,		ColorTypeConst_Constant, 1):
							policy::impl::Blend_Curve::Initialize(		res, random,	p->alpha_);
							policy::impl::Color_Constant::Initialize(	res, *random_handle, p->color_left_top_.value_,	res.Color_VertexLeftTop_Value(),	res.Color_VertexLeftTop_Range(), res.Color_VertexLeftTop_RandomChannelSync(),	p->alpha_);
							p->color_right_top_.value_		= p->color_left_top_.value_;
							break;
						case INIT_PIXLE_CASE(BlendAlphaTypeConst_Curve,		ColorTypeConst_Curve, 1):
							policy::impl::Blend_Curve::Initialize(		res, random,	p->alpha_);
							policy::impl::Color_Curve::Initialize(		res, *random_handle, p->color_left_top_.value_,	res.Color_VertexLeftTop_Value(),	res.Color_VertexLeftTop_Range(),												p->alpha_);
							p->color_right_top_.value_		= p->color_left_top_.value_;
							break;
						default: ML_ASSERT(false);	break;
					}
/*
					policy::impl::Blend_Constant::Initialize(	res, random,	p->alpha_);
					policy::impl::Blend_Curve::Initialize(		res, random,	p->alpha_);

					policy::impl::Color_Constant::Initialize(	res, random, p->color_left_top_.value_,	res.Color_VertexLeftTop_Value(),	res.Color_VertexLeftTop_Range(), res.Color_VertexLeftTop_RandomChannelSync(),	p->alpha_);
					policy::impl::Color_Curve::Initialize(		res, random, p->color_left_top_.value_,	res.Color_VertexLeftTop_Value(),	res.Color_VertexLeftTop_Range(),												p->alpha_);

					if(res.Color_Basic_Flat()){
						p->color_right_top_.value_		= p->color_left_top_.value_;
					}else{
						policy::impl::Color_Constant::Initialize(	res, random, p->color_right_top_.value_,	res.Color_VertexRightTop_Value(),	res.Color_VertexRightTop_Range(), res.Color_VertexRightTop_RandomChannelSync(),	p->alpha_);
						policy::impl::Color_Curve::Initialize(		res, random, p->color_right_top_.value_,	res.Color_VertexRightTop_Value(),	res.Color_VertexRightTop_Range(),												p->alpha_);
					}
*/				}

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

						TextureAnimationTypeConst tat = Texture_Layer_TextureAnimationType(res, TextureLayer::Type(j));
						switch(tat){
							case TextureAnimationTypeConst_Nothing:		policy::impl::Texture_Nothing::Initialize(	res, random, p->texture_[j].value_,															TextureLayer::Type(j));	break;
							case TextureAnimationTypeConst_Crop:		policy::impl::Texture_Crop::Initialize(		res, random, p->texture_[j].value_,	p->texture_[j].frame_,		p->texture_[j].step_,		TextureLayer::Type(j));	break;
							case TextureAnimationTypeConst_Scroll:		policy::impl::Texture_Scroll::Initialize(	res, random, p->texture_[j].value_,	p->texture_[j].h_scroll_,	p->texture_[j].v_scroll_,	TextureLayer::Type(j));	break;
							case TextureAnimationTypeConst_Pattern:		policy::impl::Texture_Pattern::Initialize(	res, random, p->texture_[j].value_,	p->texture_[j].frame_,		p->texture_[j].step_,		TextureLayer::Type(j));	break;
							case TextureAnimationTypeConst_Transform:	policy::impl::Texture_Transform::Initialize(res, random, p->texture_[j].value_,															TextureLayer::Type(j));	break;
							default:	ML_ASSERT(false);	break;		
						}
					}
				}

				// フィールド
				p->field_info_.Reset();
			}
			#undef INIT_PIXLE_CASE
		}
	}

	template<typename PrimitiveType, typename ResourceType>
	static void InitializeField(PrimitiveType *prim_begin, PrimitiveType *prim_end, ml::ui8* field_begin, const ResourceType &res, ml::random &random){

		CommonPrimitiveManipulator::InitializeField(prim_begin, prim_end, field_begin, res, random);
	}

	
#define PIXEL_CASE(BLEND, COLOR)	(BLEND << 0 | COLOR << 2)
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
			prim_end
		);
		update_info.OnPrimitiveUpdateBegin(uipi);
#endif

		int tex_ref_count = TextureReferenceCount(res.Texture_Basic_TextureDrawType());

		const TextureAnimationTypeConst tat[TextureLayer::Max] = {
			res.Texture_Layer1_TextureAnimationType(),
			res.Texture_Layer2_TextureAnimationType()
		};
		unsigned int pxl_type = PIXEL_CASE(res.Blend_Basic_BlendAlphaType(), res.Color_Basic_ColorType());

		int active_count			= 0;
		int	*temp_active_node_index	= *active_node_index;
		for(PrimitiveType *prim = prim_begin ; prim != prim_end ; ++prim, ++start_prim_index){

			PrimitiveType &p = *prim;

			PrefetchData<sizeof(PrimitiveType)>(prim + 1);
//			PrefetchData<sizeof(read)>(read + 2);

			bool	is_first_frame	= p.IsFirstFrame();
			p.life_.Update(update_info);

			if(p.IsActive()){
				*temp_active_node_index = start_prim_index;

				ml::ui32	random_seed	= ml::ui32(&p);
				if(is_first_frame == false){
					
					Update(p, update_info, res, tat, tex_ref_count, random_seed, random);
				}
				UpdatePixel(p, update_info, res, pxl_type, random_seed);
				temp_active_node_index++;
				active_count++;
			}
		}

#if defined(BM3_TARGET_EMBEDDED)
		update_info.OnPrimitiveUpdateEnd(uipi);
#endif

		return active_count;
	}

	template<typename PrimitiveType, typename ResourceType, typename UpdateInfo>
	static int Update(UpdateInfo &update_info, const ResourceType &res, PrimitiveType *prim_begin, int *active_node_index, int active_node_count, const PrimitiveUpdateInfo & /*pui*/, int begin_count){

		ML_PTR_ASSERT(prim_begin);

		int tex_ref_count = TextureReferenceCount(res.Texture_Basic_TextureDrawType());

		const TextureAnimationTypeConst tat[TextureLayer::Max] = {
			res.Texture_Layer1_TextureAnimationType(),
			res.Texture_Layer2_TextureAnimationType()
		};
		unsigned int pxl_type = PIXEL_CASE(res.Blend_Basic_BlendAlphaType(), res.Color_Basic_ColorType());

		for(int i=0 ; i<active_node_count ; ++i){
			int index = active_node_index[i] - begin_count;
			if(active_node_count<index)	return i;

//			PrefetchData<sizeof(PrimitiveType)>(prim + 1);

			ml::ui32	random_seed	= ml::ui32(&prim_begin[index]);
			Update(prim_begin[index], update_info, res, tat, tex_ref_count, random_seed);
			UpdatePixel(prim_begin[index], update_info, res, pxl_type, random_seed);
		}

		return active_node_count;
	}

	template<typename PrimitiveType, typename ResourceType, typename UpdateInfo>
	static BM3_FORCEINLINE void Update(PrimitiveType &p, UpdateInfo &update_info, const ResourceType &res, const TextureAnimationTypeConst *tat, int tex_ref_count, const unsigned int &random_seed, ml::random &random){

//					#define	BM3_PM_AllowFor_Scale_Basic_Square	false
#ifdef BM3_PLATFORM_psp
		#include "bm3_prim_update_matrix_psp.inc"
#else
		#include "bm3_prim_update_matrix.inc"
#endif
//					#undef	BM3_PM_AllowFor_Scale_Basic_Square

		// Color関係
		{
			policy::impl::Color_Constant::Update(	update_info, res, p.color_left_top_.value_);
			policy::impl::Color_Curve::Update(		update_info, res, p.color_left_top_.value_);
			policy::impl::Color_Constant::Update(	update_info, res, p.color_right_top_.value_);
			policy::impl::Color_Curve::Update(		update_info, res, p.color_right_top_.value_);
		}

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
	static void ZSort(PrimitiveType * /*prim_begin*/, int /*active_node_count*/, int * /*active_node_index_array*/, DrawInfo & /*draw_info*/, const ResourceType & /*res*/, WholeZInfo & /*z_info*/, PrimZList & /*z_list*/){
	}

	template<typename PrimitiveType, typename ResourceType>
	static BM3_FORCEINLINE void UpdatePixel(PrimitiveType &p, UpdateInfo & /*update_info*/,const ResourceType &res, const unsigned int &pxl_type, const unsigned int &random_seed){
		switch(pxl_type){
			case PIXEL_CASE(BlendAlphaTypeConst_Constant,	ColorTypeConst_Constant):
				policy::impl::Color_Constant::Prepare(	res, p.color_left_top_.value_,		p.alpha_);
				policy::impl::Color_Constant::Prepare(	res, p.color_right_top_.value_,		p.alpha_);
				break;
			case PIXEL_CASE(BlendAlphaTypeConst_Constant,	ColorTypeConst_Curve):
				policy::impl::Color_Curve::Prepare(	res, p.color_left_top_.value_,		res.Color_VertexLeftTop_Curve(),	res.Color_VertexLeftTop_Range(),	res.Color_VertexLeftTop_RandomChannelSync(),	p.LifeRate(), random_seed,	p.alpha_);
				if(res.Color_Basic_Flat())	p.color_right_top_.value_		= p.color_left_top_.value_;
				else						policy::impl::Color_Curve::Prepare(	res, p.color_right_top_.value_,	res.Color_VertexRightTop_Curve(),	res.Color_VertexRightTop_Range(),	res.Color_VertexRightTop_RandomChannelSync(),	p.LifeRate(), random_seed,	p.alpha_);
				break;
			case PIXEL_CASE(BlendAlphaTypeConst_Curve,		ColorTypeConst_Constant):
				policy::impl::Blend_Curve::Prepare(		res, p.alpha_,	p.LifeRate(), random_seed);
				policy::impl::Color_Constant::Prepare(	res, p.color_left_top_.value_,		p.alpha_);
				policy::impl::Color_Constant::Prepare(	res, p.color_right_top_.value_,		p.alpha_);
				break;
			case PIXEL_CASE(BlendAlphaTypeConst_Curve,		ColorTypeConst_Curve):
				policy::impl::Blend_Curve::Prepare(		res, p.alpha_,	p.LifeRate(), random_seed);
				policy::impl::Color_Curve::Prepare(	res, p.color_left_top_.value_,		res.Color_VertexLeftTop_Curve(),	res.Color_VertexLeftTop_Range(),	res.Color_VertexLeftTop_RandomChannelSync(),	p.LifeRate(), random_seed,	p.alpha_);
				if(res.Color_Basic_Flat())	p.color_right_top_.value_		= p.color_left_top_.value_;
				else						policy::impl::Color_Curve::Prepare(	res, p.color_right_top_.value_,	res.Color_VertexRightTop_Curve(),	res.Color_VertexRightTop_Range(),	res.Color_VertexRightTop_RandomChannelSync(),	p.LifeRate(), random_seed,	p.alpha_);
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
		
		unsigned int pxl_type = PIXEL_CASE(res.Blend_Basic_BlendAlphaType(), res.Color_Basic_ColorType());

		switch(pxl_type){
			case PIXEL_CASE(BlendAlphaTypeConst_Constant,	ColorTypeConst_Constant):
				for(int k=0 ;k < active_node_count; ++ k){
					int index	= active_node_index_array[k];
					PrimitiveType	*p		= &prim[index];	
					policy::impl::Color_Constant::Prepare(	res, p->color_left_top_.value_,																		p->alpha_);
					policy::impl::Color_Constant::Prepare(	res, p->color_right_top_.value_,																	p->alpha_);
				}
				break;
			case PIXEL_CASE(BlendAlphaTypeConst_Constant,	ColorTypeConst_Curve):
				for(int k=0 ;k < active_node_count; ++ k){
					int index	= active_node_index_array[k];
					PrimitiveType	*p		= &prim[index];	
					unsigned int random_seed = ml::ui32(p);

					policy::impl::Color_Curve::Prepare(	res, p->color_left_top_.value_,		res.Color_VertexLeftTop_Curve(),	res.Color_VertexLeftTop_Range(),	res.Color_VertexLeftTop_RandomChannelSync(),	p->LifeRate(), random_seed,	p->alpha_);
					if(res.Color_Basic_Flat())	p->color_right_top_.value_		= p->color_left_top_.value_;
					else						policy::impl::Color_Curve::Prepare(	res, p->color_right_top_.value_,	res.Color_VertexRightTop_Curve(),	res.Color_VertexRightTop_Range(),	res.Color_VertexRightTop_RandomChannelSync(),	p->LifeRate(), random_seed,	p->alpha_);
				}
				break;
			case PIXEL_CASE(BlendAlphaTypeConst_Curve,		ColorTypeConst_Constant):
				for(int k=0 ;k < active_node_count; ++ k){
					int index	= active_node_index_array[k];
					PrimitiveType	*p		= &prim[index];	
					unsigned int random_seed = ml::ui32(p);

					policy::impl::Blend_Curve::Prepare(		res, p->alpha_,	p->LifeRate(), random_seed);
					policy::impl::Color_Constant::Prepare(	res, p->color_left_top_.value_,																		p->alpha_);
					policy::impl::Color_Constant::Prepare(	res, p->color_right_top_.value_,																	p->alpha_);
				}
				break;
			case PIXEL_CASE(BlendAlphaTypeConst_Curve,		ColorTypeConst_Curve):
				for(int k=0 ;k < active_node_count; ++ k){
					int index	= active_node_index_array[k];
					PrimitiveType	*p		= &prim[index];	
					unsigned int random_seed = ml::ui32(p);

					policy::impl::Blend_Curve::Prepare(		res, p->alpha_,	p->LifeRate(), random_seed);
					policy::impl::Color_Curve::Prepare(	res, p->color_left_top_.value_,		res.Color_VertexLeftTop_Curve(),	res.Color_VertexLeftTop_Range(),	res.Color_VertexLeftTop_RandomChannelSync(),	p->LifeRate(), random_seed,	p->alpha_);
					if(res.Color_Basic_Flat())	p->color_right_top_.value_		= p->color_left_top_.value_;
					else						policy::impl::Color_Curve::Prepare(	res, p->color_right_top_.value_,	res.Color_VertexRightTop_Curve(),	res.Color_VertexRightTop_Range(),	res.Color_VertexRightTop_RandomChannelSync(),	p->LifeRate(), random_seed,	p->alpha_);
				}
				break;
			default: ML_ASSERT(false);	break;
		}
	}
#endif
#undef PIXEL_CASE

	template<typename PrimitiveType, typename ResourceType>
	static void PrepareField(PrimitiveType *prim, int active_node_count, int *active_node_index_array, ml::ui8* field_buffer, int field_all_size, UpdateInfo &update_info, const ResourceType &res, const obj::RootNodeBase &root){
		
		CommonPrimitiveManipulator::PrepareField(prim, active_node_count, active_node_index_array, field_buffer, field_all_size, update_info, res, root);
	}


	template<typename ResourceType>
	static void SetFacingSetting(PacketDrawInfo &pdi, DrawInfo &draw_info, const ResourceType &res){
		if(res.Detail_Basic_StripeType()==StripeTypeConst_Facing){
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
	static void MakeVertex(DrawType &dst, const PrimitiveType *prim, int active_node_count, int *active_node_index_array, DrawInfo &draw_info, const PacketDrawInfo &packet_draw_info, const ResourceType &res, void* vertex_buffer, bool is_interchanged_bm2){

		ML_MAKEVERTEX_TIMER;
		ML_PTR_ASSERT(prim);
		ML_PTR_ASSERT(active_node_index_array);

		if(is_interchanged_bm2==true)
		{
			switch(res.Detail_Basic_StripeType()){
				case StripeTypeConst_Billboard:{
					dst.template PushRangeBm2<DrawScale::X, DrawRotate::Billboard_Z, DrawColor::_2>(	prim, active_node_count, active_node_index_array, draw_info, packet_draw_info, res, vertex_buffer);
					break;
				}
				case StripeTypeConst_Polygon:{
					dst.template PushRangeBm2<DrawScale::X, DrawRotate::XYZ, DrawColor::_2>(			prim, active_node_count, active_node_index_array, draw_info, packet_draw_info, res, vertex_buffer);
					break;
				}
				case StripeTypeConst_Facing:{
					dst.template PushRangeBm2<DrawScale::X, DrawRotate::Facing, DrawColor::_2>(		prim, active_node_count, active_node_index_array, draw_info, packet_draw_info, res, vertex_buffer);
					break;
				}
				default:{
					ML_ASSERT(false);
					BM3_ASSUME(0);
					break;
				}
			}
		}
		else
		{
			switch(res.Detail_Basic_StripeType()){
				case StripeTypeConst_Billboard:{
					dst.template PushRange<DrawScale::X, DrawRotate::Billboard_Z, DrawColor::_2>(	prim, active_node_count, active_node_index_array, draw_info, packet_draw_info, res, vertex_buffer);
					break;
				}
				case StripeTypeConst_Polygon:{
					dst.template PushRange<DrawScale::X, DrawRotate::XYZ, DrawColor::_2>(			prim, active_node_count, active_node_index_array, draw_info, packet_draw_info, res, vertex_buffer);
					break;
				}
				case StripeTypeConst_Facing:{
					dst.template PushRange<DrawScale::X, DrawRotate::Facing, DrawColor::_2>(		prim, active_node_count, active_node_index_array, draw_info, packet_draw_info, res, vertex_buffer);
					break;
				}
				default:{
					ML_ASSERT(false);
					BM3_ASSUME(0);
					break;
				}
			}
		}

		// 最終位置の更新
		{
			bool	is_update_position	= false;

			{
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
					PrimitiveType	&p		= *const_cast<Primitive*>(&prim[index]);

					p.SetPreviousPosition(p.GetParentMatrix());
				}
			}
		}
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
					policy::impl::MatrixScale_SwitchResource<false>::Switch(	res, old_res, random, p.scale_.value_, p.scale_.speed_, p.scale_.acceleration_ ,pii);
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
					}else{
						policy::impl::Color_SwitchResource::Switch(		res, old_res, random, p.color_right_top_.value_,	res.Color_VertexRightTop_Value(),		res.Color_VertexRightTop_Range(), res.Color_VertexRightTop_RandomChannelSync(),			blend_value, pii);
					}
				}

				// Texture関係
				for(int j = 0;j != tex_ref_count;++ j){
					policy::impl::Texture_SwitchResource::Switch(	res, old_res, random, p.texture_[j].value_,	p.texture_[j].frame_, p.texture_[j].step_, p.texture_[j].h_scroll_,	p.texture_[j].v_scroll_, tat[j], old_tat[j], TextureLayer::Type(j));
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

}	// namespace stripe
}	// namespace prim
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_PRIMITIVE_BM3_PRIM_STRIPE_H
