#ifndef BM3_SDK_INC_SDK_INC_BM3_CORE_TEXTURE_BM3_TEX_MOVIEHANDLEBASE_H
#define BM3_SDK_INC_SDK_INC_BM3_CORE_TEXTURE_BM3_TEX_MOVIEHANDLEBASE_H

#include "../../system/bm3_Config.h"
#include "../bm3_CoreType.h"

#if defined(BM3_TARGET_IDE)
	#include "bm3_tex_IdeHandle.h"
#else
	#include "bm3_tex_HandleBase.h"
#endif

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

class UpdateInfo;
class DrawInfo;
class RendererInfo;

namespace texture{

#if defined(BM3_TARGET_IDE)
class MovieHandleBase : public ide::Handle
#else
class MovieHandleBase : public HandleBase
#endif
{
public:
	MovieHandleBase();
	virtual ~MovieHandleBase();

	virtual void PrepareUpdate(UpdateInfo &update_info);
	virtual void PrepareDraw(DrawInfo &draw_info);
};

}	// namespace texture
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// BM3_SDK_INC_SDK_INC_BM3_CORE_TEXTURE_BM3_TEX_MOVIEHANDLEBASE_H
