#ifndef BM3_SDK_INC_BM3_CORE_OBJECT_CONTROL_BM3_OBJ_CONTROLBASE_INL_H
#define BM3_SDK_INC_BM3_CORE_OBJECT_CONTROL_BM3_OBJ_CONTROLBASE_INL_H

#include "bm3_obj_ControlNull.h"
#include "../../bm3_MemoryManager.h"
#include "../../bm3_Const.h"
#include "../emitter/bm3_obj_EmitterParticle.h"
#include "../emitter/bm3_obj_EmitterQuad.h"
#include "../emitter/bm3_obj_EmitterSimpleParticle.h"
#include "../emitter/bm3_obj_EmitterMassParticle.h"
#include "../emitter/bm3_obj_EmitterModel.h"
#include "../emitter/bm3_obj_EmitterStripe.h"
#include "../../field/bm3_field_Common.h"
#include "../../field/bm3_field_Gravity.h"
#include "../../field/bm3_field_Wind.h"
#include "../../field/bm3_field_Vortex.h"
#include "../../field/bm3_field_Radialpower.h"
#include "../../field/bm3_field_Deceleration.h"
#include "../../field/bm3_field_Collision.h"
#include "../../field/bm3_field_Goal.h"
#include "../object/bm3_obj_ObjectReference.h"
#include "../../bm3_Debug.h"
#include "../../bm3_CoreType.h"
#include "../../field/bm3_field.h"
#include <ml/type/ml_static_align.h>
#include <ml/type/ml_basic.h>
#include <ml/utility/ml_assert.h>
#include <ml/utility/ml_simpletimer.h>
#include <ml/math/ml_math_const.h>
#include "../../resource/bm3_res_Quad.h"
#include "../../resource/bm3_res_Gravity.h"
#include "../../resource/bm3_res_Wind.h"
#include "../../resource/bm3_res_Vortex.h"
#include "../../resource/bm3_res_Radialpower.h"
#include "../../resource/bm3_res_Deceleration.h"
#include "../../resource/bm3_res_Collision.h"
#include "../../resource/bm3_res_Goal.h"
#include "../../resource/bm3_res_Reference.h"
#if defined(BM3_TARGET_IDE)
#include "../../resource/bm3_res_IdeNode.h"
#endif
#ifdef BM3_PLATFORM_playstation3
#include "bm3/core/!playstation3/spu/bm3_SpuHeader.h"
#include "bm3/core/!playstation3/spu/bm3_RawSpuManager.h"
#endif
#if defined(BM3_TARGET_IDE)
	#define	BM3_CHILD_RESOURCE_TYPE(NODE_TYPE)	res::IdeNode
#endif
#if defined(BM3_TARGET_EMBEDDED)
	#define	BM3_CHILD_RESOURCE_TYPE(NODE_TYPE)	res::NODE_TYPE
#endif

#define	BM3_CONTROLBASE_TEMPLATE_TEMPLATE_ARG	typename ResourceType, typename BaseClass, typename LifePolicy, typename ChildGenerateTimingPolicy
#define	BM3_CONTROLBASE_TEMPLATE_ARG			ResourceType, BaseClass, LifePolicy, ChildGenerateTimingPolicy

#define	BM3_CONTROLBASE_NODE_TYPE(NAMESPACE, NODE_TYPE)	NAMESPACE::NODE_TYPE<BM3_CHILD_RESOURCE_TYPE(NODE_TYPE)>

#define	BM3_SIZEOF_NODE(NAMESPACE, NODE_TYPE)	\
	ml::static_align<sizeof(ml::ptr_size_ui), sizeof(BM3_CONTROLBASE_NODE_TYPE(NAMESPACE, NODE_TYPE))>::value

#define	BM3_NODE_CALL_METHOD(NAMESPACE, NODE_TYPE, METHOD)	\
	reinterpret_cast<BM3_CONTROLBASE_NODE_TYPE(NAMESPACE, NODE_TYPE) *>(buffer)->METHOD

#define	BM3_NODE_CALL_CONST_METHOD(NAMESPACE, NODE_TYPE, METHOD)	\
	reinterpret_cast<const BM3_CONTROLBASE_NODE_TYPE(NAMESPACE, NODE_TYPE) *>(buffer)->METHOD


namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE
namespace obj{
namespace control{


template<BM3_CONTROLBASE_TEMPLATE_TEMPLATE_ARG>
Base<BM3_CONTROLBASE_TEMPLATE_ARG>::Base() :
	res_(NULL),
	root_(NULL),
	is_root_(false),
	child_all_byte_size_(0),
	field_all_byte_size_(0),
	is_expired_(false),

	max_gen_count_(0),
	current_gen_pointer_(0),
	active_gen_count_(0),
	generator_array_(NULL),
	node_param_array_(NULL),

	max_node_count_(0),
	active_node_count_(0),
	active_count_per_frame_(0),
	current_node_pointer_(0),

	node_base_array_(NULL),
	control_param_array_(NULL),
	active_node_index_(NULL),

	child_buffer_(NULL),
	field_buffer_(NULL),

