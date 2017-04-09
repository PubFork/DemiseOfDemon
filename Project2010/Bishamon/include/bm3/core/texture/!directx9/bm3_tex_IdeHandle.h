#ifndef BM3_SDK_INC_SDK_INC_BM3_CORE_TEXTURE_DIRECTX9_BM3_TEX_IDEHANDLE_H
#define BM3_SDK_INC_SDK_INC_BM3_CORE_TEXTURE_DIRECTX9_BM3_TEX_IDEHANDLE_H

#include "bm3/system/bm3_Config.h"
#include "../bm3_tex_HandleBase.h"
#include <windows.h>
#include <d3d9.h>

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

class UpdateInfo;
class DrawInfo;
class RendererInfo;

namespace texture{

#if defined(BM3_TARGET_IDE)
namespace ide{

class Handle : public HandleBase{

public:
	struct ImageInfo{
		ImageInfo(RendererInfo &renderer) :
			renderer_(renderer){;}

		std::wstring	filename_;
		RendererInfo	&renderer_;
	};

	Handle();
	virtual ~Handle();

	virtual void Initialize(const ImageInfo &image_info);
	virtual void Bind(TextureLayer::Type layer, DrawInfo &draw_info);

	virtual void PrepareUpdate(UpdateInfo &update_info);
	virtual void PrepareDraw(DrawInfo &draw_info);

protected:
	LPDIRECT3DTEXTURE9	texture_;

private:

	bool IsInitialize() const;
	void Finalize();
};

}	// namespace ide
#endif	// #if defined(BM3_TARGET_IDE)

}	// namespace texture
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// BM3_SDK_INC_SDK_INC_BM3_CORE_TEXTURE_DIRECTX9_BM3_TEX_IDEHANDLE_H
