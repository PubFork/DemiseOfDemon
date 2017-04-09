#ifndef BM3_SDK_INC_BM3_CORE_DIRECTX9_BM3_RENDERDEVICEPOLICY_H
#define BM3_SDK_INC_BM3_CORE_DIRECTX9_BM3_RENDERDEVICEPOLICY_H

#include "../../system/bm3_Config.h"
#include "../bm3_RenderDevicePolicyBase.h"
#include "../bm3_Shader.h"
#include "../bm3_MassShader.h"
#include "../bm3_DrawInfo.h"
#include "../bm3_RenderStateInitializeInfo.h"
#include "../bm3_CoreType.h"
#include "../texture/bm3_tex_RenderTarget.h"
#include "../bm3_VertexInfo.h"

#if defined(BM3_TARGET_IDE)
#include <map>
#endif

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

namespace internal{

class RenderDevicePolicy : public RenderDevicePolicyBase{
public:
	RenderDevicePolicy();
	~RenderDevicePolicy();

	void	PreBeginRender(DrawInfo &draw_info);
	void	PostEndRender(DrawInfo &draw_info);

	void	SetTexture(IDirect3DTexture9 *texture, TextureLayer::Type layer);
	void	SetDefaultTexture(){
		ML_STATIC_ASSERT(TextureLayer::Max == 2);
		SetTexture(NULL, TextureLayer::_1);
		SetTexture(NULL, TextureLayer::_2);
		SetTextureDrawType(TextureDrawTypeConst_Basic);
	}

	void	SetDepthTexture(IDirect3DTexture9 *texture);

	void	Apply();
	void	ApplySimpleParticle();
	void	ApplyMassParticle(MassShader::ProcType proc_type, int shader_type);
	void	ApplyModel();
	void	SetMassParticleTexture();
	MassShader&	GetMassShader(){	return mass_shader_;	}

#if defined(BM3_TARGET_IDE)
	void	ApplySimpleParticleSize1();
#endif

	void InitializeRenderDevicePolicy(RenderStateInitializeInfo &info);
	void FinalizeRenderDevicePolicy();

	void GetVertexInfo(VertexInfo &info);

	const ml::matrix44& GetWorldMatrix(){	return world_matrix_;			}

private:
	IDirect3DDevice9	*d3d_device_;			// beginからendの間だけ有効
	IDirect3DTexture9	*texture_[TextureLayer::Max];
	IDirect3DTexture9	*depth_texture_;
	bool				is_force_bind_;
	ApplyType			old_apply_type_;
	ml::matrix44		palette[32];

	struct{
		Shader *hash_[TextureDrawTypeConst_MAX][Future::All];
		#include "shader_hash.inc"

		// シンプルパーティクル用
		Shader	simpleparticle_;

		// モデル用
		Shader	model_;
		Shader	model_tex_;

#if defined(BM3_TARGET_IDE)
		Shader	simpleparticle_size1_;
#endif
	}shader_;

	MassShader	mass_shader_;

	void	ApplyCore(Shader &shader, ApplyType apply_type);
};

#if defined(BM3_TARGET_IDE)
namespace ide{

class RenderDevicePolicy/* : public RenderDevicePolicyBase*/{

public:
	RenderDevicePolicy(){
		;
	}
	~RenderDevicePolicy(){
		;
	}

	void	PreBeginRender(DrawInfo &draw_info){
		current_policy_ = &policy_map_[draw_info.Renderer().d3d_device_];
		ML_PTR_ASSERT(current_policy_);
		current_policy_->PreBeginRender(draw_info);
	}
	void	PostEndRender(DrawInfo &draw_info){
		ML_PTR_ASSERT(current_policy_);
		current_policy_->PostEndRender(draw_info);
		current_policy_ = NULL;
	}

	void PostBeginRender(DrawInfo &draw_info){				ML_PTR_ASSERT(current_policy_);	current_policy_->PostBeginRender(draw_info);	}
	void PreEndRender(DrawInfo &draw_info){					ML_PTR_ASSERT(current_policy_);	current_policy_->PreEndRender(draw_info);		}

