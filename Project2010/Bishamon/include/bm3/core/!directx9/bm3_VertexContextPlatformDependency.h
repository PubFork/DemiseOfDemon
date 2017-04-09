#ifndef BM3_SDK_INC_BM3_CORE_DIRECTX9_BM3_VERTEXCONTEXTPLATFORMDEPENDENCY_H
#define BM3_SDK_INC_BM3_CORE_DIRECTX9_BM3_VERTEXCONTEXTPLATFORMDEPENDENCY_H

#include "../../system/bm3_Config.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

	struct VertexContextPlatformDependency{
	public:
		VertexContextPlatformDependency() :
			vertex_buffer_(NULL),
			stride_(0),
			prim_type_(D3DPT_TRIANGLELIST),
			index_type_(D3DFMT_INDEX16)
		{
		}

		void	SetBuffer(void* buffer){		vertex_buffer_	= buffer;	}
		void	SetStride(int stride){			stride_			= stride;	}
		
		void	SetPrimType(D3DPRIMITIVETYPE type)	{	prim_type_	= type;	}
		D3DPRIMITIVETYPE PrimType()	const {	return prim_type_;	}
		
		void		SetIndexType(D3DFORMAT index_type){	index_type_ = index_type;	}
		D3DFORMAT	IndexType()	const {	return index_type_;	}
	
		void*		Buffer()		const {	return vertex_buffer_;	}
		int			Stride()		const {	return stride_;			}

		int PrimitiveCount(int vertex_count){
			switch(prim_type_){
				case D3DPT_POINTLIST:		return vertex_count;
				case D3DPT_LINELIST:		return vertex_count/2;
				case D3DPT_LINESTRIP:		return vertex_count-1;
				case D3DPT_TRIANGLELIST:	return vertex_count/3;
				case D3DPT_TRIANGLESTRIP:	return vertex_count-2;
				case D3DPT_TRIANGLEFAN:		return vertex_count-2;
				default:				ML_ASSERT(false);	return 0;
			}
		}
		
	protected:
		void*				vertex_buffer_;	// 頂点バッファの先頭ポインタ
		int					stride_;		// 頂点バッファの間隔
		D3DPRIMITIVETYPE	prim_type_;
		D3DFORMAT			index_type_;
	};

BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_DIRECTX9_BM3_VERTEXCONTEXTPLATFORMDEPENDENCY_H
