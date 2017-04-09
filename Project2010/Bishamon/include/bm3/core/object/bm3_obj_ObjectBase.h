#ifndef BM3_SDK_INC_BM3_CORE_OBJECT_BM3_OBJ_OBJECTBASE_H
#define BM3_SDK_INC_BM3_CORE_OBJECT_BM3_OBJ_OBJECTBASE_H

#include "../../system/bm3_Config.h"
#include "../bm3_Const.h"
#include <ml/type/ml_basic.h>
#include <ml/type/ml_matrix44.h>
#include <ml/type/ml_vector3d.h>

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE
namespace obj{


/// @brief ObjectBaseƒNƒ‰ƒX
class ObjectBase{

public:

	static void	FixGenerateTimeMatix(ml::matrix44 &dst, TranslateParentNodeInfluenceTypeConst t, RotateParentNodeInfluenceTypeConst r, ScaleParentNodeInfluenceTypeConst s);

	static const ml::matrix44 &MakeObjectHierarchyMatrix(
		ml::matrix44 &dst,
		const ml::matrix44 &parent,
		const ml::vector3d &parent_row_length,
		const ml::matrix44 &generate_time_parent,
		const ml::vector3d &generate_time_parent_row_length,
		TranslateParentNodeInfluenceTypeConst t,
		RotateParentNodeInfluenceTypeConst r,
		ScaleParentNodeInfluenceTypeConst s
	);


	template<typename ResourceType>
	static bool	IsAllDeriveParentNode(const ResourceType &res){

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

		if(	(TranslateParentNodeInfluenceTypeConst_On	== res.Basic_ParentNodeInfluence_TranslateParentNodeInfluenceType())	&&
			(RotateParentNodeInfluenceTypeConst_On		== res.Basic_ParentNodeInfluence_RotateParentNodeInfluenceType())		&&
			(ScaleParentNodeInfluenceTypeConst_On		== res.Basic_ParentNodeInfluence_ScaleParentNodeInfluenceType())		){
				return true;
		}
		return false;
	}

	template<typename ResourceType>
	static bool	IsAllDeriveGenerateParentNode(const ResourceType &res){

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

		if(	(TranslateParentNodeInfluenceTypeConst_OnGenerateTime	== res.Basic_ParentNodeInfluence_TranslateParentNodeInfluenceType())	&&
			(RotateParentNodeInfluenceTypeConst_OnGenerateTime		== res.Basic_ParentNodeInfluence_RotateParentNodeInfluenceType())		&&
			(ScaleParentNodeInfluenceTypeConst_OnGenerateTime		== res.Basic_ParentNodeInfluence_ScaleParentNodeInfluenceType())		){
				return true;
		}
		return false;
	}


	#ifndef BM3_MAX_CHILD_NODE_COUNT
		#define BM3_MAX_CHILD_NODE_COUNT	(64)
	#endif

private:
};

}	// namespace obj
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_OBJECT_BM3_OBJ_OBJECTBASE_H