	generate_child_remain_(0.0f)
	{
	;
}

template<BM3_CONTROLBASE_TEMPLATE_TEMPLATE_ARG>
Base<BM3_CONTROLBASE_TEMPLATE_ARG>::~Base(){

	Finalize();
}
template<BM3_CONTROLBASE_TEMPLATE_TEMPLATE_ARG>
bool	Base<BM3_CONTROLBASE_TEMPLATE_ARG>::Allocate(const ResourceType &res){
	MakeChildByteSize(res);
	return AllocateChild(res);
}

template<BM3_CONTROLBASE_TEMPLATE_TEMPLATE_ARG>
bool	Base<BM3_CONTROLBASE_TEMPLATE_ARG>::AllocateChild(const ResourceType &res){
	// 子供のポインタを持っておきたい
	child_buffer_			= reinterpret_cast<ml::ui8*>(					bm3::MemoryManager::instance().General_Alloc(child_all_byte_size_));

	if(child_buffer_){

		const typename ResourceType::ChildArrayType_	&child_array	= res.ChildArray();
		int												child_count		= ml::minimum(res.ChildArray().Count(), BM3_MAX_CHILD_NODE_COUNT);

		#define BM3_ALLOCATE_CHILD(NAMESPACE, NODE_TYPE)																								\
			new(buffer)	BM3_CONTROLBASE_NODE_TYPE(NAMESPACE, NODE_TYPE);																				\
			BM3_NODE_CALL_METHOD(NAMESPACE, NODE_TYPE, Allocate(reinterpret_cast<const BM3_CHILD_RESOURCE_TYPE(NODE_TYPE) &>(child_array.Child(i))));

		#define BM3_ALLOCATE_EMITTER_CHILD(NAMESPACE, NODE_TYPE)			\
			new(buffer)	BM3_CONTROLBASE_NODE_TYPE(NAMESPACE, NODE_TYPE);	

		for(int i = 0;i != child_count;++ i){
			ml::ui8*	buffer	= ml::offset_pointer<ml::ui8*>(child_buffer_, (int)child_byte_size_offset_array_[i]);
			switch(child_array.Child(i).Basic_Meta_NodeType()){
				case NodeTypeConst_ControlNull:				BM3_ALLOCATE_CHILD(control, Null);						break;
				case NodeTypeConst_EmitterQuad:				BM3_ALLOCATE_EMITTER_CHILD(emitter, Quad);				break;
				case NodeTypeConst_EmitterParticle:			BM3_ALLOCATE_EMITTER_CHILD(emitter, Particle);			break;
				case NodeTypeConst_EmitterSimpleParticle:	BM3_ALLOCATE_EMITTER_CHILD(emitter, SimpleParticle);	break;
				case NodeTypeConst_EmitterMassParticle:		BM3_ALLOCATE_EMITTER_CHILD(emitter, MassParticle);		break;
				case NodeTypeConst_EmitterModel:			BM3_ALLOCATE_EMITTER_CHILD(emitter, Model);				break;
				case NodeTypeConst_EmitterStripe:			BM3_ALLOCATE_EMITTER_CHILD(emitter, Stripe);			break;
				case NodeTypeConst_FieldGravity:			break;
				case NodeTypeConst_FieldWind:				break;
				case NodeTypeConst_FieldVortex:				break;
				case NodeTypeConst_FieldRadialpower:		break;
				case NodeTypeConst_FieldDeceleration:		break;
				case NodeTypeConst_FieldCollision:			break;
				case NodeTypeConst_FieldGoal:				break;
				case NodeTypeConst_ObjectReference:			BM3_ALLOCATE_CHILD(object, Reference);					break;
				default:							ML_ASSERT(false);	BM3_ASSUME(0);					break;
			}
		}
		#undef BM3_ALLOCATE_CHILD
		#undef BM3_ALLOCATE_EMITTER_CHILD
	}
	else{
		return false;
	}

	return true;
}

template<BM3_CONTROLBASE_TEMPLATE_TEMPLATE_ARG>
int	Base<BM3_CONTROLBASE_TEMPLATE_ARG>::FixGenCount(const ResourceType &res, InitialAllocInfo &info, const float gen_scale){
	res_		= &res;

	max_gen_count_	= info.ParentMaxGenCount();
	max_node_count_	= (int)(info.ParentMaxGenCount() * ChildGenerateTimingPolicy::CalcTotalGenerateNodeCount(res) * gen_scale);

	int child_life	= FixChildGenCount(res, max_node_count_, gen_scale);
	if(IsRoot()==false){

		int total_life	= ChildGenerateTimingPolicy::CalcTotalLifeSpan(res);
		switch(Resource().Basic_Basic_ExpirationType()){
		case ExpirationTypeConst_Life:{
		}
		break;
		case ExpirationTypeConst_AllChildExpired:{
			// 子供の寿命の分だけ生き続ける
			if(child_life==-1){
			}
			else{
				max_node_count_	= (int)(info.ParentMaxGenCount() * ChildGenerateTimingPolicy::CalcTotalGenerateNodeCount(res, child_life));
			}
		}
		break;
		default:
			ML_ASSERT(false);
			break;
		}

		return total_life;
	}
	else{	return 0;	}
}

template<BM3_CONTROLBASE_TEMPLATE_TEMPLATE_ARG>
int	Base<BM3_CONTROLBASE_TEMPLATE_ARG>::FixChildGenCount(const ResourceType &res, int node_count, const float gen_scale){

	int	total_lifespan	= 0;
	if(child_buffer_){

		const typename ResourceType::ChildArrayType_	&child_array	= res.ChildArray();
		int												child_count		= ml::minimum(res.ChildArray().Count(), BM3_MAX_CHILD_NODE_COUNT);
		InitialAllocInfo	info(node_count);

		#define BM3_FIX_GEN_COUNT(NAMESPACE, NODE_TYPE)																											\
		total_lifespan	= ml::maximum(total_lifespan, BM3_NODE_CALL_METHOD(NAMESPACE, NODE_TYPE, FixGenCount(reinterpret_cast<const BM3_CHILD_RESOURCE_TYPE(NODE_TYPE) &>(child_array.Child(i)), info, gen_scale)));

		for(int i = 0;i != child_count;++ i){
			ml::ui8*	buffer	= ml::offset_pointer<ml::ui8*>(child_buffer_, (int)child_byte_size_offset_array_[i]);
			switch(child_array.Child(i).Basic_Meta_NodeType()){
				case NodeTypeConst_ControlNull:				BM3_FIX_GEN_COUNT(control, Null);			break;
				case NodeTypeConst_EmitterQuad:				BM3_FIX_GEN_COUNT(emitter, Quad);			break;
				case NodeTypeConst_EmitterParticle:			BM3_FIX_GEN_COUNT(emitter, Particle);		break;
				case NodeTypeConst_EmitterSimpleParticle:	BM3_FIX_GEN_COUNT(emitter, SimpleParticle);	break;
				case NodeTypeConst_EmitterMassParticle:		BM3_FIX_GEN_COUNT(emitter, MassParticle);	break;
				case NodeTypeConst_EmitterModel:			BM3_FIX_GEN_COUNT(emitter, Model);			break;
				case NodeTypeConst_EmitterStripe:			BM3_FIX_GEN_COUNT(emitter, Stripe);			break;
				case NodeTypeConst_FieldGravity:			break;
				case NodeTypeConst_FieldWind:				break;
				case NodeTypeConst_FieldVortex:				break;
				case NodeTypeConst_FieldRadialpower:		break;
				case NodeTypeConst_FieldDeceleration:		break;
				case NodeTypeConst_FieldCollision:			break;
				case NodeTypeConst_FieldGoal:				break;
				case NodeTypeConst_ObjectReference:			BM3_FIX_GEN_COUNT(object, Reference);		break;
				default:							ML_ASSERT(false);	BM3_ASSUME(0);					break;
			}
		}
		#undef BM3_FIX_GEN_COUNT
	}

	return total_lifespan;
}

/*!
	生成するエフェクトノードのメモリ取得
	@return	メモリ取得に成功すればtrueを返す
*/
template<BM3_CONTROLBASE_TEMPLATE_TEMPLATE_ARG>
bool	Base<BM3_CONTROLBASE_TEMPLATE_ARG>::AllocateParam(){
#if 0
	AllocGenerator(max_gen_count_);
	AllocNode(max_node_count_);
	AllocateChildParam();
#else
	if(AllocGenerator(max_gen_count_) && AllocNode(max_node_count_) && AllocateChildParam()){
		return true;
	}
	else{
		return false;
	}
#endif
}

template<BM3_CONTROLBASE_TEMPLATE_TEMPLATE_ARG>
bool	Base<BM3_CONTROLBASE_TEMPLATE_ARG>::AllocateChildParam(){

	if(child_buffer_){
		bool result = true;
		const typename ResourceType::ChildArrayType_	&child_array	= Resource().ChildArray();
		int												child_count		= ml::minimum(Resource().ChildArray().Count(), BM3_MAX_CHILD_NODE_COUNT);

		#define BM3_ALLOC_PARAM(NAMESPACE, NODE_TYPE)		\
			result = BM3_NODE_CALL_METHOD(NAMESPACE, NODE_TYPE, AllocateParam());

		for(int i = 0;i != child_count;++ i){
			ml::ui8*	buffer	= ml::offset_pointer<ml::ui8*>(child_buffer_, (int)child_byte_size_offset_array_[i]);
			switch(child_array.Child(i).Basic_Meta_NodeType()){
				case NodeTypeConst_ControlNull:				BM3_ALLOC_PARAM(control, Null);				break;
				case NodeTypeConst_EmitterQuad:				BM3_ALLOC_PARAM(emitter, Quad);				break;
				case NodeTypeConst_EmitterParticle:			BM3_ALLOC_PARAM(emitter, Particle);			break;
				case NodeTypeConst_EmitterSimpleParticle:	BM3_ALLOC_PARAM(emitter, SimpleParticle);	break;
				case NodeTypeConst_EmitterMassParticle:		BM3_ALLOC_PARAM(emitter, MassParticle);		break;
				case NodeTypeConst_EmitterModel:			BM3_ALLOC_PARAM(emitter, Model);			break;
				case NodeTypeConst_EmitterStripe:			BM3_ALLOC_PARAM(emitter, Stripe);			break;
				case NodeTypeConst_FieldGravity:			break;
				case NodeTypeConst_FieldWind:				break;
				case NodeTypeConst_FieldVortex:				break;
				case NodeTypeConst_FieldRadialpower:		break;
				case NodeTypeConst_FieldDeceleration:		break;
				case NodeTypeConst_FieldCollision:			break;
				case NodeTypeConst_FieldGoal:				break;
				case NodeTypeConst_ObjectReference:			BM3_ALLOC_PARAM(object, Reference);			break;
				default:							ML_ASSERT(false);	BM3_ASSUME(0);					break;
			}
			if(!result)return false;
		}
		#undef BM3_ALLOC_PARAM
	}
	return true;
}

template<BM3_CONTROLBASE_TEMPLATE_TEMPLATE_ARG>
void	Base<BM3_CONTROLBASE_TEMPLATE_ARG>::MakeChildByteSize(const ResourceType &res){

	child_all_byte_size_	= 0;
	field_all_byte_size_	= 0;

	{
		const typename ResourceType::ChildArrayType_	&child_array	= res.ChildArray();
		int												child_count		= ml::minimum(res.ChildArray().Count(), BM3_MAX_CHILD_NODE_COUNT);

		for(int i = 0;i != child_count;++ i){
			switch(child_array.Child(i).Basic_Meta_NodeType()){
				case NodeTypeConst_ControlNull:				child_byte_size_offset_array_[i] = (ChildByteSizeArrayElemType)child_all_byte_size_;	child_all_byte_size_ += BM3_SIZEOF_NODE(control, Null);				break;
				case NodeTypeConst_EmitterQuad:				child_byte_size_offset_array_[i] = (ChildByteSizeArrayElemType)child_all_byte_size_;	child_all_byte_size_ += BM3_SIZEOF_NODE(emitter, Quad);				break;
				case NodeTypeConst_EmitterParticle:			child_byte_size_offset_array_[i] = (ChildByteSizeArrayElemType)child_all_byte_size_;	child_all_byte_size_ += BM3_SIZEOF_NODE(emitter, Particle);			break;
				case NodeTypeConst_EmitterSimpleParticle:	child_byte_size_offset_array_[i] = (ChildByteSizeArrayElemType)child_all_byte_size_;	child_all_byte_size_ += BM3_SIZEOF_NODE(emitter, SimpleParticle);	break;
				case NodeTypeConst_EmitterMassParticle:		child_byte_size_offset_array_[i] = (ChildByteSizeArrayElemType)child_all_byte_size_;	child_all_byte_size_ += BM3_SIZEOF_NODE(emitter, MassParticle);		break;
				case NodeTypeConst_EmitterModel:			child_byte_size_offset_array_[i] = (ChildByteSizeArrayElemType)child_all_byte_size_;	child_all_byte_size_ += BM3_SIZEOF_NODE(emitter, Model);			break;
				case NodeTypeConst_EmitterStripe:			child_byte_size_offset_array_[i] = (ChildByteSizeArrayElemType)child_all_byte_size_;	child_all_byte_size_ += BM3_SIZEOF_NODE(emitter, Stripe);			break;
				case NodeTypeConst_FieldGravity:																									field_all_byte_size_ += sizeof(field::gravity::Field);				break;
				case NodeTypeConst_FieldWind:																										field_all_byte_size_ += sizeof(field::wind::Field);					break;
				case NodeTypeConst_FieldVortex:																										field_all_byte_size_ += sizeof(field::vortex::Field);				break;
				case NodeTypeConst_FieldRadialpower:																								field_all_byte_size_ += sizeof(field::radialpower::Field);			break;
				case NodeTypeConst_FieldDeceleration:																								field_all_byte_size_ += sizeof(field::deceleration::Field);			break;
				case NodeTypeConst_FieldCollision:																									field_all_byte_size_ += sizeof(field::collision::Field);			break;
				case NodeTypeConst_FieldGoal:																										field_all_byte_size_ += sizeof(field::goal::Field);					break;
				case NodeTypeConst_ObjectReference:			child_byte_size_offset_array_[i] = (ChildByteSizeArrayElemType)child_all_byte_size_;	child_all_byte_size_ += BM3_SIZEOF_NODE(object,	 Reference);		break;
				default:							ML_ASSERT(false);	BM3_ASSUME(0);																																	break;
			}
		}
	}
}

/*!
	ノード生成とノードデータに必要なメモリを確保する
	@return	メモリ取得に成功すればtrueを返す
*/
template<BM3_CONTROLBASE_TEMPLATE_TEMPLATE_ARG>
bool	Base<BM3_CONTROLBASE_TEMPLATE_ARG>::AllocGenerator(int node_count){
	generator_array_		= reinterpret_cast<ChildGenerateTimingPolicy*>(	bm3::MemoryManager::instance().General_Alloc(node_count * sizeof(ChildGenerateTimingPolicy)));
	node_param_array_		= reinterpret_cast<NodeParam*>(					bm3::MemoryManager::instance().General_Alloc(node_count * sizeof(NodeParam)));

	if(generator_array_){
		memset(generator_array_, 0x00, node_count * sizeof(ChildGenerateTimingPolicy));
	}

	return (generator_array_ && node_param_array_)?true:false;
}

template<BM3_CONTROLBASE_TEMPLATE_TEMPLATE_ARG>
void	Base<BM3_CONTROLBASE_TEMPLATE_ARG>::FreeGenerator(){
	bm3::MemoryManager::instance().General_Free(node_param_array_);
	bm3::MemoryManager::instance().General_Free(generator_array_);

	node_param_array_		= NULL;
	generator_array_		= NULL;
}

template<BM3_CONTROLBASE_TEMPLATE_TEMPLATE_ARG>
bool	Base<BM3_CONTROLBASE_TEMPLATE_ARG>::AllocNode(int node_count){

	node_base_array_		= reinterpret_cast<BaseClass*>(					bm3::MemoryManager::instance().General_Alloc(node_count * sizeof(BaseClass)));
	control_param_array_	= reinterpret_cast<ControlParam<LifePolicy>*>(	bm3::MemoryManager::instance().General_Alloc(node_count * sizeof(ControlParam<LifePolicy>)));
	active_node_index_		= reinterpret_cast<int*>(						bm3::MemoryManager::instance().General_Alloc(node_count * sizeof(int)));
	field_buffer_			= reinterpret_cast<ml::ui8*>(					bm3::MemoryManager::instance().General_Alloc(node_count * field_all_byte_size_));

	if(IsRoot()){
		new(node_base_array_) BaseClass();
	}

	if(field_all_byte_size_){
		return (node_base_array_ && control_param_array_ && active_node_index_ && field_buffer_)?true:false;
	}
	else{
		return (node_base_array_ && control_param_array_ && active_node_index_)?true:false;
	}
}

template<BM3_CONTROLBASE_TEMPLATE_TEMPLATE_ARG>
void	Base<BM3_CONTROLBASE_TEMPLATE_ARG>::FreeNode(){

	bm3::MemoryManager::instance().General_Free(node_base_array_);
	bm3::MemoryManager::instance().General_Free(control_param_array_);
	bm3::MemoryManager::instance().General_Free(active_node_index_);
	bm3::MemoryManager::instance().General_Free(field_buffer_);

	node_base_array_		= NULL;
	control_param_array_	= NULL;
	active_node_index_		= NULL;
	field_buffer_			= NULL;
}

template<BM3_CONTROLBASE_TEMPLATE_TEMPLATE_ARG>
void	Base<BM3_CONTROLBASE_TEMPLATE_ARG>::FreeChild(){

	if(child_buffer_){

		const typename ResourceType::ChildArrayType_	&child_array	= Resource().ChildArray();
		int												child_count		= ml::minimum(Resource().ChildArray().Count(), BM3_MAX_CHILD_NODE_COUNT);

		for(int i = 0;i != child_count;++ i){
			ml::ui8*	buffer	= ml::offset_pointer<ml::ui8*>(child_buffer_, (int)child_byte_size_offset_array_[i]);
			switch(child_array.Child(i).Basic_Meta_NodeType()){
				case NodeTypeConst_ControlNull:				BM3_NODE_CALL_METHOD(control, Null,				~Null());			break;
				case NodeTypeConst_EmitterQuad:				BM3_NODE_CALL_METHOD(emitter, Quad,				~Quad());			break;
				case NodeTypeConst_EmitterParticle:			BM3_NODE_CALL_METHOD(emitter, Particle,			~Particle());		break;
				case NodeTypeConst_EmitterSimpleParticle:	BM3_NODE_CALL_METHOD(emitter, SimpleParticle,	~SimpleParticle());	break;
				case NodeTypeConst_EmitterMassParticle:		BM3_NODE_CALL_METHOD(emitter, MassParticle,		~MassParticle());	break;
				case NodeTypeConst_EmitterModel:			BM3_NODE_CALL_METHOD(emitter, Model,			~Model());			break;
				case NodeTypeConst_EmitterStripe:			BM3_NODE_CALL_METHOD(emitter, Stripe,			~Stripe());			break;
				case NodeTypeConst_FieldGravity:			break;
				case NodeTypeConst_FieldWind:				break;
				case NodeTypeConst_FieldVortex:				break;
				case NodeTypeConst_FieldRadialpower:		break;
				case NodeTypeConst_FieldDeceleration:		break;
				case NodeTypeConst_FieldCollision:			break;
				case NodeTypeConst_FieldGoal:				break;
				case NodeTypeConst_ObjectReference:			BM3_NODE_CALL_METHOD(object, Reference,			~Reference());		break;
				default:							ML_ASSERT(false);	BM3_ASSUME(0);											break;
			}
		}
	}

	bm3::MemoryManager::instance().General_Free(child_buffer_);
	child_buffer_			= NULL;
}

template<BM3_CONTROLBASE_TEMPLATE_TEMPLATE_ARG>
int		Base<BM3_CONTROLBASE_TEMPLATE_ARG>::CalcEmitterCount() const{

	int count = 0;
	if(child_buffer_){

		const typename ResourceType::ChildArrayType_	&child_array	= Resource().ChildArray();
		int												child_count		= ml::minimum(Resource().ChildArray().Count(), BM3_MAX_CHILD_NODE_COUNT);

		#define BM3_CALC_EMITTERCOUNT(NAMESPACE, NODE_TYPE)											\
			count += BM3_NODE_CALL_METHOD(						\
				NAMESPACE, NODE_TYPE,							\
				CalcEmitterCount()								\
			);

		for(int i = 0;i != child_count;++ i){
			ml::ui8*	buffer	= ml::offset_pointer<ml::ui8*>(child_buffer_, (int)child_byte_size_offset_array_[i]);
			switch(child_array.Child(i).Basic_Meta_NodeType()){
				case NodeTypeConst_ControlNull:				BM3_CALC_EMITTERCOUNT(control, Null);				break;
				case NodeTypeConst_EmitterQuad:				BM3_CALC_EMITTERCOUNT(emitter, Quad);				break;
				case NodeTypeConst_EmitterParticle:			BM3_CALC_EMITTERCOUNT(emitter, Particle);			break;
				case NodeTypeConst_EmitterSimpleParticle:	BM3_CALC_EMITTERCOUNT(emitter, SimpleParticle);		break;
				case NodeTypeConst_EmitterMassParticle:		BM3_CALC_EMITTERCOUNT(emitter, MassParticle);		break;
				case NodeTypeConst_EmitterModel:			BM3_CALC_EMITTERCOUNT(emitter, Model);				break;
				case NodeTypeConst_EmitterStripe:			BM3_CALC_EMITTERCOUNT(emitter, Stripe);				break;
				case NodeTypeConst_FieldGravity:			break;
				case NodeTypeConst_FieldWind:				break;
				case NodeTypeConst_FieldVortex:				break;
				case NodeTypeConst_FieldRadialpower:		break;
				case NodeTypeConst_FieldDeceleration:		break;
				case NodeTypeConst_FieldCollision:			break;
				case NodeTypeConst_FieldGoal:				break;
				case NodeTypeConst_ObjectReference:			BM3_CALC_EMITTERCOUNT(object, Reference);			break;
				default:							ML_ASSERT(false);	BM3_ASSUME(0);																																	break;
			}
		}
		#undef BM3_CALC_EMITTERCOUNT
	}
	return count;
}

template<BM3_CONTROLBASE_TEMPLATE_TEMPLATE_ARG>
void	Base<BM3_CONTROLBASE_TEMPLATE_ARG>::SetResEmitterInfo(InitializeInfo &initialize_info) {

	if(child_buffer_){

		const typename ResourceType::ChildArrayType_	&child_array	= Resource().ChildArray();
		int												child_count		= ml::minimum(Resource().ChildArray().Count(), BM3_MAX_CHILD_NODE_COUNT);

		#define BM3_SET_EMITTER_INFO(NAMESPACE, NODE_TYPE)											\
			BM3_NODE_CALL_METHOD(						\
				NAMESPACE, NODE_TYPE,							\
				SetResEmitterInfo(initialize_info)								\
			);

		for(int i = 0;i != child_count;++ i){
			ml::ui8*	buffer	= ml::offset_pointer<ml::ui8*>(child_buffer_, (int)child_byte_size_offset_array_[i]);
			switch(child_array.Child(i).Basic_Meta_NodeType()){
				case NodeTypeConst_ControlNull:				BM3_SET_EMITTER_INFO(control, Null);				break;
				case NodeTypeConst_EmitterQuad:				BM3_SET_EMITTER_INFO(emitter, Quad);				break;
				case NodeTypeConst_EmitterParticle:			BM3_SET_EMITTER_INFO(emitter, Particle);			break;
				case NodeTypeConst_EmitterSimpleParticle:	BM3_SET_EMITTER_INFO(emitter, SimpleParticle);		break;
				case NodeTypeConst_EmitterMassParticle:		BM3_SET_EMITTER_INFO(emitter, MassParticle);		break;
				case NodeTypeConst_EmitterModel:			BM3_SET_EMITTER_INFO(emitter, Model);				break;
				case NodeTypeConst_EmitterStripe:			BM3_SET_EMITTER_INFO(emitter, Stripe);				break;
				case NodeTypeConst_FieldGravity:			break;
				case NodeTypeConst_FieldWind:				break;
				case NodeTypeConst_FieldVortex:				break;
				case NodeTypeConst_FieldRadialpower:		break;
				case NodeTypeConst_FieldDeceleration:		break;
				case NodeTypeConst_FieldCollision:			break;
				case NodeTypeConst_FieldGoal:				break;
				case NodeTypeConst_ObjectReference:			BM3_SET_EMITTER_INFO(object, Reference);			break;
				default:							ML_ASSERT(false);	BM3_ASSUME(0);																																	break;
			}
		}
		#undef BM3_SET_EMITTER_INFO
	}
}

template<BM3_CONTROLBASE_TEMPLATE_TEMPLATE_ARG>
void	Base<BM3_CONTROLBASE_TEMPLATE_ARG>::InitializeRandom(){

	switch(Resource().Generate_Basic_RandomSeedType()){
		case RandomSeedTypeConst_RootNode:{				ml::random::seed_info	rsi(RootNodeBase::RandomSeed());				random_.set_seed(rsi);	}	break;
		case RandomSeedTypeConst_RootNodeSeedAssign:{	ml::random::seed_info	rsi(Resource().Generate_Basic_RandomSeed());	random_.set_seed(rsi);	}	break;
		default:										ML_ASSERT(false);									break;
	}
}

template<BM3_CONTROLBASE_TEMPLATE_TEMPLATE_ARG>
void	Base<BM3_CONTROLBASE_TEMPLATE_ARG>::Initialize(const ResourceType & /*res*/, InitializeInfo &initialize_info){

	root_		= initialize_info.Root();
	active_gen_count_		= 0;
	current_gen_pointer_	= 0;
	active_node_count_		= 0;
	current_node_pointer_	= 0;
	generate_child_remain_	= 0.0f;

	if(child_buffer_){

		const typename ResourceType::ChildArrayType_	&child_array	= Resource().ChildArray();
		int												child_count		= ml::minimum(Resource().ChildArray().Count(), BM3_MAX_CHILD_NODE_COUNT);

		InitializeInfo	info(&Root(), initialize_info.GenScale());
		
		#define BM3_ACTIVATE_CHILD(NAMESPACE, NODE_TYPE)												\
			BM3_NODE_CALL_METHOD(																		\
				NAMESPACE, NODE_TYPE,																	\
				Initialize(																				\
					reinterpret_cast<const BM3_CHILD_RESOURCE_TYPE(NODE_TYPE) &>(child_array.Child(i)),	\
					info)																				\
				);

		for(int i = 0;i != child_count;++ i){
			ml::ui8*	buffer	= ml::offset_pointer<ml::ui8*>(child_buffer_, (int)child_byte_size_offset_array_[i]);
			switch(child_array.Child(i).Basic_Meta_NodeType()){
				case NodeTypeConst_ControlNull:				BM3_ACTIVATE_CHILD(control, Null);				break;
				case NodeTypeConst_EmitterQuad:				BM3_ACTIVATE_CHILD(emitter, Quad);				break;
				case NodeTypeConst_EmitterParticle:			BM3_ACTIVATE_CHILD(emitter, Particle);			break;
				case NodeTypeConst_EmitterSimpleParticle:	BM3_ACTIVATE_CHILD(emitter, SimpleParticle);	break;
				case NodeTypeConst_EmitterMassParticle:		BM3_ACTIVATE_CHILD(emitter, MassParticle);		break;
				case NodeTypeConst_EmitterModel:			BM3_ACTIVATE_CHILD(emitter, Model);				break;
				case NodeTypeConst_EmitterStripe:			BM3_ACTIVATE_CHILD(emitter, Stripe);			break;
				case NodeTypeConst_FieldGravity:			break;
				case NodeTypeConst_FieldWind:				break;
				case NodeTypeConst_FieldVortex:				break;
				case NodeTypeConst_FieldRadialpower:		break;
				case NodeTypeConst_FieldDeceleration:		break;
				case NodeTypeConst_FieldCollision:			break;
				case NodeTypeConst_FieldGoal:				break;
				case NodeTypeConst_ObjectReference:			BM3_ACTIVATE_CHILD(object, Reference);			break;
				default:							ML_ASSERT(false);	BM3_ASSUME(0);																																	break;
			}
		}
		#undef BM3_ACTIVATE_CHILD
	}

	InitializeRandom();

#if defined(BM3_TARGET_IDE)
	if(field_buffer_){
		debug_life_.Initialize(*res_);
	}
#endif
}

template<BM3_CONTROLBASE_TEMPLATE_TEMPLATE_ARG>
void	Base<BM3_CONTROLBASE_TEMPLATE_ARG>::Finalize(){

	if(IsInitialized()){

		// 注意：デストラクタは呼ばれないことがあるので。
		//       メモリーを確保している物はこの中で明示的に解放する必要がある
		FreeChild();
		FreeNode();
		FreeGenerator();

		res_	= NULL;
		root_	= NULL;
	}
}

template<BM3_CONTROLBASE_TEMPLATE_TEMPLATE_ARG>
void	Base<BM3_CONTROLBASE_TEMPLATE_ARG>::SetMaxParticleCount(){
	if(child_buffer_){

		const typename ResourceType::ChildArrayType_	&child_array	= Resource().ChildArray();
		int												child_count		= ml::minimum(Resource().ChildArray().Count(), BM3_MAX_CHILD_NODE_COUNT);

		#define BM3_MAXPARTICLE_COUNT(NAMESPACE, NODE_TYPE)			\
			BM3_NODE_CALL_METHOD(NAMESPACE, NODE_TYPE, SetMaxParticleCount());

		for(int i = 0;i != child_count;++ i){
			ml::ui8*	buffer	= ml::offset_pointer<ml::ui8*>(child_buffer_, (int)child_byte_size_offset_array_[i]);
			switch(child_array.Child(i).Basic_Meta_NodeType()){
				case NodeTypeConst_ControlNull:				BM3_MAXPARTICLE_COUNT(control, Null);			break;
				case NodeTypeConst_EmitterQuad:				BM3_MAXPARTICLE_COUNT(emitter, Quad);			break;
				case NodeTypeConst_EmitterParticle:			BM3_MAXPARTICLE_COUNT(emitter, Particle);		break;
				case NodeTypeConst_EmitterSimpleParticle:	BM3_MAXPARTICLE_COUNT(emitter, SimpleParticle);	break;
				case NodeTypeConst_EmitterMassParticle:		BM3_MAXPARTICLE_COUNT(emitter, MassParticle);	break;
				case NodeTypeConst_EmitterModel:			BM3_MAXPARTICLE_COUNT(emitter, Model);			break;
				case NodeTypeConst_EmitterStripe:			BM3_MAXPARTICLE_COUNT(emitter, Stripe);			break;
				case NodeTypeConst_FieldGravity:			break;
				case NodeTypeConst_FieldWind:				break;
				case NodeTypeConst_FieldVortex:				break;
				case NodeTypeConst_FieldRadialpower:		break;
				case NodeTypeConst_FieldDeceleration:		break;
				case NodeTypeConst_FieldCollision:			break;
				case NodeTypeConst_FieldGoal:				break;
				case NodeTypeConst_ObjectReference:			BM3_MAXPARTICLE_COUNT(object, Reference);		break;
				default:							ML_ASSERT(false);	BM3_ASSUME(0);						break;
			}
		}
		#undef BM3_ACTIVATE_CHILD
	}
}

template<BM3_CONTROLBASE_TEMPLATE_TEMPLATE_ARG>
template<typename UpdateInfo>
void	Base<BM3_CONTROLBASE_TEMPLATE_ARG>::Update(UpdateInfo &info){

	ML_ASSERT(IsInitialized());

	if(is_expired_==false)
	{
		// 同族の生成
		UpdateGenerator(info);

		// 更新
		UpdateControl(info);
		UpdateField(info);
		UpdateParentInfl();	// 生成時に子供にマトリックスを参照させることがあるためここでマトリックスを確定しておく
		PrepareField(info);	// 生成時に子供にマトリックスを参照させることがあるためここでフィールドを決定しておく
		UpdateChild(info);

		is_expired_		= IsExpired();
	}
}

template<BM3_CONTROLBASE_TEMPLATE_TEMPLATE_ARG>
void	Base<BM3_CONTROLBASE_TEMPLATE_ARG>::ResetActiveCount(){
	if(child_buffer_){

		const typename ResourceType::ChildArrayType_	&child_array	= Resource().ChildArray();
		int												child_count		= ml::minimum(Resource().ChildArray().Count(), BM3_MAX_CHILD_NODE_COUNT);

		#define BM3_ACTIVE_COUNT(NAMESPACE, NODE_TYPE)			\
			BM3_NODE_CALL_METHOD(NAMESPACE, NODE_TYPE, ResetActiveCount());


		for(int i = 0;i != child_count;++ i){
			ml::ui8*	buffer	= ml::offset_pointer<ml::ui8*>(child_buffer_, (int)child_byte_size_offset_array_[i]);
			switch(child_array.Child(i).Basic_Meta_NodeType()){
				case NodeTypeConst_ControlNull:				BM3_ACTIVE_COUNT(control, Null);			break;
				case NodeTypeConst_EmitterQuad:				BM3_ACTIVE_COUNT(emitter, Quad);			break;
				case NodeTypeConst_EmitterParticle:			BM3_ACTIVE_COUNT(emitter, Particle);		break;
				case NodeTypeConst_EmitterSimpleParticle:	BM3_ACTIVE_COUNT(emitter, SimpleParticle);	break;
				case NodeTypeConst_EmitterMassParticle:		BM3_ACTIVE_COUNT(emitter, MassParticle);	break;
				case NodeTypeConst_EmitterModel:			BM3_ACTIVE_COUNT(emitter, Model);			break;
				case NodeTypeConst_EmitterStripe:			BM3_ACTIVE_COUNT(emitter, Stripe);			break;
				case NodeTypeConst_FieldGravity:			break;
				case NodeTypeConst_FieldWind:				break;
				case NodeTypeConst_FieldVortex:				break;
				case NodeTypeConst_FieldRadialpower:		break;
				case NodeTypeConst_FieldDeceleration:		break;
				case NodeTypeConst_FieldCollision:			break;
				case NodeTypeConst_FieldGoal:				break;
				case NodeTypeConst_ObjectReference:			BM3_ACTIVE_COUNT(object, Reference);		break;
				default:							ML_ASSERT(false);	BM3_ASSUME(0);					break;
			}
		}
		#undef BM3_ACTIVE_COUNT

		active_count_per_frame_	= 0;
	}
}

template<BM3_CONTROLBASE_TEMPLATE_TEMPLATE_ARG>
void	Base<BM3_CONTROLBASE_TEMPLATE_ARG>::UpdateGenerator(UpdateInfo &info){

	ML_UPDATE_TIMER;

	ChildGenerateTimingPolicy	*gen		= &generator_array_[0];
	ChildGenerateTimingPolicy	*gen_end	= &generator_array_[active_gen_count_];
	NodeParam					*param		= &node_param_array_[0];

	for(; gen!=gen_end ; ++gen, ++param){

		float count = gen->Update(info, Resource(), random_, Root()) * info.GenerateChildScale() + generate_child_remain_;
		int gen_count = (int)count;
		generate_child_remain_	= count - gen_count;
		if(0<gen_count){

			gen_count	= ml::minimum(gen_count, max_node_count_);	// frame rate によってはマックスを超える生成数を出す可能性があるため
			int over_gen_count = current_node_pointer_ + gen_count - max_node_count_;

			if(0<over_gen_count){
				ActivateNodeDependParent(	current_node_pointer_,	max_node_count_,	param->Parent());
				ActivateNodeDependParent(	0,						over_gen_count,		param->Parent());
				ActivateNode(				current_node_pointer_,	max_node_count_);
				ActivateNode(				0,						over_gen_count);
			}
			else{
				ActivateNodeDependParent(	current_node_pointer_,	current_node_pointer_ + gen_count, param->Parent());
				ActivateNode(				current_node_pointer_,	current_node_pointer_ + gen_count);
			}
			current_node_pointer_ += gen_count;
			while(max_node_count_<=current_node_pointer_){
				current_node_pointer_ -= max_node_count_;
			}
			active_node_count_	+= gen_count;
			if(max_node_count_<=active_node_count_){
				active_node_count_ = max_node_count_;
			}
		}
	}
}

template<BM3_CONTROLBASE_TEMPLATE_TEMPLATE_ARG>
void	Base<BM3_CONTROLBASE_TEMPLATE_ARG>::ActivateNode(int start_node_num, int end_node_num){

	ML_PTR_ASSERT(generator_array_);
	ML_PTR_ASSERT(control_param_array_);
	ML_ASSERT(start_node_num<end_node_num);

	{
		ControlParam<LifePolicy>	*control		= &control_param_array_[start_node_num];
		ControlParam<LifePolicy>	*control_end	= &control_param_array_[end_node_num];
		for( ; control!=control_end ; ++control){
			PrimitiveInitializeInfo	pii;
			{
				pii.random_seed_ = ml::ui32(control);
			}
			ControlManipulator::Initialize(Resource(), random_, *control, pii);
		}
	}

	if(field_all_byte_size_)
	{
		const typename ResourceType::ChildArrayType_	&child_array	= Resource().ChildArray();
		int												child_count		= ml::minimum(Resource().ChildArray().Count(), BM3_MAX_CHILD_NODE_COUNT);

		int node_count	= end_node_num - start_node_num;
		ControlParam<LifePolicy>	*control	= &control_param_array_[start_node_num];
		ml::ui8						*field		= ml::offset_pointer<ml::ui8*>(field_buffer_, start_node_num * field_all_byte_size_);
		for(int k=0 ; k<node_count ; ++k, ++control){

			for(int i=0 ; i<child_count ; ++i){

				#define	BM3_INITIALIZE_FIELD(NAMESPACE, NODE_TYPE)	{										\
					NAMESPACE::FieldManipulator::Initialize(												\
						*reinterpret_cast<NAMESPACE::Field*>(field),										\
						reinterpret_cast<const BM3_CHILD_RESOURCE_TYPE(NODE_TYPE) &>(child_array.Child(i)),	\
						random_, *control);																\
					field	+= sizeof(NAMESPACE::Field);													\
				}

				switch(child_array.Child(i).Basic_Meta_NodeType()){
					case NodeTypeConst_ControlNull:																	break;
					case NodeTypeConst_EmitterQuad:																	break;
					case NodeTypeConst_EmitterParticle:																break;
					case NodeTypeConst_EmitterSimpleParticle:														break;
					case NodeTypeConst_EmitterMassParticle:															break;
					case NodeTypeConst_EmitterModel:																break;
					case NodeTypeConst_EmitterStripe:																break;
					case NodeTypeConst_FieldGravity:		BM3_INITIALIZE_FIELD(field::gravity		,Gravity);		break;
					case NodeTypeConst_FieldWind:			BM3_INITIALIZE_FIELD(field::wind		,Wind);			break;
					case NodeTypeConst_FieldVortex:			BM3_INITIALIZE_FIELD(field::vortex		,Vortex);		break;
					case NodeTypeConst_FieldRadialpower:	BM3_INITIALIZE_FIELD(field::radialpower	,Radialpower);	break;
					case NodeTypeConst_FieldDeceleration:	BM3_INITIALIZE_FIELD(field::deceleration,Deceleration);	break;
					case NodeTypeConst_FieldCollision:		BM3_INITIALIZE_FIELD(field::collision	,Collision);	break;
					case NodeTypeConst_FieldGoal:			BM3_INITIALIZE_FIELD(field::goal		,Goal);			break;
					case NodeTypeConst_ObjectReference:			break;
					default:							ML_ASSERT(false);	BM3_ASSUME(0);							break;
				}

				#undef BM3_INITIALIZE_FIELD

			}
		}
	}
}

// 親と関わるノード処理
// ノードの初期化を済ませた後で呼び出さないとだめ。
template<BM3_CONTROLBASE_TEMPLATE_TEMPLATE_ARG>
void	Base<BM3_CONTROLBASE_TEMPLATE_ARG>::ActivateNodeDependParent(int start_node_num, int end_node_num, NodeBase *parent){

	ML_PTR_ASSERT(node_param_array_);
	ML_PTR_ASSERT(node_base_array_);
	ML_ASSERT(start_node_num<end_node_num);

	{
		if(parent){

			BaseClass	*base		= &node_base_array_[start_node_num];
			BaseClass	*base_end	= &node_base_array_[end_node_num];

			for( ; base!=base_end ; ++base){
				new(base) BaseClass();
				base->SetParent(parent);
				base->SetGenerateTimeHierarchyParentMatrix(parent->HierarchyMatrix());

				base->FixGenerateTimeMatix(
					Resource().Basic_ParentNodeInfluence_TranslateParentNodeInfluenceType(),
					Resource().Basic_ParentNodeInfluence_RotateParentNodeInfluenceType(),
					Resource().Basic_ParentNodeInfluence_ScaleParentNodeInfluenceType()
				);

				base->SetGenerateTimeHierarchyParentMatrixRowLength(base->GenerateTimeHierarchyParentMatrix().row_length());

				base->SetHierarchyMatrix(			base->GenerateTimeHierarchyParentMatrix());
				base->SetHierarchyMatrixRowLength(	base->GenerateTimeHierarchyParentMatrixRowLength());
			}
		}
	}

	// 子のジェネレータをアクティベーション
	ActivateChildGenerator(start_node_num, end_node_num);
}

template<BM3_CONTROLBASE_TEMPLATE_TEMPLATE_ARG>
void	Base<BM3_CONTROLBASE_TEMPLATE_ARG>::ActivateChildGenerator(int start_node_num, int end_node_num){

	BaseClass	*base		= &node_base_array_[start_node_num];
	BaseClass	*base_end	= &node_base_array_[end_node_num];
	for( ; base!=base_end ; ++base){
		ActivateChildGenerator(base);
	}
}


template<BM3_CONTROLBASE_TEMPLATE_TEMPLATE_ARG>
void	Base<BM3_CONTROLBASE_TEMPLATE_ARG>::ActivateChildGenerator(NodeBase *parent){
	if(child_buffer_){

		const typename ResourceType::ChildArrayType_	&child_array	= Resource().ChildArray();
		int												child_count		= ml::minimum(Resource().ChildArray().Count(), BM3_MAX_CHILD_NODE_COUNT);

		#define BM3_ACTIVATE_CHILD(NAMESPACE, NODE_TYPE)			\
			BM3_NODE_CALL_METHOD(NAMESPACE, NODE_TYPE, ActivateGenerator(parent));

		#define BM3_ACTIVATE_EMITTER_CHILD(NAMESPACE, NODE_TYPE)	\
			BM3_NODE_CALL_METHOD(NAMESPACE, NODE_TYPE, ActivateGenerator(parent));

		#define BM3_ACTIVATE_OBJECT_CHILD(NAMESPACE, NODE_TYPE)	\
			BM3_NODE_CALL_METHOD(NAMESPACE, NODE_TYPE, ActivateChildGenerator(parent));

		for(int i = 0;i != child_count;++ i){
			ml::ui8*	buffer	= ml::offset_pointer<ml::ui8*>(child_buffer_, (int)child_byte_size_offset_array_[i]);
			switch(child_array.Child(i).Basic_Meta_NodeType()){
				case NodeTypeConst_ControlNull:				BM3_ACTIVATE_CHILD(control, Null);						break;
				case NodeTypeConst_EmitterQuad:				BM3_ACTIVATE_EMITTER_CHILD(emitter, Quad);				break;
				case NodeTypeConst_EmitterParticle:			BM3_ACTIVATE_EMITTER_CHILD(emitter, Particle);			break;
				case NodeTypeConst_EmitterSimpleParticle:	BM3_ACTIVATE_EMITTER_CHILD(emitter, SimpleParticle);	break;
				case NodeTypeConst_EmitterMassParticle:		BM3_ACTIVATE_EMITTER_CHILD(emitter, MassParticle);		break;
				case NodeTypeConst_EmitterModel:			BM3_ACTIVATE_EMITTER_CHILD(emitter, Model);				break;
				case NodeTypeConst_EmitterStripe:			BM3_ACTIVATE_EMITTER_CHILD(emitter, Stripe);			break;
				case NodeTypeConst_FieldGravity:			break;
				case NodeTypeConst_FieldWind:				break;
				case NodeTypeConst_FieldVortex:				break;
				case NodeTypeConst_FieldRadialpower:		break;
				case NodeTypeConst_FieldDeceleration:		break;
				case NodeTypeConst_FieldCollision:			break;
				case NodeTypeConst_FieldGoal:				break;
				case NodeTypeConst_ObjectReference:			BM3_ACTIVATE_OBJECT_CHILD(object, Reference);			break;
				default:							ML_ASSERT(false);	BM3_ASSUME(0);								break;
			}
		}
		#undef BM3_ACTIVATE_CHILD
	}
}

template<BM3_CONTROLBASE_TEMPLATE_TEMPLATE_ARG>
void	Base<BM3_CONTROLBASE_TEMPLATE_ARG>::ActivateGenerator(NodeBase *parent){

	ChildGenerateTimingPolicy	*gen	= &generator_array_[current_gen_pointer_];
	NodeParam					*param	= &node_param_array_[current_gen_pointer_];
	is_expired_	= false;

	if(active_gen_count_==max_gen_count_){
		// 2週目
	}
	else{
		new(gen) ChildGenerateTimingPolicy();
	}
	gen->Initialize(Resource(), random_);
	param->SetParentParam(parent);


	current_gen_pointer_++;
	if(max_gen_count_<=current_gen_pointer_){
		current_gen_pointer_	= 0;
	}
	if(active_gen_count_<max_gen_count_){
		active_gen_count_++;
	}
}

template<BM3_CONTROLBASE_TEMPLATE_TEMPLATE_ARG>
void	Base<BM3_CONTROLBASE_TEMPLATE_ARG>::UpdateControl(UpdateInfo &info){

	ML_UPDATE_TIMER;
	active_count_per_frame_	= 0;

	if(active_node_count_==max_node_count_){
		active_count_per_frame_ += UpdateControl(info, current_node_pointer_,	active_node_count_,		&active_node_index_[0]);
		active_count_per_frame_ += UpdateControl(info, 0,						current_node_pointer_,	&active_node_index_[active_count_per_frame_]);
	}
	else{
		active_count_per_frame_ += UpdateControl(info, 0					,	active_node_count_,		active_node_index_);
	}
}
template<BM3_CONTROLBASE_TEMPLATE_TEMPLATE_ARG>
int	Base<BM3_CONTROLBASE_TEMPLATE_ARG>::UpdateControl(UpdateInfo &info, int begin, int end, int *active_node_index){
	ML_UPDATE_TIMER;

	ControlParam<LifePolicy>	*control		= &control_param_array_[begin];
	BaseClass					*base			= &node_base_array_[begin];
	int							*active_node	= active_node_index;
	int							active_count	= 0;

	for( int i=begin ; i!=end ; ++i, ++control, ++base){

		if(base->Parent()->IsEnabledChildGenerated()==false){
				switch(Resource().Basic_Basic_ExpirationType()){
					case ExpirationTypeConst_Life:				base->DisableChildGenerated();	break;
					case ExpirationTypeConst_AllChildExpired:									break;	// 子供の寿命を見ている場合があるため更新し続ける
					default:			ML_ASSERT(false);										break;
				}
		}
		else{
			bool	is_enabled		= false;
			bool	is_first_frame	= control->IsFirstFrame();
			control->UpdateLife(info);

			if(control->IsActive()){
				is_enabled	= true;
			}
			else{
				ML_STATIC_ASSERT(ExpirationTypeConst_Life == 0);
				ML_STATIC_ASSERT(ExpirationTypeConst_AllChildExpired == 1);
				ML_STATIC_ASSERT(ExpirationTypeConst_MAX == 2);

				switch(Resource().Basic_Basic_ExpirationType()){
					case ExpirationTypeConst_Life:				base->DisableChildGenerated();	break;
					case ExpirationTypeConst_AllChildExpired:	is_enabled	= true;				break;	// 子供の寿命を見ている場合があるため更新し続ける
					default:			ML_ASSERT(false);										break;
				}
			}

			if(is_enabled){
				*active_node		= i;
				if(is_first_frame == false){
					PrimitiveUpdateInfo	pui;
					pui.life_rate_		= control->LifeRate();
					pui.random_seed_	= ml::ui32(control);
					ControlManipulator::Update(info, Resource(), *control, pui, random_);
				}
				active_node++;
				active_count++;
			}
		}
	}

	return active_count;
}


template<BM3_CONTROLBASE_TEMPLATE_TEMPLATE_ARG>
void	Base<BM3_CONTROLBASE_TEMPLATE_ARG>::UpdateParentInfl(){

	ML_MAKEVERTEX_TIMER;
	if(IsRoot()){
		BaseClass					*nodebase	= &node_base_array_[0];
		nodebase->SetHierarchyMatrixRowLength(nodebase->HierarchyMatrix().row_length());	
	}
	else{
#define BM3_BLEND_PARENTCOLOR(c, parent_color)	((1.0f - res_->Basic_ParentNodeInfluence_ParentColorRate()) * (c) + (res_->Basic_ParentNodeInfluence_ParentColorRate()) * (parent_color))
#define BM3_BLEND_PARENTALPHA(c, parent_color)	((1.0f - res_->Basic_ParentNodeInfluence_ParentAlphaRate()) * (c) + (res_->Basic_ParentNodeInfluence_ParentAlphaRate()) * (parent_color))
		
		PrimitivePrepareInfo	ppi;
		if(Root().IsEnabledBm2()== true){

			for(int i=0 ; i<active_count_per_frame_ ; ++i){
				int							index		= active_node_index_[i];
				ControlParam<LifePolicy>	*control	= &control_param_array_[index];
				BaseClass					*nodebase	= &node_base_array_[index];

				ml::matrix44 matrix = nodebase->Matrix();
				{
					ppi.life_rate_		= control->LifeRate();
					ppi.random_seed_	= ml::ui32(control);

					ControlManipulator::PrepareBm2(Resource(), *control, matrix, ppi);
				}

				nodebase->MakeHierarchyMatrix(
					matrix,
					Resource().Basic_ParentNodeInfluence_TranslateParentNodeInfluenceType(),
					Resource().Basic_ParentNodeInfluence_RotateParentNodeInfluenceType(),
					Resource().Basic_ParentNodeInfluence_ScaleParentNodeInfluenceType()
				);
				nodebase->SetHierarchyMatrixRowLength(nodebase->HierarchyMatrix().row_length());

				ColorType& c = nodebase->BlendColor();
				c.e_.r_	= BM3_BLEND_PARENTCOLOR(control->GetColor().e_.r_, nodebase->Parent()->BlendColor().e_.r_);
				c.e_.g_	= BM3_BLEND_PARENTCOLOR(control->GetColor().e_.g_, nodebase->Parent()->BlendColor().e_.g_);
				c.e_.b_	= BM3_BLEND_PARENTCOLOR(control->GetColor().e_.b_, nodebase->Parent()->BlendColor().e_.b_);
				c.e_.a_	= BM3_BLEND_PARENTALPHA(control->GetColor().e_.a_, nodebase->Parent()->BlendColor().e_.a_);
			}
		}
		else{
			for(int i=0 ; i<active_count_per_frame_ ; ++i){
				int							index		= active_node_index_[i];
				ControlParam<LifePolicy>	*control	= &control_param_array_[index];
				BaseClass					*nodebase	= &node_base_array_[index];

				ml::matrix44 matrix = nodebase->Matrix();

				{
					ppi.life_rate_		= control->LifeRate();
					ppi.random_seed_	= ml::ui32(control);

					ControlManipulator::Prepare(Resource(), *control, matrix, ppi);
				}
				nodebase->MakeHierarchyMatrix(
					matrix,
					Resource().Basic_ParentNodeInfluence_TranslateParentNodeInfluenceType(),
					Resource().Basic_ParentNodeInfluence_RotateParentNodeInfluenceType(),
					Resource().Basic_ParentNodeInfluence_ScaleParentNodeInfluenceType()
				);
				nodebase->SetHierarchyMatrixRowLength(nodebase->HierarchyMatrix().row_length());

				ColorType& c = nodebase->BlendColor();
				c.e_.r_	= BM3_BLEND_PARENTCOLOR(control->GetColor().e_.r_, nodebase->Parent()->BlendColor().e_.r_);
				c.e_.g_	= BM3_BLEND_PARENTCOLOR(control->GetColor().e_.g_, nodebase->Parent()->BlendColor().e_.g_);
				c.e_.b_	= BM3_BLEND_PARENTCOLOR(control->GetColor().e_.b_, nodebase->Parent()->BlendColor().e_.b_);
				c.e_.a_	= BM3_BLEND_PARENTALPHA(control->GetColor().e_.a_, nodebase->Parent()->BlendColor().e_.a_);
			}
		}
#undef BM3_BLEND_PARENTALPHA
#undef BM3_BLEND_PARENTCOLOR

	}
}
template<BM3_CONTROLBASE_TEMPLATE_TEMPLATE_ARG>
void	Base<BM3_CONTROLBASE_TEMPLATE_ARG>::UpdateField(UpdateInfo &info){

	if(field_all_byte_size_)
	{
		ML_UPDATE_TIMER;

		const typename ResourceType::ChildArrayType_	&child_array	= Resource().ChildArray();
		int												child_count		= ml::minimum(Resource().ChildArray().Count(), BM3_MAX_CHILD_NODE_COUNT);

		for(int k=0 ; k<active_count_per_frame_ ; ++k){
			int							index = active_node_index_[k];
			ControlParam<LifePolicy>	*control	= &control_param_array_[index];
			ml::ui8						*field		= ml::offset_pointer<ml::ui8*>(field_buffer_, index * field_all_byte_size_);

			PrimitiveUpdateInfo	pui;
			pui.life_rate_		= control->LifeRate();
			pui.random_seed_	= ml::ui32(control);
			for(int i=0 ; i<child_count ; ++i){

				#define	BM3_UPDATE_FIELD(NAMESPACE, NODE_TYPE)	{											\
					NAMESPACE::FieldManipulator::Update(													\
						info,																				\
						*reinterpret_cast<NAMESPACE::Field*>(field),										\
						reinterpret_cast<const BM3_CHILD_RESOURCE_TYPE(NODE_TYPE) &>(child_array.Child(i)),	\
						*control,																			\
						pui);																				\
					field	+= sizeof(NAMESPACE::Field);													\
				}

				switch(child_array.Child(i).Basic_Meta_NodeType()){
					case NodeTypeConst_ControlNull:																	break;
					case NodeTypeConst_EmitterQuad:																	break;
					case NodeTypeConst_EmitterParticle:																break;
					case NodeTypeConst_EmitterSimpleParticle:														break;
					case NodeTypeConst_EmitterMassParticle:															break;
					case NodeTypeConst_EmitterModel:																break;
					case NodeTypeConst_EmitterStripe:																break;
					case NodeTypeConst_FieldGravity:		BM3_UPDATE_FIELD(field::gravity		,Gravity);		break;
					case NodeTypeConst_FieldWind:			BM3_UPDATE_FIELD(field::wind		,Wind);			break;
					case NodeTypeConst_FieldVortex:			BM3_UPDATE_FIELD(field::vortex		,Vortex);		break;
					case NodeTypeConst_FieldRadialpower:	BM3_UPDATE_FIELD(field::radialpower	,Radialpower);	break;
					case NodeTypeConst_FieldDeceleration:	BM3_UPDATE_FIELD(field::deceleration,Deceleration);	break;
					case NodeTypeConst_FieldCollision:		BM3_UPDATE_FIELD(field::collision	,Collision);	break;
					case NodeTypeConst_FieldGoal:			BM3_UPDATE_FIELD(field::goal		,Goal);			break;
					case NodeTypeConst_ObjectReference:			break;
					default:							ML_ASSERT(false);	BM3_ASSUME(0);							break;
				}

				#undef BM3_UPDATE_FIELD
			}
		}
#if defined(BM3_TARGET_IDE)
		debug_life_.Initialize(*res_);
#endif
	}
}

