#ifndef BM3_SDK_INC_BM3_CORE_PRIMITIVE_BM3_PRIM_PRIMITIVE_H
#define BM3_SDK_INC_BM3_CORE_PRIMITIVE_BM3_PRIM_PRIMITIVE_H

#include "../../system/bm3_Config.h"
#include "../bm3_CoreType.h"
#include "bm3/core/policy/Life/bm3_policy_Life_Normal.h"
#include "bm3/core/policy/Life/bm3_policy_Life_Group.h"
#include "bm3/core/policy/Matrix/bm3_policy_MatrixScale_Constant.h"
#include "bm3/core/policy/Matrix/bm3_policy_MatrixScale_Newtondynamics.h"
#include "bm3/core/policy/Matrix/bm3_policy_MatrixScale_CurveSpeed.h"
#include "bm3/core/policy/Matrix/bm3_policy_MatrixScale_CurveValue.h"
#include "bm3/core/policy/Matrix/bm3_policy_MatrixRotate_Constant.h"
#include "bm3/core/policy/Matrix/bm3_policy_MatrixRotate_Newtondynamics.h"
#include "bm3/core/policy/Matrix/bm3_policy_MatrixRotate_CurveSpeed.h"
#include "bm3/core/policy/Matrix/bm3_policy_MatrixRotate_CurveValue.h"
#include "bm3/core/policy/GenerateShape/bm3_policy_GenerateShape_Point.h"
#include "bm3/core/policy/GenerateShape/bm3_policy_GenerateShape_Cube.h"
#include "bm3/core/policy/GenerateShape/bm3_policy_GenerateShape_Sphere.h"
#include "bm3/core/policy/GenerateShape/bm3_policy_GenerateShape_Cylinder.h"
#include "bm3/core/policy/GenerateShape/bm3_policy_GenerateShape_Model.h"
#include "bm3/core/policy/Matrix/bm3_policy_MatrixTranslate_Constant.h"
#include "bm3/core/policy/Matrix/bm3_policy_MatrixTranslate_Newtondynamics.h"
#include "bm3/core/policy/Matrix/bm3_policy_MatrixTranslate_CurveSpeed.h"
#include "bm3/core/policy/Matrix/bm3_policy_MatrixTranslate_CurveValue.h"
#include "bm3/core/policy/Color/bm3_policy_Color_Constant.h"
#include "bm3/core/policy/Color/bm3_policy_Color_Curve.h"
#include "bm3/core/policy/Color/bm3_policy_Blend_Constant.h"
#include "bm3/core/policy/Color/bm3_policy_Blend_Curve.h"
#include "bm3/core/policy/Texture/bm3_policy_Texture_Nothing.h"
#include "bm3/core/policy/Texture/bm3_policy_Texture_Crop.h"
#include "bm3/core/policy/Texture/bm3_policy_Texture_Scroll.h"
#include "bm3/core/policy/Texture/bm3_policy_Texture_Pattern.h"
#include "bm3/core/policy/Texture/bm3_policy_Texture_Transform.h"
#include "bm3/core/policy/Transform/bm3_policy_Transform_Constant.h"
#include "bm3/core/policy/Transform/bm3_policy_Transform_Curve.h"
#include "bm3/core/policy/Transform/bm3_policy_Transform_Vertex.h"
#include "bm3/core/policy/ModelTexture/bm3_policy_ModelTexture_Scroll.h"
#include "bm3/core/object/bm3_obj_NodeBase.h"
#include "bm3_Primitive_Packet.h"
#include "bm3_Primitive_Common.h"
#include "bm3/core/policy/Matrix/bm3_policy_MatrixScale_SwitchResource.h"
#include "bm3/core/policy/Matrix/bm3_policy_MatrixRotate_SwitchResource.h"
#include "bm3/core/policy/Matrix/bm3_policy_MatrixTranslate_SwitchResource.h"
#include "bm3/core/policy/Color/bm3_policy_Blend_SwitchResource.h"
#include "bm3/core/policy/Color/bm3_policy_Color_SwitchResource.h"
#include "bm3/core/policy/Texture/bm3_policy_Texture_SwitchResource.h"
#include "bm3/core/policy/Transform/bm3_policy_Transform_SwitchResource.h"
#include "bm3/core/field/bm3_field_Common.h"
#include "bm3/core/draw/policy/bm3_draw_policy_Common.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

