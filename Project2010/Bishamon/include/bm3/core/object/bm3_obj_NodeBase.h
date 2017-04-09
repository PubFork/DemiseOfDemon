#ifndef BM3_SDK_INC_BM3_CORE_OBJECT_BM3_OBJ_NODEBASE_H
#define BM3_SDK_INC_BM3_CORE_OBJECT_BM3_OBJ_NODEBASE_H

#include "bm3_obj_ObjectBase.h"
#include "bm3/system/bm3_Config.h"
#include "bm3/core/bm3_CoreType.h"

#include <ml/type/ml_matrix44.h>

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE
namespace obj{

/// @brief NodeBaseクラス
class NodeBase : public ObjectBase{

public:
	NodeBase() : is_enabled_childgenerated_(true){
		blended_color_	= ColorType(1.0f, 1.0f, 1.0f, 1.0f);
	}

	const ml::matrix44	&Matrix() const{										return matrix_;												}
	const ml::matrix44	&GenerateTimeHierarchyParentMatrix() const{				return generate_time_hierarchy_parent_matrix_;				}
	const ml::vector3d	&GenerateTimeHierarchyParentMatrixRowLength() const{	return generate_time_hierarchy_parent_matrix_row_length_;	}
	const ml::matrix44	&HierarchyMatrix() const{								return hierarchy_matrix_;									}
	const ml::vector3d	&HierarchyMatrixRowLength() const{						return hierarchy_matrix_row_length_;						}

	void	SetMatrix(const ml::matrix44 &m){
		generate_time_hierarchy_parent_matrix_ = m;
		hierarchy_matrix_ = m;

		generate_time_hierarchy_parent_matrix_row_length_	= generate_time_hierarchy_parent_matrix_.row_length();
		hierarchy_matrix_row_length_						= hierarchy_matrix_.row_length();
	}

	void	SetHierarchyMatrix(const ml::matrix44 &m){								hierarchy_matrix_				= m;	}
	void	AddHierarchyMatrixTranslation(const ml::vector3d &trans){				hierarchy_matrix_.add_translate(trans);	}
	void	SetHierarchyMatrixRowLength(const ml::vector3d &v){						hierarchy_matrix_row_length_	= v;	}
	void	SetGenerateTimeHierarchyParentMatrix(const ml::matrix44 &m){			generate_time_hierarchy_parent_matrix_				= m;	}
	void	SetGenerateTimeHierarchyParentMatrixRowLength(const ml::vector3d &v){	generate_time_hierarchy_parent_matrix_row_length_	= v;	}

	void	FixGenerateTimeMatix(TranslateParentNodeInfluenceTypeConst t, RotateParentNodeInfluenceTypeConst r, ScaleParentNodeInfluenceTypeConst s){
		ObjectBase::FixGenerateTimeMatix(generate_time_hierarchy_parent_matrix_, t, r, s);
	}

	const ml::matrix44 &	MakeHierarchyMatrix(
		const ml::matrix44 &current,
		TranslateParentNodeInfluenceTypeConst t,
		RotateParentNodeInfluenceTypeConst r,
		ScaleParentNodeInfluenceTypeConst s
	){
		ml::ni::matrix44	temp;
		return hierarchy_matrix_.mul(
			current, 
			ObjectBase::MakeObjectHierarchyMatrix(
				temp,
				parent_->HierarchyMatrix(),	parent_->HierarchyMatrixRowLength(),
				generate_time_hierarchy_parent_matrix_, generate_time_hierarchy_parent_matrix_row_length_,
				t, r, s
			)
		);
	}

	void SetParent(NodeBase *parent){	parent_	= parent;	}
	NodeBase* Parent() const {	return parent_;	}

	void DisableChildGenerated(){			is_enabled_childgenerated_	= false;	}
	bool IsEnabledChildGenerated() const{	return is_enabled_childgenerated_;		}

	void SetBlendColor(ColorType& c){	blended_color_	= c;	}
	ColorType& BlendColor(){			return blended_color_;	}
protected:
	ml::matrix44	matrix_;
	ml::matrix44	generate_time_hierarchy_parent_matrix_;				// 生成時の親の行列
	ml::vector3d	generate_time_hierarchy_parent_matrix_row_length_;	// 生成時の親の行列
	ml::matrix44	hierarchy_matrix_;									// 親から自分までの合成した行列
	ml::vector3d	hierarchy_matrix_row_length_;						// 親から自分までの合成した行列
	NodeBase		*parent_;
	bool			is_enabled_childgenerated_;
	ColorType		blended_color_;		// 自分と親のカラーをブレンドしたもの
};


}	// namespace obj
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_OBJECT_BM3_OBJ_NODEBASE_H
