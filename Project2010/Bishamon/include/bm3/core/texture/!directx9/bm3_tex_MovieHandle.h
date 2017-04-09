#ifndef BM3_SDK_INC_SDK_INC_BM3_CORE_TEXTURE_DIRECTX9_BM3_TEX_MOVIEHANDLE_H
#define BM3_SDK_INC_SDK_INC_BM3_CORE_TEXTURE_DIRECTX9_BM3_TEX_MOVIEHANDLE_H

#include "bm3/system/bm3_Config.h"
#include "../bm3_tex_MovieHandleBase.h"
#include <windows.h>
#include <VFW.H>

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

class UpdateInfo;
class DrawInfo;
class RendererInfo;
class ImageInfo;

namespace texture{

class MovieHandle : public MovieHandleBase{

public:
	MovieHandle();
	virtual ~MovieHandle();

	virtual void Bind(TextureLayer::Type layer, DrawInfo &draw_info);

	virtual void PrepareUpdate(UpdateInfo &update_info);
	virtual void PrepareDraw(DrawInfo &draw_info);

protected:
	void Initialize(const wchar_t *filename, LPDIRECT3DDEVICE9 d3d_device);

private:
	LPDIRECT3DTEXTURE9	texture_;

	int					width_;
	int					height_;

	PAVISTREAM			pavistream_;
	PGETFRAME			pgetframe_;
	AVISTREAMINFO		avistreaminfo_;

	float				avi_start_time_;

	bool IsInitialize() const;
	void Finalize();

	int		CurrentFrameNo();
	void	UpdateTexture();
};

}	// namespace texture
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// BM3_SDK_INC_SDK_INC_BM3_CORE_TEXTURE_DIRECTX9_BM3_TEX_MOVIEHANDLE_H
