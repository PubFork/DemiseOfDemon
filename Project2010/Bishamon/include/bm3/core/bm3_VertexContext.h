#ifndef BM3_SDK_INC_BM3_CORE_BM3_VERTEXCONTEXT_H
#define BM3_SDK_INC_BM3_CORE_BM3_VERTEXCONTEXT_H

#include "../system/bm3_Config.h"

#if defined(BM3_PLATFORM_win_opengl)
	#include "!opengl/bm3_VertexContextPlatformDependency.h"
#endif
#if defined(BM3_PLATFORM_win_directx9)
	#include "!directx9/bm3_VertexContextPlatformDependency.h"
#endif
#if defined(BM3_PLATFORM_win_directx10)
	#include "!directx10/bm3_VertexContextPlatformDependency.h"
#endif
#if defined(BM3_PLATFORM_playstation3)
	#include "!playstation3/bm3_VertexContextPlatformDependency.h"
#endif
#if defined(BM3_PLATFORM_xbox360)
	#include "!xbox360/bm3_VertexContextPlatformDependency.h"
#endif
#if defined(BM3_PLATFORM_psp)
	#include "!psp/bm3_VertexContextPlatformDependency.h"
#endif
#if defined(BM3_PLATFORM_gamebryodx9)
	#include "!gamebryodx9/bm3_VertexContextPlatformDependency.h"
#endif
#if defined(BM3_PLATFORM_opengles)
	#include "!opengles/bm3_VertexContextPlatformDependency.h"
#endif
#if defined(BM3_PLATFORM_nw4c)
	#include "!nw4c/bm3_VertexContextPlatformDependency.h"
#endif
#if defined(BM3_PLATFORM_psvita)
	#include "!psvita/bm3_VertexContextPlatformDependency.h"
#endif

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

	/// @brief VertexContextクラス
	struct VertexContext : public VertexContextPlatformDependency{
	public:
		
		VertexContext() :
		VertexContextPlatformDependency(),
		index_buffer_(NULL),
		stream_(0),
		offset_byte_(0),
		handle_(NULL),
		index_handle_(NULL),
		vertex_count_(0)
		{
			;
		}
		
		void	SetIndexBuffer(const void* buffer){	index_buffer_	= buffer;	}
		void	SetStream(int stream){			stream_			= stream;	}
		void	ResetOffset(){					offset_byte_	= 0;		}
		void	SetOffset(int offset){			offset_byte_	= offset;	}
		void	AddOffset(int offset){			offset_byte_	+= offset;	}
		void	SetHandle(void* handle){		handle_			= handle;	}
		void	SetIndexHandle(void* handle){	index_handle_	= handle;	}
		void	SetVertexCount(int count){		vertex_count_	= count;	}
		
		
		const void*	IndexBuffer()	const {	return index_buffer_;	}
		int			Stream()		const {	return stream_;			}
		int			Offset()		const {	return offset_byte_;	}
		void*		Handle()		const {	return handle_;			}
		void*		IndexHandle()	const {	return index_handle_;	}
		int			VertexCount()	const {	return vertex_count_;	}

	private:
		const void*	index_buffer_;	// インデックスバッファの先頭ポインタ
		int			stream_;		// ストリーム番号
		int			offset_byte_;	// 頂点バッファの先頭ポインタからのオフセット
		void*		handle_;		// 頂点バッファのハンドル
		void*		index_handle_;	// インデックスバッファのハンドル
		int			vertex_count_;	// 頂点バッファの数
	};

BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3



#endif	// #ifndef BM3_SDK_INC_BM3_CORE_BM3_VERTEXCONTEXT_H
