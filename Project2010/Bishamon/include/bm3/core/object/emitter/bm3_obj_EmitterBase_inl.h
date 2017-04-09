#ifndef BM3_SDK_INC_BM3_CORE_OBJECT_EMITTER_BM3_OBJ_EMITTERBASE_INL_H
#define BM3_SDK_INC_BM3_CORE_OBJECT_EMITTER_BM3_OBJ_EMITTERBASE_INL_H

#include "../../bm3_MemoryManager.h"
#include "../../bm3_InitializeInfo.h"
#include "../../draw/bm3_draw_Common.h"
#include "../../field/bm3_field_Gravity.h"
#include "../../field/bm3_field_Wind.h"
#include "../../field/bm3_field_Vortex.h"
#include "../../field/bm3_field_Radialpower.h"
#include "../../field/bm3_field_Deceleration.h"
#include "../../field/bm3_field_Collision.h"
#include "../../field/bm3_field_Goal.h"
#include "../../resource/bm3_res_Gravity.h"
#include "../../resource/bm3_res_Wind.h"
#include "../../resource/bm3_res_Vortex.h"
#include "../../resource/bm3_res_Radialpower.h"
#include "../../resource/bm3_res_Deceleration.h"
#include "../../resource/bm3_res_Collision.h"
#include "../../resource/bm3_res_Goal.h"
#include "../../field/bm3_field_Draw.h"
#include "../../primitive/bm3_prim_CommonManipulator.h"
#include <ml/utility/ml_assert.h>
#include <ml/utility/ml_simpletimer.h>
#ifdef BM3_PLATFORM_playstation3
#include "../../!playstation3/spu/bm3_SpuHeader.h"
#include "../../!playstation3/spu/bm3_RawSpuManager.h"
#endif



#if defined(BM3_TARGET_IDE)
	#define	BM3_CHILD_RESOURCE_TYPE(NODE_TYPE)	res::IdeNode
#endif
#if defined(BM3_TARGET_EMBEDDED)
	#define	BM3_CHILD_RESOURCE_TYPE(NODE_TYPE)	res::NODE_TYPE
#endif

#define	BM3_EMITTERBASE_TEMPLATE_TEMPLATE_ARG	typename ResourceType, typename DrawPacketType, typename Primitive, typename PrimitiveManipulator, typename LifePolicy, typename ChildGenerateTimingPolicy
#define	BM3_EMITTERBASE_TEMPLATE_ARG			ResourceType, DrawPacketType, Primitive, PrimitiveManipulator, LifePolicy, ChildGenerateTimingPolicy

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE
namespace obj{
namespace emitter{


template<BM3_EMITTERBASE_TEMPLATE_TEMPLATE_ARG>
Base<BM3_EMITTERBASE_TEMPLATE_ARG>::Base() :
	root_(NULL),
	res_(NULL),
	resource_emitter_index_(0),

	field_all_byte_size_(0),
	is_expired_(false),

	max_gen_count_(0),
	current_gen_pointer_(0),
	active_gen_count_(0),
	max_node_count_per_gen_(0),
	generator_array_(NULL),

	max_node_count_(0),
	active_node_count_(0),

	prim_param_array_(NULL),
	packet_array_(NULL),
	parent_info_array_(NULL),
	active_node_index_(NULL),

	field_buffer_(NULL),

