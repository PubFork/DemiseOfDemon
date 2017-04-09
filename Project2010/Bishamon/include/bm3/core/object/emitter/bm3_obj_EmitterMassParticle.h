#ifndef BM3_SDK_INC_BM3_CORE_OBJECT_EMITTER_BM3_OBJ_EMITTERMASSPARTICLE_H
#define BM3_SDK_INC_BM3_CORE_OBJECT_EMITTER_BM3_OBJ_EMITTERMASSPARTICLE_H

#include "bm3_obj_EmitterBase.h"
#include "bm3/core/primitive/bm3_prim_MassParticle.h"
#include "bm3/core/resource/bm3_res_MassParticle.h"
#include "bm3/core/draw/bm3_draw_MassParticle.h"
#include "bm3/core/policy/Life/bm3_policy_Life_Once.h"
#include "bm3/core/policy/ChildGenerateTiming/bm3_policy_ChildGenerateTiming_Once.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

namespace obj{
namespace emitter{

template<typename ResourceType = res::MassParticle>
class MassParticle :
	public Base<
		ResourceType,
		draw::MassParticle,
		prim::massparticle::Primitive,
		prim::massparticle::PrimitiveManipulator,
		policy::Life_Infinity_Countable,
		policy::ChildGenerateTiming_Once
	>{

	bool	IsAllDeriveParentMatrix(const void * /*res*/) const{
//		const ResourceType& resource = *reinterpret_cast<const ResourceType*>(res);
		return true;
//		return (resource.Basic_ParentNodeInfluence_MatrixParentNodeInfluenceType()==MatrixParentNodeInfluenceTypeConst_On);
	};

	bool	IsAllDeriveGenerateParentMatrix(const void *res) const{
		const ResourceType& resource = *reinterpret_cast<const ResourceType*>(res);
		return (resource.Basic_ParentNodeInfluence_MatrixParentNodeInfluenceType()==MatrixParentNodeInfluenceTypeConst_OnGenerateTime);
	};

	void FixGenerateTimeMatrix(ml::matrix44 & /*matrix*/, const void * /*res*/){
	}
};

}	// namespace emitter
}	// namespace obj
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_OBJECT_EMITTER_BM3_OBJ_EMITTERMASSPARTICLE_H