	// 以下、internal::RenderDevicePolicyのパブリックメソッド
	void	SetTexture(IDirect3DTexture9 *texture, TextureLayer::Type layer){
															ML_PTR_ASSERT(current_policy_);	current_policy_->SetTexture(texture, layer);	}
	void	SetDepthTexture(IDirect3DTexture9 *texture){	ML_PTR_ASSERT(current_policy_);	current_policy_->SetDepthTexture(texture);		}
	void	SetDefaultTexture(){							ML_PTR_ASSERT(current_policy_);	current_policy_->SetDefaultTexture();			}
	void	Apply(){										ML_PTR_ASSERT(current_policy_);	current_policy_->Apply();						}
#if 0
	void	Apply(const ml::matrix44 *bind_matrix = NULL, const ml::matrix44 *inverse_bind_matrix = NULL, int inverse_bind_matrix_count = 0, const ml::matrix44 *joint_matrix = NULL, int joint_matrix_count = 0, const ml::matrix44 *node_matrix = NULL){										ML_PTR_ASSERT(current_policy_);	current_policy_->Apply(bind_matrix, inverse_bind_matrix inverse_bind_matrix_count, joint_matrix, joint_matrix_count, node_matrix);						}
#endif
	void	ApplySimpleParticle(){							ML_PTR_ASSERT(current_policy_);	current_policy_->ApplySimpleParticle();			}
	void	ApplySimpleParticleSize1(){						ML_PTR_ASSERT(current_policy_);	current_policy_->ApplySimpleParticleSize1();	}
	void	ApplyMassParticle(MassShader::ProcType proc_type, int shader_type){	ML_PTR_ASSERT(current_policy_);	current_policy_->ApplyMassParticle(proc_type, shader_type);		}
	void	ApplyModel() { ML_PTR_ASSERT(current_policy_); current_policy_->ApplyModel(); }
	void	SetMassParticleTexture(){						ML_PTR_ASSERT(current_policy_);	current_policy_->SetMassParticleTexture();		}
	
	MassShader& GetMassShader(){	ML_PTR_ASSERT(current_policy_);	return current_policy_->GetMassShader();	}

	// 以下、RenderDevicePolicyBaseのパブリックメソッド
	void SetProjectionMatrix(const ml::matrix44 &m){		ML_PTR_ASSERT(current_policy_);	current_policy_->SetProjectionMatrix(m);		}
	void SetViewMatrix(const ml::matrix44 &m){				ML_PTR_ASSERT(current_policy_);	current_policy_->SetViewMatrix(m);				}
	void SetWorldMatrix(const ml::matrix44 &m){				ML_PTR_ASSERT(current_policy_);	current_policy_->SetWorldMatrix(m);				}
	const ml::matrix44& GetWorldMatrix(){					ML_PTR_ASSERT(current_policy_);	return current_policy_->GetWorldMatrix();		}

	void SetDiffuseColor(const ColorType &c){				ML_PTR_ASSERT(current_policy_);	current_policy_->SetDiffuseColor(c);			}
	void SetEmissionColor(const ColorType &c){				ML_PTR_ASSERT(current_policy_);	current_policy_->SetEmissionColor(c);			}
	void SetSpecularColor(const ColorType &c){				ML_PTR_ASSERT(current_policy_);	current_policy_->SetSpecularColor(c);			}
	void SetSpecularPower(float pow){						ML_PTR_ASSERT(current_policy_);	current_policy_->SetSpecularPower(pow);			}

	void SetTextureDrawType(TextureDrawTypeConst t){		ML_PTR_ASSERT(current_policy_);	current_policy_->SetTextureDrawType(t);			}
	void SetTexture0Scroll(const ml::vector3d &v){			ML_PTR_ASSERT(current_policy_);	current_policy_->SetTexture0Scroll(v);			}
	void SetTexture1Scroll(const ml::vector3d &v){			ML_PTR_ASSERT(current_policy_);	current_policy_->SetTexture1Scroll(v);			}

	void SetColorScale(const ColorType &c){					ML_PTR_ASSERT(current_policy_);	current_policy_->SetColorScale(c);				}
	void SetOffsetPosition(const ml::vector3d &p){			ML_PTR_ASSERT(current_policy_);	current_policy_->SetOffsetPosition(p);			}

	void SetAmbientLightColor(const ColorType &c){			ML_PTR_ASSERT(current_policy_);	current_policy_->SetAmbientLightColor(c);		}
	void SetLightDirection(const ml::vector3d &v){			ML_PTR_ASSERT(current_policy_);	current_policy_->SetLightDirection(v);			}
	void SetLightColor(const ColorType &c){					ML_PTR_ASSERT(current_policy_);	current_policy_->SetLightColor(c);				}