template<BM3_CONTROLBASE_TEMPLATE_TEMPLATE_ARG>
void	Base<BM3_CONTROLBASE_TEMPLATE_ARG>::UpdateChild(UpdateInfo &info){
	if(child_buffer_){
		ML_UPDATE_TIMER;

		const typename ResourceType::ChildArrayType_	&child_array	= Resource().ChildArray();
		int												child_count		= ml::minimum(Resource().ChildArray().Count(), BM3_MAX_CHILD_NODE_COUNT);

		#define BM3_UPDATE_CHILD(NAMESPACE, NODE_TYPE)	BM3_NODE_CALL_METHOD(NAMESPACE, NODE_TYPE, Update(info));

		for(int i = 0;i != child_count;++ i){
			ml::ui8*	buffer	= ml::offset_pointer<ml::ui8*>(child_buffer_, (int)child_byte_size_offset_array_[i]);
			switch(child_array.Child(i).Basic_Meta_NodeType()){
				case NodeTypeConst_ControlNull:				BM3_UPDATE_CHILD(control, Null);			break;
				case NodeTypeConst_EmitterQuad:				BM3_UPDATE_CHILD(emitter, Quad);			break;
				case NodeTypeConst_EmitterParticle:			BM3_UPDATE_CHILD(emitter, Particle);		break;
				case NodeTypeConst_EmitterSimpleParticle:	BM3_UPDATE_CHILD(emitter, SimpleParticle);	break;
				case NodeTypeConst_EmitterMassParticle:		BM3_UPDATE_CHILD(emitter, MassParticle);	break;
				case NodeTypeConst_EmitterModel:			BM3_UPDATE_CHILD(emitter, Model);			break;
				case NodeTypeConst_EmitterStripe:			BM3_UPDATE_CHILD(emitter, Stripe);			break;
				case NodeTypeConst_FieldGravity:			break;
				case NodeTypeConst_FieldWind:				break;
				case NodeTypeConst_FieldVortex:				break;
				case NodeTypeConst_FieldRadialpower:		break;
				case NodeTypeConst_FieldDeceleration:		break;
				case NodeTypeConst_FieldCollision:			break;
				case NodeTypeConst_FieldGoal:				break;
				case NodeTypeConst_ObjectReference:			BM3_UPDATE_CHILD(object, Reference);		break;
				default:							ML_ASSERT(false);	BM3_ASSUME(0);					break;
			}
		}
		#undef BM3_UPDATE_CHILD
	}
}

