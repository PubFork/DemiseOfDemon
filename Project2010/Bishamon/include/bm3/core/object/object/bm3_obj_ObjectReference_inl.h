#ifndef BM3_SDK_INC_BM3_CORE_OBJECT_OBJECT_BM3_OBJ_OBJECTREFERENCE_INL_H
#define BM3_SDK_INC_BM3_CORE_OBJECT_OBJECT_BM3_OBJ_OBJECTREFERENCE_INL_H

#include "../../bm3_MemoryManager.h"
#include "../../bm3_Const.h"
#include "../../bm3_CoreType.h"
#include "../control/bm3_obj_ControlNull.h"
#include "../emitter/bm3_obj_EmitterParticle.h"
#include "../emitter/bm3_obj_EmitterQuad.h"
#include "../emitter/bm3_obj_EmitterSimpleParticle.h"
#include "../emitter/bm3_obj_EmitterMassParticle.h"
#include "../emitter/bm3_obj_EmitterModel.h"
#include "../emitter/bm3_obj_EmitterStripe.h"
#include "../../field/bm3_field.h"
#include <ml/type/ml_static_align.h>
#include <ml/type/ml_basic.h>
#include <ml/utility/ml_assert.h>
#include <ml/utility/ml_simpletimer.h>
#if defined(BM3_TARGET_IDE)
#include "../../resource/bm3_res_IdeNode.h"
#endif
#include "../../resource/bm3_res_Root.h"

#if defined(BM3_TARGET_IDE)
	#define	BM3_CHILD_RESOURCE_TYPE(NODE_TYPE)	res::IdeNode
#endif
#if defined(BM3_TARGET_EMBEDDED)
	#define	BM3_CHILD_RESOURCE_TYPE(NODE_TYPE)	res::NODE_TYPE
#endif

#define	BM3_OBJECTBASE_TEMPLATE_TEMPLATE_ARG	typename ResourceType, typename BaseClass, typename LifePolicy, typename ChildGenerateTimingPolicy
#define	BM3_OBJECTBASE_TEMPLATE_ARG			ResourceType, BaseClass, LifePolicy, ChildGenerateTimingPolicy

#define	BM3_OBJECTBASE_NODE_TYPE(NAMESPACE, NODE_TYPE)	NAMESPACE::NODE_TYPE<BM3_CHILD_RESOURCE_TYPE(NODE_TYPE)>

#define	BM3_SIZEOF_NODE(NAMESPACE, NODE_TYPE)	\
	ml::static_align<sizeof(ml::ptr_size_ui), sizeof(BM3_OBJECTBASE_NODE_TYPE(NAMESPACE, NODE_TYPE))>::value

#define	BM3_NODE_CALL_METHOD(NAMESPACE, NODE_TYPE, METHOD)	\
	reinterpret_cast<BM3_OBJECTBASE_NODE_TYPE(NAMESPACE, NODE_TYPE) *>(buffer)->METHOD

#define	BM3_NODE_CALL_CONST_METHOD(NAMESPACE, NODE_TYPE, METHOD)	\
	reinterpret_cast<const BM3_OBJECTBASE_NODE_TYPE(NAMESPACE, NODE_TYPE) *>(buffer)->METHOD


namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE
namespace obj{
namespace object{


template<BM3_OBJECTBASE_TEMPLATE_TEMPLATE_ARG>
Reference<BM3_OBJECTBASE_TEMPLATE_ARG>::Reference() :
	res_(NULL),
	root_(NULL),
	child_all_byte_size_(0),
	ref_handle_(NULL),
	child_array_(NULL),
	child_count_(0)
{
}

template<BM3_OBJECTBASE_TEMPLATE_TEMPLATE_ARG>
Reference<BM3_OBJECTBASE_TEMPLATE_ARG>::~Reference(){

	Finalize();
}

template<BM3_OBJECTBASE_TEMPLATE_TEMPLATE_ARG>
bmsln::HandleBase* Reference<BM3_OBJECTBASE_TEMPLATE_ARG>::RefHandle(const ResourceType &res){	
	if(ref_handle_==NULL){
		ref_handle_	= ResourceManager::instance().BmslnHandleFromName(res.Basic_Basic_ReferenceNodeName());

#ifdef ML_ASSERT_ENABLE
		if(ref_handle_){
#ifdef BM3_TARGET_IDE
			const BM3_CHILD_RESOURCE_TYPE(Root) &root = *reinterpret_cast<const BM3_CHILD_RESOURCE_TYPE(Root)*>(ref_handle_->Image());
			const typename ResourceType::ChildArrayType_ &child_array		= root.ChildArray().Child(0).ChildArray();			// ソリューションの下にルートがいるのでそれを飛ばす
#else
			const BM3_CHILD_RESOURCE_TYPE(Root) &root = *reinterpret_cast<const BM3_CHILD_RESOURCE_TYPE(Root)*>(fileformat::bmb::RootNodeRes(ref_handle_->Image()));
			const typename ResourceType::ChildArrayType_ &child_array		= root.ChildArray();
#endif
			ML_ASSERT(IsValidedRefData(child_array));
		}
#endif
		
	}
	return ref_handle_;
}

template<BM3_OBJECTBASE_TEMPLATE_TEMPLATE_ARG>
bool Reference<BM3_OBJECTBASE_TEMPLATE_ARG>::IsValidedRefData(const typename ResourceType::ChildArrayType_ &child_array){

	bool is_valided = true;
	int child_count		= ml::minimum(child_array.Count(), BM3_MAX_CHILD_NODE_COUNT);
	for(int i = 0;i != child_count;++ i){
		switch(child_array.Child(i).Basic_Meta_NodeType()){
#ifdef BM3_TARGET_IDE
			case NodeTypeConst_ControlNull:					is_valided = IsValidedRefData(reinterpret_cast<const res::IdeNode&>(child_array.Child(i)).ChildArray());	break;
#else
			case NodeTypeConst_ControlNull:					is_valided = IsValidedRefData(reinterpret_cast<const res::Null&>(child_array.Child(i)).ChildArray());	break;
#endif
			case NodeTypeConst_EmitterQuad:					break;
			case NodeTypeConst_EmitterParticle:				break;
			case NodeTypeConst_EmitterSimpleParticle:		break;
			case NodeTypeConst_EmitterMassParticle:			break;
			case NodeTypeConst_EmitterModel:				break;
			case NodeTypeConst_EmitterStripe:				break;
			case NodeTypeConst_FieldGravity:				break;
			case NodeTypeConst_FieldWind:					break;
			case NodeTypeConst_FieldVortex:					break;
			case NodeTypeConst_FieldRadialpower:			break;
			case NodeTypeConst_FieldDeceleration:			break;
			case NodeTypeConst_FieldCollision:				break;
			case NodeTypeConst_FieldGoal:					break;
			case NodeTypeConst_ObjectReference:			ML_ASSERT(false);	break;	// 参照ノードから参照ノードを参照できない（仕様）
			default:							ML_ASSERT(false);	BM3_ASSUME(0);																break;
		}
		if(is_valided==false)	break;
	}

	return is_valided;
}

template<BM3_OBJECTBASE_TEMPLATE_TEMPLATE_ARG>
bool	Reference<BM3_OBJECTBASE_TEMPLATE_ARG>::Allocate(const ResourceType &res){
	MakeChildByteSize(res);
	return AllocateChild(res);
}

template<BM3_OBJECTBASE_TEMPLATE_TEMPLATE_ARG>
bool	Reference<BM3_OBJECTBASE_TEMPLATE_ARG>::AllocateChild(const ResourceType & /*res*/){

	// 子供のポインタを持っておきたい
	child_buffer_			= reinterpret_cast<ml::ui8*>(					bm3::MemoryManager::instance().General_Alloc(child_all_byte_size_));

	if(child_buffer_){

//		const typename ResourceType::ChildArrayType_	&child_array	= res.ChildArray();
//		int												child_count		= ml::minimum(res.ChildArray().Count(), BM3_MAX_CHILD_NODE_COUNT);

		#define BM3_ALLOCATE_CHILD(NAMESPACE, NODE_TYPE)																								\
			new(buffer)	BM3_OBJECTBASE_NODE_TYPE(NAMESPACE, NODE_TYPE);																				\
			BM3_NODE_CALL_METHOD(NAMESPACE, NODE_TYPE, Allocate(reinterpret_cast<const BM3_CHILD_RESOURCE_TYPE(NODE_TYPE) &>(child_array_->Child(i))));

		#define BM3_ALLOCATE_EMITTER_CHILD(NAMESPACE, NODE_TYPE)			\
			new(buffer)	BM3_OBJECTBASE_NODE_TYPE(NAMESPACE, NODE_TYPE);	

		for(int i = 0;i != child_count_;++ i){
			ml::ui8*	buffer	= ml::offset_pointer<ml::ui8*>(child_buffer_, (int)child_byte_size_offset_array_[i]);
			switch(child_array_->Child(i).Basic_Meta_NodeType()){
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
				case NodeTypeConst_ObjectReference:			break;
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

template<BM3_OBJECTBASE_TEMPLATE_TEMPLATE_ARG>
int	Reference<BM3_OBJECTBASE_TEMPLATE_ARG>::FixGenCount(const ResourceType &res, InitialAllocInfo &info, const float gen_scale){
	res_		= &res;

//	int max_node_count	= (int)(info.ParentMaxGenCount() * ChildGenerateTimingPolicy::CalcTotalGenerateNodeCount(res) * gen_scale);

	return FixChildGenCount(res, info.ParentMaxGenCount(), gen_scale);
}

template<BM3_OBJECTBASE_TEMPLATE_TEMPLATE_ARG>
int	Reference<BM3_OBJECTBASE_TEMPLATE_ARG>::FixChildGenCount(const ResourceType & /*res*/, int node_count, const float gen_scale){

	int	total_lifespan	= 0;
	if(child_buffer_){

//		const typename ResourceType::ChildArrayType_	&child_array	= res.ChildArray();
//		int												child_count		= ml::minimum(res.ChildArray().Count(), BM3_MAX_CHILD_NODE_COUNT);
		InitialAllocInfo	info(node_count);

		#define BM3_FIX_GEN_COUNT(NAMESPACE, NODE_TYPE)																											\
		total_lifespan	= ml::maximum(total_lifespan, BM3_NODE_CALL_METHOD(NAMESPACE, NODE_TYPE, FixGenCount(reinterpret_cast<const BM3_CHILD_RESOURCE_TYPE(NODE_TYPE) &>(child_array_->Child(i)), info, gen_scale)));

		for(int i = 0;i != child_count_;++ i){
			ml::ui8*	buffer	= ml::offset_pointer<ml::ui8*>(child_buffer_, (int)child_byte_size_offset_array_[i]);
			switch(child_array_->Child(i).Basic_Meta_NodeType()){
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
				case NodeTypeConst_ObjectReference:			break;
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
template<BM3_OBJECTBASE_TEMPLATE_TEMPLATE_ARG>
bool	Reference<BM3_OBJECTBASE_TEMPLATE_ARG>::AllocateParam(){
	
	if(AllocateChildParam()){
		return true;
	}
	else{
		return false;
	}
}

template<BM3_OBJECTBASE_TEMPLATE_TEMPLATE_ARG>
bool	Reference<BM3_OBJECTBASE_TEMPLATE_ARG>::AllocateChildParam(){

	if(child_buffer_){
		bool result = true;
//		const typename ResourceType::ChildArrayType_	&child_array	= Resource().ChildArray();
//		int												child_count		= ml::minimum(Resource().ChildArray().Count(), BM3_MAX_CHILD_NODE_COUNT);

		#define BM3_ALLOC_PARAM(NAMESPACE, NODE_TYPE)		\
			result = BM3_NODE_CALL_METHOD(NAMESPACE, NODE_TYPE, AllocateParam());

		for(int i = 0;i != child_count_;++ i){
			ml::ui8*	buffer	= ml::offset_pointer<ml::ui8*>(child_buffer_, (int)child_byte_size_offset_array_[i]);
			switch(child_array_->Child(i).Basic_Meta_NodeType()){
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
				case NodeTypeConst_ObjectReference:			break;
				default:							ML_ASSERT(false);	BM3_ASSUME(0);					break;
			}
			if(!result)return false;
		}
		#undef BM3_ALLOC_PARAM
	}
	
	return true;
}

template<BM3_OBJECTBASE_TEMPLATE_TEMPLATE_ARG>
int		Reference<BM3_OBJECTBASE_TEMPLATE_ARG>::CalcEmitterCount() const{

	int count = 0;
	if(child_buffer_){
//		const typename ResourceType::ChildArrayType_	&child_array	= Resource().ChildArray();
//		int												child_count		= ml::minimum(Resource().ChildArray().Count(), BM3_MAX_CHILD_NODE_COUNT);

		#define BM3_CALC_EMITTERCOUNT(NAMESPACE, NODE_TYPE)		\
			count += BM3_NODE_CALL_METHOD(NAMESPACE, NODE_TYPE, CalcEmitterCount());

		for(int i = 0;i != child_count_;++ i){
			ml::ui8*	buffer	= ml::offset_pointer<ml::ui8*>(child_buffer_, (int)child_byte_size_offset_array_[i]);
			switch(child_array_->Child(i).Basic_Meta_NodeType()){
				case NodeTypeConst_ControlNull:				BM3_CALC_EMITTERCOUNT(control, Null);			break;
				case NodeTypeConst_EmitterQuad:				BM3_CALC_EMITTERCOUNT(emitter, Quad);			break;
				case NodeTypeConst_EmitterParticle:			BM3_CALC_EMITTERCOUNT(emitter, Particle);		break;
				case NodeTypeConst_EmitterSimpleParticle:	BM3_CALC_EMITTERCOUNT(emitter, SimpleParticle);	break;
				case NodeTypeConst_EmitterMassParticle:		BM3_CALC_EMITTERCOUNT(emitter, MassParticle);	break;
				case NodeTypeConst_EmitterModel:			BM3_CALC_EMITTERCOUNT(emitter, Model);			break;
				case NodeTypeConst_EmitterStripe:			BM3_CALC_EMITTERCOUNT(emitter, Stripe);			break;
				case NodeTypeConst_FieldGravity:			break;
				case NodeTypeConst_FieldWind:				break;
				case NodeTypeConst_FieldVortex:				break;
				case NodeTypeConst_FieldRadialpower:		break;
				case NodeTypeConst_FieldDeceleration:		break;
				case NodeTypeConst_FieldCollision:			break;
				case NodeTypeConst_FieldGoal:				break;
				case NodeTypeConst_ObjectReference:			break;
				default:							ML_ASSERT(false);	BM3_ASSUME(0);					break;
			}
		}
		#undef BM3_CALC_EMITTERCOUNT
	}

	return count;
}

template<BM3_OBJECTBASE_TEMPLATE_TEMPLATE_ARG>
void		Reference<BM3_OBJECTBASE_TEMPLATE_ARG>::SetResEmitterInfo(InitializeInfo &initialize_info) {

	if(child_buffer_){
//		const typename ResourceType::ChildArrayType_	&child_array	= Resource().ChildArray();
//		int												child_count		= ml::minimum(Resource().ChildArray().Count(), BM3_MAX_CHILD_NODE_COUNT);

		#define BM3_SET_EMITTER_INFO(NAMESPACE, NODE_TYPE)		\
			BM3_NODE_CALL_METHOD(NAMESPACE, NODE_TYPE, SetResEmitterInfo(initialize_info));

		for(int i = 0;i != child_count_;++ i){
			ml::ui8*	buffer	= ml::offset_pointer<ml::ui8*>(child_buffer_, (int)child_byte_size_offset_array_[i]);
			switch(child_array_->Child(i).Basic_Meta_NodeType()){
				case NodeTypeConst_ControlNull:				BM3_SET_EMITTER_INFO(control, Null);			break;
				case NodeTypeConst_EmitterQuad:				BM3_SET_EMITTER_INFO(emitter, Quad);			break;
				case NodeTypeConst_EmitterParticle:			BM3_SET_EMITTER_INFO(emitter, Particle);		break;
				case NodeTypeConst_EmitterSimpleParticle:	BM3_SET_EMITTER_INFO(emitter, SimpleParticle);	break;
				case NodeTypeConst_EmitterMassParticle:		BM3_SET_EMITTER_INFO(emitter, MassParticle);	break;
				case NodeTypeConst_EmitterModel:			BM3_SET_EMITTER_INFO(emitter, Model);			break;
				case NodeTypeConst_EmitterStripe:			BM3_SET_EMITTER_INFO(emitter, Stripe);			break;
				case NodeTypeConst_FieldGravity:			break;
				case NodeTypeConst_FieldWind:				break;
				case NodeTypeConst_FieldVortex:				break;
				case NodeTypeConst_FieldRadialpower:		break;
				case NodeTypeConst_FieldDeceleration:		break;
				case NodeTypeConst_FieldCollision:			break;
				case NodeTypeConst_FieldGoal:				break;
				case NodeTypeConst_ObjectReference:			break;
				default:							ML_ASSERT(false);	BM3_ASSUME(0);					break;
			}
		}
		#undef BM3_SET_EMITTER_INFO
	}
}

template<BM3_OBJECTBASE_TEMPLATE_TEMPLATE_ARG>
void	Reference<BM3_OBJECTBASE_TEMPLATE_ARG>::MakeChildByteSize(const ResourceType &res){

	bmsln::HandleBase *handle = RefHandle(res);
	if(handle){
#ifdef BM3_TARGET_IDE
		const BM3_CHILD_RESOURCE_TYPE(Root) &root = *reinterpret_cast<const BM3_CHILD_RESOURCE_TYPE(Root)*>(handle->Image());
		// ソリューションの下にルートがいるのでそれを飛ばす
		child_array_		= &root.ChildArray().Child(0).ChildArray();
		child_count_		= ml::minimum(root.ChildArray().Child(0).ChildArray().Count(), BM3_MAX_CHILD_NODE_COUNT);
#else
		const BM3_CHILD_RESOURCE_TYPE(Root) &root = *reinterpret_cast<const BM3_CHILD_RESOURCE_TYPE(Root)*>(fileformat::bmb::RootNodeRes(handle->Image()));
		child_array_		= &root.ChildArray();
		child_count_		= ml::minimum(root.ChildArray().Count(), BM3_MAX_CHILD_NODE_COUNT);
#endif


		child_all_byte_size_	= 0;

		{
//			const typename ResourceType::ChildArrayType_	&child_array	= root.ChildArray();
//			int												child_count		= ml::minimum(root.ChildArray().Count(), BM3_MAX_CHILD_NODE_COUNT);

			for(int i = 0;i != child_count_;++ i){
				switch(child_array_->Child(i).Basic_Meta_NodeType()){
					case NodeTypeConst_ControlNull:				child_byte_size_offset_array_[i] = (ChildByteSizeArrayElemType)child_all_byte_size_;	child_all_byte_size_ += BM3_SIZEOF_NODE(control, Null);				break;
					case NodeTypeConst_EmitterQuad:				child_byte_size_offset_array_[i] = (ChildByteSizeArrayElemType)child_all_byte_size_;	child_all_byte_size_ += BM3_SIZEOF_NODE(emitter, Quad);				break;
					case NodeTypeConst_EmitterParticle:			child_byte_size_offset_array_[i] = (ChildByteSizeArrayElemType)child_all_byte_size_;	child_all_byte_size_ += BM3_SIZEOF_NODE(emitter, Particle);			break;
					case NodeTypeConst_EmitterSimpleParticle:	child_byte_size_offset_array_[i] = (ChildByteSizeArrayElemType)child_all_byte_size_;	child_all_byte_size_ += BM3_SIZEOF_NODE(emitter, SimpleParticle);	break;
					case NodeTypeConst_EmitterMassParticle:		child_byte_size_offset_array_[i] = (ChildByteSizeArrayElemType)child_all_byte_size_;	child_all_byte_size_ += BM3_SIZEOF_NODE(emitter, MassParticle);		break;
					case NodeTypeConst_EmitterModel:			child_byte_size_offset_array_[i] = (ChildByteSizeArrayElemType)child_all_byte_size_;	child_all_byte_size_ += BM3_SIZEOF_NODE(emitter, Model);			break;
					case NodeTypeConst_EmitterStripe:			child_byte_size_offset_array_[i] = (ChildByteSizeArrayElemType)child_all_byte_size_;	child_all_byte_size_ += BM3_SIZEOF_NODE(emitter, Stripe);			break;
					case NodeTypeConst_FieldGravity:																									break;
					case NodeTypeConst_FieldWind:																										break;
					case NodeTypeConst_FieldVortex:																										break;
					case NodeTypeConst_FieldRadialpower:																								break;
					case NodeTypeConst_FieldDeceleration:																								break;
					case NodeTypeConst_FieldCollision:																									break;
					case NodeTypeConst_FieldGoal:																										break;
					case NodeTypeConst_ObjectReference:			ML_ASSERT(false);	break;	// 参照ノードから参照ノードを参照できない（仕様）
					default:							ML_ASSERT(false);	BM3_ASSUME(0);																break;
				}
			}
		}
	}
}


template<BM3_OBJECTBASE_TEMPLATE_TEMPLATE_ARG>
void	Reference<BM3_OBJECTBASE_TEMPLATE_ARG>::FreeChild(){

	if(child_buffer_){

//		const typename ResourceType::ChildArrayType_	&child_array	= Resource().ChildArray();
//		int												child_count		= ml::minimum(Resource().ChildArray().Count(), BM3_MAX_CHILD_NODE_COUNT);

		for(int i = 0;i != child_count_;++ i){
			ml::ui8*	buffer	= ml::offset_pointer<ml::ui8*>(child_buffer_, (int)child_byte_size_offset_array_[i]);
			switch(child_array_->Child(i).Basic_Meta_NodeType()){
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
				case NodeTypeConst_ObjectReference:			break;
				default:							ML_ASSERT(false);	BM3_ASSUME(0);											break;
			}
		}
	}

	bm3::MemoryManager::instance().General_Free(child_buffer_);
	child_buffer_			= NULL;
}

template<BM3_OBJECTBASE_TEMPLATE_TEMPLATE_ARG>
void	Reference<BM3_OBJECTBASE_TEMPLATE_ARG>::Initialize(const ResourceType & /*res*/, InitializeInfo &initialize_info){

	root_		= initialize_info.Root();

	if(child_buffer_){

//		const typename ResourceType::ChildArrayType_	&child_array	= Resource().ChildArray();
//		int												child_count		= ml::minimum(Resource().ChildArray().Count(), BM3_MAX_CHILD_NODE_COUNT);

		InitializeInfo	info(&Root(), initialize_info.GenScale());
		
		#define BM3_ACTIVATE_CHILD(NAMESPACE, NODE_TYPE)												\
			BM3_NODE_CALL_METHOD(																		\
				NAMESPACE, NODE_TYPE,																	\
				Initialize(																				\
					reinterpret_cast<const BM3_CHILD_RESOURCE_TYPE(NODE_TYPE) &>(child_array_->Child(i)),	\
					info)																				\
				);

		for(int i = 0;i != child_count_;++ i){
			ml::ui8*	buffer	= ml::offset_pointer<ml::ui8*>(child_buffer_, (int)child_byte_size_offset_array_[i]);
			switch(child_array_->Child(i).Basic_Meta_NodeType()){
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
				case NodeTypeConst_ObjectReference:			break;
				default:							ML_ASSERT(false);	BM3_ASSUME(0);																																	break;
			}
		}
		#undef BM3_ACTIVATE_CHILD
	}
}

template<BM3_OBJECTBASE_TEMPLATE_TEMPLATE_ARG>
void	Reference<BM3_OBJECTBASE_TEMPLATE_ARG>::Finalize(){

	if(IsInitialized()){

		// 注意：デストラクタは呼ばれないことがあるので。
		//       メモリーを確保している物はこの中で明示的に解放する必要がある
		FreeChild();

		res_	= NULL;
		root_	= NULL;
	}
	
}

template<BM3_OBJECTBASE_TEMPLATE_TEMPLATE_ARG>
void	Reference<BM3_OBJECTBASE_TEMPLATE_ARG>::SetMaxParticleCount(){
	if(child_buffer_){

//		const typename ResourceType::ChildArrayType_	&child_array	= Resource().ChildArray();
//		int												child_count		= ml::minimum(Resource().ChildArray().Count(), BM3_MAX_CHILD_NODE_COUNT);

		#define BM3_MAXPARTICLE_COUNT(NAMESPACE, NODE_TYPE)			\
			BM3_NODE_CALL_METHOD(NAMESPACE, NODE_TYPE, SetMaxParticleCount());

		for(int i = 0;i != child_count_;++ i){
			ml::ui8*	buffer	= ml::offset_pointer<ml::ui8*>(child_buffer_, (int)child_byte_size_offset_array_[i]);
			switch(child_array_->Child(i).Basic_Meta_NodeType()){
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
				case NodeTypeConst_ObjectReference:			break;
				default:							ML_ASSERT(false);	BM3_ASSUME(0);						break;
			}
		}
		#undef BM3_ACTIVATE_CHILD
	}

}

template<BM3_OBJECTBASE_TEMPLATE_TEMPLATE_ARG>
template<typename UpdateInfo>
void	Reference<BM3_OBJECTBASE_TEMPLATE_ARG>::Update(UpdateInfo &info){

	ML_ASSERT(IsInitialized());

	UpdateChild(info);

	// 非表示ではアクティブなパーティクル数を0としてmakevertex,drawで処理しないようにする
	if(Resource().Basic_Meta_Display()==false){
		ResetActiveCount();
	}
}

template<BM3_OBJECTBASE_TEMPLATE_TEMPLATE_ARG>
void	Reference<BM3_OBJECTBASE_TEMPLATE_ARG>::ResetActiveCount(){
	if(child_buffer_){

//		const typename ResourceType::ChildArrayType_	&child_array	= Resource().ChildArray();
//		int												child_count		= ml::minimum(Resource().ChildArray().Count(), BM3_MAX_CHILD_NODE_COUNT);

		#define BM3_ACTIVE_COUNT(NAMESPACE, NODE_TYPE)			\
			BM3_NODE_CALL_METHOD(NAMESPACE, NODE_TYPE, ResetActiveCount());


		for(int i = 0;i != child_count_;++ i){
			ml::ui8*	buffer	= ml::offset_pointer<ml::ui8*>(child_buffer_, (int)child_byte_size_offset_array_[i]);
			switch(child_array_->Child(i).Basic_Meta_NodeType()){
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
				case NodeTypeConst_ObjectReference:			break;
				default:							ML_ASSERT(false);	BM3_ASSUME(0);					break;
			}
		}
		#undef BM3_ACTIVE_COUNT
	}
}


template<BM3_OBJECTBASE_TEMPLATE_TEMPLATE_ARG>
void	Reference<BM3_OBJECTBASE_TEMPLATE_ARG>::ActivateChildGenerator(NodeBase *parent){
	if(child_buffer_){

//		const typename ResourceType::ChildArrayType_	&child_array	= Resource().ChildArray();
//		int												child_count		= ml::minimum(Resource().ChildArray().Count(), BM3_MAX_CHILD_NODE_COUNT);

		#define BM3_ACTIVATE_CHILD(NAMESPACE, NODE_TYPE)			\
			BM3_NODE_CALL_METHOD(NAMESPACE, NODE_TYPE, ActivateGenerator(parent));

		#define BM3_ACTIVATE_EMITTER_CHILD(NAMESPACE, NODE_TYPE)	\
			BM3_NODE_CALL_METHOD(NAMESPACE, NODE_TYPE, ActivateGenerator(parent));

		for(int i = 0;i != child_count_;++ i){
			ml::ui8*	buffer	= ml::offset_pointer<ml::ui8*>(child_buffer_, (int)child_byte_size_offset_array_[i]);
			switch(child_array_->Child(i).Basic_Meta_NodeType()){
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
				case NodeTypeConst_ObjectReference:			break;
				default:							ML_ASSERT(false);	BM3_ASSUME(0);								break;
			}
		}
		#undef BM3_ACTIVATE_CHILD
	}
}

template<BM3_OBJECTBASE_TEMPLATE_TEMPLATE_ARG>
void	Reference<BM3_OBJECTBASE_TEMPLATE_ARG>::UpdateChild(UpdateInfo &info){
	if(child_buffer_){
		ML_UPDATE_TIMER;

//		const typename ResourceType::ChildArrayType_	&child_array	= Resource().ChildArray();
//		int												child_count		= ml::minimum(Resource().ChildArray().Count(), BM3_MAX_CHILD_NODE_COUNT);

		#define BM3_UPDATE_CHILD(NAMESPACE, NODE_TYPE)	BM3_NODE_CALL_METHOD(NAMESPACE, NODE_TYPE, Update(info));

		for(int i = 0;i != child_count_;++ i){
			ml::ui8*	buffer	= ml::offset_pointer<ml::ui8*>(child_buffer_, (int)child_byte_size_offset_array_[i]);
			switch(child_array_->Child(i).Basic_Meta_NodeType()){
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
				case NodeTypeConst_ObjectReference:			break;
				default:							ML_ASSERT(false);	BM3_ASSUME(0);					break;
			}
		}
		#undef BM3_UPDATE_CHILD
	}
}

template<BM3_OBJECTBASE_TEMPLATE_TEMPLATE_ARG>
bool	Reference<BM3_OBJECTBASE_TEMPLATE_ARG>::Child_IsExpired() const{
	if(child_buffer_){

//		const typename ResourceType::ChildArrayType_	&child_array	= Resource().ChildArray();
//		int												child_count		= ml::minimum(Resource().ChildArray().Count(), BM3_MAX_CHILD_NODE_COUNT);

		#define BM3_EXPIRED_CHILD(NAMESPACE, NODE_TYPE)						\
			BM3_NODE_CALL_METHOD(NAMESPACE, NODE_TYPE, IsAllExpired())

		for(int i = 0;i != child_count_;++ i){
			ml::ui8*	buffer	= ml::offset_pointer<ml::ui8*>(child_buffer_, (int)child_byte_size_offset_array_[i]);
			switch(child_array_->Child(i).Basic_Meta_NodeType()){
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
				case NodeTypeConst_ObjectReference:			break;
				default:							ML_ASSERT(false);	BM3_ASSUME(0);													break;
			}
		}
		#undef BM3_EXPIRED_CHILD
	}

	return true;
}


template<BM3_OBJECTBASE_TEMPLATE_TEMPLATE_ARG>
void	Reference<BM3_OBJECTBASE_TEMPLATE_ARG>::MakeVertex(DrawInfo &info){

	ML_MAKEVERTEX_TIMER;
	ML_ASSERT(IsInitialized());

	MakeChildVertex(info);
}

template<BM3_OBJECTBASE_TEMPLATE_TEMPLATE_ARG>
void	Reference<BM3_OBJECTBASE_TEMPLATE_ARG>::MakeChildVertex(DrawInfo &info){
	if(child_buffer_){
		ML_MAKEVERTEX_TIMER;

//		const typename ResourceType::ChildArrayType_	&child_array	= Resource().ChildArray();
//		int												child_count		= ml::minimum(Resource().ChildArray().Count(), BM3_MAX_CHILD_NODE_COUNT);

		#define BM3_MAKEVERTEX_CHILD(NAMESPACE, NODE_TYPE)		\
			BM3_NODE_CALL_METHOD(NAMESPACE, NODE_TYPE, MakeVertex(info));

		for(int i = 0;i != child_count_;++ i){
			ml::ui8*	buffer	= ml::offset_pointer<ml::ui8*>(child_buffer_, (int)child_byte_size_offset_array_[i]);
			switch(child_array_->Child(i).Basic_Meta_NodeType()){
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
				case NodeTypeConst_ObjectReference:			break;
				default:							ML_ASSERT(false);	BM3_ASSUME(0);		break;
			}
		}
		#undef BM3_MAKEVERTEX_CHILD
	}
}

template<BM3_OBJECTBASE_TEMPLATE_TEMPLATE_ARG>
int	Reference<BM3_OBJECTBASE_TEMPLATE_ARG>::SwitchResource(const ResourceType & /*res*/, InitialAllocInfo &info){

	// 子供についてもやる
	if(child_buffer_){

		InitialAllocInfo alloc_info(info.ParentMaxGenCount());
//		const typename ResourceType::ChildArrayType_	&child_array	= Resource().ChildArray();
//		int												child_count		= ml::minimum(Resource().ChildArray().Count(), BM3_MAX_CHILD_NODE_COUNT);

		#define BM3_SWITCHRESOURCE_CHILD(NAMESPACE, NODE_TYPE)														\
			BM3_NODE_CALL_METHOD(NAMESPACE, NODE_TYPE,																\
			SwitchResource(reinterpret_cast<const BM3_CHILD_RESOURCE_TYPE(NODE_TYPE) &>(child_array_->Child(i)), alloc_info));

		for(int i = 0;i != child_count_;++ i){
			ml::ui8*	buffer	= ml::offset_pointer<ml::ui8*>(child_buffer_, (int)child_byte_size_offset_array_[i]);
			switch(child_array_->Child(i).Basic_Meta_NodeType()){
				case NodeTypeConst_ControlNull:				BM3_SWITCHRESOURCE_CHILD(control, Null);			break;
				case NodeTypeConst_EmitterQuad:				BM3_SWITCHRESOURCE_CHILD(emitter, Quad);			break;
				case NodeTypeConst_EmitterParticle:			BM3_SWITCHRESOURCE_CHILD(emitter, Particle);		break;
				case NodeTypeConst_EmitterSimpleParticle:	BM3_SWITCHRESOURCE_CHILD(emitter, SimpleParticle);	break;
				case NodeTypeConst_EmitterMassParticle:		BM3_SWITCHRESOURCE_CHILD(emitter, MassParticle);	break;
				case NodeTypeConst_EmitterModel:			BM3_SWITCHRESOURCE_CHILD(emitter, Model);			break;
				case NodeTypeConst_EmitterStripe:			BM3_SWITCHRESOURCE_CHILD(emitter, Stripe);			break;
				case NodeTypeConst_FieldGravity:			break;
				case NodeTypeConst_FieldWind:				break;
				case NodeTypeConst_FieldVortex:				break;
				case NodeTypeConst_FieldRadialpower:		break;
				case NodeTypeConst_FieldDeceleration:		break;
				case NodeTypeConst_FieldCollision:			break;
				case NodeTypeConst_FieldGoal:				break;
				case NodeTypeConst_ObjectReference:			break;
				default:							ML_ASSERT(false);	BM3_ASSUME(0);					break;
			}
		}
		#undef BM3_SWITCHRESOURCE_CHILD
	}

	return 0;
}

template<BM3_OBJECTBASE_TEMPLATE_TEMPLATE_ARG>
int	Reference<BM3_OBJECTBASE_TEMPLATE_ARG>::MaxVertexBufferSize() const {

	int size = 0;
	if(child_buffer_){

//		const typename ResourceType::ChildArrayType_	&child_array	= Resource().ChildArray();
//		int												child_count		= ml::minimum(Resource().ChildArray().Count(), BM3_MAX_CHILD_NODE_COUNT);

		#define BM3_UPDATE_CHILD(NAMESPACE, NODE_TYPE)	size += BM3_NODE_CALL_METHOD(NAMESPACE, NODE_TYPE, MaxVertexBufferSize());

		for(int i = 0;i != child_count_;++ i){
			ml::ui8*	buffer	= ml::offset_pointer<ml::ui8*>(child_buffer_, (int)child_byte_size_offset_array_[i]);
			switch(child_array_->Child(i).Basic_Meta_NodeType()){
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
				case NodeTypeConst_ObjectReference:			break;
				default:							ML_ASSERT(false);	BM3_ASSUME(0);					break;
			}
		}
		#undef BM3_UPDATE_CHILD
	}

	return size;
}

template<BM3_OBJECTBASE_TEMPLATE_TEMPLATE_ARG>
int	Reference<BM3_OBJECTBASE_TEMPLATE_ARG>::MaxPrimCount() const {

	int count = 0;
	if(child_buffer_){

//		const typename ResourceType::ChildArrayType_	&child_array	= Resource().ChildArray();
//		int												child_count		= ml::minimum(Resource().ChildArray().Count(), BM3_MAX_CHILD_NODE_COUNT);

		#define BM3_UPDATE_CHILD(NAMESPACE, NODE_TYPE)	count += BM3_NODE_CALL_METHOD(NAMESPACE, NODE_TYPE, MaxPrimCount());

		for(int i = 0;i != child_count_;++ i){
			ml::ui8*	buffer	= ml::offset_pointer<ml::ui8*>(child_buffer_, (int)child_byte_size_offset_array_[i]);
			switch(child_array_->Child(i).Basic_Meta_NodeType()){
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
				case NodeTypeConst_ObjectReference:			break;
				default:							ML_ASSERT(false);	BM3_ASSUME(0);					break;
			}
		}
		#undef BM3_UPDATE_CHILD
	}

	return count;
}

#ifdef BM3_PLATFORM_playstation3

template<BM3_OBJECTBASE_TEMPLATE_TEMPLATE_ARG>
template<typename UpdateInfo>
void	Reference<BM3_OBJECTBASE_TEMPLATE_ARG>::RawSpuUpdate(UpdateInfo &info){
	ML_ASSERT(IsInitialized());
	
	RawSpuUpdateChild(info);

}

template<BM3_OBJECTBASE_TEMPLATE_TEMPLATE_ARG>
template<typename UpdateInfo>
void	Reference<BM3_OBJECTBASE_TEMPLATE_ARG>::RawSpuUpdateField(UpdateInfo &info){
	if(child_buffer_){
		ML_UPDATE_TIMER;

//		const typename ResourceType::ChildArrayType_	&child_array	= Resource().ChildArray();
//		int												child_count		= ml::minimum(Resource().ChildArray().Count(), BM3_MAX_CHILD_NODE_COUNT);

		#define BM3_UPDATE_CHILD(NAMESPACE, NODE_TYPE)	BM3_NODE_CALL_METHOD(NAMESPACE, NODE_TYPE, RawSpuUpdateField(info));

		for(int i = 0;i != child_count_;++ i){
			ml::ui8*	buffer	= ml::offset_pointer<ml::ui8*>(child_buffer_, (int)child_byte_size_offset_array_[i]);
			switch(child_array_->Child(i).Basic_Meta_NodeType()){
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
				case NodeTypeConst_ObjectReference:			break;
				default:							ML_ASSERT(false);	BM3_ASSUME(0);					break;
			}
		}
		#undef BM3_UPDATE_CHILD
	}

}

template<BM3_OBJECTBASE_TEMPLATE_TEMPLATE_ARG>
void	Reference<BM3_OBJECTBASE_TEMPLATE_ARG>::RawSpuUpdateChild(UpdateInfo &info){

	if(child_buffer_){
		ML_UPDATE_TIMER;

//		const typename ResourceType::ChildArrayType_	&child_array	= Resource().ChildArray();
//		int												child_count		= ml::minimum(Resource().ChildArray().Count(), BM3_MAX_CHILD_NODE_COUNT);

		#define BM3_UPDATE_CHILD(NAMESPACE, NODE_TYPE)	BM3_NODE_CALL_METHOD(NAMESPACE, NODE_TYPE, RawSpuUpdate(info));

		for(int i = 0;i != child_count_;++ i){
			ml::ui8*	buffer	= ml::offset_pointer<ml::ui8*>(child_buffer_, (int)child_byte_size_offset_array_[i]);
			switch(child_array_->Child(i).Basic_Meta_NodeType()){
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
				case NodeTypeConst_ObjectReference:			break;
				default:							ML_ASSERT(false);	BM3_ASSUME(0);					break;
			}
		}
		#undef BM3_UPDATE_CHILD
	}

}

template<BM3_OBJECTBASE_TEMPLATE_TEMPLATE_ARG>
void	Reference<BM3_OBJECTBASE_TEMPLATE_ARG>::MakeUpdateList(UpdateInfo &info){

	MakeChildUpdateList(info);
}

template<BM3_OBJECTBASE_TEMPLATE_TEMPLATE_ARG>
void	Reference<BM3_OBJECTBASE_TEMPLATE_ARG>::MakeChildUpdateList(UpdateInfo &info){
	if(child_buffer_){

//		const typename ResourceType::ChildArrayType_	&child_array	= Resource().ChildArray();
//		int												child_count		= ml::minimum(Resource().ChildArray().Count(), BM3_MAX_CHILD_NODE_COUNT);

		#define BM3_UPDATE_CHILD(NAMESPACE, NODE_TYPE)	BM3_NODE_CALL_METHOD(NAMESPACE, NODE_TYPE, MakeUpdateList(info));

		for(int i = 0;i != child_count_;++ i){
			ml::ui8*	buffer	= ml::offset_pointer<ml::ui8*>(child_buffer_, (int)child_byte_size_offset_array_[i]);
			switch(child_array_->Child(i).Basic_Meta_NodeType()){
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
				case NodeTypeConst_ObjectReference:			break;
				default:							ML_ASSERT(false);	BM3_ASSUME(0);					break;
			}
		}
		#undef BM3_UPDATE_CHILD
	}
}

template<BM3_OBJECTBASE_TEMPLATE_TEMPLATE_ARG>
void	Reference<BM3_OBJECTBASE_TEMPLATE_ARG>::MakeVertexList(DrawInfo &info){
	MakeChildVertexList(info);
}
template<BM3_OBJECTBASE_TEMPLATE_TEMPLATE_ARG>
void	Reference<BM3_OBJECTBASE_TEMPLATE_ARG>::MakeChildVertexList(DrawInfo &info){
	if(child_buffer_){

//		const typename ResourceType::ChildArrayType_	&child_array	= Resource().ChildArray();
//		int												child_count		= ml::minimum(Resource().ChildArray().Count(), BM3_MAX_CHILD_NODE_COUNT);

		#define BM3_MAKEVERTEX_CHILD(NAMESPACE, NODE_TYPE)	BM3_NODE_CALL_METHOD(NAMESPACE, NODE_TYPE, MakeVertexList(info));

		for(int i = 0;i != child_count_;++ i){
			ml::ui8*	buffer	= ml::offset_pointer<ml::ui8*>(child_buffer_, (int)child_byte_size_offset_array_[i]);
			switch(child_array_->Child(i).Basic_Meta_NodeType()){
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
				case NodeTypeConst_ObjectReference:			break;
				default:							ML_ASSERT(false);	BM3_ASSUME(0);					break;
			}
		}
		#undef BM3_MAKEVERTEX_CHILD
	}

}

template<BM3_OBJECTBASE_TEMPLATE_TEMPLATE_ARG>
void	Reference<BM3_OBJECTBASE_TEMPLATE_ARG>::RawSpuMakeVertex(DrawInfo &info){
	
	MakeChildVertexList(info);
}
#endif	

}	// namespace object
}	// namespace obj
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#undef	BM3_OBJECTBASE_TEMPLATE_TEMPLATE_ARG
#undef	BM3_OBJECTBASE_TEMPLATE_ARG
#undef	BM3_SIZEOF_NODE
#undef	BM3_NODE_CALL_METHOD
#undef	BM3_NODE_CALL_METHOD_FORWARD_BUFFER
#undef	BM3_NODE_CALL_CONST_METHOD

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_OBJECT_OBJECT_BM3_OBJ_OBJECTREFERENCE_INL_H