	void SetDefaultColor(){									ML_PTR_ASSERT(current_policy_);	current_policy_->SetDefaultColor();				}

	void SetCameraDirection(const ml::vector3d &v){			ML_PTR_ASSERT(current_policy_);	current_policy_->SetCameraDirection(v);			}

	void SetSoftThreshold(float t){							ML_PTR_ASSERT(current_policy_);	current_policy_->SetSoftThreshold(t);			}
	void SetModelVertexCount(float c){						ML_PTR_ASSERT(current_policy_);	current_policy_->SetModelVertexCount(c);		}
	
	void SetFogColor(const ColorType &c){					ML_PTR_ASSERT(current_policy_);	current_policy_->SetFogColor(c);				}
	void SetFogArea(const ml::vector3d &v){					ML_PTR_ASSERT(current_policy_);	current_policy_->SetFogArea(v);					}
	void SetFogDensity(const float v){						ML_PTR_ASSERT(current_policy_);	current_policy_->SetFogDensity(v);				}

	void EnableVertexColor(){								ML_PTR_ASSERT(current_policy_);	current_policy_->EnableVertexColor();			}
	void EnableTexture(){									ML_PTR_ASSERT(current_policy_);	current_policy_->EnableTexture();				}
	void EnableSpecular(){									ML_PTR_ASSERT(current_policy_);	current_policy_->EnableSpecular();				}
	void EnableLight(){										ML_PTR_ASSERT(current_policy_);	current_policy_->EnableLight();					}
	void EnableSoft(){										ML_PTR_ASSERT(current_policy_);	current_policy_->EnableSoft();					}
	void EnableSoftN(){										ML_PTR_ASSERT(current_policy_);	current_policy_->EnableSoftN();					}
	void EnableFog(){										ML_PTR_ASSERT(current_policy_);	current_policy_->EnableFog();					}

	void DisableVertexColor(){								ML_PTR_ASSERT(current_policy_);	current_policy_->DisableVertexColor();			}
	void DisableTexture(){									ML_PTR_ASSERT(current_policy_);	current_policy_->DisableTexture();				}
	void DisableSpecular(){									ML_PTR_ASSERT(current_policy_);	current_policy_->DisableSpecular();				}
	void DisableLight(){									ML_PTR_ASSERT(current_policy_);	current_policy_->DisableLight();				}
	void DisableSoft(){										ML_PTR_ASSERT(current_policy_);	current_policy_->DisableSoft();					}
	void DisableFog(){										ML_PTR_ASSERT(current_policy_);	current_policy_->DisableFog();					}

	void DisableAllFuture(){								ML_PTR_ASSERT(current_policy_);	current_policy_->DisableAllFuture();			}

	bool IsEnableVertexColor() const{						ML_PTR_ASSERT(current_policy_);	return current_policy_->IsEnableVertexColor();	}
	bool IsEnableTexture() const{							ML_PTR_ASSERT(current_policy_);	return current_policy_->IsEnableTexture();		}
	bool IsEnableSpecular() const{							ML_PTR_ASSERT(current_policy_);	return current_policy_->IsEnableSpecular();		}
	bool IsEnableLight() const{								ML_PTR_ASSERT(current_policy_);	return current_policy_->IsEnableLight();		}
	bool IsEnableSoft() const{								ML_PTR_ASSERT(current_policy_);	return current_policy_->IsEnableSoft();			}
	bool IsEnableFog() const{								ML_PTR_ASSERT(current_policy_);	return current_policy_->IsEnableFog();			}

	void FunctionMask_EnableVertexColor(){					ML_PTR_ASSERT(current_policy_);	current_policy_->FunctionMask_EnableVertexColor();			}
	void FunctionMask_EnableTexture(){						ML_PTR_ASSERT(current_policy_);	current_policy_->FunctionMask_EnableTexture();				}
	void FunctionMask_EnableSpecular(){						ML_PTR_ASSERT(current_policy_);	current_policy_->FunctionMask_EnableSpecular();				}
	void FunctionMask_EnableLight(){						ML_PTR_ASSERT(current_policy_);	current_policy_->FunctionMask_EnableLight();				}
	void FunctionMask_EnableSoft(){							ML_PTR_ASSERT(current_policy_);	current_policy_->FunctionMask_EnableSoft();					}
	void FunctionMask_EnableFog(){							ML_PTR_ASSERT(current_policy_);	current_policy_->FunctionMask_EnableFog();					}