template<BM3_CONTROLBASE_TEMPLATE_TEMPLATE_ARG>
bool	Base<BM3_CONTROLBASE_TEMPLATE_ARG>::IsExpired() const{

	ML_STATIC_ASSERT(ExpirationTypeConst_Life == 0);
	ML_STATIC_ASSERT(ExpirationTypeConst_AllChildExpired == 1);
	ML_STATIC_ASSERT(ExpirationTypeConst_MAX == 2);

	switch(Resource().Basic_Basic_ExpirationType()){
		case ExpirationTypeConst_Life:{
			if(IsRoot()){
				{
					ChildGenerateTimingPolicy	*gen		= &generator_array_[0];
					ChildGenerateTimingPolicy	*gen_end	= &generator_array_[max_gen_count_];
					for(; gen != gen_end ; ++gen){
						if(gen->IsAllChildGenerated(Resource(), Root())==false)	return false;
					}
				}
				return (active_count_per_frame_ == 0);
			}
			else{
				// すべて放出しきったかにかかわらず寿命により期限を決める
				{
					ChildGenerateTimingPolicy	*gen		= &generator_array_[0];
					ChildGenerateTimingPolicy	*gen_end	= &generator_array_[max_gen_count_];
					for(; gen != gen_end ; ++gen){
						if(gen->IsAllChildGenerated(Resource(), Root())==false)	return false;
					}
					
					ControlParam<LifePolicy>	*c		= &control_param_array_[0];
					ControlParam<LifePolicy>	*c_end	= &control_param_array_[active_node_count_];
					for( ; c != c_end ; ++c){
						if(c->IsExpired()==false) return false;
					}
				}
				return (active_node_count_!=0);	// 一度も生成していない状態がありうるのでそれを回避
			}
		}
		case ExpirationTypeConst_AllChildExpired:{
			{
				ChildGenerateTimingPolicy	*gen		= &generator_array_[0];
				ChildGenerateTimingPolicy	*gen_end	= &generator_array_[max_gen_count_];

				for(; gen != gen_end ; ++gen){
					if(gen->IsAllChildGenerated(Resource(), Root())==false)	return false;
				}
			}
			return Child_IsExpired();
//			return
//				child_generate_timing_.IsAllChildGenerated(Resource(), Root()) ?
//					is_last_update_exist_active_child_ == false :
//					false;
		}
		default:{
			ML_ASSERT(false);
			break;
		}
	}
	return false;
}