namespace texture{
namespace ide{
class MovieHandle;
}	// namespace ide
}	// namespace texture

namespace prim{

namespace quad{
struct Primitive{
	const ml::matrix44*	parent_matrix_;
	const ml::vector3d*	parent_scale_;
	const ml::matrix44*	generate_time_parent_matrix_;
	const ml::vector3d*	generate_time_parent_scale_;
	policy::Life_Normal	life_;
	MatrixParam			scale_;
	MatrixParam			rotate_;
	MatrixParam			translate_;
	float				alpha_;
	ColorParam			color_left_top_;
	ColorParam			color_right_top_;
	ColorParam			color_left_bottom_;
	ColorParam			color_right_bottom_;
#ifdef BM3_PLATFORM_psp
	TextureParam		texture_[1];
#else
	TextureParam		texture_[TextureLayer::Max];
#endif
	TransformParam		transform_;
	GenerateShape		generate_shape_;
	ml::vector3d		directional_;
	ml::vector3d		previous_position_;
	field::FieldInfo	field_info_;
	void				*user_object_;
	float				z_value_;

	template<typename ResourceType>
	BM3_FORCEINLINE void	Initialize(const ResourceType &res, ml::random &random){
		life_.Initialize(res, random);
		life_.ForceActive();
		user_object_ = NULL;
		previous_position_.set_unit();
		directional_.set_unit();
	}

	template<typename ResourceType>
	BM3_FORCEINLINE void	SwitchResource(const ResourceType &res, ml::random &random){
		life_.SwitchResource(res, random);
	}

	BM3_FORCEINLINE bool	IsActive() const{		return life_.IsActive();		}
	BM3_FORCEINLINE bool	IsExpired() const{		return life_.IsExpired();		}
	BM3_FORCEINLINE float	LifeRate() const{		return life_.LifeRate();		}
	BM3_FORCEINLINE bool	IsFirstFrame() const{	return life_.IsFirstFrame();	}

	BM3_FORCEINLINE const ml::vector3d&	GetTranslate()	const{	return translate_.value_;	}

	BM3_FORCEINLINE const ml::vector3d&	GetPreviousPosition()	const{	return previous_position_;	}
	BM3_FORCEINLINE const ml::vector3d&	GetDirection()			const{	return directional_;	}
	BM3_FORCEINLINE void SetPreviousPosition(const ml::matrix44 &matrix){
		ml::vector3d	v;
		draw::policy::MakeCenterPosition(v, *this, matrix);
		if(v.is_equal(previous_position_) == false){
			directional_.sub(v ,previous_position_);
			previous_position_	= v;
		}
	}

	BM3_FORCEINLINE void SetParentMatrix(const ml::matrix44 &m)	{	parent_matrix_ = &m;		}
	BM3_FORCEINLINE void SetParentScale(const ml::vector3d &v)	{	parent_scale_ = &v;		}
	BM3_FORCEINLINE const ml::matrix44&	GetParentMatrix()		const{	return *parent_matrix_;		}
	BM3_FORCEINLINE const ml::vector3d&	GetParentScale()		const{	return *parent_scale_;		}

	BM3_FORCEINLINE void SetGenerateTimeParentMatrix(const ml::matrix44 &m)	{	generate_time_parent_matrix_ = &m;		}
	BM3_FORCEINLINE void SetGenerateTimeParentScale(const ml::vector3d &v)	{	generate_time_parent_scale_ = &v;		}
	BM3_FORCEINLINE const ml::matrix44&	GetGenerateTimeParentMatrix()		const{	return *generate_time_parent_matrix_;		}
	BM3_FORCEINLINE const ml::vector3d&	GetGenerateTimeParentScale()		const{	return *generate_time_parent_scale_;		}