	generate_child_remain_(0.0f)
	{
	;
}

template<BM3_EMITTERBASE_TEMPLATE_TEMPLATE_ARG>
Base<BM3_EMITTERBASE_TEMPLATE_ARG>::~Base(){

	FreeNode();
	FreeGenerator();
	if(&Root()){
		if(Root().IsEnableZSort()){
			MemoryManager::instance().General_Free(prim_z_list_.Buffer());
		}
	}
}

template<BM3_EMITTERBASE_TEMPLATE_TEMPLATE_ARG>
int Base<BM3_EMITTERBASE_TEMPLATE_ARG>::FixGenCount(const ResourceType &res, InitialAllocInfo &info, const float gen_scale){
	res_		= &res;

	MakeChildByteSize(res);

	max_gen_count_	= info.ParentMaxGenCount();
	max_node_count_per_gen_	= (int)(ChildGenerateTimingPolicy::CalcTotalGenerateNodeCount(res) * gen_scale);
	max_node_count_	= (info.ParentMaxGenCount() * max_node_count_per_gen_);

	return ChildGenerateTimingPolicy::CalcTotalLifeSpan(res);
}

template<BM3_EMITTERBASE_TEMPLATE_TEMPLATE_ARG>
bool Base<BM3_EMITTERBASE_TEMPLATE_ARG>::AllocateParam(){

	return (AllocGenerator(max_gen_count_) && AllocNode(Resource(), max_node_count_))? true:false;
}


template<BM3_EMITTERBASE_TEMPLATE_TEMPLATE_ARG>
void	Base<BM3_EMITTERBASE_TEMPLATE_ARG>::MakeChildByteSize(const ResourceType &res){

	field_all_byte_size_	= 0;

	{
		const typename ResourceType::ChildArrayType_	&child_array	= res.ChildArray();
		int												child_count		= ml::minimum(res.ChildArray().Count(), BM3_MAX_CHILD_NODE_COUNT);

		for(int i = 0;i != child_count;++ i){
			switch(child_array.Child(i).Basic_Meta_NodeType()){
				case NodeTypeConst_ControlNull:				break;
				case NodeTypeConst_EmitterQuad:				break;
				case NodeTypeConst_EmitterParticle:			break;
				case NodeTypeConst_EmitterSimpleParticle:	break;
				case NodeTypeConst_EmitterMassParticle:		break;
				case NodeTypeConst_EmitterModel:			break;
				case NodeTypeConst_EmitterStripe:			break;
				case NodeTypeConst_FieldGravity:			field_all_byte_size_ += sizeof(field::gravity::Field);				break;
				case NodeTypeConst_FieldWind:				field_all_byte_size_ += sizeof(field::wind::Field);					break;
				case NodeTypeConst_FieldVortex:				field_all_byte_size_ += sizeof(field::vortex::Field);				break;
				case NodeTypeConst_FieldRadialpower:		field_all_byte_size_ += sizeof(field::radialpower::Field);			break;
				case NodeTypeConst_FieldDeceleration:		field_all_byte_size_ += sizeof(field::deceleration::Field);			break;
				case NodeTypeConst_FieldCollision:			field_all_byte_size_ += sizeof(field::collision::Field);			break;
				case NodeTypeConst_FieldGoal:				field_all_byte_size_ += sizeof(field::goal::Field);					break;
				case NodeTypeConst_ObjectReference:			break;
				default:							ML_ASSERT(false);	BM3_ASSUME(0);																																	break;
			}
		}
	}
}

template<BM3_EMITTERBASE_TEMPLATE_TEMPLATE_ARG>
bool	Base<BM3_EMITTERBASE_TEMPLATE_ARG>::AllocGenerator(int node_count){
	generator_array_		= reinterpret_cast<PrimGenerator*>(	bm3::MemoryManager::instance().General_Alloc(node_count * sizeof(PrimGenerator)));
	return (generator_array_)?true:false;
}

template<BM3_EMITTERBASE_TEMPLATE_TEMPLATE_ARG>
void	Base<BM3_EMITTERBASE_TEMPLATE_ARG>::FreeGenerator(){
	bm3::MemoryManager::instance().General_Free(generator_array_);

	generator_array_		= NULL;
}

template<BM3_EMITTERBASE_TEMPLATE_TEMPLATE_ARG>
bool	Base<BM3_EMITTERBASE_TEMPLATE_ARG>::AllocNode(const ResourceType &res, int node_count){

	prim_param_array_	=							(			bm3::MemoryManager::instance().AllocPrimitive<Primitive>(node_count * sizeof(Primitive)));
	packet_array_		= reinterpret_cast<prim::Packet*>(		bm3::MemoryManager::instance().General_Alloc(node_count * PacketSize(res)));
	parent_info_array_	= reinterpret_cast<prim::ParentParam*>(	bm3::MemoryManager::instance().General_Alloc(node_count * sizeof(prim::ParentParam)));
	active_node_index_	= reinterpret_cast<int*>(				bm3::MemoryManager::instance().General_Alloc(node_count * sizeof(int)));
	field_buffer_		= reinterpret_cast<ml::ui8*>(			bm3::MemoryManager::instance().General_Alloc(node_count * field_all_byte_size_));

	// packet_array_とfield_buffer_はメモリ確保しない場合があるので考慮する
	if(PacketSize(res)){
		if(field_all_byte_size_){
			return (prim_param_array_ && packet_array_ && parent_info_array_ && active_node_index_ && field_buffer_)?true:false;
		}
		else{
			return (prim_param_array_ && packet_array_ && parent_info_array_ && active_node_index_)?true:false;
		}		
	}
	else{
		if(field_all_byte_size_){
			return (prim_param_array_ && parent_info_array_ && active_node_index_ && field_buffer_)?true:false;
		}
		else{
			return (prim_param_array_ && parent_info_array_ && active_node_index_)?true:false;
		}
	}
}

template<BM3_EMITTERBASE_TEMPLATE_TEMPLATE_ARG>
int	Base<BM3_EMITTERBASE_TEMPLATE_ARG>::PacketSize(const ResourceType &res){

	// 親の継承。メモリ効率が悪いので場合分け
	if(		IsAllDeriveParentMatrix(&res)			){	return 0;	 						}	//その時のマトリックスを使えばよい
	else if(IsAllDeriveGenerateParentMatrix(&res)	){	return sizeof(prim::Packet);		}	// 生成時のマトリックスを格納
	else{											return sizeof(prim::Packet) * 2;	}	// 頂点を作るとき用と生成時のマトリックス格納用
}

template<BM3_EMITTERBASE_TEMPLATE_TEMPLATE_ARG>
void	Base<BM3_EMITTERBASE_TEMPLATE_ARG>::FreeNode(){

	bm3::MemoryManager::instance().General_Free(prim_param_array_);
	bm3::MemoryManager::instance().General_Free(packet_array_);
	bm3::MemoryManager::instance().General_Free(parent_info_array_);
	bm3::MemoryManager::instance().General_Free(active_node_index_);
	bm3::MemoryManager::instance().General_Free(field_buffer_);

	prim_param_array_	= NULL;
	packet_array_		= NULL;
	parent_info_array_	= NULL;
	active_node_index_	= NULL;
	field_buffer_		= NULL;
}


template<BM3_EMITTERBASE_TEMPLATE_TEMPLATE_ARG>
void		Base<BM3_EMITTERBASE_TEMPLATE_ARG>::SetResEmitterInfo(InitializeInfo &initialize_info){
	initialize_info.Root()->SetResEmitterInfo(Resource().Basic_Basic_SortPriority(), Resource().Basic_Basic_EmitterType(), (const void*)&Resource());
}

template<BM3_EMITTERBASE_TEMPLATE_TEMPLATE_ARG>
void	Base<BM3_EMITTERBASE_TEMPLATE_ARG>::InitializeRandom(){
	
	switch(Resource().Generate_Basic_RandomSeedType()){
		case RandomSeedTypeConst_RootNode:				random_seed_	= RootNodeBase::RandomSeed();				break;
		case RandomSeedTypeConst_RootNodeSeedAssign:	random_seed_	= Resource().Generate_Basic_RandomSeed();	break;
		default:										ML_ASSERT(false);									break;
	}
	
	ml::random::seed_info	rsi(random_seed_);
	random_.set_seed(rsi);
}

template<BM3_EMITTERBASE_TEMPLATE_TEMPLATE_ARG>
void	Base<BM3_EMITTERBASE_TEMPLATE_ARG>::Initialize(const ResourceType & /*res*/, InitializeInfo &initialize_info){
	root_		= initialize_info.Root();
	resource_emitter_index_	= root_->EmitterIndex(reinterpret_cast<const void*>(res_));

	active_gen_count_	= 0;
	current_gen_pointer_= 0;
	active_node_count_	= 0;
	generate_child_remain_	= 0.0f;

	if(Root().IsEnableZSort()){
		if(prim_z_list_.Buffer()==NULL){
			prim_z_list_.Initialize(max_node_count_, MemoryManager::instance().General_Alloc(prim_z_list_.Size(max_node_count_)));
			Root().AddPrimZList(ResourceEmitterIndex(), &prim_z_list_);
		}
	}

	InitializeRandom();

#if defined(BM3_TARGET_IDE)
	if(field_buffer_){
		debug_life_.Initialize(*res_);
	}
#endif
}

template<BM3_EMITTERBASE_TEMPLATE_TEMPLATE_ARG>
void	Base<BM3_EMITTERBASE_TEMPLATE_ARG>::ActivateGenerator(NodeBase *parent){

	PrimGenerator				*gen	= &generator_array_[current_gen_pointer_];
	is_expired_	= false;

	if(active_gen_count_==max_gen_count_){
		// 2週目
		gen->ClearActiveNode(current_gen_pointer_ * max_node_count_per_gen_, max_node_count_per_gen_);
		gen->Initialize(Resource(), random_, parent);
	}
	else{
		new(gen) PrimGenerator(current_gen_pointer_ * max_node_count_per_gen_, max_node_count_per_gen_);
		gen->Initialize(Resource(), random_, parent);
	}

	current_gen_pointer_++;
	if(max_gen_count_<=current_gen_pointer_){
		current_gen_pointer_	= 0;
	}
	if(active_gen_count_<max_gen_count_){
		active_gen_count_++;
	}

	is_expired_	= false;
}

template<BM3_EMITTERBASE_TEMPLATE_TEMPLATE_ARG>
void	Base<BM3_EMITTERBASE_TEMPLATE_ARG>::SetMaxParticleCount(){
	if(&Root()){
		Root().AddChildCount(ResourceEmitterIndex(), max_node_count_);
		Root().AddPushCount(ResourceEmitterIndex() ,max_node_count_ ? max_gen_count_ : 0);
	}
}


template<BM3_EMITTERBASE_TEMPLATE_TEMPLATE_ARG>
template<typename UpdateInfo>
void	Base<BM3_EMITTERBASE_TEMPLATE_ARG>::Update(UpdateInfo &info){

	if(is_expired_==false)
	{
		ML_UPDATE_TIMER;

		// 同族の生成
		UpdateGenerator(info);

		// 更新
		UpdatePrimitive(info);
		UpdateField(info);

		if(0 < active_node_count_){

			MakeDeriveMatrix();

//			PrimGenerator	*gen		= &generator_array_[0];
//			PrimGenerator	*gen_end	= &generator_array_[active_gen_count_];
//			for(; gen != gen_end ; ++gen){
//				if(gen->ActiveCountPerFrame()){
//					Prepare(info, gen->GetParentParam().ParentAlpha());
					PrepareField(info);
//				}
//			}
		}

		is_expired_	= IsExpired();
	}
}

template<BM3_EMITTERBASE_TEMPLATE_TEMPLATE_ARG>
void	Base<BM3_EMITTERBASE_TEMPLATE_ARG>::ResetActiveCount(){
	if(&Root())
		Root().ResetChildCount(ResourceEmitterIndex());
}


template<BM3_EMITTERBASE_TEMPLATE_TEMPLATE_ARG>
void	Base<BM3_EMITTERBASE_TEMPLATE_ARG>::UpdateGenerator(UpdateInfo &info){

	ML_UPDATE_TIMER;

	PrimGenerator	*gen		= &generator_array_[0];
	PrimGenerator	*gen_end	= &generator_array_[active_gen_count_];

	// parent dependence 
	for(; gen!=gen_end ; gen++){

		float count		= gen->Update(info, Resource(), random_, Root()) * info.GenerateChildScale() + generate_child_remain_;
		int gen_count	= (int)count;
		generate_child_remain_	= count - gen_count;
		if(0<gen_count){

			gen_count	= ml::minimum(gen_count, max_node_count_per_gen_);	// frame rate によってはマックスを超える生成数を出す可能性があるため
			int over_gen_count = gen->CurrentPointer() + gen_count - gen->EndPointer();
			if(0<over_gen_count){
				ActivateNodeDependParent(	gen->CurrentPointer(),	gen->EndPointer(),						gen->GetParentParam());
				ActivateNodeDependParent(	gen->StartPointer(),	gen->StartPointer() + over_gen_count,	gen->GetParentParam());
				ActivateNode(				gen->CurrentPointer(),	gen->EndPointer(), gen);
				ActivateNode(				gen->StartPointer(),	gen->StartPointer() + over_gen_count, gen);
			}
			else{
				ActivateNodeDependParent(	gen->CurrentPointer(), gen->CurrentPointer() + gen_count,		gen->GetParentParam());
				ActivateNode(				gen->CurrentPointer(), gen->CurrentPointer() + gen_count, gen);
			}
			gen->AddCurrent(gen_count, max_node_count_per_gen_);
			gen->AddActive(gen_count, max_node_count_per_gen_);
		}
	}
}


template<BM3_EMITTERBASE_TEMPLATE_TEMPLATE_ARG>
void	Base<BM3_EMITTERBASE_TEMPLATE_ARG>::ActivateNode(int start_node_num, int end_node_num, PrimGenerator *gen){

	ML_PTR_ASSERT(generator_array_);
	ML_PTR_ASSERT(prim_param_array_);
	ML_ASSERT(start_node_num<end_node_num);

	{
		PrimitiveManipulator::Initialize(
			&prim_param_array_[start_node_num],
			&prim_param_array_[end_node_num],
			Resource(),
			random_,
			random_seed_ + ml::ui32(gen)
		);

		if(field_all_byte_size_){
			PrimitiveManipulator::InitializeField(
				&prim_param_array_[start_node_num],
				&prim_param_array_[end_node_num],
				ml::offset_pointer<ml::ui8*>(field_buffer_, start_node_num * field_all_byte_size_),
				Resource(),
				random_
			);
		}
	}
}

// 親と関わるノード処理
// ノードの初期化を済ませた後で呼び出さないとだめ。
template<BM3_EMITTERBASE_TEMPLATE_TEMPLATE_ARG>
void	Base<BM3_EMITTERBASE_TEMPLATE_ARG>::ActivateNodeDependParent(int start_node_num, int end_node_num, prim::ParentParam &parent){

	ML_PTR_ASSERT(generator_array_);
	ML_ASSERT(start_node_num<end_node_num);

	// 生成時のみ継承する場合は親のマトリックスのポインタを渡しておく
	if(IsAllDeriveParentMatrix(&Resource())){
		prim::CommonPrimitiveManipulator::SetAllDriveParentMatrix(
			&prim_param_array_[start_node_num],
			&prim_param_array_[end_node_num],
			parent.Base().HierarchyMatrix(),
			parent.Base().HierarchyMatrixRowLength()
		);
	}
	else{
		prim::Packet *pp = &packet_array_[start_node_num];

		pp->generate_time_hierarchy_parent_matrix_				= parent.Base().HierarchyMatrix();
		pp->generate_time_hierarchy_parent_matrix_row_length_	= parent.Base().HierarchyMatrixRowLength();

		FixGenerateTimeMatrix(pp->generate_time_hierarchy_parent_matrix_, &Resource());

		if(IsAllDeriveGenerateParentMatrix(&Resource())){
			// 生成時のマトリックスを常に参照するため生成時のポインタの方へは渡さない。
			prim::CommonPrimitiveManipulator::SetAllDriveParentMatrix(
				&prim_param_array_[start_node_num],
				&prim_param_array_[end_node_num],
				pp->generate_time_hierarchy_parent_matrix_,
				pp->generate_time_hierarchy_parent_matrix_row_length_
			);
		}
		else{
			// 頂点を作る前にマトリックスを参照するため予め渡しておく。
			prim::CommonPrimitiveManipulator::SetAllDriveGenerateTimeParentMatrix(
				&prim_param_array_[start_node_num],
				&prim_param_array_[end_node_num],
				pp->generate_time_hierarchy_parent_matrix_,
				pp->generate_time_hierarchy_parent_matrix_row_length_
			);
		}
	}

	// 親の情報をそれぞれ取っておく
	{
		prim::ParentParam	*pp		= &parent_info_array_[start_node_num];
		prim::ParentParam	*pp_end = &parent_info_array_[end_node_num];
		for(; pp != pp_end ; ++pp){
			*pp = parent;
		}
	}
}


template<BM3_EMITTERBASE_TEMPLATE_TEMPLATE_ARG>
void	Base<BM3_EMITTERBASE_TEMPLATE_ARG>::UpdatePrimitive(UpdateInfo &info){

	ML_UPDATE_TIMER;

	active_node_count_	= 0;

	PrimGenerator	*gen		= &generator_array_[0];
	PrimGenerator	*gen_end	= &generator_array_[active_gen_count_];
	for( ; gen!=gen_end ; ++gen){
		if(gen->IsEnabledParentLife()==false){
			gen->SetActiveCountPerFrame(0);
		}
		else{
			if(gen->ActiveNodeCount()==max_node_count_per_gen_){
				int count = 0;
				count += UpdatePrimitive(info, gen->CurrentPointer(),	gen->EndPointer(),		active_node_count_);
				count += UpdatePrimitive(info, gen->StartPointer(),		gen->CurrentPointer(),	active_node_count_ + count);
				gen->SetActiveCountPerFrame(count);
			}
			else{
				gen->SetActiveCountPerFrame(
					UpdatePrimitive(
						info,
						gen->StartPointer(),
						gen->StartPointer() + gen->ActiveNodeCount(),
						active_node_count_
					)
				);
			}
			active_node_count_ += gen->ActiveCountPerFrame();

			Root().AddChildCount(ResourceEmitterIndex(), gen->ActiveCountPerFrame());
			Root().AddPushCount(ResourceEmitterIndex() ,gen->ActiveCountPerFrame() ? 1 : 0);
		}
	}
}
template<BM3_EMITTERBASE_TEMPLATE_TEMPLATE_ARG>
int	Base<BM3_EMITTERBASE_TEMPLATE_ARG>::UpdatePrimitive(UpdateInfo &info, int begin, int end, int offset_index){

	ML_ASSERT(begin<=end);
	ML_ASSERT(end<=max_node_count_);

	int active_count = 0;
	if(prim_param_array_){
		int *active_node_index = active_node_index_ + offset_index;
		active_count = 
			PrimitiveManipulator::Update(
				&prim_param_array_[begin],
				&prim_param_array_[end],
				begin,
				&active_node_index,
				info,
				Resource(),
				random_
			);
	}

	return active_count;
}

template<BM3_EMITTERBASE_TEMPLATE_TEMPLATE_ARG>
void	Base<BM3_EMITTERBASE_TEMPLATE_ARG>::UpdateField(UpdateInfo &info){

	if(field_all_byte_size_)
	{
		ML_UPDATE_TIMER;

		const typename ResourceType::ChildArrayType_	&child_array	= Resource().ChildArray();
		int												child_count		= ml::minimum(Resource().ChildArray().Count(), BM3_MAX_CHILD_NODE_COUNT);

		for(int k=0 ; k<active_node_count_ ; ++k){
			int							index = active_node_index_[k];
			Primitive	*prim		= &prim_param_array_[index];
			ml::ui8		*field		= ml::offset_pointer<ml::ui8*>(field_buffer_, index * field_all_byte_size_);

			PrimitiveUpdateInfo	pui;
			pui.life_rate_		= prim->LifeRate();
			pui.random_seed_	= ml::ui32(prim);
			for(int i=0 ; i<child_count ; ++i){

				#define	BM3_UPDATE_FIELD(NAMESPACE, NODE_TYPE)	{											\
					NAMESPACE::FieldManipulator::Update(													\
						info,																				\
						*reinterpret_cast<NAMESPACE::Field*>(field),										\
						reinterpret_cast<const BM3_CHILD_RESOURCE_TYPE(NODE_TYPE) &>(child_array.Child(i)),	\
						*prim,																				\
						pui);																				\
						field	+= sizeof(NAMESPACE::Field);												\
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
		debug_life_.Update(info);
#endif

	}
}

template<BM3_EMITTERBASE_TEMPLATE_TEMPLATE_ARG>
bool	Base<BM3_EMITTERBASE_TEMPLATE_ARG>::IsExpired() const{

	if(active_gen_count_ == 0)	return false;

	PrimGenerator	*gen		= &generator_array_[0];
	PrimGenerator	*gen_end	= &generator_array_[active_gen_count_];

	for(; gen != gen_end ; ++gen){
		if(gen->IsAllChildGenerated(Resource(), Root())==false)	return false;
	}

	return (active_node_count_ == 0);
}

template<BM3_EMITTERBASE_TEMPLATE_TEMPLATE_ARG>
void	Base<BM3_EMITTERBASE_TEMPLATE_ARG>::MakeVertex(DrawInfo &info){

	ML_MAKEVERTEX_TIMER;

	ML_ASSERT(IsInitialized());

	MakeVertex(*reinterpret_cast<DrawPacketType *>(Root().EmitterBuffer(ResourceEmitterIndex())), info);
}
#if 0
template<BM3_EMITTERBASE_TEMPLATE_TEMPLATE_ARG>
void	Base<BM3_EMITTERBASE_TEMPLATE_ARG>::Prepare(UpdateInfo &info, const float alpha){

	ML_MAKEVERTEX_TIMER;
	ML_PTR_ASSERT(prim_param_array_);
	ML_PTR_ASSERT(active_node_index_);
	ML_PTR_ASSERT(parent_info_array_);

	PrimitiveManipulator::Prepare(
		prim_param_array_,
		active_node_count_,
		active_node_index_,
		info,
		Resource(),
		Root(),
		alpha
	);
}
#endif
template<BM3_EMITTERBASE_TEMPLATE_TEMPLATE_ARG>
void	Base<BM3_EMITTERBASE_TEMPLATE_ARG>::PrepareField(UpdateInfo &update_info){

	if(field_all_byte_size_)
	{
		ML_MAKEVERTEX_TIMER;
		PrimitiveManipulator::PrepareField(
			prim_param_array_, 
			active_node_count_, 
			active_node_index_, 
			field_buffer_, 
			field_all_byte_size_, 
			update_info,
			Resource(),
			Root()
		);
	}
}

template<BM3_EMITTERBASE_TEMPLATE_TEMPLATE_ARG>
void	Base<BM3_EMITTERBASE_TEMPLATE_ARG>::MakeVertex(DrawPacketType &dpt, DrawInfo &info){

	if(0 < active_node_count_){
		ML_MAKEVERTEX_TIMER;

		{
			WholeZInfo	z_info;
			z_info.is_enabled_		= Root().IsEnableZSort();
			z_info.vertex_head_		= Root().VertexBuffer(ResourceEmitterIndex(), 0);
			z_info.prim_vtx_size_	= dpt.VertexBufferSize(1);
			z_info.emitter_index_	= ResourceEmitterIndex();
#ifdef BM3_PLATFORM_psvita
			z_info.index_head_		= ml::offset_pointer<void*>(z_info.vertex_head_ ,Root().VertexBufferSize(ResourceEmitterIndex()));
			z_info.prim_index_size_	= dpt.IndexBufferSize(1, 0);
#endif
			PrimitiveManipulator::ZSort(prim_param_array_, active_node_count_, active_node_index_, info, Resource(), z_info, prim_z_list_);
		}

		PrimGenerator	*gen		= &generator_array_[0];
		PrimGenerator	*gen_end	= &generator_array_[active_gen_count_];
		PacketDrawInfo	pdi;
		{
			pdi.index_				= 0;
			pdi.color_scale_		= Root().ColorScale();
			pdi.offset_position_	= Root().OffsetPosition();
			pdi.facing_position_	= Root().HierarchyMatrix().translate();
		}
		PrimitiveManipulator::SetFacingSetting(pdi, info, Resource());

		int activatied_count	= 0;
		for(; gen != gen_end ; ++gen){
			if(gen->ActiveCountPerFrame()){
				dpt.BeginMakeVertex(Root(), ResourceEmitterIndex());

				void*	vertex_buffer = Root().VertexBuffer(ResourceEmitterIndex(), dpt.VertexBufferSize(gen->ActiveCountPerFrame()));

				if(Resource().Basic_Meta_Display() && (vertex_buffer!=NULL)){

					pdi.all_active_child_count_	= gen->ActiveCountPerFrame();//AllActiveChildCount();
					pdi.parent_color_	= &gen->GetParentParam().ParentColor();

					PrimitiveManipulator::MakeVertex(
						dpt, 
						prim_param_array_,
						gen->ActiveCountPerFrame(),
						&active_node_index_[activatied_count],
						info,
						pdi,
						Resource(),
						vertex_buffer,
						Root().IsEnabledBm2()
					);
					activatied_count	+= gen->ActiveCountPerFrame();
				}

				dpt.EndMakeVertex(Root(), ResourceEmitterIndex());
			}
		}

#ifdef BM3_PLATFORM_psvita
		dpt.PostMakeVertex(Root(), ResourceEmitterIndex() ,active_node_count_);
#endif

#if defined(BM3_TARGET_IDE)
		DrawGenerateShape(info);
		field::FieldDraw(info, Resource(), Root().HierarchyMatrix(), debug_life_);
#endif
	}
}

#if defined(BM3_TARGET_IDE)
template<BM3_EMITTERBASE_TEMPLATE_TEMPLATE_ARG>
void	Base<BM3_EMITTERBASE_TEMPLATE_ARG>::DrawGenerateShape(DrawInfo &draw_info){

	if(Resource().IsSelected() && Resource().Basic_Meta_Display() && Resource().GenerateShape_Basic_Display()){

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
	}
}
#endif

template<BM3_EMITTERBASE_TEMPLATE_TEMPLATE_ARG>
void	Base<BM3_EMITTERBASE_TEMPLATE_ARG>::MakeDeriveMatrix(){

	if(0 < active_node_count_){
		ML_MAKEVERTEX_TIMER;

		if(IsAllDeriveParentMatrix(&Resource())){

			;	// 初期化時に処理されている
		}else
		if(IsAllDeriveGenerateParentMatrix(&Resource())){
			;	// 初期化時に処理されている
		}
		else{
			prim::Packet *p = ml::offset_pointer<prim::Packet *>(packet_array_, sizeof(prim::Packet) * max_node_count_);
			PrimitiveManipulator::SetDriveMatrix(
				p,
				prim_param_array_, 
				active_node_count_, 
				active_node_index_,
				Resource(),
				parent_info_array_
			);
		}
	}
}

template<BM3_EMITTERBASE_TEMPLATE_TEMPLATE_ARG>
int	Base<BM3_EMITTERBASE_TEMPLATE_ARG>::SwitchResource(const ResourceType &res, InitialAllocInfo &info){

	ML_ASSERT(IsValidResource(res));

	const ResourceType	&old_res	= *res_;
	res_							= &res;

	ReallocParam(old_res, res, info);
	ReallocGenerator(old_res, res, info);	// 生成子当たりの生成数を知りたいから先にパラメータから処理する

	{
		PrimGenerator *gen		= &generator_array_[0];
		PrimGenerator *gen_end	= &generator_array_[active_gen_count_];
		int count = 0;
		for(; gen!=gen_end ; ++gen, ++count){
			gen->SwitchResource(Resource());
		}
	}

	{
		if(active_node_count_){
			PrimitiveManipulator::SwitchResource(
				prim_param_array_,
				active_node_count_,
				active_node_index_,
				Resource(),
				old_res,
				random_
			);


			if(field_all_byte_size_){
				PrimitiveManipulator::SwitchResourceField(
					prim_param_array_,
					active_node_count_,
					active_node_index_,
					field_buffer_,
					field_all_byte_size_,
					Resource(),
					old_res,
					random_
				);
			}
		}
	}

	return 0;
}

template<BM3_EMITTERBASE_TEMPLATE_TEMPLATE_ARG>
bool	Base<BM3_EMITTERBASE_TEMPLATE_ARG>::IsValidResource(const ResourceType &res) const{

	ML_ASSERT(Resource().Basic_Meta_NodeType()	==	res.Basic_Meta_NodeType());
	ML_ASSERT(Resource().ChildArray().Count()	==	res.ChildArray().Count());

	return true;
}
template<BM3_EMITTERBASE_TEMPLATE_TEMPLATE_ARG>
void	Base<BM3_EMITTERBASE_TEMPLATE_ARG>::ReallocGenerator(const ResourceType & /*old_res*/, const ResourceType & /*res*/, InitialAllocInfo &info){

	if(max_gen_count_<info.ParentMaxGenCount()){
		int node_count = info.ParentMaxGenCount();
		PrimGenerator	*temp_generator_array	= reinterpret_cast<PrimGenerator*>(	bm3::MemoryManager::instance().General_Alloc(node_count * sizeof(PrimGenerator)));

		memcpy(temp_generator_array,	generator_array_,	max_gen_count_ * sizeof(PrimGenerator));
		// 継承元の親のアドレスがわからない

		FreeGenerator();

		max_gen_count_		= info.ParentMaxGenCount();
		generator_array_	= temp_generator_array;
	}

}

template<BM3_EMITTERBASE_TEMPLATE_TEMPLATE_ARG>
void	Base<BM3_EMITTERBASE_TEMPLATE_ARG>::ReallocParam(const ResourceType &old_res, const ResourceType &res, InitialAllocInfo &info){
	
	int max_node_count_per_gen	= ChildGenerateTimingPolicy::CalcTotalGenerateNodeCount(res);
	int new_node_count = info.ParentMaxGenCount() * max_node_count_per_gen;
	int node_count = new_node_count;
	if(max_node_count_<new_node_count){
		Primitive				*temp_prim_param_array		= reinterpret_cast<Primitive*>(			bm3::MemoryManager::instance().General_Alloc(node_count * sizeof(Primitive)));
		prim::ParentParam		*temp_parent_info_array		= reinterpret_cast<prim::ParentParam*>(	bm3::MemoryManager::instance().General_Alloc(node_count * sizeof(prim::ParentParam)));
		int						*temp_active_node_index		= reinterpret_cast<int*>(				bm3::MemoryManager::instance().General_Alloc(node_count * sizeof(int)));
		ml::ui8					*temp_field_buffer			= reinterpret_cast<ml::ui8*>(			bm3::MemoryManager::instance().General_Alloc(field_all_byte_size_));

		std::memcpy(temp_prim_param_array,		prim_param_array_,		max_node_count_ * sizeof(Primitive));
		std::memcpy(temp_parent_info_array,		parent_info_array_,		max_node_count_ * sizeof(prim::ParentParam));
		std::memcpy(temp_active_node_index,		active_node_index_,		max_node_count_ * sizeof(int));

		if(field_all_byte_size_){
			std::memcpy(temp_field_buffer,		field_buffer_,			max_node_count_ * field_all_byte_size_);
		}

		bm3::MemoryManager::instance().General_Free(prim_param_array_);
		bm3::MemoryManager::instance().General_Free(parent_info_array_);
		bm3::MemoryManager::instance().General_Free(active_node_index_);
		bm3::MemoryManager::instance().General_Free(field_buffer_);

		prim_param_array_		= temp_prim_param_array;
		parent_info_array_		= temp_parent_info_array;
		active_node_index_		= temp_active_node_index;
		field_buffer_			= temp_field_buffer;

		max_node_count_	= new_node_count;
	}

	int old_packet_size = PacketSize(old_res);
	int new_packet_size = PacketSize(res);
	if(old_packet_size<new_packet_size){
		prim::Packet	*temp_packet_array			= reinterpret_cast<prim::Packet*>(		bm3::MemoryManager::instance().General_Alloc(node_count * new_packet_size));
		if(packet_array_){
			std::memcpy(temp_packet_array,			packet_array_,			max_node_count_ * old_packet_size);
			bm3::MemoryManager::instance().General_Free(packet_array_);
		}
		packet_array_	= temp_packet_array;
	}
}

template<BM3_EMITTERBASE_TEMPLATE_TEMPLATE_ARG>
int	Base<BM3_EMITTERBASE_TEMPLATE_ARG>::MaxVertexBufferSize() const {
	return max_node_count_ * Root().UnitVertexBufferSize(ResourceEmitterIndex());
}

template<BM3_EMITTERBASE_TEMPLATE_TEMPLATE_ARG>
int	Base<BM3_EMITTERBASE_TEMPLATE_ARG>::MaxPrimCount() const {
	return max_node_count_;
}

#ifdef BM3_PLATFORM_playstation3
template<BM3_EMITTERBASE_TEMPLATE_TEMPLATE_ARG>
template<typename UpdateInfo>
void	Base<BM3_EMITTERBASE_TEMPLATE_ARG>::RawSpuUpdate(UpdateInfo &info){

	if(is_expired_==false)
	{
		ML_UPDATE_TIMER;

		UpdateGenerator(info);		// 同族の生成

		// 更新
		RawSpuUpdateLife(info);
		UpdateField(info);

		is_expired_	= IsExpired();
	}
}

template<BM3_EMITTERBASE_TEMPLATE_TEMPLATE_ARG>
void	Base<BM3_EMITTERBASE_TEMPLATE_ARG>::RawSpuUpdateLife(UpdateInfo &info){

	ML_UPDATE_TIMER;

	active_node_count_	= 0;

	PrimGenerator	*gen		= &generator_array_[0];
	PrimGenerator	*gen_end	= &generator_array_[active_gen_count_];
	for( ; gen!=gen_end ; ++gen){
		if(gen->ActiveNodeCount()==max_node_count_per_gen_){
			int count = 0;
			count += RawSpuUpdateLife(info, gen->CurrentPointer(),	gen->EndPointer(),		active_node_count_);
			count += RawSpuUpdateLife(info, gen->StartPointer(),	gen->CurrentPointer(),	active_node_count_ + count);
			gen->SetActiveCountPerFrame(count);
		}
		else{
			gen->SetActiveCountPerFrame(
				RawSpuUpdateLife(
					info,
					gen->StartPointer(),
					gen->StartPointer() + gen->ActiveNodeCount(),
					active_node_count_
				)
			);
		}
		active_node_count_ += gen->ActiveCountPerFrame();

		Root().AddChildCount(ResourceEmitterIndex(), gen->ActiveCountPerFrame());
		Root().AddPushCount(ResourceEmitterIndex() ,gen->ActiveCountPerFrame() ? 1 : 0);
	}
}
template<BM3_EMITTERBASE_TEMPLATE_TEMPLATE_ARG>
int	Base<BM3_EMITTERBASE_TEMPLATE_ARG>::RawSpuUpdateLife(UpdateInfo &info, int begin, int end, int offset_index){

	ML_ASSERT(begin<=end);
	ML_ASSERT(end<=max_node_count_);

	int *active_node_index = active_node_index_ + offset_index;
	int active_count = 
		prim::CommonPrimitiveManipulator::UpdateLife(
			&prim_param_array_[begin],
			&prim_param_array_[end],
			begin,
			&active_node_index,
			info,
			Resource()
		);

	return active_count;
}

template<BM3_EMITTERBASE_TEMPLATE_TEMPLATE_ARG>
template<typename UpdateInfo>
void	Base<BM3_EMITTERBASE_TEMPLATE_ARG>::MakeUpdateList(UpdateInfo &info){
	if(active_node_count_){
		//Task	*task = bm3::RawSpuManager().instance().NextTask();
		//task->type_					= Resource().Basic_Meta_NodeType();
		//task->resource_				= (int)res_;
		//task->active_count_			= active_node_count_;
		//task->param_buffer_			= (int)prim_param_array_;
		//task->active_index_array_	= (int)active_node_index_;
		//task->update_rate_			= info.GetUpdateFrame();
		//bm3::RawSpuManager().instance().EnqueueTask();
		info.JobInitialize();
		info.CreateJobQueue();
		info.PushUserData(Resource().Basic_Meta_NodeType());
		info.PushUserData((int)res_);
		info.PushUserData(active_node_count_);
		info.PushUserData((int)prim_param_array_);
		info.PushUserData((int)active_node_index_);
		info.PushUserData(info.GetUpdateFrame());
		info.BootSpurs();
	}
}

template<BM3_EMITTERBASE_TEMPLATE_TEMPLATE_ARG>
template<typename DrawInfo>
void	Base<BM3_EMITTERBASE_TEMPLATE_ARG>::MakeVertexList(DrawInfo &info){

	MakeVertexTaskList(*reinterpret_cast<DrawPacketType *>(Root().EmitterBuffer(ResourceEmitterIndex())), info);

}

template<BM3_EMITTERBASE_TEMPLATE_TEMPLATE_ARG>
template<typename DrawInfo>
void	Base<BM3_EMITTERBASE_TEMPLATE_ARG>::MakeVertexTaskList(DrawPacketType &dpt, DrawInfo &info){
	if( active_node_count_){
		ML_MAKEVERTEX_TIMER;
		
		{
			WholeZInfo	z_info;
			z_info.is_enabled_		= Root().IsEnableZSort();
			z_info.vertex_head_		= Root().VertexBuffer(ResourceEmitterIndex(), 0);
			z_info.prim_vtx_size_	= dpt.VertexBufferSize(1);
			z_info.emitter_index_	= ResourceEmitterIndex();
			PrimitiveManipulator::ZSort(prim_param_array_, active_node_count_, active_node_index_, info, Resource(), z_info, prim_z_list_);
		}

		PrimGenerator	*gen		= &generator_array_[0];
		PrimGenerator	*gen_end	= &generator_array_[active_gen_count_];
		int temp_active_node_count = 0;
		PacketDrawInfo	pdi;
		{
			pdi.index_					= 0;
			pdi.color_scale_			= Root().ColorScale();
			pdi.offset_position_		= Root().OffsetPosition();
			pdi.facing_position_		= Root().HierarchyMatrix().translate();
		}
		PrimitiveManipulator::SetFacingSetting(pdi, info, Resource());
		for(; gen != gen_end ; ++gen){
			if(gen->ActiveCountPerFrame()){
				dpt.BeginMakeVertex(Root(), ResourceEmitterIndex());

				void*	vertex_buffer = Root().VertexBuffer(ResourceEmitterIndex(), dpt.VertexBufferSize(gen->ActiveCountPerFrame()));

				if(Resource().Basic_Meta_Display() && (vertex_buffer!=NULL)){

					pdi.all_active_child_count_	= gen->ActiveCountPerFrame();//AllActiveChildCount();

					VertexTask	*task = bm3::RawSpuManager().instance().NextVertexTask();
					task->type_					= Resource().Basic_Meta_NodeType();
					task->resource_				= (int)res_;
					task->active_count_			= active_node_count_;
					task->param_buffer_			= (int)prim_param_array_;
					task->active_index_array_	= (int)active_node_index_;
					task->packet_draw_info_		= (int)&pdi;
					task->draw_packet_type_		= (int)&dpt;
					task->draw_info_			= (int)&info;
					task->vertex_buffer_		= (int)vertex_buffer;
					task->is_enable_bm2_		= (int)Root().IsEnabledBm2();
					bm3::RawSpuManager().instance().EnqueueVertexTask();
				}

				dpt.EndMakeVertex(Root(), ResourceEmitterIndex());
			}
		}
	}
}

template<BM3_EMITTERBASE_TEMPLATE_TEMPLATE_ARG>
template<typename UpdateInfo>
void	Base<BM3_EMITTERBASE_TEMPLATE_ARG>::RawSpuUpdateField(UpdateInfo &info){

	if(is_expired_==false)
	{
		ML_UPDATE_TIMER;

		if(0 < active_node_count_){
			MakeDeriveMatrix();
			PrepareField(info);
		}
		is_expired_	= IsExpired();
	}
}

#endif


}	// namespace emitter
}	// namespace obj
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#undef	BM3_EMITTERBASE_TEMPLATE_TEMPLATE_ARG
#undef	BM3_EMITTERBASE_TEMPLATE_ARG

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_OBJECT_EMITTER_BM3_OBJ_EMITTEREMITTERBASE_INL_H
