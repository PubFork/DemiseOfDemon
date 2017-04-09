#ifndef BM3_SDK_INC_BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_COMMON_H
#define BM3_SDK_INC_BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_COMMON_H

#include "bm3/system/bm3_Config.h"
#include "../../bm3_DrawInfo.h"
#include "../../bm3_CoreType.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE
namespace draw{
namespace policy{

template<typename PrimitiveType>
BM3_FORCEINLINE void MakeCenterPosition(ml::vector3d &dst, const PrimitiveType &prim, const ml::matrix44 &matrix){

	dst.mul(prim.translate_.value_, matrix);
	dst.add(prim.field_info_.position_);
}

template<typename PrimitiveType>
BM3_FORCEINLINE void MakeCenterPositionNoField(ml::vector3d &dst, const PrimitiveType &prim, const ml::matrix44 &matrix){

	dst.mul(prim.translate_.value_, matrix);
}

template<typename PrimitiveType>
BM3_FORCEINLINE void MakeMatrixTranslate(ml::matrix44 &dst, const PrimitiveType &prim ,const ml::matrix44 &matrix){

	dst.set_translate(prim.translate_.value_);
	dst.mul(matrix);
	dst.add_translate(prim.field_info_.position_);
}
template<typename PrimitiveType>
BM3_FORCEINLINE void MakeMatrixTranslateNoField(ml::matrix44 &dst, const PrimitiveType &prim ,const ml::matrix44 &matrix){

	dst.set_translate(prim.translate_.value_);
	dst.mul(matrix);
}

template<typename PrimitiveType>
BM3_FORCEINLINE void MakeDirectionalMatrix(ml::matrix44 &dst, const PrimitiveType &prim, const ml::vector3d &position){

	ml::vector3d	direction = position;
	if(direction.is_equal(prim.GetPreviousPosition())){	direction	= prim.GetDirection();				}
	else{												direction.sub(prim.GetPreviousPosition());		}
	direction.normalize();

	float			angle;
	ml::vector3d	axis;
	ml::vector3d	yp( 0.0f, +1.0f,  0.0f, 1.0f);

	axis	= yp.cross(direction);
	angle	= ml::acosf(yp.dot(direction));

	if(axis.length_sq()!=0.0f)	dst.mul_rot_axis(angle ,axis.normalize());
	else						dst.mul_rot_axis(angle ,ml::vector3d( 0.0f,  0.0f, +1.0f, 1.0f));
}

template<typename PrimitiveType>
BM3_FORCEINLINE void MakeDierctionalPolylineMatrix(ml::matrix44 &dst, const PrimitiveType &prim, ml::vector3d &position, DrawInfo &draw_info){

	// �ŏI�I�Ȉʒu���猻�݌������Ă������������o��
	ml::vector3d	direction	= position;
	{
		if(direction.is_equal(prim.GetPreviousPosition())){	direction	= prim.GetDirection();				}
		else{												direction.sub(prim.GetPreviousPosition());		}
		direction.normalize();

		// �i�s�����ւ̉�]���Ɗp�x���v�Z����
		float			angle;
		ml::vector3d	axis;
		ml::vector3d	yp( 0.0f, +1.0f,  0.0f, 1.0f);
		axis	= yp.cross(direction);
		angle	= ml::acosf(yp.dot(direction));

		if(axis.length_sq()!=0.0f)	dst.mul_rot_axis(angle ,axis.normalize());
		else						dst.mul_rot_axis(angle ,ml::vector3d( 0.0f,  0.0f, +1.0f, 1.0f));
	}
	{
		/*
			�i�s���������ɉ�]������
			�@�������A�J������������i�s�����̐���������
			���̓�̃x�N�g���̓��ς�����Ă��̊Ԃ̊p�x�����o��
				�i�s���������������Ă��̊Ԃ̊p�x�����߂�
			��������p�x���킩�邪�ǂ����ɉ�]����Ηǂ����킩��Ȃ�
			�i�s�����x�N�g���Ɓu�J���������Ɩ@�������̊O�ρv�x�N�g���̓��ς̕�������ǂ�����]����Ηǂ����킩��
		*/
		// �|���S���̖@���������v�Z
		ml::vector3d	z( 0.0f, 0.0f,  +1.0f, 1.0f);	// �|���S���̃f�t�H���g�̖@������
		z.mul33(dst).normalize();	// �r���{�[�h���|����O�̌����Ă���ʂ̕���
		// �i�s�����̐���������
		ml::vector3d z_sub = direction;
		z_sub.mul(z.dot(direction));
		z.sub(z_sub).normalize();

		// �J�����̕������v�Z
#ifdef BM3_TARGET_IDE
		ml::vector3d	camera_dir;
		camera_dir = draw_info.ViewMatrix().translate();
		camera_dir.mul(draw_info.InverseViewMatrix());
		camera_dir.mul(-1.0f);	// ��������Ȃ��ƌ���������Ȃ�
#else
		ml::vector3d	camera_dir = draw_info.InverseViewMatrix().translate();	// �J�����̂������
#endif
		camera_dir.sub(position).normalize();
		// �i�s�����̐���������
		ml::vector3d c_sub = direction;
		c_sub.mul(camera_dir.dot(direction));
		camera_dir.sub(c_sub).normalize();

		// �i�s����������̉�]�p�x���v�Z
		float angle = ml::acosf(z.dot(camera_dir));

		// ��]����������
		if(direction.dot(camera_dir.cross(z))>0){
			angle = -angle;
		}

		// �f�B���N�V���i���̌v�Z�ɉ�]��������
		ml::matrix44 billboard;
		billboard.set_rot_axis(angle, direction);
		billboard.set_translate(0,0,0);
		dst.mul33(billboard);
	}
}

// �S�p�`���R�p�`�Q�ɂ����Ƃ��̒��_�C���f�b�N�X
struct QuadTo2Triangle{
	enum Type{
		_00	= 0,
		_01	= 1,
		_02	= 2,
		_10	= 0,
		_11	= 2,
		_12	= 3
	};
};

struct	VectorType{
	float	x_;
	float	y_;
	float	z_;
};

typedef	VectorType	PositionType;
typedef	VectorType	TangentType;
typedef	VectorType	BinormalType;
typedef	VectorType	NormalType;

#ifdef BM3_PLATFORM_psvita
struct	TangentSpaceType{
	NormalType		normal_;
	TangentType		tangent_;
	BinormalType	binormal_;
};
#else
struct	TangentSpaceType{
	TangentType		tangent_;
	BinormalType	binormal_;
	NormalType		normal_;
};
#endif

struct	ByteColorArgb{