	BM3_FORCEINLINE void	SetZ(float z){	z_value_	= z;	}
	BM3_FORCEINLINE float	GetZ() const{	return z_value_;	}
	
#ifdef BM3_PLATFORM_playstation3
} __attribute__ ((aligned(128)));
#else
};	
#endif
}	// namespace quad

namespace stripe{
struct Primitive{
	const ml::matrix44*	parent_matrix_;
	const ml::vector3d*	parent_scale_;
	const ml::matrix44*	generate_time_parent_matrix_;
	const ml::vector3d*	generate_time_parent_scale_;
	policy::Life_Normal	life_;
	MatrixParam			scale_;
	MatrixParam			rotate_;
	MatrixParam			translate_;
	float				alpha_;
	ColorParam			color_left_top_;
	ColorParam			color_right_top_;
#ifdef BM3_PLATFORM_psp
	TextureParam		texture_[1];
#else
	TextureParam		texture_[TextureLayer::Max];
#endif
	GenerateShape		generate_shape_;
	ml::vector3d		previous_position_;
	field::FieldInfo	field_info_;
	void				*user_object_;

	template<typename ResourceType>
	BM3_FORCEINLINE void	Initialize(const ResourceType &res, ml::random &random){
		life_.Initialize(res, random);
		life_.ForceActive();
		user_object_ = NULL;
		previous_position_.set_unit();
	}

	template<typename ResourceType>
	BM3_FORCEINLINE void	SwitchResource(const ResourceType &res, ml::random &random){
		life_.SwitchResource(res, random);
	}

	BM3_FORCEINLINE bool	IsActive() const{		return life_.IsActive();		}
	BM3_FORCEINLINE bool	IsExpired() const{		return life_.IsExpired();		}
	BM3_FORCEINLINE float	LifeRate() const{		return life_.LifeRate();		}
	BM3_FORCEINLINE bool	IsFirstFrame() const{	return life_.IsFirstFrame();	}

	BM3_FORCEINLINE const ml::vector3d&	GetTranslate()	const{	return translate_.value_;	}
	BM3_FORCEINLINE const ml::vector3d&	GetPreviousPosition()	const{	return previous_position_;	}
	BM3_FORCEINLINE void SetPreviousPosition(const ml::matrix44 &matrix){
		ml::vector3d	v;
		draw::policy::MakeCenterPosition(v, *this, matrix);
		if(v.is_equal(previous_position_) == false){
			previous_position_	= v;
		}
	}

	BM3_FORCEINLINE void SetParentMatrix(const ml::matrix44 &m)	{	parent_matrix_ = &m;		}
	BM3_FORCEINLINE void SetParentScale(const ml::vector3d &v)	{	parent_scale_ = &v;		}
	BM3_FORCEINLINE const ml::matrix44&	GetParentMatrix()		const{	return *parent_matrix_;		}
	BM3_FORCEINLINE const ml::vector3d&	GetParentScale()		const{	return *parent_scale_;		}

	BM3_FORCEINLINE void SetGenerateTimeParentMatrix(const ml::matrix44 &m)	{	generate_time_parent_matrix_ = &m;		}
	BM3_FORCEINLINE void SetGenerateTimeParentScale(const ml::vector3d &v)	{	generate_time_parent_scale_ = &v;		}
	BM3_FORCEINLINE const ml::matrix44&	GetGenerateTimeParentMatrix()		const{	return *generate_time_parent_matrix_;		}
	BM3_FORCEINLINE const ml::vector3d&	GetGenerateTimeParentScale()		const{	return *generate_time_parent_scale_;		}

#ifdef BM3_PLATFORM_playstation3
} __attribute__ ((aligned(128)));
#else
};	
#endif
}	// namespace stripe