	void FunctionMask_DisableVertexColor(){					ML_PTR_ASSERT(current_policy_);	current_policy_->FunctionMask_DisableVertexColor();			}
	void FunctionMask_DisableTexture(){						ML_PTR_ASSERT(current_policy_);	current_policy_->FunctionMask_DisableTexture();				}
	void FunctionMask_DisableSpecular(){					ML_PTR_ASSERT(current_policy_);	current_policy_->FunctionMask_DisableSpecular();			}
	void FunctionMask_DisableLight(){						ML_PTR_ASSERT(current_policy_);	current_policy_->FunctionMask_DisableLight();				}
	void FunctionMask_DisableSoft(){						ML_PTR_ASSERT(current_policy_);	current_policy_->FunctionMask_DisableSoft();				}
	void FunctionMask_DisableFog(){							ML_PTR_ASSERT(current_policy_);	current_policy_->FunctionMask_DisableFog();					}

	bool FunctionMask_IsEnableVertexColor() const{			ML_PTR_ASSERT(current_policy_);	return current_policy_->FunctionMask_IsEnableVertexColor();	}
	bool FunctionMask_IsEnableTexture() const{				ML_PTR_ASSERT(current_policy_);	return current_policy_->FunctionMask_IsEnableTexture();		}
	bool FunctionMask_IsEnableSpecular() const{				ML_PTR_ASSERT(current_policy_);	return current_policy_->FunctionMask_IsEnableSpecular();	}
	bool FunctionMask_IsEnableLight() const{				ML_PTR_ASSERT(current_policy_);	return current_policy_->FunctionMask_IsEnableLight();		}
	bool FunctionMask_IsEnableSoft() const{					ML_PTR_ASSERT(current_policy_);	return current_policy_->FunctionMask_IsEnableSoft();		}
	bool FunctionMask_IsEnableFog() const{					ML_PTR_ASSERT(current_policy_);	return current_policy_->FunctionMask_IsEnableFog();			}


	void GetDirectionalLight(RenderDevicePolicyBase::DirectionalLight &light)	{	ML_PTR_ASSERT(current_policy_);	current_policy_->GetDirectionalLight(light);	}
	void SetDirectionalLight(RenderDevicePolicyBase::DirectionalLight &light)	{	ML_PTR_ASSERT(current_policy_);	current_policy_->SetDirectionalLight(light);	}
	void NonDirectionalLight()													{	ML_PTR_ASSERT(current_policy_);	current_policy_->NonDirectionalLight();			}

	void InitializeRenderDevicePolicy(RenderStateInitializeInfo &info){

		if(policy_map_.find(info.d3d_device_) == policy_map_.end()){
			policy_map_[info.d3d_device_].InitializeRenderDevicePolicy(info);
		}

		current_policy_ = &policy_map_[info.d3d_device_];
	}

	void FinalizeRenderDevicePolicy(){

		PolicyMapI i = policy_map_.begin();
		for(;i != policy_map_.end();++ i){
			i->second.FinalizeRenderDevicePolicy();
		}

		policy_map_.clear();
	}

	void GetVertexInfo(VertexInfo &info){
	}
	
	void SetBindShapeMatrix(ml::matrix44 *matrix)  { ML_PTR_ASSERT(current_policy_); current_policy_->SetBindShapeMatrix(matrix); }
	void SetJointMatrices(ml::matrix44 *matrices) { ML_PTR_ASSERT(current_policy_); current_policy_->SetJointMatrices(matrices); } 
	void SetModelMatrix(ml::matrix44 *matrix)        { ML_PTR_ASSERT(current_policy_); current_policy_->SetModelMatrix(matrix); }
	void SetJointCount(int count) { ML_PTR_ASSERT(current_policy_); current_policy_->SetJointCount(count); }
	ml::matrix44 *GetMatrixPalette() { ML_PTR_ASSERT(current_policy_); return current_policy_->GetMatrixPalette(); }
	
private:
	typedef	std::map<IDirect3DDevice9 *, internal::RenderDevicePolicy>	PolicyMap;
	typedef	PolicyMap::iterator											PolicyMapI;

	PolicyMap						policy_map_;
	internal::RenderDevicePolicy	*current_policy_;		// beginからendの間だけ有効
};

}	// namespace ide
#endif	// #if defined(BM3_TARGET_IDE)

}	// namespace internal
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_DIRECTX9_BM3_RENDERDEVICEPOLICY_H
