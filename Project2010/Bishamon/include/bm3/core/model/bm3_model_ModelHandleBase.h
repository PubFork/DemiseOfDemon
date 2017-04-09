#ifndef BM3_SDK_INC_BM3_CORE_DRAW_BM3_MODEL_MODELHANDLEBASE_H
#define BM3_SDK_INC_BM3_CORE_DRAW_BM3_MODEL_MODELHANDLEBASE_H

#include "../../system/bm3_Config.h"
#include <ml/type/ml_basic.h>
#include <ml/type/ml_matrix44.h>

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

class DrawInfo;
class RendererInfo;
struct ModelDrawInfo;
struct ShaderInfo;

namespace model{

/// @brief HandleBaseƒNƒ‰ƒX
class HandleBase{

public:
	HandleBase();
	virtual ~HandleBase();

	const void *Image() const{	return image_;	}

	virtual void Draw(const ModelDrawInfo &mdi, DrawInfo &draw_info, ShaderInfo &shader_info) = 0;

	virtual void DebugDraw(const ml::matrix44 &m, DrawInfo &draw_info);

	virtual void InstanceDraw(const ModelDrawInfo &mdi, DrawInfo &draw_info, int instance_count, ShaderInfo &shader_info);

	virtual void SetRenderType() = 0;
protected:
	void	*image_;
};

}	// namespace model
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_DRAW_BM3_MODEL_MODELHANDLEBASE_H