namespace particle{
struct Primitive{
	const ml::matrix44*	parent_matrix_;
	const ml::vector3d*	parent_scale_;
	const ml::matrix44*	generate_time_parent_matrix_;
	const ml::vector3d*	generate_time_parent_scale_;
	policy::Life_Normal	life_;
	MatrixParam			scale_;
	MatrixParam			rotate_;
	MatrixParam			translate_;
	float				alpha_;
	ColorParam			color_center_;
#ifdef BM3_PLATFORM_psp
	TextureParam		texture_[1];
#else
	TextureParam		texture_[TextureLayer::Max];
#endif
	GenerateShape		generate_shape_;
	ml::vector3d		previous_position_;
	field::FieldInfo	field_info_;
	void				*user_object_;
	float				z_value_;

	template<typename ResourceType>
	BM3_FORCEINLINE void	Initialize(const ResourceType &res, ml::random &random){
		life_.Initialize(res, random);
		life_.ForceActive();
		user_object_ = NULL;
		previous_position_.set_unit();
	}

	template<typename ResourceType>
	BM3_FORCEINLINE void	SwitchResource(const ResourceType &res, ml::random &random){
		life_.SwitchResource(res, random);
	}

	BM3_FORCEINLINE bool	IsActive() const{		return life_.IsActive();		}
	BM3_FORCEINLINE bool	IsExpired() const{		return life_.IsExpired();		}
	BM3_FORCEINLINE float	LifeRate() const{		return life_.LifeRate();		}
	BM3_FORCEINLINE bool	IsFirstFrame() const{	return life_.IsFirstFrame();	}

	BM3_FORCEINLINE const ml::vector3d&	GetTranslate()	const{	return translate_.value_;	}
	BM3_FORCEINLINE const ml::vector3d&	GetPreviousPosition()	const{	return previous_position_;	}
	BM3_FORCEINLINE void SetPreviousPosition(const ml::matrix44 &matrix){
		ml::vector3d	v;
		draw::policy::MakeCenterPosition(v, *this, matrix);
		if(v.is_equal(previous_position_) == false){
			previous_position_	= v;
		}
	}

	BM3_FORCEINLINE void SetParentMatrix(const ml::matrix44 &m)	{	parent_matrix_ = &m;		}
	BM3_FORCEINLINE void SetParentScale(const ml::vector3d &v)	{	parent_scale_ = &v;		}
	BM3_FORCEINLINE const ml::matrix44&	GetParentMatrix()		const{	return *parent_matrix_;		}
	BM3_FORCEINLINE const ml::vector3d&	GetParentScale()		const{	return *parent_scale_;		}

	BM3_FORCEINLINE void SetGenerateTimeParentMatrix(const ml::matrix44 &m)	{	generate_time_parent_matrix_ = &m;		}
	BM3_FORCEINLINE void SetGenerateTimeParentScale(const ml::vector3d &v)	{	generate_time_parent_scale_ = &v;		}
	BM3_FORCEINLINE const ml::matrix44&	GetGenerateTimeParentMatrix()		const{	return *generate_time_parent_matrix_;		}
	BM3_FORCEINLINE const ml::vector3d&	GetGenerateTimeParentScale()		const{	return *generate_time_parent_scale_;		}

	BM3_FORCEINLINE void	SetZ(float z){	z_value_	= z;	}
	BM3_FORCEINLINE float	GetZ() const{	return z_value_;	}
	
#ifdef BM3_PLATFORM_playstation3
} __attribute__ ((aligned(128)));
#else
};	
#endif
}	// namespace particle

