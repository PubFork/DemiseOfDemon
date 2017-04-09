#ifndef BM3_SDK_INC_BM3_CORE_OBJECT_EMITTER_BM3_OBJ_EMITTEREMITTERBASE_H
#define BM3_SDK_INC_BM3_CORE_OBJECT_EMITTER_BM3_OBJ_EMITTEREMITTERBASE_H

#if defined(BM3_PLATFORM_nw4c)
#pragma diag_suppress 2819
#endif

#include "../bm3_obj_ObjectBase.h"
#include "bm3/core/bm3_InitialAllocInfo.h"
#include "bm3/core/bm3_InitializeInfo.h"
#include "bm3/core/primitive/bm3_Primitive.h"
#include "bm3/core/primitive/bm3_Primitive_Common.h"
#include "bm3/core/policy/Life/bm3_policy_Life_Normal.h"
#include "bm3/core/policy/Life/bm3_policy_Life_Infinity_Countable.h"
#include "bm3/core/policy/ChildGenerateTiming/bm3_policy_ChildGenerateTiming_Generate.h"
#include "bm3/core/policy/ChildGenerateTiming/bm3_policy_ChildGenerateTiming_Const.h"
#include "bm3/core/policy/ChildGenerateTiming/bm3_policy_ChildGenerateTiming_Timing.h"
#include "bm3/core/manipulator/bm3_ControlManipulator.h"
#include "bm3/core/type/bm3_Buffer.h"
#include <ml/type/ml_basic.h>
#include <ml/type/ml_bool.h>
#if defined(BM3_TARGET_IDE)
#include "bm3/core/policy/Life/bm3_policy_Life_Debug.h"
#endif

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

class InitializeInfo;
class AllocateInfo;
class DrawInfo;

namespace obj{

class RootNodeBase;
class NodeBase;

namespace emitter{

template<
	typename ResourceType,
	typename DrawPacketType,
	typename Primitive,
	typename PrimitiveManipulator,
	typename LifePolicy					= policy::Life_Normal,
	typename ChildGenerateTimingPolicy	= policy::ChildGenerateTiming_Generate<LifePolicy>
>
class Base{

public:
	Base();
	virtual ~Base();

	int		FixGenCount(const ResourceType &res, InitialAllocInfo &info, const float gen_scale);
	bool	AllocateParam();
	int		CalcEmitterCount() const {	return 1;	}
	void	SetResEmitterInfo(InitializeInfo &initialize_info);

	void	Initialize(const ResourceType &res, InitializeInfo &initialize_info);
	bool	IsInitialized() const{	return res_ != NULL;	}
	void	InitializeRandom();
	void	Finalize();

	void	ActivateGenerator(NodeBase *parent);

	template<typename UpdateInfo>
	void	Update(UpdateInfo &info);

	void	MakeVertex(DrawInfo &info);

	enum{
		DrawPacketSize	= sizeof(DrawPacketType)
	};

	bool	IsAllExpired() const {	return is_expired_;	}

	void	SetMaxParticleCount();

	int		SwitchResource(const ResourceType &res, InitialAllocInfo &info);

	int		MaxVertexBufferSize() const;
	int		MaxPrimCount() const;

	// activeなprimitive数を強制的に0にする
	void	ResetActiveCount();

#ifdef BM3_PLATFORM_playstation3
	template<typename UpdateInfo>
	void	RawSpuUpdate(UpdateInfo &info);
	template<typename UpdateInfo>
	void	MakeUpdateList(UpdateInfo &info);
	template<typename UpdateInfo>
	void	RawSpuUpdateField(UpdateInfo &info);
	template<typename DrawInfo>
	void	MakeVertexList(DrawInfo &info);
	template<typename DrawInfo>
	void	MakeVertexTaskList(DrawPacketType &dpt, DrawInfo &info);
#endif

protected:
	//
	struct PrimGenerator{
		PrimGenerator(int start_buffer_point, int max) :
			start_node_pointer_(start_buffer_point),
			current_node_pointer_(start_buffer_point),
			active_node_count_(0),
			active_count_per_frame_(0){
			end_node_pointer_	= start_buffer_point + max;
		}

			void Initialize(const ResourceType &res, ml::random &random, NodeBase *base){
			generator_.Initialize(res, random);
			parent_param_.SetBase(base);
		}

		int Update(UpdateInfo &info, const ResourceType &res, ml::random &random, const RootNodeBase &root){
			return generator_.Update(info, res, random, root);
		}

		bool IsAllChildGenerated(const ResourceType &res, const RootNodeBase &root){
			return generator_.IsAllChildGenerated(res, root);
		}

		void SwitchResource(const ResourceType &res){
			generator_.SwitchResource(res);
		}

		void	SetStart(int point){	start_node_pointer_ = point;	}
		void	AddCurrent(int num, int max){	
			current_node_pointer_ += num;
			while(start_node_pointer_ + max<=current_node_pointer_){
				current_node_pointer_ -= max;
			}
			ML_ASSERT(current_node_pointer_<end_node_pointer_);
		}
		int StartPointer()		const {	return start_node_pointer_;	}
		int EndPointer()		const {	return end_node_pointer_;		}
		int CurrentPointer()	const {	return current_node_pointer_;	}

