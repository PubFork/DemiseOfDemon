#ifndef BM3_SDK_INC_BM3_CORE_DIRECTX9_BM3_SEQENTIALVERTEX_H
#define BM3_SDK_INC_BM3_CORE_DIRECTX9_BM3_SEQENTIALVERTEX_H

#include "../../system/bm3_Config.h"
#include "../bm3_CoreType.h"
#include "../bm3_DrawInfo.h"
#include "../bm3_VertexManagerInterface.h"
#include <ml/utility/ml_singleton.h>
#include <ml/utility/ml_assert.h>
#include <ml/utility/ml_debug.h>
#include <ml/utility/ml_offset_pointer.h>

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

	
#define BM3_MAX_SEQENTIAL_VERTEX_BUFFER_SIZE	(4)

#define SeqentialVertex	SequentialVertex	// スペルミス修正
class SequentialVertex : public VertexManagerInterface{
public:
	SequentialVertex() :
		max_size_(0),
		vertex_buffer_count_(0),
		current_size_(0),
		current_fix_size_(0),
		make_vertex_index_(0),
		draw_index_(0)
	{
		for(int i=0 ; i<BM3_MAX_SEQENTIAL_VERTEX_BUFFER_SIZE ; ++i){
			vertex_buffer_array_[i] 	= NULL;
			vertex_buffer_header_[i]	= NULL;
		}
	}
	~SequentialVertex(){

	}

	void Release(){
		for(int i=0 ; i<vertex_buffer_count_ ; ++i){
			if(vertex_buffer_array_[i]){
				vertex_buffer_array_[i]->Release();
				vertex_buffer_array_[i] 	= NULL;
				vertex_buffer_header_[i]	= NULL;
			}
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

	void	Initialize(LPDIRECT3DDEVICE9 d3d_device, int max_vertex_size, int vertex_buffer_count = 1){
		ML_PTR_ASSERT(d3d_device);
		ML_ASSERT(max_vertex_size > 0);
		ML_ASSERT(vertex_buffer_count<BM3_MAX_SEQENTIAL_VERTEX_BUFFER_SIZE);

		vertex_buffer_count_	= vertex_buffer_count;
		max_size_ = max_vertex_size / vertex_buffer_count;
		make_vertex_index_	= 0;
		draw_index_			= 0;

		for(int i=0 ; i<vertex_buffer_count ; ++i){
			HRESULT r = d3d_device->CreateVertexBuffer(max_size_, D3DUSAGE_DYNAMIC, 0, D3DPOOL_DEFAULT, &vertex_buffer_array_[i], NULL);
			r;	ML_ASSERT(r == D3D_OK);
		}
	}

	void BeginMakeVertex(){
		ML_PTR_ASSERT(vertex_buffer_array_[make_vertex_index_]);
		ML_ASSERT(max_size_ > 0);

		void* buffer = NULL;
		HRESULT r = vertex_buffer_array_[make_vertex_index_]->Lock(0, 0, &buffer, D3DLOCK_DISCARD);
		r; ML_ASSERT(r == D3D_OK);
		
		vertex_buffer_header_[make_vertex_index_]	= buffer;
	}

	void EndMakeVertex(){
		ML_PTR_ASSERT(vertex_buffer_array_[make_vertex_index_]);
		ML_ASSERT(max_size_ > 0);

		HRESULT r = vertex_buffer_array_[make_vertex_index_]->Unlock();
		r; ML_ASSERT(r == D3D_OK);
	}

	void BeginMakeVertex(DrawInfo & /*draw_info*/, VertexMemoryInfo & /*vertex_info*/){
	}
	void EndMakeVertex(DrawInfo & /*draw_info*/, VertexMemoryInfo & /*vertex_info*/){
	}
	
	void PrepareDrawPrimitive(DrawInfo &draw_info, VertexContext &ctx){
		ML_PTR_ASSERT(vertex_buffer_array_[draw_index_]);
		ML_PTR_ASSERT(ctx.Buffer());
		ML_ASSERT(ctx.Stride() > 0);

		HRESULT r;
		r = draw_info.Renderer().d3d_device_->SetStreamSource(ctx.Stream(), vertex_buffer_array_[draw_index_], VertexBufferIndex(ctx.Buffer()), ctx.Stride());
		r;ML_ASSERT(r == D3D_OK);
	}

	void DrawPrimitive(DrawInfo &draw_info, VertexContext &ctx){
		ML_PTR_ASSERT(vertex_buffer_array_[draw_index_]);
		
		HRESULT r;
		r = draw_info.Renderer().d3d_device_->DrawPrimitive((D3DPRIMITIVETYPE)ctx.PrimType(), 0, ctx.PrimitiveCount(ctx.VertexCount()));
		r;ML_ASSERT(r == D3D_OK);
	}
	
	void SetMakeVertexIndex(int index){	ML_ASSERT(index<=vertex_buffer_count_); make_vertex_index_	= index;	}
	void SetDrawIndex(int index){		ML_ASSERT(index<=vertex_buffer_count_); draw_index_ 		= index;	}

	int MakeVertexIndex()	const {	return make_vertex_index_;		}
	int DrawIndex()			const {	return draw_index_;				}
	int VertexBufferCount()	const {	return vertex_buffer_count_;	}
	int CurrentSize()		const {	return current_size_ + current_fix_size_;			}
	void ResetCurrentPointer(){		current_size_	= 0;			}
	
private:
	int			max_size_;
	int			vertex_buffer_count_;
	
	int			current_size_;
	int			current_fix_size_;	// 頂点を書き変えないタイプのバッファのサイズ
	int			make_vertex_index_;
	int			draw_index_;
	
	LPDIRECT3DVERTEXBUFFER9	vertex_buffer_array_[BM3_MAX_SEQENTIAL_VERTEX_BUFFER_SIZE];	
	void*					vertex_buffer_header_[BM3_MAX_SEQENTIAL_VERTEX_BUFFER_SIZE];
	
	unsigned int VertexBufferIndex(const void *buffer) const{
		ML_PTR_ASSERT(buffer);
		return int(buffer) - int(vertex_buffer_header_[draw_index_]);
	}
	
	bool Allocable(int size){
		return ((current_fix_size_ + current_size_ + size) <= max_size_);
	}
};
#undef BM3_MAX_SEQENTIAL_VERTEX_BUFFER_SIZE

BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_DIRECTX9_BM3_SEQENTIALVERTEX_H