namespace simpleparticle{
struct Primitive{
	const ml::matrix44*	parent_matrix_;
	const ml::vector3d*	parent_scale_;
	const ml::matrix44*	generate_time_parent_matrix_;
	const ml::vector3d*	generate_time_parent_scale_;
	policy::Life_Normal	life_;
	ScalarParam			scale_;									// MatrixParam
	ScalarParam			rotate_;								// MatrixParam
	MatrixParam			translate_;
	float				alpha_;
	ColorParam			color_center_;
	GenerateShape		generate_shape_;
	ml::vector3d		previous_position_;
	field::FieldInfo	field_info_;
	void				*user_object_;
	float				z_value_;

	template<typename ResourceType>
	BM3_FORCEINLINE void	Initialize(const ResourceType &res, ml::random &random){
		life_.Initialize(res, random);
		life_.ForceActive();
		user_object_ = NULL;
		previous_position_.set_unit();
	}

	template<typename ResourceType>
	BM3_FORCEINLINE void	SwitchResource(const ResourceType &res, ml::random &random){
		life_.SwitchResource(res, random);
	}

	BM3_FORCEINLINE bool	IsActive() const{		return life_.IsActive();		}
	BM3_FORCEINLINE bool	IsExpired() const{		return life_.IsExpired();		}
	BM3_FORCEINLINE float	LifeRate() const{		return life_.LifeRate();		}
	BM3_FORCEINLINE bool	IsFirstFrame() const{	return life_.IsFirstFrame();	}

	BM3_FORCEINLINE const ml::vector3d&	GetTranslate()	const{	return translate_.value_;	}
	BM3_FORCEINLINE const ml::vector3d&	GetPreviousPosition()	const{	return previous_position_;	}
	BM3_FORCEINLINE void SetPreviousPosition(const ml::matrix44 &matrix){
		ml::vector3d	v;
		draw::policy::MakeCenterPosition(v, *this, matrix);
		if(v.is_equal(previous_position_) == false){
			previous_position_	= v;
		}
	}

	BM3_FORCEINLINE void SetParentMatrix(const ml::matrix44 &m)	{	parent_matrix_ = &m;		}
	BM3_FORCEINLINE void SetParentScale(const ml::vector3d &v)	{	parent_scale_ = &v;		}
	BM3_FORCEINLINE const ml::matrix44&	GetParentMatrix()		const{	return *parent_matrix_;		}
	BM3_FORCEINLINE const ml::vector3d&	GetParentScale()		const{	return *parent_scale_;		}

	BM3_FORCEINLINE void SetGenerateTimeParentMatrix(const ml::matrix44 &m)	{	generate_time_parent_matrix_ = &m;		}
	BM3_FORCEINLINE void SetGenerateTimeParentScale(const ml::vector3d &v)	{	generate_time_parent_scale_ = &v;		}
	BM3_FORCEINLINE const ml::matrix44&	GetGenerateTimeParentMatrix()		const{	return *generate_time_parent_matrix_;		}
	BM3_FORCEINLINE const ml::vector3d&	GetGenerateTimeParentScale()		const{	return *generate_time_parent_scale_;		}

	BM3_FORCEINLINE void	SetZ(float z){	z_value_	= z;	}
	BM3_FORCEINLINE float	GetZ() const{	return z_value_;	}

#ifdef BM3_PLATFORM_playstation3
} __attribute__ ((aligned(128)));
#else
};	
#endif
}	// namespace simpleparticle


