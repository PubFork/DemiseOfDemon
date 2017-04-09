#ifndef BM3_SDK_SRC_BM3_CORE_DIRECTX9_BM3_MASSSHADER_H
#define BM3_SDK_SRC_BM3_CORE_DIRECTX9_BM3_MASSSHADER_H

#include <d3d9.h>
#include <d3dx9shader.h>
#include "bm3/system/bm3_Config.h"
#include "../bm3_ShaderCommon.h"
#include "../bm3_RendererInfo.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

class MassShader : public ShaderCommon{

public:
	
	enum ProcType{
		ProcType_Setup,
		ProcType_Update,
		ProcType_Render
	};
	
	enum SetupShaderType{
		ShaderType_Setup,
		ShaderType_SetupShape,
		ShaderType_SetupShapeModel,
		ShaderType_SetupSphere,
		ShaderType_SetupCylinder,
		ShaderType_SetupCube,
		ShaderType_SetupSpeed,
		ShaderType_SetupSpeedSphere,
		ShaderType_SetupSpeedCylinder,
		ShaderType_SetupSpeedCube,
		ShaderType_SetupSpeedModel,
		ShaderType_SetupColor,
		ShaderType_SetupSyncColor,
		
		ShaderType_SetupMax,
	};

	enum UpdateShaderType{
		ShaderType_Update,
		ShaderType_UpdateOneShoot,
		ShaderType_UpdateParentInfl,
		ShaderType_UpdateOneShootParentInfl,
		
		ShaderType_UpdateMax
	};
	
	enum RenderShaderType{
		ShaderType_Render,
		ShaderType_RenderGoal,
		ShaderType_RenderBM2,
		ShaderType_RenderGoalBM2,

		ShaderType_RenderMax
	};

	enum SetupShaderParamIndex{
		// setup foundation
		SetupShaderParam_ps_time,
		SetupShaderParam_ps_noiseuv_offset,
		SetupShaderParam_ps_w_param,
		// setup parameter
		SetupShaderParam_ps_initial_value,
		SetupShaderParam_ps_variance_scalar,
		SetupShaderParam_ps_noise_scalar,
		SetupShaderParam_ps_noise_offset,
		SetupShaderParam_ps_noise_texture,
		// setup parameter shape type
		SetupShaderParam_ps_shape_texture,
		// setup make shape shader
		SetupShaderParam_shape_ps_time,
		SetupShaderParam_shape_ps_noiseuv_offset,
		SetupShaderParam_shape_ps_noise_texture,
		SetupShaderParam_shape_ps_radius,
		SetupShaderParam_shape_ps_angle,
		// generate shape type model
		SetupShaderParam_shape_ps_modelparam,
		SetupShaderParam_shape_ps_modelindex,
		SetupShaderParam_shape_ps_modelmeasure,
		SetupShaderParam_shape_ps_modelposition,
		SetupShaderParam_shape_ps_modelnormal,
		
		SetupShaderParam_Max
	};
	
	enum UpdateShaderParamIndex{
		UpdateShaderParam_ps_elapsed_time,
		UpdateShaderParam_ps_tex_center_offset,
		UpdateShaderParam_ps_deceleration,
		UpdateShaderParam_ps_noise_param,
		UpdateShaderParam_ps_position_map,
		UpdateShaderParam_ps_velocity_map,
		UpdateShaderParam_ps_noise_texture,
		UpdateShaderParam_ps_gravity,
		UpdateShaderParam_ps_w_matrix,
		UpdateShaderParam_ps_orig_position_map,
		UpdateShaderParam_ps_orig_velocity_map,
		
		UpdateShaderParam_Max
	};
	
	enum RenderShaderParamIndex{
		RenderShaderParam_vs_wvp_matrix,
		RenderShaderParam_vs_tex_center_offset,
		RenderShaderParam_vs_position_map,
		RenderShaderParam_vs_velocity_map,
		RenderShaderParam_vs_colorsize_map,
		RenderShaderParam_vs_alpha_map,
		RenderShaderParam_ps_alpha_map,
		RenderShaderParam_vs_size_param,
		RenderShaderParam_vs_color_scale,
		RenderShaderParam_vs_offset_position,
		RenderShaderParam_ps_particle_texture,
		RenderShaderParam_ps_pattern_texture,
		RenderShaderParam_vs_goal_map,
		RenderShaderParam_vs_goal_param,
		RenderShaderParam_vs_goal_position,
		
		RenderShaderParam_Max
	};
	
	MassShader();
	~MassShader();
	
	void Initialize(IDirect3DDevice9 *d3d_device);

	template<typename VertexShaderType, typename PixelShaderType, int VertexShaderLength, int PixelShaderLength, int VertexDeclarationLength>
	MassShader *Load(
		IDirect3DDevice9 *d3d_device,
		const VertexShaderType (&vs_binary)[VertexShaderLength],
		const PixelShaderType (&ps_binary)[PixelShaderLength],
		const D3DVERTEXELEMENT9 (&vertex_decl_array)[VertexDeclarationLength],
		ProcType	type){

		ML_PTR_ASSERT(d3d_device);
		ML_STATIC_ASSERT(VertexShaderLength > 0);
		ML_STATIC_ASSERT(PixelShaderLength > 0);
		ML_STATIC_ASSERT(VertexDeclarationLength > 0);

		Load(
			d3d_device,
			vs_binary, VertexShaderLength,
			ps_binary, PixelShaderLength,
			vertex_decl_array, VertexDeclarationLength,
			type
		);

		return this;
	}
	
