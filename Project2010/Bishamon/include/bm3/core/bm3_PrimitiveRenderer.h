#ifndef BM3_SDK_INC_BM3_CORE_BM3_PRIMITIVERENDERER_H
#define BM3_SDK_INC_BM3_CORE_BM3_PRIMITIVERENDERER_H

#include "../system/bm3_Config.h"
#include "bm3_MemoryManager.h"
#include "bm3_InitializeInfo.h"
#include "bm3_DrawInfo.h"
#include "bm3_RestoreInfo.h"
#include "bm3_Const.h"
#include "bm3_VertexInfo.h"
#include "bm3_InitInfo.h"
#include "bm3_ResetInfo.h"


namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE


/// @brief	PrimitiveRendererクラス
class PrimitiveRenderer{

public:
	PrimitiveRenderer();
	~PrimitiveRenderer();

	bool	Initialize(InitializeInfo &info, int emitter_count, int vertex_buffer_count);
	void	Initialize(InitInfo &init_info);
	void	Finalize(int vertex_buffer_count);

	//j 初期状態に戻す
	void	Reset(ResetInfo &reset_info);

	//j 頂点作成前処理
	void	BeginVertexBuffer(DrawInfo &info, int vertex_index);
	//j 頂点作成後処理
	void	EndVertexBuffer(DrawInfo &info, int vertex_index);
	void*	GetVertexBuffer(int emitter_index, int size, int vertex_index);
	//j 指定されたインデックスの描画オブジェクトを取得する
	void*	EmitterBuffer(int emitter_index, int vertex_index);
	//j プリミティブ一つ分の頂点バッファサイズ
	int		UnitVertexBufferSize(int emitter_index, int vertex_index) const ;
	//j 使用する頂点バッファサイズを取得する
	int		VertexBufferSize(int emitter_index, int vertex_index) const ;
	//j 現在使用される全頂点バッファのサイズを取得する
	int		TotalVertexBufferSize(int vertex_index) const;

	//j 生成されたエミッターの生成子の数を取得する
	int		PushCount(int emitter_index, int vertex_index) const;

	//j プリミティブを描画する
	void	Draw(DrawInfo &info, int vertex_index);
	//j Zソートされたプリミティブを描画する
	void	Draw(DrawInfo &draw_info, int vertex_index, PrimZPList *list);

	void	NodeVertexInfo(int emitter_id, VertexInfo &info, int vertex_index);

	void	DestroyResource(int vertex_index);
	void	RestoreResource(RestoreInfo &info, int vertex_index);
	void	ResetEmitterResource(int vertex_index);


#ifdef BM3_PLATFORM_xbox360
	BOOL PrimitiveRenderer::IsVertexBufferBusy();
#endif

private:
	const obj::RootNodeBase	&Root() const{	ML_PTR_ASSERT(root_);	return *root_;		}
	//j エミッターの描画オブジェクトのメモリを確保する
	void	AllocEmitterBuffer(InitializeInfo &info, int vertex_index);
	void	MapEmitterBuffer(int vertex_index);

	/*j
		@brief	各エミッターの頂点バッファサイズを確定する
		@param	vertex_index	頂点バッファの番号
		@return	全エミッターの頂点バッファサイズ
	*/
	int		FixVertexBuffeSize(int vertex_index);

	class VertexBuffer{
	public:

		VertexBuffer(EmitterTypeConst emitter_type, const void* address);
		~VertexBuffer();

		void	SetChildCount(int child_count){ child_count_ = child_count; }
		void	SetPushCount(int push_count){ push_count_ = push_count; }
		void	DecideSize(){	size_	= BufferSize(child_count_, push_count_);	}
		void	Asign(void* buffer){
			pointer_	= buffer_	= buffer;
		}

		int		ChildCount() const{ return child_count_; }
		int		PushCount() const{ return push_count_; }
		int		Size() 		const{ return size_;	}

		int		BufferSize(int chid_count ,int push_count);			//j 書き換えない頂点バッファは含めないサイズ
		int		TotalBufferSize(int chid_count ,int push_count);	//j 書き換えない頂点バッファも含めたサイズ
		void*	TopBuffer() const { return buffer_; }
		void*	Buffer(int size);

		bool	IsFully() const { return (((int)pointer_ - (int)buffer_) == size_); }

		void	SetEmitterResource(const void *buffer){	emitter_res_buffer_	= buffer;	}
		void	SetEmitterBuffer(void* buffer){			emitter_buffer_	= buffer;	}

		EmitterTypeConst EmitterType()	const{ return emitter_type_;		}
		const void* EmitterResource()	const{ return emitter_res_buffer_;	}
		void*	EmitterBuffer()		const{ return emitter_buffer_;		}	//j エミッターの描画オブジェクト

		VertexBuffer &operator=(const VertexBuffer &){ return *this; }	//j warning を抑えるため

		void	NodeVertexInfo(VertexInfo &info);
	private:
		void*	buffer_;		//j 頂点バッファ
		void*	pointer_;		//j 現在の頂点バッファの位置
		int		child_count_;
		int		push_count_;
		int		size_;			//j 頂点バッファのサイズ
		const EmitterTypeConst	emitter_type_;
		const void*				emitter_res_buffer_;
		void*					emitter_buffer_;	//j エミッターの描画オブジェクト
	};

	obj::RootNodeBase	*root_;
	ml::ui8*			emitter_buffers_;	// すべてのエミッターの描画オブジェクトのバッファポインタ
	int					vertex_buffer_array_size_;

	struct VertexBufferContext{
		VertexBufferContext() :
			vertex_buffer_array_(NULL),
			vtx_handle_(NULL),
			max_vertex_size_(0)
		{
		}

		VertexBuffer*		vertex_buffer_array_;
		void*				vtx_handle_;
		int					max_vertex_size_;
	};

	VertexBufferContext		*vtx_buf_ctx_;
};


BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_BM3_PRIMITIVERENDERER_H