namespace massparticle{
struct Primitive{
	const ml::matrix44*	parent_matrix_;
	const ml::vector3d*	parent_scale_;
	const ml::matrix44*	generate_time_parent_matrix_;
	const ml::vector3d*	generate_time_parent_scale_;
	policy::Life_Group	life_;
	float				generate_scale_;	// ê∂ê¨êîÇÃäÑçá
	float				scale_;				// ä|ÇØçáÇÌÇ≥ÇÍÇÈ
	ml::vector3d		position_;			// ë´ÇµçáÇÌÇ≥ÇÍÇÈ
	float				velocity_;			// ë´ÇµçáÇÌÇ≥ÇÍÇÈ
	ml::vector3d		gravity_;			// ë´ÇµçáÇÌÇ≥ÇÍÇÈ
	float				deceleration_;		// ä|ÇØçáÇÌÇ≥ÇÍÇÈ
	float				noise_scale_;		// ä|ÇØçáÇÌÇ≥ÇÍÇÈ
	float				noise_frequency_;	// ä|ÇØçáÇÌÇ≥ÇÍÇÈ
	ColorParam			color_center_;		// ä|ÇØçáÇÌÇ≥ÇÍÇÈ
	float				alpha_;				// ä|ÇØçáÇÌÇ≥ÇÍÇÈ
	void				*user_object_;
	bool				is_updated_;
	float				z_value_;
	float				update_frame_;

	template<typename ResourceType>
	BM3_FORCEINLINE void	Initialize(const ResourceType & res, ml::random & random){
		life_.Initialize(res, random);
		user_object_ = NULL;
	}

	template<typename ResourceType>
	BM3_FORCEINLINE void	SwitchResource(const ResourceType &res, ml::random &random){
		life_.SwitchResource(res, random);
	}

	BM3_FORCEINLINE bool	IsActive() const{		return life_.IsActive();		}
	BM3_FORCEINLINE bool	IsExpired() const{		return life_.IsExpired();		}
	BM3_FORCEINLINE float	LifeRate() const{		return life_.LifeRate();		}
	BM3_FORCEINLINE bool	IsFirstFrame() const{	return life_.IsFirstFrame();	}


	BM3_FORCEINLINE void SetParentMatrix(const ml::matrix44 &m)	{	parent_matrix_ = &m;		}
	BM3_FORCEINLINE void SetParentScale(const ml::vector3d &v)	{	parent_scale_ = &v;		}
	BM3_FORCEINLINE const ml::matrix44&	GetParentMatrix()		const{	return *parent_matrix_;		}
	BM3_FORCEINLINE const ml::vector3d&	GetParentScale()		const{	return *parent_scale_;		}

	BM3_FORCEINLINE void SetGenerateTimeParentMatrix(const ml::matrix44 &m)	{	generate_time_parent_matrix_ = &m;		}
	BM3_FORCEINLINE void SetGenerateTimeParentScale(const ml::vector3d &v)	{	generate_time_parent_scale_ = &v;		}
	BM3_FORCEINLINE const ml::matrix44&	GetGenerateTimeParentMatrix()		const{	return *generate_time_parent_matrix_;		}
	BM3_FORCEINLINE const ml::vector3d&	GetGenerateTimeParentScale()		const{	return *generate_time_parent_scale_;		}

	BM3_FORCEINLINE void	SetZ(float z){	z_value_	= z;	}
	BM3_FORCEINLINE float	GetZ() const{	return z_value_;	}

#ifdef BM3_PLATFORM_playstation3
} __attribute__ ((aligned(128)));
#else
};	
#endif
}	// namespace massparticle

namespace model{
struct Primitive{
	const ml::matrix44*	parent_matrix_;
	const ml::vector3d*	parent_scale_;
	const ml::matrix44*	generate_time_parent_matrix_;
	const ml::vector3d*	generate_time_parent_scale_;
	policy::Life_Normal	life_;
	MatrixParam			scale_;
	MatrixParam			rotate_;
	MatrixParam			translate_;
	float				alpha_;
	ColorParam			color_center_;
	ModelTextureParam	texture_[1];
	GenerateShape		generate_shape_;
	ml::vector3d		previous_position_;
	ml::vector3d		directional_;
	field::FieldInfo	field_info_;
	void				*user_object_;
	float				z_value_;

	template<typename ResourceType>
	BM3_FORCEINLINE void	Initialize(const ResourceType &res, ml::random &random){
		life_.Initialize(res, random);
		life_.ForceActive();
		user_object_ = NULL;
		previous_position_.set_unit();
	}