template<BM3_CONTROLBASE_TEMPLATE_TEMPLATE_ARG>
bool	Base<BM3_CONTROLBASE_TEMPLATE_ARG>::Child_IsExpired() const{
	if(child_buffer_){

		const typename ResourceType::ChildArrayType_	&child_array	= Resource().ChildArray();
		int												child_count		= ml::minimum(Resource().ChildArray().Count(), BM3_MAX_CHILD_NODE_COUNT);

		#define BM3_EXPIRED_CHILD(NAMESPACE, NODE_TYPE)						\
			BM3_NODE_CALL_METHOD(NAMESPACE, NODE_TYPE, IsAllExpired())

		for(int i = 0;i != child_count;++ i){
			ml::ui8*	buffer	= ml::offset_pointer<ml::ui8*>(child_buffer_, (int)child_byte_size_offset_array_[i]);
			switch(child_array.Child(i).Basic_Meta_NodeType()){
				case NodeTypeConst_ControlNull:				if(BM3_EXPIRED_CHILD(control, Null)				==false){	return false;	}	break;
				case NodeTypeConst_EmitterQuad:				if(BM3_EXPIRED_CHILD(emitter, Quad)				==false){	return false;	}	break;
				case NodeTypeConst_EmitterParticle:			if(BM3_EXPIRED_CHILD(emitter, Particle)			==false){	return false;	}	break;
				case NodeTypeConst_EmitterSimpleParticle:	if(BM3_EXPIRED_CHILD(emitter, SimpleParticle)	==false){	return false;	}	break;
				case NodeTypeConst_EmitterMassParticle:		if(BM3_EXPIRED_CHILD(emitter, MassParticle)		==false){	return false;	}	break;
				case NodeTypeConst_EmitterModel:			if(BM3_EXPIRED_CHILD(emitter, Model)			==false){	return false;	}	break;
				case NodeTypeConst_EmitterStripe:			if(BM3_EXPIRED_CHILD(emitter, Stripe)			==false){	return false;	}	break;
				case NodeTypeConst_FieldGravity:			break;
				case NodeTypeConst_FieldWind:				break;
				case NodeTypeConst_FieldVortex:				break;
				case NodeTypeConst_FieldRadialpower:		break;
				case NodeTypeConst_FieldDeceleration:		break;
				case NodeTypeConst_FieldCollision:			break;
				case NodeTypeConst_FieldGoal:				break;
				case NodeTypeConst_ObjectReference:			if(BM3_EXPIRED_CHILD(object, Reference)			==false){	return false;	}	break;
				default:							ML_ASSERT(false);	BM3_ASSUME(0);													break;
			}
		}
		#undef BM3_EXPIRED_CHILD
	}

	return true;
}


