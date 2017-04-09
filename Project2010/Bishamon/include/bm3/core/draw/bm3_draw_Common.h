#ifndef BM3_SDK_INC_BM3_CORE_DRAW_BM3_DRAW_COMMON_H
#define BM3_SDK_INC_BM3_CORE_DRAW_BM3_DRAW_COMMON_H

#include "../../system/bm3_Config.h"
#include "../bm3_CoreType.h"
#include "ml/utility/ml_offset_pointer.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE
namespace draw{

/// @brief BeginInfoƒNƒ‰ƒX
struct	BeginInfo{
	BeginInfo(){}
	BeginInfo(
		const ColorType	&color_scale,
		const ml::vector3d &offset_position
	) :	color_scale_(color_scale),
		offset_position_(offset_position){;}

	ColorType		color_scale_;
	ml::vector3d	offset_position_;
};

#if defined(BM3_PLATFORM_xbox360)

BM3_FORCEINLINE void MulRotXYZ(ml::matrix44 &m, const ml::vector3d &r){

	int ix = _CountLeadingZeros((int &)r.e_.x_);
	int iy = _CountLeadingZeros((int &)r.e_.y_);
	int iz = _CountLeadingZeros((int &)r.e_.z_);

	int s =
		((ix >> (5-0))         ) |
		((iy >> (5-1)) & (1<<1)) |
		((iz >> (5-2)) & (1<<2));

	switch(s){
		case ((0<<0)|(0<<1)|(0<<2)):{	m.mul_rot_x_nozero(r.e_.x_);	m.mul_rot_y_nozero(r.e_.y_);	m.mul_rot_z_nozero(r.e_.z_);	break;	}
		case ((1<<0)|(0<<1)|(0<<2)):{									m.mul_rot_y_nozero(r.e_.y_);	m.mul_rot_z_nozero(r.e_.z_);	break;	}
		case ((0<<0)|(1<<1)|(0<<2)):{	m.mul_rot_x_nozero(r.e_.x_);									m.mul_rot_z_nozero(r.e_.z_);	break;	}
		case ((1<<0)|(1<<1)|(0<<2)):{																	m.mul_rot_z_nozero(r.e_.z_);	break;	}
		case ((0<<0)|(0<<1)|(1<<2)):{	m.mul_rot_x_nozero(r.e_.x_);	m.mul_rot_y_nozero(r.e_.y_);									break;	}
		case ((1<<0)|(0<<1)|(1<<2)):{									m.mul_rot_y_nozero(r.e_.y_);									break;	}
		case ((0<<0)|(1<<1)|(1<<2)):{	m.mul_rot_x_nozero(r.e_.x_);																	break;	}
		case ((1<<0)|(1<<1)|(1<<2)):{																									break;	}
		default:{						ML_ASSERT(false);	BM3_ASSUME(0);																break;	}
	}
}

BM3_FORCEINLINE void MulRotZXY(ml::matrix44 &m, const ml::vector3d &r){

	int ix = _CountLeadingZeros((int &)r.e_.x_);
	int iy = _CountLeadingZeros((int &)r.e_.y_);
	int iz = _CountLeadingZeros((int &)r.e_.z_);

	int s =
		((ix >> (5-0))         ) |
		((iy >> (5-1)) & (1<<1)) |
		((iz >> (5-2)) & (1<<2));

	switch(s){
		case ((0<<0)|(0<<1)|(0<<2)):{	m.mul_rot_z_nozero(r.e_.z_);	m.mul_rot_x_nozero(r.e_.x_);	m.mul_rot_y_nozero(r.e_.y_);	break;	}
		case ((1<<0)|(0<<1)|(0<<2)):{	m.mul_rot_z_nozero(r.e_.z_);									m.mul_rot_y_nozero(r.e_.y_);	break;	}
		case ((0<<0)|(1<<1)|(0<<2)):{	m.mul_rot_z_nozero(r.e_.z_);	m.mul_rot_x_nozero(r.e_.x_);									break;	}
		case ((1<<0)|(1<<1)|(0<<2)):{	m.mul_rot_z_nozero(r.e_.z_);																	break;	}
		case ((0<<0)|(0<<1)|(1<<2)):{									m.mul_rot_x_nozero(r.e_.x_);	m.mul_rot_y_nozero(r.e_.y_);	break;	}
		case ((1<<0)|(0<<1)|(1<<2)):{																	m.mul_rot_y_nozero(r.e_.y_);	break;	}
		case ((0<<0)|(1<<1)|(1<<2)):{									m.mul_rot_x_nozero(r.e_.x_);									break;	}
		case ((1<<0)|(1<<1)|(1<<2)):{																									break;	}
		default:{						ML_ASSERT(false);	BM3_ASSUME(0);																break;	}
	}
}

#define	BM3_M_PackVector4ToD3DCOLOR(input, pDest)	{					\
	__vector4 result = __vmaddfp( input, vPackMul8888, vPackAdd );		\
	result = __vpkd3d( result, result, VPACK_D3DCOLOR, VPACK_32, 0 );	\
	result = __vspltw( result, 3 );										\
	__stvewx( result, (DWORD *)pDest, 0 );								\
}

#endif


}	// namespace draw
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_DRAW_BM3_DRAW_COMMON_H
