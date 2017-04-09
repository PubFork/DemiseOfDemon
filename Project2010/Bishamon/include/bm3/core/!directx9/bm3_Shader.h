#ifndef BM3_SDK_SRC_BM3_CORE_DIRECTX9_BM3_SHADER_H
#define BM3_SDK_SRC_BM3_CORE_DIRECTX9_BM3_SHADER_H

#include <d3d9.h>
#include <d3dx9shader.h>
#include "bm3/system/bm3_Config.h"
#include "../bm3_ShaderCommon.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

class Shader : public ShaderCommon{

public:
	Shader();
	~Shader();

	template<typename VertexShaderType, typename PixelShaderType, int VertexShaderLength, int PixelShaderLength, int VertexDeclarationLength>
	Shader *Load(
		IDirect3DDevice9 *d3d_device,
		const VertexShaderType (&vs_binary)[VertexShaderLength],
		const PixelShaderType (&ps_binary)[PixelShaderLength],
		const D3DVERTEXELEMENT9 (&vertex_decl_array)[VertexDeclarationLength]){

		ML_PTR_ASSERT(d3d_device);
		ML_STATIC_ASSERT(VertexShaderLength > 0);
		ML_STATIC_ASSERT(PixelShaderLength > 0);
		ML_STATIC_ASSERT(VertexDeclarationLength > 0);

		Load(
			d3d_device,
			vs_binary, VertexShaderLength,
			ps_binary, PixelShaderLength,
			vertex_decl_array, VertexDeclarationLength
		);

		return this;
	}

	bool SetMatrixParam(IDirect3DDevice9 *d3d_device, MatrixParamType type, const ml::matrix44 &m);
	bool SetMatrixParam(IDirect3DDevice9 *d3d_device, MatrixParamType type, const ml::matrix44 *m, int size);
	bool SetVectorParam(IDirect3DDevice9 *d3d_device, VectorParamType type, const ml::vector3d &v);
	bool SetColorParam(IDirect3DDevice9 *d3d_device, VectorParamType type, const ml::color_rgba<float> &c){
		return SetVectorParam(d3d_device, type, reinterpret_cast<const ml::vector3d &>(c));
	}

	void SetTexture(IDirect3DDevice9 *d3d_device, int stage, IDirect3DTexture9 *texture);
	void SetDepthTexture(IDirect3DDevice9 *d3d_device, IDirect3DTexture9 *texture);

	void Bind(IDirect3DDevice9 *d3d_device);

	void VertexElementType(D3DVERTEXELEMENT9* element, unsigned int *num){
		vertex_decl_->GetDeclaration(element, num);
	}
private:
	typedef DWORD ShaderParam;

	IDirect3DVertexDeclaration9	*vertex_decl_;
	IDirect3DVertexShader9		*vertex_shader_;
	IDirect3DPixelShader9		*pixel_shader_;

	ShaderParam	matrix_param_[MatrixParamType_Max];
	ShaderParam	vector_param_[VectorParamType_Max];

	enum{
		VertexShaderBit	= 0x40000000,
		PixelShaderBit	= 0x80000000,
//		ShaderMask		= 0x80000000,

		InvalidParam	= ShaderParam(0x3FFFFFFF),
		ParamMask		= 0x3FFFFFFF
	};

	void Load(
		IDirect3DDevice9 *d3d_device,
		const void *vs_binary,						int vs_size,
		const void *ps_binary,						int ps_size,
		const D3DVERTEXELEMENT9 *vertex_decl_array,	int VertexDeclarationLength
	);

	static bool IsValid(Shader::ShaderParam param){	return param != InvalidParam;	}
	static ShaderParam ParamByName(ID3DXConstantTable *vertex_const_table, ID3DXConstantTable *pixel_const_table, const char *name);
};


BM3_END_PLATFORM_NAMESPACE
} // namespace bm3

#endif // BM3_SDK_SRC_BM3_CORE_DIRECTX9_BM3_SHADER_H