template<BM3_CONTROLBASE_TEMPLATE_TEMPLATE_ARG>
void	Base<BM3_CONTROLBASE_TEMPLATE_ARG>::MakeVertex(DrawInfo &info){

	ML_MAKEVERTEX_TIMER;
	ML_ASSERT(IsInitialized());

	MakeChildVertex(info);

#if defined(BM3_TARGET_IDE)
	DrawControl(info);
	DrawGenerateShape(info);
	field::FieldDraw(info, Resource(), Root().HierarchyMatrix(), debug_life_);
#endif
}

#if defined(BM3_TARGET_IDE)
template<BM3_CONTROLBASE_TEMPLATE_TEMPLATE_ARG>
void	Base<BM3_CONTROLBASE_TEMPLATE_ARG>::DrawGenerateShape(DrawInfo &draw_info){

	if(Resource().IsSelected() && Resource().Basic_Meta_Display() && Resource().GenerateShape_Basic_Display()){

		ChildGenerateTimingPolicy	*gen		= &generator_array_[0];
		ChildGenerateTimingPolicy	*gen_end	= &generator_array_[max_gen_count_];

		for(; gen != gen_end ; ++gen){
		}

/*
		PrimGenerator	*gen		= &generator_array_[0];
		PrimGenerator	*gen_end	= &generator_array_[active_gen_count_];
		int temp_active_node_count = 0;
		for(; gen != gen_end ; ++gen){
			if(gen->ActiveCountPerFrame()){

				prim::CommonPrimitiveManipulator::DrawGenerateShape(
					prim_param_array_, gen->ActiveCountPerFrame(), &active_node_index_[temp_active_node_count], draw_info, Resource());
				temp_active_node_count	+= gen->ActiveCountPerFrame();
			}
		}
*/
	}
}
#endif

template<BM3_CONTROLBASE_TEMPLATE_TEMPLATE_ARG>
void	Base<BM3_CONTROLBASE_TEMPLATE_ARG>::PrepareField(UpdateInfo &update_info){

	if(field_all_byte_size_)
	{
		ML_MAKEVERTEX_TIMER;

		const typename ResourceType::ChildArrayType_	&child_array	= Resource().ChildArray();
		int												child_count		= ml::minimum(Resource().ChildArray().Count(), BM3_MAX_CHILD_NODE_COUNT);

		for(int k=0 ; k<active_count_per_frame_ ; ++k){
			int							index = active_node_index_[k];
			ControlParam<LifePolicy>	*control	= &control_param_array_[index];
			NodeBase					*nodebase	= &node_base_array_[index];
			ml::ui8						*field		= ml::offset_pointer<ml::ui8*>(field_buffer_, index * field_all_byte_size_);

			control->field_info_.Reset();
			for(int i=0 ; i<child_count ; ++i){

				#define	BM3_PREPARE_FIELD(NAMESPACE, NODE_TYPE)	{											\
					NAMESPACE::FieldManipulator::Prepare(													\
						control->field_info_,																\
						*reinterpret_cast<NAMESPACE::Field*>(field),										\
						reinterpret_cast<const BM3_CHILD_RESOURCE_TYPE(NODE_TYPE) &>(child_array.Child(i)),	\
						nodebase->Matrix(),																	\
						update_info,																		\
						Root(),																				\
						*control																			\
					);																						\
					field	+= sizeof(NAMESPACE::Field);													\
				}

				switch(child_array.Child(i).Basic_Meta_NodeType()){
					case NodeTypeConst_ControlNull:																break;
					case NodeTypeConst_EmitterQuad:																break;
					case NodeTypeConst_EmitterParticle:															break;
					case NodeTypeConst_EmitterSimpleParticle:													break;
					case NodeTypeConst_EmitterMassParticle:														break;
					case NodeTypeConst_EmitterModel:															break;
					case NodeTypeConst_EmitterStripe:															break;
					case NodeTypeConst_FieldGravity:		BM3_PREPARE_FIELD(field::gravity,		Gravity);		break;
					case NodeTypeConst_FieldWind:			BM3_PREPARE_FIELD(field::wind,			Wind);			break;
					case NodeTypeConst_FieldVortex:			BM3_PREPARE_FIELD(field::vortex,		Vortex);		break;
					case NodeTypeConst_FieldRadialpower:	BM3_PREPARE_FIELD(field::radialpower,	Radialpower);	break;
					case NodeTypeConst_FieldDeceleration:	BM3_PREPARE_FIELD(field::deceleration,	Deceleration);	break;
					case NodeTypeConst_FieldCollision:		
						BM3_PREPARE_FIELD(field::collision,		Collision);		
						control->SetPreviousPosition(nodebase->Matrix());
						break;
					case NodeTypeConst_FieldGoal:			BM3_PREPARE_FIELD(field::goal,			Goal);			break;
					case NodeTypeConst_ObjectReference:			break;
					default:							ML_ASSERT(false);	BM3_ASSUME(0);							break;
				}

				#undef BM3_PREPARE_FIELD
			}
			nodebase->AddHierarchyMatrixTranslation(control->field_info_.Position());
		}
	}
}

template<BM3_CONTROLBASE_TEMPLATE_TEMPLATE_ARG>
void	Base<BM3_CONTROLBASE_TEMPLATE_ARG>::MakeChildVertex(DrawInfo &info){
	if(child_buffer_){
		ML_MAKEVERTEX_TIMER;

		const typename ResourceType::ChildArrayType_	&child_array	= Resource().ChildArray();
		int												child_count		= ml::minimum(Resource().ChildArray().Count(), BM3_MAX_CHILD_NODE_COUNT);

		#define BM3_MAKEVERTEX_CHILD(NAMESPACE, NODE_TYPE)		\
			BM3_NODE_CALL_METHOD(NAMESPACE, NODE_TYPE, MakeVertex(info));

		for(int i = 0;i != child_count;++ i){
			ml::ui8*	buffer	= ml::offset_pointer<ml::ui8*>(child_buffer_, (int)child_byte_size_offset_array_[i]);
			switch(child_array.Child(i).Basic_Meta_NodeType()){
				case NodeTypeConst_ControlNull:				BM3_MAKEVERTEX_CHILD(control, Null);			break;
				case NodeTypeConst_EmitterQuad:				BM3_MAKEVERTEX_CHILD(emitter, Quad);			break;
				case NodeTypeConst_EmitterParticle:			BM3_MAKEVERTEX_CHILD(emitter, Particle);		break;
				case NodeTypeConst_EmitterSimpleParticle:	BM3_MAKEVERTEX_CHILD(emitter, SimpleParticle);	break;
				case NodeTypeConst_EmitterMassParticle:
#if SHADER_MODEL2_0
					//シェーダモデル2.0でコンパイル時はマスパーティクルの処理をしない
#else
					BM3_MAKEVERTEX_CHILD(emitter, MassParticle);
#endif
					break;
				case NodeTypeConst_EmitterModel:			BM3_MAKEVERTEX_CHILD(emitter, Model);			break;
				case NodeTypeConst_EmitterStripe:			BM3_MAKEVERTEX_CHILD(emitter, Stripe);			break;
				case NodeTypeConst_FieldGravity:			break;
				case NodeTypeConst_FieldWind:				break;
				case NodeTypeConst_FieldVortex:				break;
				case NodeTypeConst_FieldRadialpower:		break;
				case NodeTypeConst_FieldDeceleration:		break;
				case NodeTypeConst_FieldCollision:			break;
				case NodeTypeConst_FieldGoal:				break;
				case NodeTypeConst_ObjectReference:			BM3_MAKEVERTEX_CHILD(object, Reference);		break;
				default:							ML_ASSERT(false);	BM3_ASSUME(0);		break;
			}
		}
		#undef BM3_MAKEVERTEX_CHILD
	}
}

template<BM3_CONTROLBASE_TEMPLATE_TEMPLATE_ARG>
void	Base<BM3_CONTROLBASE_TEMPLATE_ARG>::DrawControl(DrawInfo &info){

	if(Resource().Basic_Meta_Display()){

		switch(Resource().Basic_Basic_ExpirationType()){
			case ExpirationTypeConst_Life:{
				for(int i=0 ; i<active_count_per_frame_ ; ++i){
					int			index		= active_node_index_[i];
					BaseClass	&nodebase	= node_base_array_[index];
					ControlParam<LifePolicy>	&control = control_param_array_[index];
					if(control.IsActive()){
						debug::DrawAxis(info, nodebase.HierarchyMatrix(), 1.0f);
					}
				}
			}
			break;
			case ExpirationTypeConst_AllChildExpired:{
				if(Child_IsExpired()==false){
					for(int i=0 ; i<active_count_per_frame_ ; ++i){
						int			index		= active_node_index_[i];
						BaseClass	&nodebase	= node_base_array_[index];
						ControlParam<LifePolicy>	&control = control_param_array_[index];
						debug::DrawAxis(info, nodebase.HierarchyMatrix(), 1.0f);
					}
				}
			}
			break;
			default:	break;
		}
	}
}

template<BM3_CONTROLBASE_TEMPLATE_TEMPLATE_ARG>
int	Base<BM3_CONTROLBASE_TEMPLATE_ARG>::SwitchResource(const ResourceType &res, InitialAllocInfo &info){

	ML_ASSERT(IsInitialized());
	ML_ASSERT(IsValidResource(res));

	const ResourceType &old_res = *res_;
	res_						= &res;

	ReallocParam(old_res, res, info);
	ReallocGenerator(old_res, res, info);	// 生成子当たりの生成数を知りたいから先にパラメータから処理する

	{
		ChildGenerateTimingPolicy	*gen		= &generator_array_[0];
		ChildGenerateTimingPolicy	*gen_end	= &generator_array_[active_gen_count_];
		for(; gen!=gen_end ; ++gen){
			gen->SwitchResource(Resource());
		}
	}
	SwitchResourceField(old_res);

	{
		PrimitiveInitializeInfo	pii;
		for(int i=0 ;i<active_count_per_frame_ ; ++i){
			int	index	= active_node_index_[i];
			ControlParam<LifePolicy>	&control = control_param_array_[index];
			pii.random_seed_ = ml::ui32(&control);
			ControlManipulator::SwitchResource(Resource(), old_res, random_, control, pii);
		}
	}

	// 子供についてもやる
	if(child_buffer_){

		InitialAllocInfo alloc_info(max_node_count_);
		const typename ResourceType::ChildArrayType_	&child_array	= Resource().ChildArray();
		int												child_count		= ml::minimum(Resource().ChildArray().Count(), BM3_MAX_CHILD_NODE_COUNT);

		#define BM3_SWITCHRESOURCE_CHILD(NAMESPACE, NODE_TYPE)														\
			BM3_NODE_CALL_METHOD(NAMESPACE, NODE_TYPE,																\
			SwitchResource(reinterpret_cast<const BM3_CHILD_RESOURCE_TYPE(NODE_TYPE) &>(child_array.Child(i)), alloc_info));

		for(int i = 0;i != child_count;++ i){
			ml::ui8*	buffer	= ml::offset_pointer<ml::ui8*>(child_buffer_, (int)child_byte_size_offset_array_[i]);
			switch(child_array.Child(i).Basic_Meta_NodeType()){
				case NodeTypeConst_ControlNull:				BM3_SWITCHRESOURCE_CHILD(control, Null);			break;
				case NodeTypeConst_EmitterQuad:				BM3_SWITCHRESOURCE_CHILD(emitter, Quad);			break;
				case NodeTypeConst_EmitterParticle:			BM3_SWITCHRESOURCE_CHILD(emitter, Particle);		break;
				case NodeTypeConst_EmitterSimpleParticle:	BM3_SWITCHRESOURCE_CHILD(emitter, SimpleParticle);	break;
				case NodeTypeConst_EmitterMassParticle:
#if SHADER_MODEL2_0
					//シェーダモデル2.0でコンパイル時はマスパーティクルの処理をしない
#else
					BM3_SWITCHRESOURCE_CHILD(emitter, MassParticle);
#endif
					break;
				case NodeTypeConst_EmitterModel:			BM3_SWITCHRESOURCE_CHILD(emitter, Model);			break;
				case NodeTypeConst_EmitterStripe:			BM3_SWITCHRESOURCE_CHILD(emitter, Stripe);			break;
				case NodeTypeConst_FieldGravity:			break;
				case NodeTypeConst_FieldWind:				break;
				case NodeTypeConst_FieldVortex:				break;
				case NodeTypeConst_FieldRadialpower:		break;
				case NodeTypeConst_FieldDeceleration:		break;
				case NodeTypeConst_FieldCollision:			break;
				case NodeTypeConst_FieldGoal:				break;
				case NodeTypeConst_ObjectReference:			BM3_SWITCHRESOURCE_CHILD(object, Reference);		break;
				default:							ML_ASSERT(false);	BM3_ASSUME(0);					break;
			}
		}
		#undef BM3_SWITCHRESOURCE_CHILD
	}
	return 0;
}

