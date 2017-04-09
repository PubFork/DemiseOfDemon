#ifndef BM3_SDK_INC_BM3_CORE_DRAW_DIRECTX9_BM3_MODEL_M3DGMODELHANDLE_H
#define BM3_SDK_INC_BM3_CORE_DRAW_DIRECTX9_BM3_MODEL_M3DGMODELHANDLE_H

#include "bm3/system/bm3_Config.h"
#include "../bm3_model_ModelHandleBase.h"
#include "../../bm3_RendererInfo.h"
#include <m3dg/model/m3dg_model.h>
#include <d3d9.h>

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

class DrawInfo;
struct ModelDrawInfo;
struct ShaderInfo;

namespace model{

class M3dgModelHandle : public HandleBase{

public:
	M3dgModelHandle();
	virtual ~M3dgModelHandle();

	void Initialize(void *image, LPDIRECT3DDEVICE9 d3d_device);
	void Finalize();

	void Draw(const ModelDrawInfo &mdi, DrawInfo &draw_info, ShaderInfo &shader_info);
	void DebugDraw(const ml::matrix44 &m, DrawInfo &draw_info);

	void SetRenderType();

	m3dg::model::Model *animationModel;

public : 

#if defined(BM3_TARGET_IDE)
	static HandleBase *Create(void *image, RendererInfo &renderer){
		M3dgModelHandle *handle = new M3dgModelHandle;
		handle->Initialize(image, renderer.d3d_device_);
		return handle;
	}
#endif
};

}	// namespace model
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_DRAW_DIRECTX9_BM3_MODEL_M3DGMODELHANDLE_H
