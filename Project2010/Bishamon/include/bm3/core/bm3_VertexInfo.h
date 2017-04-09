#ifndef BM3_SDK_INC_BM3_CORE_BM3_VERTEXINFO_H
#define BM3_SDK_INC_BM3_CORE_BM3_VERTEXINFO_H

#include "../system/bm3_Config.h"

#if defined(BM3_PLATFORM_win_opengl)
	#include "!opengl/bm3_VertexInfoPlatformDependency.h"
#endif
#if defined(BM3_PLATFORM_win_directx9)
	#include "!directx9/bm3_VertexInfoPlatformDependency.h"
#endif
#if defined(BM3_PLATFORM_win_directx10)
	#include "!directx10/bm3_VertexInfoPlatformDependency.h"
#endif
#if defined(BM3_PLATFORM_playstation3)
	#include "!playstation3/bm3_VertexInfoPlatformDependency.h"
#endif
#if defined(BM3_PLATFORM_xbox360)
	#include "!xbox360/bm3_VertexInfoPlatformDependency.h"
#endif
#if defined(BM3_PLATFORM_psp)
	#include "!psp/bm3_VertexInfoPlatformDependency.h"
#endif
#if defined(BM3_PLATFORM_gamebryodx9)
	#include "!gamebryodx9/bm3_VertexInfoPlatformDependency.h"
#endif
#if defined(BM3_PLATFORM_opengles)
	#include "!opengles/bm3_VertexInfoPlatformDependency.h"
#endif
#if defined(BM3_PLATFORM_nw4c)
	#include "!nw4c/bm3_VertexInfoPlatformDependency.h"
#endif
#if defined(BM3_PLATFORM_psvita)
	#include "!psvita/bm3_VertexInfoPlatformDependency.h"
#endif


namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

/// @brief	VertexInfoクラス
class VertexInfo: public internal::VertexInfoPlatformDependency{
public:
	VertexInfo(){;}
	~VertexInfo(){;}
	void* 				Buffer() const { 			return buffer_; 				}
	unsigned int		Count()const {				return count_;					}
	unsigned int 		Size()const { 				return size_; 					}
	unsigned int 		CountPerPrimitive()const {	return count_per_primitive_;	}
	EmitterTypeConst	EmitterType()const { 		return emitter_type_; 			}
	unsigned int 		EmitterID()const { 			return emitter_id_;				}
	unsigned int 		EmitterCount()const { 		return emitter_count_;			}
	
	void SetBuffer(void* buffer){ 					buffer_					= buffer;	}
	void SetCount(unsigned int count){				count_					= count;	}
	void SetSize(unsigned int size){ 				size_					= size; 	}
	void SetCountPerPrimitive(unsigned int count){	count_per_primitive_	= count; 	}
	void SetEmitterType(EmitterTypeConst type){ 	emitter_type_			= type; 	}
	void SetEmitterID(unsigned int id){		 		emitter_id_				= id;	 	}
	void SetEmitterCount(unsigned int count){		emitter_count_			= count; 	}

private:
	void					*buffer_;					// 先頭ポインタ
	unsigned int			count_;						// 頂点数
	unsigned int 			size_;						// 1頂点当たりのサイズ
	unsigned int			count_per_primitive_;		// 1プリミティブあたりの頂点数
	EmitterTypeConst		emitter_type_;				// エミッタのタイプ
	unsigned int			emitter_id_;				// エミッタのID
	unsigned int			emitter_count_;				// エミッタの数
};


BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_BM3_VERTEXINFO_H
