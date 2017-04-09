#ifndef BM3_SDK_INC_BM3_CORE_BM3_VERTEXMANAGERINTERFACE_H
#define BM3_SDK_INC_BM3_CORE_BM3_VERTEXMANAGERINTERFACE_H

#include "../system/bm3_Config.h"
#include "bm3_CoreType.h"
#include "bm3_VertexContext.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

/// @brief BeginVertexInfoクラス
struct BeginVertexInfo{
	BeginVertexInfo(int size, void* handle) :
		size_(size), handle_(handle){
	}
	
	void SetHandle(void* handle){	handle_	= handle;	}

	int	Size()		const {	return size_;			}
	void* Handle()	const {	return handle_;			}
	
private:
	int		size_;
	void*	handle_;
};

/// @brief EndVertexInfoクラス
struct EndVertexInfo{
	EndVertexInfo(int size, void* handle) :
		size_(size), handle_(handle){
	}
	
	int	Size()		const {	return size_;			}
	void* Handle()	const {	return handle_;			}
private:
	int		size_;
	void*	handle_;
};
	
/// @brief VertexMemoryInfoクラス
struct VertexMemoryInfo{
public:
	VertexMemoryInfo(int size, int max_size, void* handle) :
		size_(size), max_size_(max_size), handle_(handle){
	}
	
	void SetHandle(void* handle){	handle_	= handle;	}

	int	Size()		const {	return size_;			}
	int MaxSize()	const {	return max_size_;		}
	void* Handle()	const {	return handle_;			}
	
private:
	int		size_;
	int		max_size_;
	void*	handle_;
};

/// @brief VertexManagerInterfaceクラス
class VertexManagerInterface{
public:
	virtual ~VertexManagerInterface(){}
	
	virtual void BeginEffect(BeginVertexInfo & info)	= 0;	//!< エフェクト作成時に呼び出される
	virtual void EndEffect(EndVertexInfo & info)		= 0;	//!< エフェクト終了時に呼び出される

	virtual void BeginMakeVertex(DrawInfo &draw_info, VertexMemoryInfo &vertex_info)	= 0;//!< エフェクトの頂点作成開始ごとに呼び出される
	virtual void EndMakeVertex(DrawInfo &draw_info, VertexMemoryInfo &vertex_info)	= 0;	//!< エフェクトの頂点作成終了ごとに呼び出される

	virtual void* Alloc(VertexMemoryInfo &info)		= 0;	//!< BeginMakeVertex後に呼び出され、バッファを渡す
	virtual bool Alloc(int size)		= 0;				//!< 頂点バッファを書き換えないタイプのバッファを確保するときにサイズの確認のみ行う
	virtual void Free(void* handle, int size)	= 0;	//!< エフェクトデストラクタ時に呼び出され、バッファを開放する
	virtual void Free(int size)	= 0;					//!< 頂点バッファを書き換えないタイプのバッファのサイズを戻す

	virtual int MakeVertexIndex() 	const	= 0;	//!< 現在の頂点バッファインデックス
	virtual int DrawIndex()			const	= 0;	//!< 現在の描画バッファインデックス
	virtual int VertexBufferCount()	const	= 0;	//!< 頂点バッファの本数

	virtual void PrepareDrawPrimitive(DrawInfo &draw_info, VertexContext &ctx)	= 0;//!< 描画準備
	virtual void DrawPrimitive(DrawInfo &draw_info, VertexContext &ctx)	= 0;		//!< 描画
#ifdef BM3_PLATFORM_psp	
	virtual void DrawPrimitiveN(DrawInfo &draw_info, VertexContext &ctx)	= 0;		//!< 描画
#endif
	
#ifdef BM3_PLATFORM_playstation3
	virtual int LocationType()	= 0;	//!< 頂点バッファのタイプ（メイン（マップ済み）かローカル）

#endif
};

BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_BM3_VERTEXMANAGERINTERFACE_H
