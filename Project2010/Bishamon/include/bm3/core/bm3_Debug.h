#ifndef BM3_SDK_INC_BM3_CORE_BM3_DEBUG_H
#define BM3_SDK_INC_BM3_CORE_BM3_DEBUG_H

#include "../system/bm3_Config.h"
#include <ml/type/ml_matrix44.h>

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

class DrawInfo;

namespace debug{

struct SphereArea{
public:
	SphereArea(float horizontal_s, float horizontal_e, float vertical_s, float vertical_e) :
		horizontal_s_(horizontal_s),
		horizontal_e_(horizontal_e),
		vertical_s_(vertical_s),
		vertical_e_(vertical_e)
	{
	}

	float HorizontalStartRadian()	const {	return ml::to_radian(ml::minimum(horizontal_s_,	horizontal_e_));	}
	float HorizontalEndRadian()		const {	return ml::to_radian(ml::maximum(horizontal_s_,	horizontal_e_));	}
	float VerticalStartRadian()		const {	return ml::to_radian(ml::minimum(vertical_s_,	vertical_e_));	}
	float VerticalEndRadian()		const {	return ml::to_radian(ml::maximum(vertical_s_,	vertical_e_));	}

	float	horizontal_s_;
	float	horizontal_e_;
	float	vertical_s_;
	float	vertical_e_;
};

namespace internal{

void DrawAxis(DrawInfo &info, const ml::matrix44 &matrix, float x, float y, float z);

void DrawPoint(DrawInfo &info, const ml::matrix44 &matrix, const ml::vector3d &position);

void DrawSphere(DrawInfo &info, const ml::matrix44 &matrix, const ml::vector3d &position, const ml::vector3d &size, const SphereArea &area);

void DrawCylinder(DrawInfo &info, const ml::matrix44 &matrix, const ml::vector3d &position, const ml::vector3d &size, const SphereArea &area);

void DrawCube(DrawInfo &draw_info, const ml::matrix44 &matrix, const ml::vector3d &position, const ml::vector3d &size);

void DrawModel(DrawInfo &info, const ml::matrix44 &matrix, float x, float y, float z);
	

}	// namespace internal


inline void DrawAxis(DrawInfo &info, const ml::matrix44 &matrix, float length){
	internal::DrawAxis(info, matrix, length, length, length);
}

inline void DrawAxis(DrawInfo &info, const ml::matrix44 &matrix, float x, float y, float z){
	internal::DrawAxis(info, matrix, x, y, z);
}

inline void DrawPoint(DrawInfo &info, const ml::matrix44 &matrix, const ml::vector3d &position){
	internal::DrawPoint(info, matrix, position);
}

inline void DrawSphere(DrawInfo &info, const ml::matrix44 &matrix, const ml::vector3d &position, const ml::vector3d &size, const SphereArea &area){
	internal::DrawSphere(info, matrix, position, size, area);
}

inline void DrawCylinder(DrawInfo &info, const ml::matrix44 &matrix, const ml::vector3d &position, const ml::vector3d &size, const SphereArea &area){
	internal::DrawCylinder(info, matrix, position, size, area);
}

inline void DrawCube(DrawInfo &info, const ml::matrix44 &matrix, const ml::vector3d &position, const ml::vector3d &size){
	internal::DrawCube(info, matrix, position, size);
}

inline void DrawModel(DrawInfo &info, const ml::matrix44 &matrix, float x, float y, float z){
	internal::DrawModel(info, matrix, x, y, z);
}
	
}

BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_BM3_DEBUG_H
