#ifndef BM3_SDK_INC_BM3_CORE_DIRECTX9_BM3_DISCRETEVERTEX_H
#define BM3_SDK_INC_BM3_CORE_DIRECTX9_BM3_DISCRETEVERTEX_H

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

	
class DiscreteVertex : public VertexManagerInterface{

public:
	DiscreteVertex() :
		max_size_(0),
		vertex_buffer_count_(0),
	
		current_size_(0),
		current_fix_size_(0),
		make_vertex_index_(0),
		draw_index_(0),
		is_locked_(false),
		d3d_device_(NULL)
	{
		;
	}
	~DiscreteVertex(){
		ML_ASSERT(current_size_+current_fix_size_==0);
	}
	
	void Initialize(LPDIRECT3DDEVICE9 d3d_device, int max_size, int vertex_buffer_count = 1){
		max_size_				= max_size;
		vertex_buffer_count_	= vertex_buffer_count;
		current_size_	= 0;
		d3d_device_		= d3d_device;
	}

	void Release(){
	}
	
	void BeginEffect(BeginVertexInfo & info){
		if(0<info.Size()){
			if(Allocable(info.Size())){
			
				LPDIRECT3DVERTEXBUFFER9	vtx_buffer	= NULL;
				d3d_device_->CreateVertexBuffer(info.Size(), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, 0, D3DPOOL_DEFAULT, &vtx_buffer, NULL);
				
				info.SetHandle(reinterpret_cast<void*>(vtx_buffer));
				current_size_ += info.Size();
			}
			else{
				ML_DEBUG_MESSAGE("頂点バッファを確保出来ませんでした\n");
			}
		}
	}
	
	void EndEffect(EndVertexInfo &info){
		if(info.Handle()){
			LPDIRECT3DVERTEXBUFFER9	vtx_buffer = reinterpret_cast<LPDIRECT3DVERTEXBUFFER9>(info.Handle());
			if(vtx_buffer)	vtx_buffer->Release();
			current_size_ -= info.Size();
		}
	}
	
	void*	Alloc(VertexMemoryInfo &info){

		if(0<info.Size()){
			if(info.Handle()){

				ML_ASSERT(is_locked_==false);
				LPDIRECT3DVERTEXBUFFER9	vtx_buffer = reinterpret_cast<LPDIRECT3DVERTEXBUFFER9>(info.Handle());
				void* buffer = NULL;
				vtx_buffer->Lock(0, 0, &buffer, D3DLOCK_NOOVERWRITE);
				is_locked_	= true;
				return buffer;
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

	void BeginMakeVertex(){}
	void EndMakeVertex(){}
	
	void BeginMakeVertex(DrawInfo & /*draw_info*/, VertexMemoryInfo & /*vertex_info*/){
	}
	
	void EndMakeVertex(DrawInfo & /*draw_info*/, VertexMemoryInfo &vertex_info){
		if(vertex_info.Handle() && is_locked_){
			LPDIRECT3DVERTEXBUFFER9	vtx_buffer = reinterpret_cast<LPDIRECT3DVERTEXBUFFER9>(vertex_info.Handle());
			vtx_buffer->Unlock();
			is_locked_	= false;
		}
	}
	
	void PrepareDrawPrimitive(DrawInfo &draw_info, VertexContext &ctx){
		ML_PTR_ASSERT(ctx.Handle());
		ML_PTR_ASSERT(ctx.Buffer());
		ML_ASSERT(ctx.Stride() > 0);

		HRESULT r;
		r = draw_info.Renderer().d3d_device_->SetStreamSource(ctx.Stream(), reinterpret_cast<LPDIRECT3DVERTEXBUFFER9>(ctx.Handle()), ctx.Offset(), ctx.Stride());
		r; ML_ASSERT(r==D3D_OK);
	}

	void DrawPrimitive(DrawInfo &draw_info, VertexContext &ctx){
		ML_PTR_ASSERT(ctx.Handle());

		HRESULT r;
		r = draw_info.Renderer().d3d_device_->DrawPrimitive(ctx.PrimType(), 0, ctx.PrimitiveCount(ctx.VertexCount()));
		r; ML_ASSERT(r==D3D_OK);
	}
	
	void SetMakeVertexIndex(int index){	ML_ASSERT(index<=vertex_buffer_count_); make_vertex_index_	= index;	}
	void SetDrawIndex(int index){		ML_ASSERT(index<=vertex_buffer_count_); draw_index_ 		= index;	}
	
	int MakeVertexIndex()	const {	return make_vertex_index_;		}
	int DrawIndex()			const {	return draw_index_;				}
	int VertexBufferCount()	const {	return vertex_buffer_count_;	}
	int CurrentSize()		const {	return current_size_ + current_fix_size_;			}
	void ResetCurrentPointer(){		current_size_	= 0;			}
	
private:
	int		max_size_;
	int		vertex_buffer_count_;
	
	int 	current_size_;
	int		current_fix_size_;	// 頂点を書き変えないタイプのバッファのサイズ
	int		make_vertex_index_;	// このクラス内では使われない。ライブラリ内部で使用される。
	int		draw_index_;		// このクラス内では使われない。ライブラリ内部で使用される。
	
	bool	is_locked_;
	LPDIRECT3DDEVICE9	d3d_device_;				// D3Dデバイス

	bool Allocable(int size){
		return ((current_fix_size_ + current_size_ + size) <= max_size_);
	}
};

BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_DIRECTX9_BM3_DISCRETEVERTEX_H
