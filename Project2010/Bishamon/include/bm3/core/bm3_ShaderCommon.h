#ifndef BM3_SDK_INC_BM3_CORE_BM3_SHADERCOMMON_H
#define BM3_SDK_INC_BM3_CORE_BM3_SHADERCOMMON_H

#include <ml/type/ml_matrix44.h>
#include <ml/type/ml_vector3d.h>
#include <ml/type/ml_color.h>
#include <ml/utility/ml_assert.h>
#include "../system/bm3_Config.h"
#include "bm3_CoreType.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

/// @brief ShaderCommonƒNƒ‰ƒX
struct ShaderCommon{

	enum MatrixParamType{
#if defined(BM3_PLATFORM_nw4c)
		MatrixParamType_world,
#endif
		MatrixParamType_world_view_proj,
		MatrixParamType_world_view,
		MatrixParamType_world_view_proj_tex,
		MatrixParamType_inverse_proj,
		MatrixParamType_bind_shape,
		MatrixParamType_joint,
		MatrixParamType_model,
		MatrixParamType_world,
		MatrixParamType_proj,
		MatrixParamType_view,
		//
		MatrixParamType_Max
	};

	enum VectorParamType{
		VectorParamType_diffuse_color,
		VectorParamType_specular_color_and_power,
		VectorParamType_color_scale,
		VectorParamType_offset_position,
		VectorParamType_ambient_light_color,
		VectorParamType_light_color,
		VectorParamType_light_dir,
		VectorParamType_emission_color,
		VectorParamType_texture0_scroll,
		VectorParamType_texture1_scroll,
		VectorParamType_camera_dir,
		VectorParamType_soft_threshold,
		VectorParamType_model_vertex_count,
		VectorParamType_point_sprite_scale,
		VectorParamType_fog_color,
		VectorParamType_fog_area,
		VectorParamType_ps_specular_color_and_power,
		//
		VectorParamType_Max
	};

	enum TextureParamType{
		TextureParamType_base,
		TextureParamType_sub,
		TextureParamType_depth,
		//
		TextureParamType_Max
	};
};

BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_BM3_SHADERCOMMON_H
