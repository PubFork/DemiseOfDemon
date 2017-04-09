#ifndef BM3_SDK_INC_BM3_CORE_BM3_CORETYPESWITCH_h
#define BM3_SDK_INC_BM3_CORE_BM3_CORETYPESWITCH_h

#include "bm3_CoreType.h"
#include "type/bm3_Curve.h"
#include "type/bm3_Texture.h"
#include "type/bm3_UserData.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

#if defined(BM3_TARGET_IDE)
	typedef	ide::Curve<ml::color_rgba<float> >		ColorCurveType;
	typedef	ide::Curve<float>						FloatCurveType;
	typedef	ide::Curve<ml::vector3d>				VectorCurveType;

	typedef	ide::Texture							TextureType;
	typedef	ide::UserData							UserDataType;
#endif
#if defined(BM3_TARGET_EMBEDDED)
	typedef	embedded::Curve<ml::color_rgba<float> >	ColorCurveType;
	typedef	embedded::Curve<float>					FloatCurveType;
	typedef	embedded::Curve<ml::vector3d>			VectorCurveType;

	typedef	embedded::Texture						TextureType;
	typedef	embedded::UserData						UserDataType;
#endif

typedef	FloatCurveType							BlendCurveType;
typedef	VectorCurveType							ScaleCurveValueType;
typedef	VectorCurveType							TransformCurveType;
typedef	FloatCurveType							GenerateTimingCurveType;

typedef	embedded::Curve<ml::color_rgba<float> >	EmbeddedColorCurveType;
typedef	embedded::Curve<float>					EmbeddedFloatCurveType;
typedef	embedded::Curve<ml::vector3d>			EmbeddedVectorCurveType;

typedef	embedded::Texture						EmbeddedTextureType;
typedef	embedded::UserData						EmbeddedUserDataType;

BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// BM3_SDK_INC_BM3_CORE_BM3_CORETYPESWITCH_h
