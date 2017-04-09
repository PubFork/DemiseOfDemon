#ifndef BM3_SDK_INC_BM3_CORE_OBJECT_OBJECT_BM3_OBJ_OBJECTREFERENCE_H
#define BM3_SDK_INC_BM3_CORE_OBJECT_OBJECT_BM3_OBJ_OBJECTREFERENCE_H

#include "../bm3_obj_NodeBase.h"
#include "../../policy/Life/bm3_policy_Life_Normal.h"
#include "../../bm3_DrawInfo.h"
#include "../../bm3_InitialAllocInfo.h"
#include "../../bm3_InitializeInfo.h"
#include <ml/type/ml_basic.h>
#include <ml/type/ml_bool.h>
#include <ml/type/ml_vector3d.h>


namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

class InitializeInfo;

namespace obj{

class RootNodeBase;

namespace object{

template<
	typename ResourceType,
	typename BaseClass					= NodeBase,
	typename LifePolicy					= policy::Life_Normal,
	typename ChildGenerateTimingPolicy	= policy::ChildGenerateTiming_GenerateNull<LifePolicy>
>
class Reference{

public:
	typedef	ResourceType	ResourceType_;

	Reference();
	~Reference();

	bool	Allocate(const ResourceType &res);
	int		FixGenCount(const ResourceType &res, InitialAllocInfo &info, const float gen_scale);
	bool	AllocateParam();
	int		CalcEmitterCount() const;
	void	SetResEmitterInfo(InitializeInfo &initialize_info);

	void	Initialize(const ResourceType &res, InitializeInfo &initialize_info);
	bool	IsInitialized() const{	return res_ != NULL;	}
	void	Finalize();

	template<typename UpdateInfo>
	void	Update(UpdateInfo &info);

	void	MakeVertex(DrawInfo &info);

	int		SwitchResource(const ResourceType &res, InitialAllocInfo &info);

	bool	Child_IsExpired() const;
	bool	IsAllExpired() const {	return Child_IsExpired();	}

	void	SetMaxParticleCount();

	int		MaxVertexBufferSize() const;
	int		MaxPrimCount() const;

	// activeÇ»primitiveêîÇã≠êßìIÇ…0Ç…Ç∑ÇÈ
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

	void	ActivateChildGenerator(NodeBase *parent);

protected:

	const ResourceType	&Resource() const{	ML_PTR_ASSERT(res_);		return *res_;		}
	RootNodeBase		&Root(){		ML_PTR_ASSERT(root_);	return *root_;		}
	const RootNodeBase	&Root() const{	ML_PTR_ASSERT(root_);	return *root_;		}

	// alloc
	bool	AllocateChild(const ResourceType &res);
	int		FixChildGenCount(const ResourceType &res, int node_count, const float gen_scale);
	bool	AllocateChildParam();
	void	MakeChildByteSize(const ResourceType &res);
	void	FreeChild();

protected:
	void	UpdateChild(UpdateInfo &info);
	bool	IsExpired() const;

#ifdef BM3_PLATFORM_playstation3
	void	RawSpuUpdateChild(UpdateInfo &info);
	void	MakeChildUpdateList(UpdateInfo &info);
	void	MakeChildVertexList(DrawInfo &info);
#endif

	// make vertex
	void	MakeChildVertex(DrawInfo &info);
	bmsln::HandleBase* RefHandle(const ResourceType &res);
	bool IsValidedRefData(const typename ResourceType::ChildArrayType_ &child_array);

	// manage
	const ResourceType	*res_;
	RootNodeBase		*root_;

	int					child_all_byte_size_;
	typedef	short	ChildByteSizeArrayElemType;
	ChildByteSizeArrayElemType	child_byte_size_offset_array_[BM3_MAX_CHILD_NODE_COUNT];

	// child
	ml::ui8						*child_buffer_;
	bmsln::HandleBase			*ref_handle_;
	const typename ResourceType::ChildArrayType_	*child_array_;
	int												child_count_;
};

}	// namespace object
}	// namespace obj
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#include "bm3_obj_ObjectReference_inl.h"

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_OBJECT_OBJECT_BM3_OBJ_OBJECTREFERENCE_H
