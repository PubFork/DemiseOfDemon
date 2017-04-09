#ifndef BM3_SDK_INC_BM3_CORE_PRIMITIVE_BM3_PRIM_PRIMITIVE_COMMON_H
#define BM3_SDK_INC_BM3_CORE_PRIMITIVE_BM3_PRIM_PRIMITIVE_COMMON_H

#include "../bm3_CoreType.h"
#include "../bm3_Const.h"
#include "../object/bm3_obj_NodeBase.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

//class obj::NodeBase;	

namespace prim{

/// @brief MatrixParam�N���X
struct MatrixParam{
	ml::ni::vector3d	value_;
	ml::ni::vector3d	speed_;
	ml::ni::vector3d	acceleration_;

	void Initialize0(){
		value_.set_unit();
		speed_.set_unit();
		acceleration_.set_unit();
	}

	BM3_FORCEINLINE float	ValueX() const{	return value_.e_.x_;	}
	BM3_FORCEINLINE float	ValueY() const{	return value_.e_.y_;	}
	BM3_FORCEINLINE float	ValueZ() const{	return value_.e_.z_;	}
};

struct ScalarParam{
	float				value_;
	float				speed_;
	float				acceleration_;

	void Initialize0(){
		value_			= 0;
		speed_			= 0;
		acceleration_	= 0;
	}

	BM3_FORCEINLINE float	ValueX() const{	return value_;	}
	BM3_FORCEINLINE float	ValueY() const{	return value_;	}
	BM3_FORCEINLINE float	ValueZ() const{	return value_;	}
};

struct ColorParam{
	ColorType			value_;
};

/// @brief	�e�N�X�`���A�j���[�V���������̍\����
struct TextureParam{
	int					frame_;		//!< �N���b�v�̂h�c
	float				step_;		//!< �N���b�v�؂�ւ��h�c
	float				h_scroll_;	//!< ���������̃X�N���[�����W
	float				v_scroll_;	//!< ���������̃X�N���[�����W
	TextureCrop			value_;		//!< �N���b�v�̗̈�
};

/// @brief ModelTextureParam�\����
struct ModelTextureParam{
	float				h_scroll_;
	float				v_scroll_;
};

/// @brief TransformParam�\����
struct TransformParam{
	ml::ni::vector3d	left_top_;
	ml::ni::vector3d	right_top_;
	ml::ni::vector3d	left_bottom_;
	ml::ni::vector3d	right_bottom_;
};

/// @brief GenerateShape�\����
struct GenerateShape{
	ml::ni::vector3d	position_;
	ml::ni::vector3d	direction_;
};

/// @brief ParentParam�N���X
struct ParentParam{
public:
	void	SetBase(obj::NodeBase *base){	base_			= base;		}

	obj::NodeBase		&Base()			const { return *base_;		}
	ColorType&	ParentColor()	const { return base_->BlendColor();	}

private:
	obj::NodeBase*		base_;
};

}	// namespace prim


BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_PRIMITIVE_BM3_PRIM_PRIMITIVE_COMMON_H
