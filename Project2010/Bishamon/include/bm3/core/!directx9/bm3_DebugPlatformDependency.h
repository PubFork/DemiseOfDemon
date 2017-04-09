#ifndef BM3_SDK_INC_BM3_CORE_DIRECTX9_BM3_DEBUGPLATFORMDEPENDENCY_H
#define BM3_SDK_INC_BM3_CORE_DIRECTX9_BM3_DEBUGPLATFORMDEPENDENCY_H

#include "../../system/bm3_Config.h"
#include <ml/type/ml_matrix44.h>

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

class DrawInfo;

namespace debug{

struct SphereArea;

namespace internal{

void DrawAxis(DrawInfo &info, const ml::matrix44 &matrix, float x, float y, float z);

void DrawPoint(DrawInfo &info, const ml::matrix44 &matrix, const ml::vector3d &position);

void DrawSphere(DrawInfo &info, const ml::matrix44 &matrix, const ml::vector3d &position, const ml::vector3d &size, const SphereArea &area);

void DrawCylinder(DrawInfo &info, const ml::matrix44 &matrix, const ml::vector3d &position, const ml::vector3d &size, const SphereArea &area);

void DrawCube(DrawInfo &draw_info, const ml::matrix44 &matrix, const ml::vector3d &position, const ml::vector3d &size);

void DrawModel(DrawInfo &info, const ml::matrix44 &matrix, float x, float y, float z);
	

}	// namespace internal
}	// namespace debug
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_DIRECTX9_BM3_DEBUGPLATFORMDEPENDENCY_H
