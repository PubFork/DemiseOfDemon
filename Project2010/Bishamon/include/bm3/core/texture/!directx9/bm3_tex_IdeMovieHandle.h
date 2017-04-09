#ifndef BM3_SDK_INC_SDK_INC_BM3_CORE_TEXTURE_DIRECTX9_BM3_TEX_IDEMOVIEHANDLE_H
#define BM3_SDK_INC_SDK_INC_BM3_CORE_TEXTURE_DIRECTX9_BM3_TEX_IDEMOVIEHANDLE_H

#include "bm3/system/bm3_Config.h"
#include "../bm3_tex_IdeHandle.h"
#include "../bm3_tex_MovieHandle.h"
#include <windows.h>
#include <VFW.H>

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

class ImageInfo;

namespace texture{

#if defined(BM3_TARGET_IDE)
namespace ide{

class MovieHandle : public texture::MovieHandle{

public:
	MovieHandle();
	virtual ~MovieHandle();

	virtual void Initialize(const ImageInfo &image_info);
};

}	// namespace ide
#endif	// #if defined(BM3_TARGET_IDE)

}	// namespace texture
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// BM3_SDK_INC_SDK_INC_BM3_CORE_TEXTURE_DIRECTX9_BM3_TEX_IDEMOVIEHANDLE_H
