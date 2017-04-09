#ifndef BM3_SDK_INC_BM3_CORE_DIRECTX9_BM3_SEQENTIALUPVERTEX_H
#define BM3_SDK_INC_BM3_CORE_DIRECTX9_BM3_SEQENTIALUPVERTEX_H

#include "../system/bm3_Config.h"
#include "bm3_CoreType.h"
#include "bm3_DrawInfo.h"
#include "bm3_VertexManagerInterface.h"
#include <ml/utility/ml_singleton.h>
#include <ml/utility/ml_assert.h>
#include <ml/utility/ml_debug.h>
#include <ml/utility/ml_offset_pointer.h>

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

#define BM3_MAX_SEQENTIALUP_VERTEX_BUFFER_SIZE	(4)
	
#define SeqentialUpVertex	SequentialUpVertex	// スペルミス修正
class SequentialUpVertex : public VertexManagerInterface{

public:
	SequentialUpVertex() :
		header_(NULL),
		max_size_(0),
		vertex_buffer_count_(0),
	
		current_size_(0),
		current_fix_size_(0),
		make_vertex_index_(0),
		draw_index_(0)
	{
		for(int i=0 ; i<BM3_MAX_SEQENTIALUP_VERTEX_BUFFER_SIZE ; ++i){
			vertex_buffer_header_[i]	= NULL;
		}
	}
	~SequentialUpVertex(){
	}

	void	Initialize(LPDIRECT3DDEVICE9 /*d3d_device*/, unsigned int max_size, int vertex_buffer_count = 1){
		ML_ASSERT(max_size > 0);

		header_					= bm3::MemoryManager::instance().Shader_Alloc(max_size);
		ML_PTR_ASSERT(header_);
		
		max_size_				= max_size / vertex_buffer_count;
		vertex_buffer_count_	= vertex_buffer_count;
		
		current_size_		= 0;
		make_vertex_index_	= 0;
		draw_index_			= 0;
		
		for(int i=0 ; i<vertex_buffer_count_ ; ++i){
			vertex_buffer_header_[i]	= ml::offset_pointer<void*>(header_, i * max_size_);
		}
		
	}
	
	void Release(){
		if(header_){
			bm3::MemoryManager::instance().Shader_Free(header_);
			header_	= NULL;
		}
	}

	void BeginEffect(BeginVertexInfo & /*info*/){
	}
	
	void EndEffect(EndVertexInfo & /*info*/){
	}
	
	void*	Alloc(VertexMemoryInfo &info){

		if(0<info.Size()){
			void* buffer_header = vertex_buffer_header_[make_vertex_index_];
			if(buffer_header!=NULL){

				if(Allocable(info.Size())){
					int offset = current_size_;
					current_size_	+= info.Size();
					return ml::offset_pointer<void*>(buffer_header, offset);
				}
			}
			
			ML_DEBUG_MESSAGE("頂点バッファを確保出来ませんでした\n");
		}
		return NULL;
	}
	bool Alloc(int size){
		if(Allocable(size)){
			current_fix_size_	+= size;
			return true;
		}
		else{
			ML_DEBUG_MESSAGE("頂点バッファを確保出来ませんでした\n");
		}
		return false;
	}
	void	Free(void * /*handle*/, int /*size*/){
	}
	void	Free(int size){
		current_fix_size_ -= size;
	}

	void BeginMakeVertex(){
	}

	void EndMakeVertex(){
	}

	void BeginMakeVertex(DrawInfo & /*draw_info*/, VertexMemoryInfo & /*vertex_info*/){
	}
	void EndMakeVertex(DrawInfo & /*draw_info*/, VertexMemoryInfo & /*vertex_info*/){
	}

	void PrepareDrawPrimitive(DrawInfo & /*draw_info*/, VertexContext & /*ctx*/){
	}
	
	void DrawPrimitive(DrawInfo &draw_info, VertexContext &ctx){
		ML_PTR_ASSERT(header_);
		ML_PTR_ASSERT(ctx.Buffer());
		ML_ASSERT(ctx.Stride() > 0);

		HRESULT r = draw_info.Renderer().d3d_device_->DrawPrimitiveUP(ctx.PrimType(), ctx.PrimitiveCount(ctx.VertexCount()), ctx.Buffer(), ctx.Stride());
		r; ML_ASSERT(r==D3D_OK);
	}

	void SetMakeVertexIndex(int index){	ML_ASSERT(index<=vertex_buffer_count_); make_vertex_index_	= index;	}
	void SetDrawIndex(int index){		ML_ASSERT(index<=vertex_buffer_count_); draw_index_ 		= index;	}
	
	int MakeVertexIndex()	const {	return make_vertex_index_;		}
	int DrawIndex()			const {	return draw_index_;				}	// 描画はポインタで持っているから使われない
	int VertexBufferCount()	const {	return vertex_buffer_count_;	}
	int CurrentSize()		const {	return current_size_ + current_fix_size_;			}
	void ResetCurrentPointer(){		current_size_	= 0;			}
	
private:
	void*		header_;
	int			max_size_;
	int			vertex_buffer_count_;
	
	int			current_size_;
	int			current_fix_size_;	// 頂点を書き変えないタイプのバッファのサイズ
	int			make_vertex_index_;
	int			draw_index_;
	void*		vertex_buffer_header_[BM3_MAX_SEQENTIALUP_VERTEX_BUFFER_SIZE];

	bool Allocable(int size){
		return ((current_fix_size_ + current_size_ + size) <= max_size_);
	}
	
};
#undef BM3_MAX_SEQENTIALUP_VERTEX_BUFFER_SIZE	

BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_DIRECTX9_BM3_SEQENTIALUPVERTEX_H
