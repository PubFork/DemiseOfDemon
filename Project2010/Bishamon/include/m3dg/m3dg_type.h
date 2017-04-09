#ifndef M3DG_M3DG_TYPE_H
#define M3DG_M3DG_TYPE_H

#if defined(M3DG_PLATFORM_win_opengl)
	#include "!opengl/bm3_TypePlatformDependency.h"
#endif
#if defined(M3DG_PLATFORM_opengles)
	#include "!opengles/bm3_TypePlatformDependency.h"
#endif
#if defined(M3DG_PLATFORM_win_directx9)
	#include "!directx9/bm3_TypePlatformDependency.h"
#endif
#if defined(M3DG_PLATFORM_win_directx10)
	#include "!directx10/bm3_TypePlatformDependency.h"
#endif
#if defined(M3DG_PLATFORM_playstation3)
	#include "!playstation3/bm3_TypePlatformDependency.h"
#endif
#if defined(M3DG_PLATFORM_xbox360)
	#include "!xbox360/bm3_TypePlatformDependency.h"
#endif
#if defined(M3DG_PLATFORM_psp)
	#include "!psp/bm3_TypePlatformDependency.h"
#endif
#if defined(M3DG_PLATFORM_gamebryodx9)
	#include "!gamebryodx9/bm3_TypePlatformDependency.h"
#endif
#if defined(M3DG_PLATFORM_opengles)
	#include "!opengles/bm3_TypePlatformDependency.h"
#endif
#if defined(M3DG_PLATFORM_nw4c)
	#include "!nw4c/bm3_TypePlatformDependency.h"
#endif
#if defined(M3DG_PLATFORM_psvita)
	#include "!psvita/bm3_TypePlatformDependency.h"
#endif

// bm3 sdk�Ŏg����Ƃ��͎����I��
#if defined(BM3_PLATFORM_win_opengl)
	#include "!opengl/bm3_TypePlatformDependency.h"
#endif
#if defined(BM3_PLATFORM_opengles)
	#include "!opengles/bm3_TypePlatformDependency.h"
#endif
#if defined(BM3_PLATFORM_win_directx9)
	#include "!directx9/bm3_TypePlatformDependency.h"
#endif
#if defined(BM3_PLATFORM_win_directx10)
	#include "!directx10/bm3_TypePlatformDependency.h"
#endif
#if defined(BM3_PLATFORM_playstation3)
	#include "!playstation3/bm3_TypePlatformDependency.h"
#endif
#if defined(BM3_PLATFORM_xbox360)
	#include "!xbox360/bm3_TypePlatformDependency.h"
#endif
#if defined(BM3_PLATFORM_psp)
	#include "!psp/bm3_TypePlatformDependency.h"
#endif
#if defined(BM3_PLATFORM_gamebryodx9)
	#include "!gamebryodx9/bm3_TypePlatformDependency.h"
#endif
#if defined(BM3_PLATFORM_opengles)
	#include "!opengles/bm3_TypePlatformDependency.h"
#endif
#if defined(BM3_PLATFORM_nw4c)
	#include "!nw4c/bm3_TypePlatformDependency.h"
#endif
#if defined(BM3_PLATFORM_psvita)
	#include "!psvita/bm3_TypePlatformDependency.h"
#endif

#if !defined(M3DG_PLATFORM_win_opengl) && !defined(M3DG_PLATFORM_win_directx9) && !defined(M3DG_PLATFORM_win_directx10) && !defined(M3DG_PLATFORM_playstation3) && !defined(M3DG_PLATFORM_xbox360) && !defined(M3DG_PLATFORM_psp) && !defined(M3DG_PLATFORM_gamebryodx9) && !defined(M3DG_PLATFORM_opengles) && !defined(M3DG_PLATFORM_nw4c) && !defined(M3DG_PLATFORM_psvita) && !defined(BM3_PLATFORM_win_opengl) && !defined(BM3_PLATFORM_win_directx9) && !defined(BM3_PLATFORM_playstation3) && !defined(BM3_PLATFORM_xbox360) && !defined(BM3_PLATFORM_psp) && !defined(BM3_PLATFORM_gamebryodx9) && !defined(BM3_PLATFORM_opengles) && !defined(BM3_PLATFORM_nw4c) && !defined(BM3_PLATFORM_psvita)
	#error UNDEFINED 'M3DG_PLATFORM / BM3_PLATFORM'
#endif

#include <ml/type/ml_color.h>
#include <ml/type/ml_basic.h>

namespace m3dg{

struct ModelInitializeInfo : public internal::ModelInitializeInfoPlatformDependency{
};

struct ModelDrawInfo : public internal::ModelDrawInfoPlatformDependency{
	float					frame_;
	float					life_frame_;
	ml::color_rgba<float>	diffuse_color_;
};

typedef void (* BindTextureCallback)(const wchar_t *texture_name, void *user_data);

struct BoundingSphere{
	float			size_;
	ml::vector3d	center_;
};

struct VertexArrayInfo{
	typedef	ml::ui16	IndexType;

	int					vertex_count_;
	const IndexType *	index_array_;

	const ml::f32 *		position_array_;
	int					position_array_stride_;

	const ml::f32 *		normal_array_;
	int					normal_array_stride_;

	ml::f32				all_triangle_measure_;			// ���ׂĂ̎O�p�`�ʐ�
	const ml::f32 *		triangle_addition_measure_;		// �O�p�`���Z�ʐρB 0.0 �J�n

	const BoundingSphere*		bounding_sphere_;	// �o�E���f�B���O�X�t�B�A
};

typedef void (* ForeachVertexArrayCallback)(const VertexArrayInfo &info, void *user_data);

}	// namespace m3dg

#endif	// #ifndef M3DG_M3DG_TYPE_H