		void AddActive(int num, int max){
			active_node_count_ += num;
			while(max<active_node_count_){
				active_node_count_	= max;
			}
		}
		int	ActiveNodeCount() const {	return active_node_count_;	}

		prim::ParentParam &GetParentParam()	{	return parent_param_;	}

		void	SetActiveCountPerFrame(int count){	active_count_per_frame_ = count;	}
		int		ActiveCountPerFrame(){	return active_count_per_frame_;	}

		void	ClearActiveNode(int start_buffer_point, int max)
		{
			start_node_pointer_ = start_buffer_point;
			current_node_pointer_ = start_buffer_point;
			active_node_count_ = 0;
			active_count_per_frame_ = 0;
			end_node_pointer_	= start_buffer_point + max;
		}

		bool	IsEnabledParentLife()	const {	return parent_param_.Base().IsEnabledChildGenerated();	}

	private:
		ChildGenerateTimingPolicy	generator_;
		prim::ParentParam			parent_param_;
		int							start_node_pointer_;	// start buffer point for gen
		int							end_node_pointer_;		// end buffer point for gen
		int							current_node_pointer_;	// next active point for gen
		int							active_node_count_;		// active node count for gen
		int							active_count_per_frame_;// active node count per frame for gen
	};	//PrimGenerator

	RootNodeBase		&Root(){		ML_PTR_ASSERT(root_);	return *root_;		}
	const RootNodeBase	&Root() const{	ML_PTR_ASSERT(root_);	return *root_;		}
	const ResourceType	&Resource() const{	ML_PTR_ASSERT(res_);return *res_;		}
	int	ResourceEmitterIndex() const { return resource_emitter_index_; }

	// alloc
	void	MakeChildByteSize(const ResourceType &res);
	bool	AllocGenerator(int node_count);
	int		PacketSize(const ResourceType &res);
	void	FreeGenerator();
	bool	AllocNode(const ResourceType &res, int node_count);
	void	FreeNode();

	// update
	void	UpdateGenerator(UpdateInfo &info);
	void	ActivateNode(int start_node_num, int end_node_num, PrimGenerator *gen);
	// 親に依存しない場合、一辺に処理できるので処理を分ける。
	void	ActivateNodeDependParent(int start_node_num, int end_node_num, prim::ParentParam &parent_node);

	void	UpdatePrimitive(UpdateInfo &info);
	int		UpdatePrimitive(UpdateInfo &info, int begin, int end, int offset_index);
	void	UpdateField(UpdateInfo &info);
	bool	IsExpired() const;

	virtual bool	IsAllDeriveParentMatrix(const void* /*res*/) const{
		ML_ASSERT(false);
		return false;
	};

	virtual bool	IsAllDeriveGenerateParentMatrix(const void * /*res*/) const{
		ML_ASSERT(false);
		return false;
	}

	virtual void FixGenerateTimeMatrix(ml::matrix44 & /*matrix*/, const void * /*res*/){
		ML_ASSERT(false);
	}

#ifdef BM3_PLATFORM_playstation3
	void	RawSpuUpdateLife(UpdateInfo &info);
	int		RawSpuUpdateLife(UpdateInfo &info, int begin, int end, int offset_index);
#endif

	// make vertex
	void	Prepare(UpdateInfo &info, const float alpha);
	void	PrepareField(UpdateInfo &update_info);
	void	MakeVertex(DrawPacketType &dst, DrawInfo &info);
	void	MakeDeriveMatrix();

	// switch resource
	bool	IsValidResource(const ResourceType &res) const;
	void	ReallocGenerator(const ResourceType &old_res, const ResourceType &res, InitialAllocInfo &info);
	void	ReallocParam(const ResourceType &old_res, const ResourceType &res, InitialAllocInfo &info);


	// manage
	RootNodeBase *		root_;
	const ResourceType	*res_;
	int					resource_emitter_index_;

	int					field_all_byte_size_;
	typedef	short	ChildByteSizeArrayElemType;
	bool				is_expired_;

	// gen
	int							max_gen_count_;
	int							current_gen_pointer_;
	int							active_gen_count_;
	int							max_node_count_per_gen_;
	PrimGenerator				*generator_array_;

	// node
	int					max_node_count_;
	int					active_node_count_;
	Primitive					*prim_param_array_;
	prim::Packet				*packet_array_;
	prim::ParentParam			*parent_info_array_;
	int							*active_node_index_;

	//
	ml::ui8						*field_buffer_;

	float						generate_child_remain_;

	PrimZList					prim_z_list_;

	ml::random					random_;
	unsigned int				random_seed_;

#if defined(BM3_TARGET_IDE)
	policy::Life_Debug			debug_life_;
	void	DrawGenerateShape(DrawInfo &draw_info);
#endif

};

}	// namespace emitter
}	// namespace obj
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#include "bm3_obj_EmitterBase_inl.h"

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_OBJECT_EMITTER_BM3_OBJ_EMITTEREMITTERBASE_H
