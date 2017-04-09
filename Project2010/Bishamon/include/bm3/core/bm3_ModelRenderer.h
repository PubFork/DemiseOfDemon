#ifndef BM3_SDK_INC_BM3_CORE_BM3_MODELRENDERER_H
#define BM3_SDK_INC_BM3_CORE_BM3_MODELRENDERER_H

#include "../system/bm3_Config.h"
#include "model/bm3_model_ModelHandleBase.h"
#include "bm3_CoreType.h"
#include "draw/bm3_draw_Common.h"
#include "bm3/core/bm3_ResourceManager.h"

#include <ml/utility/ml_singleton.h>
#include <m3dg/model/m3dg_model.h>
#include <m3dg/m3dg_type.h>

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

class DrawInfo;
struct ShaderInfo;

namespace internal{

/// @brief	ModelRendererImplƒNƒ‰ƒX
class ModelRendererImpl{
public:
	ModelRendererImpl();
	~ModelRendererImpl();

	void	Draw(const StringType &name, const ModelDrawInfo &mdi, DrawInfo &draw_info, ShaderInfo &shader_info) {
		model::HandleBase *model = ResourceManager::instance().ModelHandleFromName(name, draw_info.Renderer());
		if(model){
			Draw(model, mdi, draw_info, shader_info);
		}
	}
	void	Draw(model::HandleBase *model, const ModelDrawInfo &mdi, DrawInfo &draw_info, ShaderInfo &shader_info){
		ML_PTR_ASSERT(model);
		model->Draw(mdi, draw_info, shader_info);
	}

	void	DebugDraw(const StringType &name, const ml::matrix44 &m, DrawInfo &draw_info);
	void	DebugDraw(model::HandleBase *model, const ml::matrix44 &m, DrawInfo &draw_info){
		ML_PTR_ASSERT(model);
		model->DebugDraw(m, draw_info);
	}

	void	InstanceDraw(model::HandleBase *model, const ModelDrawInfo &mdi, DrawInfo &draw_info, int instance_count, ShaderInfo &shader_info){
		ML_PTR_ASSERT(model);
		model->InstanceDraw(mdi, draw_info, instance_count, shader_info);
	}

	void	SetRenderType(const StringType &name);
private:
};

}	// namespace internal

typedef	ml::singleton<internal::ModelRendererImpl>	ModelRenderer;

BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_BM3_MODELRENDERER_H