	template<typename ResourceType>
	BM3_FORCEINLINE void	SwitchResource(const ResourceType &res, ml::random &random){
		life_.SwitchResource(res, random);
	}

	BM3_FORCEINLINE bool	IsActive() const{		return life_.IsActive();		}
	BM3_FORCEINLINE bool	IsExpired() const{		return life_.IsExpired();		}
	BM3_FORCEINLINE float	LifeRate() const{		return life_.LifeRate();		}
	BM3_FORCEINLINE float	LifeFrame() const {		return life_.LifeFrame();		}
	BM3_FORCEINLINE bool	IsFirstFrame() const{	return life_.IsFirstFrame();	}

	BM3_FORCEINLINE const ml::vector3d&	GetTranslate()	const{	return translate_.value_;	}

	BM3_FORCEINLINE const ml::vector3d&	GetPreviousPosition() const{	return previous_position_;	}
	BM3_FORCEINLINE const ml::vector3d&	GetDirection()			const{	return directional_;	}
	BM3_FORCEINLINE void SetPreviousPosition(const ml::matrix44 &matrix){
		ml::vector3d	v;
		draw::policy::MakeCenterPosition(v, *this, matrix);
		if(v.is_equal(previous_position_) == false){
			directional_.sub(v ,previous_position_);
			previous_position_	= v;
		}
	}

	BM3_FORCEINLINE void SetParentMatrix(const ml::matrix44 &m)	{	parent_matrix_ = &m;		}
	BM3_FORCEINLINE void SetParentScale(const ml::vector3d &v)	{	parent_scale_ = &v;		}
	BM3_FORCEINLINE const ml::matrix44&	GetParentMatrix()		const{	return *parent_matrix_;		}
	BM3_FORCEINLINE const ml::vector3d&	GetParentScale()		const{	return *parent_scale_;		}

	BM3_FORCEINLINE void SetGenerateTimeParentMatrix(const ml::matrix44 &m)	{	generate_time_parent_matrix_ = &m;		}
	BM3_FORCEINLINE void SetGenerateTimeParentScale(const ml::vector3d &v)	{	generate_time_parent_scale_ = &v;		}
	BM3_FORCEINLINE const ml::matrix44&	GetGenerateTimeParentMatrix()		const{	return *generate_time_parent_matrix_;		}
	BM3_FORCEINLINE const ml::vector3d&	GetGenerateTimeParentScale()		const{	return *generate_time_parent_scale_;		}

	BM3_FORCEINLINE void	SetZ(float z){	z_value_	= z;	}
	BM3_FORCEINLINE float	GetZ() const{	return z_value_;	}
#ifdef BM3_PLATFORM_playstation3
} __attribute__ ((aligned(128)));
#else
};	
#endif
}	// namespace model

}	// namespace prim

template<NodeTypeConst N>	struct NodeTypeToPrimitive{};
template<>	 				struct NodeTypeToPrimitive<NodeTypeConst_EmitterQuad>{				typedef prim::quad::Primitive			Type;	};
template<>	 				struct NodeTypeToPrimitive<NodeTypeConst_EmitterParticle>{			typedef prim::particle::Primitive		Type;	};
template<>	 				struct NodeTypeToPrimitive<NodeTypeConst_EmitterSimpleParticle>{	typedef prim::simpleparticle::Primitive	Type;	};
template<>	 				struct NodeTypeToPrimitive<NodeTypeConst_EmitterMassParticle>{		typedef prim::massparticle::Primitive	Type;	};
template<>	 				struct NodeTypeToPrimitive<NodeTypeConst_EmitterModel>{				typedef prim::model::Primitive			Type;	};
template<>	 				struct NodeTypeToPrimitive<NodeTypeConst_EmitterStripe>{			typedef prim::stripe::Primitive			Type;	};

BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_PRIMITIVE_BM3_PRIM_PRIMITIVE_H