template<BM3_CONTROLBASE_TEMPLATE_TEMPLATE_ARG>
void	Base<BM3_CONTROLBASE_TEMPLATE_ARG>::ReallocGenerator(const ResourceType & /*old_res*/, const ResourceType & /*res*/, InitialAllocInfo &info){
	if(max_gen_count_<info.ParentMaxGenCount()){
		int node_count = info.ParentMaxGenCount();
		ChildGenerateTimingPolicy	*temp_generator_array	= reinterpret_cast<ChildGenerateTimingPolicy*>(	bm3::MemoryManager::instance().General_Alloc(node_count * sizeof(ChildGenerateTimingPolicy)));
		NodeParam					*temp_node_param_array	= reinterpret_cast<NodeParam*>(					bm3::MemoryManager::instance().General_Alloc(node_count * sizeof(NodeParam)));

		memcpy(temp_generator_array,	generator_array_,	max_gen_count_ * sizeof(ChildGenerateTimingPolicy));
		memcpy(temp_node_param_array,	node_param_array_,	max_gen_count_ * sizeof(NodeParam));
		// 親のアドレスの位置がわからない

		FreeGenerator();

		max_gen_count_		= info.ParentMaxGenCount();
		generator_array_	= temp_generator_array;
		node_param_array_	= temp_node_param_array;
	}
}

template<BM3_CONTROLBASE_TEMPLATE_TEMPLATE_ARG>
void	Base<BM3_CONTROLBASE_TEMPLATE_ARG>::ReallocParam(const ResourceType & /*old_res*/, const ResourceType &res, InitialAllocInfo &info){

	int new_node_count = info.ParentMaxGenCount() * ChildGenerateTimingPolicy::CalcTotalGenerateNodeCount(res);
	if(max_node_count_<new_node_count){
		int node_count = new_node_count;
		BaseClass					*temp_node_base_array		= reinterpret_cast<BaseClass*>(					bm3::MemoryManager::instance().General_Alloc(node_count * sizeof(BaseClass)));
		ControlParam<LifePolicy>	*temp_control_param_array	= reinterpret_cast<ControlParam<LifePolicy>*>(	bm3::MemoryManager::instance().General_Alloc(node_count * sizeof(ControlParam<LifePolicy>)));
		int							*temp_active_node_index		= reinterpret_cast<int*>(						bm3::MemoryManager::instance().General_Alloc(node_count * sizeof(int)));
		ml::ui8						*temp_field_buffer			= reinterpret_cast<ml::ui8*>(					bm3::MemoryManager::instance().General_Alloc(field_all_byte_size_));

		std::memcpy(temp_node_base_array,	node_base_array_,		max_node_count_ * sizeof(BaseClass));
		std::memcpy(temp_control_param_array,control_param_array_,	max_node_count_ * sizeof(ControlParam<LifePolicy>));
		std::memcpy(temp_active_node_index,	active_node_index_,		max_node_count_ * sizeof(int));

		if(field_all_byte_size_){
			std::memcpy(temp_field_buffer,		field_buffer_,			max_node_count_ * field_all_byte_size_);
		}

		FreeNode();

		node_base_array_		= temp_node_base_array;
		control_param_array_	= temp_control_param_array;
		active_node_index_		= temp_active_node_index;
		field_buffer_			= temp_field_buffer;

		max_node_count_	= new_node_count;
	}
}

template<BM3_CONTROLBASE_TEMPLATE_TEMPLATE_ARG>
bool	Base<BM3_CONTROLBASE_TEMPLATE_ARG>::IsValidResource(const ResourceType &res) const {

	ML_ASSERT(Resource().Basic_Meta_NodeType()	==	res.Basic_Meta_NodeType());
	ML_ASSERT(Resource().ChildArray().Count()	==	res.ChildArray().Count());

	return true;
}

template<BM3_CONTROLBASE_TEMPLATE_TEMPLATE_ARG>
int	Base<BM3_CONTROLBASE_TEMPLATE_ARG>::SwitchResourceField(const ResourceType &old_res){

	if(field_all_byte_size_){

		const typename ResourceType::ChildArrayType_	&child_array		= Resource().ChildArray();
		const typename ResourceType::ChildArrayType_	&old_child_array	= old_res.ChildArray();
		int												child_count			= ml::minimum(Resource().ChildArray().Count(), BM3_MAX_CHILD_NODE_COUNT);

		for(int k=0 ; k<active_count_per_frame_ ; ++k){
			int							index		= active_node_index_[k];
			ml::ui8						*field		= ml::offset_pointer<ml::ui8*>(field_buffer_, index * field_all_byte_size_);

			#define	BM3_SWITCHRESOURCE_FIELD(NAMESPACE, NODE_TYPE)													\
				NAMESPACE::FieldManipulator::SwitchResource(*reinterpret_cast<NAMESPACE::Field*>(field) ,			\
				reinterpret_cast<const BM3_CHILD_RESOURCE_TYPE(NODE_TYPE) &>(child_array.Child(i)),					\
				reinterpret_cast<const BM3_CHILD_RESOURCE_TYPE(NODE_TYPE) &>(old_child_array.Child(i)), random_);	\
				field += sizeof(NAMESPACE::Field);

			for(int i = 0;i != child_count;++ i){
				switch(child_array.Child(i).Basic_Meta_NodeType()){
					case NodeTypeConst_ControlNull:																		break;
					case NodeTypeConst_EmitterQuad:																		break;
					case NodeTypeConst_EmitterParticle:																	break;
					case NodeTypeConst_EmitterSimpleParticle:															break;
					case NodeTypeConst_EmitterMassParticle:																break;
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
					default:							ML_ASSERT(false);	BM3_ASSUME(0);								break;
				}
			}
			#undef BM3_SWITCHRESOURCE_FIELD
		}
	}
	return 0;
}

template<BM3_CONTROLBASE_TEMPLATE_TEMPLATE_ARG>
int	Base<BM3_CONTROLBASE_TEMPLATE_ARG>::MaxVertexBufferSize() const {

	int size = 0;
	if(child_buffer_){

		const typename ResourceType::ChildArrayType_	&child_array	= Resource().ChildArray();
		int												child_count		= ml::minimum(Resource().ChildArray().Count(), BM3_MAX_CHILD_NODE_COUNT);

		#define MAKEVERTEXBUFFERSIZE(NAMESPACE, NODE_TYPE)	size += BM3_NODE_CALL_METHOD(NAMESPACE, NODE_TYPE, MaxVertexBufferSize());

		for(int i = 0;i != child_count;++ i){
			ml::ui8*	buffer	= ml::offset_pointer<ml::ui8*>(child_buffer_, (int)child_byte_size_offset_array_[i]);
			switch(child_array.Child(i).Basic_Meta_NodeType()){
				case NodeTypeConst_ControlNull:				MAKEVERTEXBUFFERSIZE(control, Null);			break;
				case NodeTypeConst_EmitterQuad:				MAKEVERTEXBUFFERSIZE(emitter, Quad);			break;
				case NodeTypeConst_EmitterParticle:			MAKEVERTEXBUFFERSIZE(emitter, Particle);		break;
				case NodeTypeConst_EmitterSimpleParticle:	MAKEVERTEXBUFFERSIZE(emitter, SimpleParticle);	break;
				case NodeTypeConst_EmitterMassParticle:		MAKEVERTEXBUFFERSIZE(emitter, MassParticle);	break;
				case NodeTypeConst_EmitterModel:			MAKEVERTEXBUFFERSIZE(emitter, Model);			break;
				case NodeTypeConst_EmitterStripe:			MAKEVERTEXBUFFERSIZE(emitter, Stripe);			break;
				case NodeTypeConst_FieldGravity:			break;
				case NodeTypeConst_FieldWind:				break;
				case NodeTypeConst_FieldVortex:				break;
				case NodeTypeConst_FieldRadialpower:		break;
				case NodeTypeConst_FieldDeceleration:		break;
				case NodeTypeConst_FieldCollision:			break;
				case NodeTypeConst_FieldGoal:				break;
				case NodeTypeConst_ObjectReference:			MAKEVERTEXBUFFERSIZE(object, Reference);		break;
				default:							ML_ASSERT(false);	BM3_ASSUME(0);					break;
			}
		}
		#undef MAKEVERTEXBUFFERSIZE
	}

	return size;
}

template<BM3_CONTROLBASE_TEMPLATE_TEMPLATE_ARG>
int	Base<BM3_CONTROLBASE_TEMPLATE_ARG>::MaxPrimCount() const {

	int count = 0;
	if(child_buffer_){

		const typename ResourceType::ChildArrayType_	&child_array	= Resource().ChildArray();
		int												child_count		= ml::minimum(Resource().ChildArray().Count(), BM3_MAX_CHILD_NODE_COUNT);

		#define MAXPRIMCOUNT(NAMESPACE, NODE_TYPE)	count += BM3_NODE_CALL_METHOD(NAMESPACE, NODE_TYPE, MaxPrimCount());

		for(int i = 0;i != child_count;++ i){
			ml::ui8*	buffer	= ml::offset_pointer<ml::ui8*>(child_buffer_, (int)child_byte_size_offset_array_[i]);
			switch(child_array.Child(i).Basic_Meta_NodeType()){
				case NodeTypeConst_ControlNull:				MAXPRIMCOUNT(control, Null);			break;
				case NodeTypeConst_EmitterQuad:				MAXPRIMCOUNT(emitter, Quad);			break;
				case NodeTypeConst_EmitterParticle:			MAXPRIMCOUNT(emitter, Particle);		break;
				case NodeTypeConst_EmitterSimpleParticle:	MAXPRIMCOUNT(emitter, SimpleParticle);	break;
				case NodeTypeConst_EmitterMassParticle:		MAXPRIMCOUNT(emitter, MassParticle);	break;
				case NodeTypeConst_EmitterModel:			MAXPRIMCOUNT(emitter, Model);			break;
				case NodeTypeConst_EmitterStripe:			MAXPRIMCOUNT(emitter, Stripe);			break;
				case NodeTypeConst_FieldGravity:			break;
				case NodeTypeConst_FieldWind:				break;
				case NodeTypeConst_FieldVortex:				break;
				case NodeTypeConst_FieldRadialpower:		break;
				case NodeTypeConst_FieldDeceleration:		break;
				case NodeTypeConst_FieldCollision:			break;
				case NodeTypeConst_FieldGoal:				break;
				case NodeTypeConst_ObjectReference:			MAXPRIMCOUNT(object, Reference);		break;
				default:							ML_ASSERT(false);	BM3_ASSUME(0);					break;
			}
		}
		#undef MAXPRIMCOUNT
	}

	return count;
}

#ifdef BM3_PLATFORM_playstation3
template<BM3_CONTROLBASE_TEMPLATE_TEMPLATE_ARG>
template<typename UpdateInfo>
void	Base<BM3_CONTROLBASE_TEMPLATE_ARG>::RawSpuUpdate(UpdateInfo &info){
	ML_ASSERT(IsInitialized());
	

	if(is_expired_==false)
	{
		// 依存関係の計算
		UpdateGenerator(info);		// 同族の生成
		UpdateLife(info);

		UpdateControl(info);
		UpdateField(info);
		UpdateParentInfl();	// 生成時に子供にマトリックスを参照させることがあるためここでマトリックスを確定しておく
		PrepareField(info);	// 生成時に子供にマトリックスを参照させることがあるためここでフィールドを決定しておく

		RawSpuUpdateChild(info);

		is_expired_		= IsExpired();
	}
}

