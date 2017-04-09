#ifndef BM3_SDK_INC_LIBBM3_H
#define BM3_SDK_INC_LIBBM3_H

#include "bm3/utility/bm3_Environment.h"

#include "bm3/core/bm3_MemoryAllocator.h"
#include "bm3/core/bm3_MemoryManager.h"
#include "bm3/core/bm3_VertexManager.h"

#include "bm3/core/bm3_ProfileManager.h"

#include "bm3/utility/bm3_Hash.h"
#include "bm3/utility/bm3_StandardHash.h"

#include "bm3/core/bm3_System.h"
#include "bm3/core/bm3_ResourceManager.h"
#include "bm3/core/bm3_RenderStateManager.h"
#include "bm3/core/bm3_RenderStateInitializeInfo.h"
#include "bm3/core/bm3_Effect.h"

#include "bm3/core/bm3_InitInfo.h"
#include "bm3/core/bm3_UpdateInfo.h"
#include "bm3/core/bm3_DrawInfo.h"
#include "bm3/core/bm3_RestoreInfo.h"
#include "bm3/core/bm3_ResetInfo.h"

#include "bm3/core/texture/bm3_tex_HandleBase.h"
#include "bm3/core/texture/bm3_tex_MovieHandle.h"
#include "bm3/core/texture/bm3_tex_MovieHandleBase.h"

#include "bm3/core/model/bm3_model_ModelHandleBase.h"
#include "bm3/core/model/bm3_model_M3dgModelHandle.h"

#include "bm3/core/bmsln/bm3_bmsln_HandleBase.h"

#include "bm3/core/fileformat/bm3_bmb.h"
#include "bm3/core/fileformat/bm3_bmb_UpdateBmbInfo.h"
#include "bm3/core/bm3_VertexInfo.h"


#ifdef BM3_PLATFORM_playstation3
#include "bm3/core/!playstation3/spu/bm3_SpuCommon.h"
#include "bm3/core/!playstation3/spu/bm3_SpuHeader.h"
#include "bm3/core/!playstation3/spu/bm3_RawSpuInfo.h"
#ifdef SPU
#include "bm3/core/!playstation3/spu/bm3_SpuNodeProcesser.h"
#include "bm3/core/!playstation3/spu/bm3_SpuTransferUtility.h"
#else
#include "bm3/core/!playstation3/spu/bm3_RawSpuManager.h"
#endif

#endif


#endif	// #ifndef BM3_SDK_INC_LIBBM3_H
