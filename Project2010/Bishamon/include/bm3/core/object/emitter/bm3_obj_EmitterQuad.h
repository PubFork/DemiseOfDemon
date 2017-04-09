#ifndef BM3_SDK_INC_BM3_CORE_OBJECT_EMITTER_BM3_OBJ_EMITTERQUAD_H
#define BM3_SDK_INC_BM3_CORE_OBJECT_EMITTER_BM3_OBJ_EMITTERQUAD_H

#include "bm3_obj_EmitterBase.h"
#include "bm3/core/primitive/bm3_prim_Quad.h"
#include "bm3/core/resource/bm3_res_Quad.h"
#include "bm3/core/draw/bm3_draw_Quad.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

namespace obj{
namespace emitter{

template<typename ResourceType = res::Quad>
class Quad :
	public Base<
		ResourceType,
		draw::Quad,
		prim::quad::Primitive,
		prim::quad::PrimitiveManipulator
	>{
	bool	IsAllDeriveParentMatrix(const void* res) const{
		const ResourceType& resource = *reinterpret_cast<const ResourceType*>(res);
		return ObjectBase::IsAllDeriveParentNode(resource);
	};

	bool	IsAllDeriveGenerateParentMatrix(const void *res) const{
		const ResourceType& resource = *reinterpret_cast<const ResourceType*>(res);
		return ObjectBase::IsAllDeriveGenerateParentNode(resource);
	}

	void FixGenerateTimeMatrix(ml::matrix44 &matrix, const void *res){
		const ResourceType& resource = *reinterpret_cast<const ResourceType*>(res);
		ObjectBase::FixGenerateTimeMatix(
			matrix,
			resource.Basic_ParentNodeInfluence_TranslateParentNodeInfluenceType(),
			resource.Basic_ParentNodeInfluence_RotateParentNodeInfluenceType(),
			resource.Basic_ParentNodeInfluence_ScaleParentNodeInfluenceType()
		);
	}

};


}	// namespace emitter
}	// namespace obj
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_OBJECT_EMITTER_BM3_OBJ_EMITTERQUAD_H
