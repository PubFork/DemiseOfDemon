#ifndef BM3_SDK_INC_SDK_INC_BM3_CORE_TEXTURE_DIRECTX9_BM3_TEX_HANDLEBASE_H
#define BM3_SDK_INC_SDK_INC_BM3_CORE_TEXTURE_DIRECTX9_BM3_TEX_HANDLEBASE_H

#include <d3d9.h>
#include "bm3/system/bm3_Config.h"
#include "../../bm3_CoreType.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

class DrawInfo;
class RendererInfo;

namespace texture{

class HandleBase{

public:
	HandleBase();
	virtual ~HandleBase();

	virtual void Bind(TextureLayer::Type layer, DrawInfo &draw_info) = 0;

protected:
	void Bind(TextureLayer::Type layer, DrawInfo &draw_info, LPDIRECT3DTEXTURE9 texture);
};

}	// namespace texture
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// BM3_SDK_INC_SDK_INC_BM3_CORE_TEXTURE_DIRECTX9_BM3_TEX_HANDLEBASE_H
