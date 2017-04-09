#ifndef BM3_SDK_INC_BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_HASNOTEXTURE_H
#define BM3_SDK_INC_BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_HASNOTEXTURE_H

#include "bm3/system/bm3_Config.h"
#include "bm3_draw_policy_Common.h"
#include "../../bm3_DrawInfo.h"
#include "../../bm3_CoreType.h"
#include "../../texture/bm3_tex_HandleBase.h"
#include "../../bm3_RenderStateManager.h"
#include "../../bm3_ResourceManager.h"
#include "../../bm3_ProfileManager.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE
namespace draw{
namespace policy{

class HasNoTexture{

protected:
	template<typename ResourceType>
	void	SetTexture(DrawInfo &/*draw_info*/, const ResourceType &/*res*/){

		RenderStateManager::instance().SetTextureDrawType(TextureDrawTypeConst_Basic);
	}
};

}	// namespace policy
}	// namespace draw
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_DRAW_POLICY_BM3_DRAW_POLICY_HASTEXTURE_H
