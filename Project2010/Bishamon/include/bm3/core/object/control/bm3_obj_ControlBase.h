#ifndef BM3_SDK_INC_BM3_CORE_OBJECT_CONTROL_BM3_OBJ_CONTROLBASE_H
#define BM3_SDK_INC_BM3_CORE_OBJECT_CONTROL_BM3_OBJ_CONTROLBASE_H

#include "bm3/core/object/bm3_obj_NodeBase.h"
#include "bm3/core/policy/Life/bm3_policy_Life_Normal.h"
#include "bm3/core/policy/ChildGenerateTiming/bm3_policy_ChildGenerateTiming_GenerateNull.h"
#include "bm3/core/manipulator/bm3_ControlManipulator.h"
#include "bm3/core/type/bm3_Buffer.h"
#include "bm3/core/bm3_DrawInfo.h"
#include "bm3/core/bm3_InitialAllocInfo.h"
#include "bm3/core/bm3_InitializeInfo.h"
#include <ml/type/ml_basic.h>
#include <ml/type/ml_bool.h>
#include <ml/type/ml_vector3d.h>
#if defined(BM3_TARGET_IDE)
#include "../../policy/Life/bm3_policy_Life_Debug.h"
#endif

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

class InitializeInfo;

namespace obj{

class RootNodeBase;

namespace control{

template<
	typename ResourceType,
	typename BaseClass					= NodeBase,
	typename LifePolicy					= policy::Life_Normal,
	typename ChildGenerateTimingPolicy	= policy::ChildGenerateTiming_GenerateNull<LifePolicy>
>
class Base{

public:
	typedef	ResourceType	ResourceType_;

	Base();
	~Base();

	bool	Allocate(const ResourceType &res);
	int		FixGenCount(const ResourceType &res, InitialAllocInfo &info, const float gen_scale);
	bool	AllocateParam();
	int		CalcEmitterCount() const;
	void	SetResEmitterInfo(InitializeInfo &initialize_info);

	void	Initialize(const ResourceType &res, InitializeInfo &initialize_info);
	bool	IsInitialized() const{	return res_ != NULL;	}
	void	InitializeRandom();
	void	Finalize();

	template<typename UpdateInfo>
	void	Update(UpdateInfo &info);
	void	ActivateGenerator(NodeBase *parent);

	void	MakeVertex(DrawInfo &info);

	int		SwitchResource(const ResourceType &res, InitialAllocInfo &info);

	bool	Child_IsExpired() const;
	bool	IsAllExpired() const {	return is_expired_;	}

	void	SetMaxParticleCount();

	int		MaxVertexBufferSize() const;
	int		MaxPrimCount() const;

	// activeなprimitive数を強制的に0にする
	void	ResetActiveCount();

#ifdef BM3_PLATFORM_playstation3
	template<typename UpdateInfo>
	void	RawSpuUpdate(UpdateInfo &info);
	template<typename UpdateInfo>
	void	RawSpuUpdateField(UpdateInfo &info);
	void	MakeUpdateList(UpdateInfo &info);
	void	RawSpuMakeVertex(DrawInfo &info);
	void	MakeVertexList(DrawInfo &info);
#endif	
	
protected:
	struct NodeParam{
	public:
		NodeParam() :
			parent_(NULL){
			  ;
		}

			void	SetParentParam(NodeBase *parent){
				parent_			= parent;
			}
			NodeBase* Parent()				const { return parent_;	}
			bool	IsEnabledParentLife()	const {	return parent_->IsEnabledChildGenerated();	}

			ColorType& ParentColor()	const {	return parent_->BlendColor();	}

	private:
        NodeBase					*parent_;
	};

	const ResourceType	&Resource() const{	ML_PTR_ASSERT(res_);		return *res_;		}
	RootNodeBase		&Root(){		ML_PTR_ASSERT(root_);	return *root_;		}
	const RootNodeBase	&Root() const{	ML_PTR_ASSERT(root_);	return *root_;		}
	bool				IsRoot() const {	return is_root_;	}

	// alloc
	bool	AllocateChild(const ResourceType &res);
	int		FixChildGenCount(const ResourceType &res, int node_count, const float gen_scale);
	bool	AllocateChildParam();
	void	MakeChildByteSize(const ResourceType &res);
	void	MakeChildByteSizeForReference(const ResourceType &res);
	bool	AllocGenerator(int node_count);
	void	FreeGenerator();
	bool	AllocNode(int node_count);
	void	FreeNode();
	void	FreeChild();

	// update
	void	UpdateGenerator(UpdateInfo &info);
	void	ActivateNode(int start_node_num, int end_node_num);
	// 親に依存しない場合、一辺に処理できるので処理を分ける。
	void	ActivateNodeDependParent(int start_node_num, int end_node_num, NodeBase *parent_node);
	void	ActivateChildGenerator(int start_node_num, int end_node_num);
	void	ActivateChildGenerator(NodeBase *parent);

	void	UpdateControl(UpdateInfo &info);
	int		UpdateControl(UpdateInfo &info, int begin, int end, int *active_node_index);
	void	UpdateParentInfl();
	void	UpdateField(UpdateInfo &info);
	void	UpdateChild(UpdateInfo &info);
	bool	IsExpired() const;

#ifdef BM3_PLATFORM_playstation3
	void	UpdateLife(UpdateInfo &info);
	int		UpdateLife(UpdateInfo &info, int begin, int end, int **active_node_index);
	void	RawSpuUpdateChild(UpdateInfo &info);
	void	MakeChildUpdateList(UpdateInfo &info);
	void	MakeChildVertexList(DrawInfo &info);
#endif

	// make vertex
	void	PrepareField(UpdateInfo &update_info);
	void	MakeChildVertex(DrawInfo &info);
	void	DrawControl(DrawInfo &info);

	//swtich resource
	bool	IsValidResource(const ResourceType &res) const ;
	int		SwitchResourceField(const ResourceType &old_res);
	void	ReallocGenerator(const ResourceType &old_res, const ResourceType &res, InitialAllocInfo &info);
	void	ReallocParam(const ResourceType &old_res, const ResourceType &res, InitialAllocInfo &info);

	// manage
	const ResourceType	*res_;
	RootNodeBase		*root_;
	bool				is_root_;

	int					child_all_byte_size_;
	int					field_all_byte_size_;
	typedef	short	ChildByteSizeArrayElemType;
	ChildByteSizeArrayElemType	child_byte_size_offset_array_[BM3_MAX_CHILD_NODE_COUNT];
	bool				is_expired_;

	// gen
	int					max_gen_count_;
	int					current_gen_pointer_;
	int					active_gen_count_;
	ChildGenerateTimingPolicy	*generator_array_;
	NodeParam					*node_param_array_;

	// node 
	int					max_node_count_;
	int					active_node_count_;
	int					active_count_per_frame_;
	int					current_node_pointer_;

	BaseClass					*node_base_array_;
	ControlParam<LifePolicy>	*control_param_array_;
	int							*active_node_index_;

	// child
	ml::ui8						*child_buffer_;
	ml::ui8						*field_buffer_;

	float						generate_child_remain_;

	ml::random					random_;

#if defined(BM3_TARGET_IDE)
	policy::Life_Debug			debug_life_;
	void	DrawGenerateShape(DrawInfo &draw_info);
#endif
};

}	// namespace control
}	// namespace obj
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#include "bm3_obj_ControlBase_inl.h"

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_OBJECT_CONTROL_BM3_OBJ_CONTROLBASE_H