template<BM3_CONTROLBASE_TEMPLATE_TEMPLATE_ARG>
template<typename UpdateInfo>
void	Base<BM3_CONTROLBASE_TEMPLATE_ARG>::RawSpuUpdateField(UpdateInfo &info){
	if(child_buffer_){
		ML_UPDATE_TIMER;

		const typename ResourceType::ChildArrayType_	&child_array	= Resource().ChildArray();
		int												child_count		= ml::minimum(Resource().ChildArray().Count(), BM3_MAX_CHILD_NODE_COUNT);

		#define BM3_UPDATE_CHILD(NAMESPACE, NODE_TYPE)	BM3_NODE_CALL_METHOD(NAMESPACE, NODE_TYPE, RawSpuUpdateField(info));

		for(int i = 0;i != child_count;++ i){
			ml::ui8*	buffer	= ml::offset_pointer<ml::ui8*>(child_buffer_, (int)child_byte_size_offset_array_[i]);
			switch(child_array.Child(i).Basic_Meta_NodeType()){
				case NodeTypeConst_ControlNull:				BM3_UPDATE_CHILD(control, Null);			break;
				case NodeTypeConst_EmitterQuad:				BM3_UPDATE_CHILD(emitter, Quad);			break;
				case NodeTypeConst_EmitterParticle:			BM3_UPDATE_CHILD(emitter, Particle);		break;
				case NodeTypeConst_EmitterSimpleParticle:	BM3_UPDATE_CHILD(emitter, SimpleParticle);	break;
				case NodeTypeConst_EmitterMassParticle:		break;
				case NodeTypeConst_EmitterModel:			BM3_UPDATE_CHILD(emitter, Model);			break;
				case NodeTypeConst_EmitterStripe:			BM3_UPDATE_CHILD(emitter, Stripe);			break;
				case NodeTypeConst_FieldGravity:			break;
				case NodeTypeConst_FieldWind:				break;
				case NodeTypeConst_FieldVortex:				break;
				case NodeTypeConst_FieldRadialpower:		break;
				case NodeTypeConst_FieldDeceleration:		break;
				case NodeTypeConst_FieldCollision:			break;
				case NodeTypeConst_FieldGoal:				break;
				case NodeTypeConst_ObjectReference:			BM3_UPDATE_CHILD(object, Reference);		break;
				default:							ML_ASSERT(false);	BM3_ASSUME(0);					break;
			}
		}
		#undef BM3_UPDATE_CHILD
	}
}

template<BM3_CONTROLBASE_TEMPLATE_TEMPLATE_ARG>
void	Base<BM3_CONTROLBASE_TEMPLATE_ARG>::UpdateLife(UpdateInfo &info){

	ML_UPDATE_TIMER;
	active_count_per_frame_	= 0;

	if(active_node_count_==max_node_count_){
		int		**active_node	= &active_node_index_;
		active_count_per_frame_ += UpdateLife(info, current_node_pointer_,	active_node_count_,		&active_node[0]);
		active_count_per_frame_ += UpdateLife(info, 0,						current_node_pointer_,	&active_node[active_count_per_frame_]);
	}
	else{
		active_count_per_frame_ += UpdateLife(info, 0					,	active_node_count_,		&active_node_index_);
	}
}
template<BM3_CONTROLBASE_TEMPLATE_TEMPLATE_ARG>
int	Base<BM3_CONTROLBASE_TEMPLATE_ARG>::UpdateLife(UpdateInfo &info, int begin, int end, int **active_node_index){
	ML_UPDATE_TIMER;

	ControlParam<LifePolicy>	*control		= &control_param_array_[begin];
	int							*active_node	= *active_node_index;
	int							active_count	= 0;

	for( int i=begin ; i!=end ; ++i, ++control){

		control->UpdateLife(info);

		// 子供の寿命を見ている場合があるため更新し続ける
//		if(control->IsActive())
		{
			*active_node		= i;

			active_node++;
			active_count++;
		}
	}

	return active_count;
}


template<BM3_CONTROLBASE_TEMPLATE_TEMPLATE_ARG>
void	Base<BM3_CONTROLBASE_TEMPLATE_ARG>::RawSpuUpdateChild(UpdateInfo &info){
	if(child_buffer_){
		ML_UPDATE_TIMER;

		const typename ResourceType::ChildArrayType_	&child_array	= Resource().ChildArray();
		int												child_count		= ml::minimum(Resource().ChildArray().Count(), BM3_MAX_CHILD_NODE_COUNT);

		#define BM3_UPDATE_CHILD(NAMESPACE, NODE_TYPE)	BM3_NODE_CALL_METHOD(NAMESPACE, NODE_TYPE, RawSpuUpdate(info));

		for(int i = 0;i != child_count;++ i){
			ml::ui8*	buffer	= ml::offset_pointer<ml::ui8*>(child_buffer_, (int)child_byte_size_offset_array_[i]);
			switch(child_array.Child(i).Basic_Meta_NodeType()){
				case NodeTypeConst_ControlNull:				BM3_UPDATE_CHILD(control, Null);			break;
				case NodeTypeConst_EmitterQuad:				BM3_UPDATE_CHILD(emitter, Quad);			break;
				case NodeTypeConst_EmitterParticle:			BM3_UPDATE_CHILD(emitter, Particle);		break;
				case NodeTypeConst_EmitterSimpleParticle:	BM3_UPDATE_CHILD(emitter, SimpleParticle);	break;
				case NodeTypeConst_EmitterMassParticle:		BM3_NODE_CALL_METHOD(emitter, MassParticle, Update(info));	break;
				case NodeTypeConst_EmitterModel:			BM3_UPDATE_CHILD(emitter, Model);			break;
				case NodeTypeConst_EmitterStripe:			BM3_UPDATE_CHILD(emitter, Stripe);			break;
				case NodeTypeConst_FieldGravity:			break;
				case NodeTypeConst_FieldWind:				break;
				case NodeTypeConst_FieldVortex:				break;
				case NodeTypeConst_FieldRadialpower:		break;
				case NodeTypeConst_FieldDeceleration:		break;
				case NodeTypeConst_FieldCollision:			break;
				case NodeTypeConst_FieldGoal:				break;
				case NodeTypeConst_ObjectReference:			BM3_UPDATE_CHILD(object, Reference);		break;
				default:							ML_ASSERT(false);	BM3_ASSUME(0);					break;
			}
		}
		#undef BM3_UPDATE_CHILD
	}
}

template<BM3_CONTROLBASE_TEMPLATE_TEMPLATE_ARG>
void	Base<BM3_CONTROLBASE_TEMPLATE_ARG>::MakeUpdateList(UpdateInfo &info){
	if(IsRoot()){
		MakeChildUpdateList(info);
	}
	else{
		if(is_expired_==false){
			//Task	*task = bm3::RawSpuManager().instance().NextTask();
			//task->type_					= NodeTypeConst_ControlNull;
			//task->resource_				= (int)res_;
			//task->active_count_			= active_node_count_;
			//task->param_buffer_			= (int)control_param_array_;
			//task->active_index_array_	= (int)active_node_index_;
			//task->update_rate_			= info.GetUpdateFrame();
			//bm3::RawSpuManager().instance().EnqueueTask();
#if SPURS
			info.JobInitialize();
			info.CreateJobQueue();
			info.PushUserData(NodeTypeConst_ControlNull);
			info.PushUserData((int)res_);
			info.PushUserData(active_node_count_);
			info.PushUserData((int)control_param_array_);
			info.PushUserData((int)active_node_index_);
			info.PushUserData(info.GetUpdateFrame());
			info.BootSpurs();
			MakeChildUpdateList(info);
#endif
		}
	}
}
template<BM3_CONTROLBASE_TEMPLATE_TEMPLATE_ARG>
void	Base<BM3_CONTROLBASE_TEMPLATE_ARG>::MakeChildUpdateList(UpdateInfo &info){
	if(child_buffer_){

		const typename ResourceType::ChildArrayType_	&child_array	= Resource().ChildArray();
		int												child_count		= ml::minimum(Resource().ChildArray().Count(), BM3_MAX_CHILD_NODE_COUNT);

		#define BM3_UPDATE_CHILD(NAMESPACE, NODE_TYPE)	BM3_NODE_CALL_METHOD(NAMESPACE, NODE_TYPE, MakeUpdateList(info));

		for(int i = 0;i != child_count;++ i){
			ml::ui8*	buffer	= ml::offset_pointer<ml::ui8*>(child_buffer_, (int)child_byte_size_offset_array_[i]);
			switch(child_array.Child(i).Basic_Meta_NodeType()){
				case NodeTypeConst_ControlNull:				BM3_UPDATE_CHILD(control, Null);			break;
				case NodeTypeConst_EmitterQuad:				BM3_UPDATE_CHILD(emitter, Quad);			break;
				case NodeTypeConst_EmitterParticle:			BM3_UPDATE_CHILD(emitter, Particle);		break;
				case NodeTypeConst_EmitterSimpleParticle:	BM3_UPDATE_CHILD(emitter, SimpleParticle);	break;
				case NodeTypeConst_EmitterMassParticle:		break;
				case NodeTypeConst_EmitterModel:			BM3_UPDATE_CHILD(emitter, Model);			break;
				case NodeTypeConst_EmitterStripe:			BM3_UPDATE_CHILD(emitter, Stripe);			break;
				case NodeTypeConst_FieldGravity:			break;
				case NodeTypeConst_FieldWind:				break;
				case NodeTypeConst_FieldVortex:				break;
				case NodeTypeConst_FieldRadialpower:		break;
				case NodeTypeConst_FieldDeceleration:		break;
				case NodeTypeConst_FieldCollision:			break;
				case NodeTypeConst_FieldGoal:				break;
				case NodeTypeConst_ObjectReference:			BM3_UPDATE_CHILD(object, Reference);		break;
				default:							ML_ASSERT(false);	BM3_ASSUME(0);					break;
			}
		}
		#undef BM3_UPDATE_CHILD
	}
}

template<BM3_CONTROLBASE_TEMPLATE_TEMPLATE_ARG>
void	Base<BM3_CONTROLBASE_TEMPLATE_ARG>::MakeVertexList(DrawInfo &info){
	if(IsRoot()){
		MakeChildVertexList(info);
	}
	else{
		if(is_expired_==false){
			VertexTask	*task = bm3::RawSpuManager().instance().NextVertexTask();
			task->type_					= NodeTypeConst_ControlNull;
			task->resource_				= (int)res_;
			task->active_count_			= active_node_count_;
			task->param_buffer_			= (int)control_param_array_;
			task->active_index_array_	= (int)active_node_index_;
			bm3::RawSpuManager().instance().EnqueueVertexTask();
			MakeChildVertexList(info);
		}
	}
}
template<BM3_CONTROLBASE_TEMPLATE_TEMPLATE_ARG>
void	Base<BM3_CONTROLBASE_TEMPLATE_ARG>::MakeChildVertexList(DrawInfo &info){
	if(child_buffer_){

		const typename ResourceType::ChildArrayType_	&child_array	= Resource().ChildArray();
		int												child_count		= ml::minimum(Resource().ChildArray().Count(), BM3_MAX_CHILD_NODE_COUNT);

		#define BM3_MAKEVERTEX_CHILD(NAMESPACE, NODE_TYPE)	BM3_NODE_CALL_METHOD(NAMESPACE, NODE_TYPE, MakeVertexList(info));

		for(int i = 0;i != child_count;++ i){
			ml::ui8*	buffer	= ml::offset_pointer<ml::ui8*>(child_buffer_, (int)child_byte_size_offset_array_[i]);
			switch(child_array.Child(i).Basic_Meta_NodeType()){
				case NodeTypeConst_ControlNull:				BM3_MAKEVERTEX_CHILD(control, Null);			break;
				case NodeTypeConst_EmitterQuad:				BM3_MAKEVERTEX_CHILD(emitter, Quad);			break;
				case NodeTypeConst_EmitterParticle:			BM3_MAKEVERTEX_CHILD(emitter, Particle);		break;
				case NodeTypeConst_EmitterSimpleParticle:	BM3_MAKEVERTEX_CHILD(emitter, SimpleParticle);	break;
				case NodeTypeConst_EmitterMassParticle:		BM3_MAKEVERTEX_CHILD(emitter, MassParticle);	break;
				case NodeTypeConst_EmitterModel:			BM3_MAKEVERTEX_CHILD(emitter, Model);			break;
				case NodeTypeConst_EmitterStripe:			BM3_MAKEVERTEX_CHILD(emitter, Stripe);			break;
				case NodeTypeConst_FieldGravity:			break;
				case NodeTypeConst_FieldWind:				break;
				case NodeTypeConst_FieldVortex:				break;
				case NodeTypeConst_FieldRadialpower:		break;
				case NodeTypeConst_FieldDeceleration:		break;
				case NodeTypeConst_FieldCollision:			break;
				case NodeTypeConst_FieldGoal:				break;
				case NodeTypeConst_ObjectReference:			BM3_MAKEVERTEX_CHILD(object, Reference);		break;
				default:							ML_ASSERT(false);	BM3_ASSUME(0);					break;
			}
		}
		#undef BM3_MAKEVERTEX_CHILD
	}
}

template<BM3_CONTROLBASE_TEMPLATE_TEMPLATE_ARG>
void	Base<BM3_CONTROLBASE_TEMPLATE_ARG>::RawSpuMakeVertex(DrawInfo &info){
	ML_MAKEVERTEX_TIMER;
	ML_ASSERT(IsInitialized());
	
	// 依存関係の計算
	// フィールド
	// 親のマトリックス
	
	// リストの作成
	RawSpuManager::instance().BeginTaskManager();
	MakeChildVertexList(info);
	RawSpuManager::instance().EndTaskManager();
}
#endif	

}	// namespace control
}	// namespace obj
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#undef	BM3_CONTROLBASE_TEMPLATE_TEMPLATE_ARG
#undef	BM3_CONTROLBASE_TEMPLATE_ARG
#undef	BM3_SIZEOF_NODE
#undef	BM3_NODE_CALL_METHOD
#undef	BM3_NODE_CALL_METHOD_FORWARD_BUFFER
#undef	BM3_NODE_CALL_CONST_METHOD

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_OBJECT_CONTROL_BM3_OBJ_CONTROLBASE_INL_H