	void	SetVectorParam(RendererInfo &info, int type, const float *vector);
	void	SetMatrixParam(RendererInfo &info, int type, const ml::matrix44 &m);
	void	SetTexture(RendererInfo &info, int type, IDirect3DTexture9 *t){
		SetTexture(info.d3d_device_, type, t);
	}
	void	SetTexture(IDirect3DDevice9 *d3d_device, int type, IDirect3DTexture9 *t);
	void	SetVTexture(RendererInfo &info, int type, IDirect3DVolumeTexture9 *t);
	void	SetTextureAddressType(RendererInfo &info, int type, D3DTEXTUREADDRESS address_type);
	void	SetTextureFilterType(RendererInfo &info, int type, D3DTEXTUREFILTERTYPE filter_type);

	void Bind(IDirect3DDevice9 *d3d_device);
	void SetShader(ProcType proc_type, int shader_type){
		SetShaderHandle(proc_type, shader_type);
		SetShaderParamHandle(proc_type, shader_type);
	}
	void SetShaderHandle(ProcType proc_type, int shader_type){
		switch(proc_type){
		case ProcType_Setup:	shader_handle_	= &setup_shader_[shader_type];	break;
		case ProcType_Update:	shader_handle_	= &update_shader_[shader_type];	break;
		case ProcType_Render:	shader_handle_	= &render_shader_[shader_type];	break;
		default:				ML_ASSERT(false);	break;
		}
	};
	void SetShaderParamHandle(ProcType proc_type, int shader_type){
		switch(proc_type){
		case ProcType_Setup:	shader_param_handle_	= setup_shader_param_[shader_type];		break;
		case ProcType_Update:	shader_param_handle_	= update_shader_param_[shader_type];	break;
		case ProcType_Render:	shader_param_handle_	= render_shader_param_[shader_type];	break;
		default:				ML_ASSERT(false);	break;
		}
	}

//	void VertexElementType(D3DVERTEXELEMENT9* element, unsigned int *num){
//		vertex_decl_->GetDeclaration(element, num);
//	}

	void	EnableNoiseTexture(RendererInfo &info, int type){
		SetVTexture(info, type, noise_texture_);
		SetTextureAddressType(info,	type,	D3DTADDRESS_WRAP);
		SetTextureFilterType(info,	type,	D3DTEXF_POINT);
	}
	void	DisableNoiseTexture(RendererInfo &info, int type){
		SetVTexture(info, type, NULL);
	}

	void	EnableParticleTexture(RendererInfo &info){
		SetTexture(info, RenderShaderParam_ps_particle_texture, particle_texture_);
	}
	void	DisableParticleTexture(RendererInfo &info){
		SetTexture(info, RenderShaderParam_ps_particle_texture, NULL);
	}

private:
	struct ShaderInstance{
		ShaderInstance() : 
			vertex_decl_(NULL),
			vertex_shader_(NULL),
			pixel_shader_(NULL)
		{
		}
		
		~ShaderInstance(){
			if(vertex_decl_)	vertex_decl_->Release();
			if(vertex_shader_)	vertex_shader_->Release();
			if(pixel_shader_)	pixel_shader_->Release();
		}
		
		IDirect3DVertexDeclaration9	*vertex_decl_;
		IDirect3DVertexShader9		*vertex_shader_;
		IDirect3DPixelShader9		*pixel_shader_;
	};
	
	ShaderInstance			setup_shader_[ShaderType_SetupMax];
	ShaderInstance			update_shader_[ShaderType_UpdateMax];
	ShaderInstance			render_shader_[ShaderType_RenderMax];
	ShaderInstance			*shader_handle_;
	
	typedef DWORD ShaderParam;
	ShaderParam		setup_shader_param_[ShaderType_SetupMax][SetupShaderParam_Max];
	ShaderParam		update_shader_param_[ShaderType_UpdateMax][UpdateShaderParam_Max];
	ShaderParam		render_shader_param_[ShaderType_RenderMax][RenderShaderParam_Max];
	ShaderParam		*shader_param_handle_;

	IDirect3DVolumeTexture9		*noise_texture_;
	IDirect3DTexture9			*particle_texture_;
	
	enum{
		VertexShaderBit	= 0x40000000,
		PixelShaderBit	= 0x80000000,
//		ShaderMask		= 0x80000000,

		InvalidParam	= ShaderParam(0x3FFFFFFF),
		InvalidTexParam	= 0xFFFFFFFF,

		ParamMask		= 0x3FFFFFFF
	};

	void Load(
		IDirect3DDevice9 *d3d_device,
		const void *vs_binary,						int vs_size,
		const void *ps_binary,						int ps_size,
		const D3DVERTEXELEMENT9 *vertex_decl_array,	int VertexDeclarationLength,
		ProcType	proc_type
	);

	static bool IsValid(ShaderParam param){	return param != InvalidParam;	}
	static ShaderParam ParamByName(ID3DXConstantTable *pixel_const_table, const char *name);
	static ShaderParam ParamByNameTex(ID3DXConstantTable *const_table, const char *name);
	bool InitNoiseMap(LPDIRECT3DDEVICE9 device);
	bool InitParticleTexture(LPDIRECT3DDEVICE9 device);
};


BM3_END_PLATFORM_NAMESPACE
} // namespace bm3

#endif // BM3_SDK_SRC_BM3_CORE_DIRECTX9_BM3_MASSSHADER_H