	typedef	ml::ui8	ChannelType;
	typedef struct _Element{
		ml::ui8	a_;
		ml::ui8	r_;
		ml::ui8	g_;
		ml::ui8	b_;
	} Element;

	union{
		Element		e_;
		ml::ui32	rgba_;
	};
};

struct	ByteColorBgra{

	typedef	ml::ui8	ChannelType;
	typedef struct _Element{
		ml::ui8	b_;
		ml::ui8	g_;
		ml::ui8	r_;
		ml::ui8	a_;
	} Element;

	union{
		Element		e_;
		ml::ui32	rgba_;
	};
};

struct	ByteColorRgba{

	typedef	ml::ui8	ChannelType;
	typedef struct _Element{
		ml::ui8	r_;
		ml::ui8	g_;
		ml::ui8	b_;
		ml::ui8	a_;
	} Element;

	union{
		Element		e_;
		ml::ui32	rgba_;
	};
};
	
struct VertexElement{
	enum Type{
#ifdef BM3_PLATFORM_win_directx9
		PC0 = 0,	PC1 = 1,	PC2 = 2,	PC3 = 3,	T0 = 0,	T1 = 1,	T2 = 2,	T3 = 3
#endif
#ifdef BM3_PLATFORM_win_opengl
		PC0 = 0,	PC1 = 1,	PC2 = 2,	PC3 = 3,	T0 = 0,	T1 = 1,	T2 = 2,	T3 = 3
#endif
#ifdef BM3_PLATFORM_playstation3
		PC0 = 0,	PC1 = 1,	PC2 = 2,	PC3 = 3,	T0 = 0,	T1 = 1,	T2 = 2,	T3 = 3
#endif
#ifdef BM3_PLATFORM_xbox360
		PC0 = 0,	PC1 = 1,	PC2 = 2,	PC3 = 3,	T0 = 0,	T1 = 1,	T2 = 2,	T3 = 3
#endif
#ifdef BM3_PLATFORM_psp
		PC0 = 1,	PC1 = 0,	PC2 = 2,	PC3 = 3,	T0 = 1,	T1 = 0,	T2 = 2,	T3 = 3
#endif
#ifdef BM3_PLATFORM_gamebryodx9
		PC0 = 0,	PC1 = 1,	PC2 = 2,	PC3 = 3,	T0 = 0,	T1 = 1,	T2 = 2,	T3 = 3
#endif
#ifdef BM3_PLATFORM_opengles
		PC0 = 0,	PC1 = 1,	PC2 = 2,	PC3 = 3,	T0 = 0,	T1 = 1,	T2 = 2,	T3 = 3
#endif
#ifdef BM3_PLATFORM_nw4c
		PC0 = 0,	PC1 = 1,	PC2 = 2,	PC3 = 3,	T0 = 0,	T1 = 1,	T2 = 2,	T3 = 3
#endif
#ifdef BM3_PLATFORM_psvita
		PC0 = 0,	PC1 = 1,	PC2 = 2,	PC3 = 3,	T0 = 0,	T1 = 1,	T2 = 2,	T3 = 3
#endif
	};
};

#ifdef BM3_PLATFORM_nw4c
#define SET_V(v) ( (1.0f) - (v) )
#else
#define SET_V(v) ((v))	
#endif
}	// namespace policy
}	// namespace draw
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_COMMON_H
