#ifndef BM3_SDK_INC_BM3_CORE_BM3_VERTEXMANAGER_H
#define BM3_SDK_INC_BM3_CORE_BM3_VERTEXMANAGER_H

#include "../system/bm3_Config.h"
#include "bm3_CoreType.h"
#include "bm3_DrawInfo.h"
#include "bm3_VertexManagerInterface.h"
#include "bm3_VertexContext.h"
#include <ml/utility/ml_singleton.h>


	
#if defined(BM3_PLATFORM_win_opengl)
	#include "!opengl/bm3_SeqentialUpVertex.h"
#endif
#if defined(BM3_PLATFORM_win_directx9)
	#include "!directx9/bm3_SeqentialUpVertex.h"
	#include "!directx9/bm3_SeqentialVertex.h"
	#include "!directx9/bm3_DiscreteVertex.h"
#endif
#if defined(BM3_PLATFORM_win_directx10)
	#include "!directx10/bm3_SeqentialUpVertex.h"
	#include "!directx10/bm3_SeqentialVertex.h"
	#include "!directx10/bm3_DiscreteVertex.h"
#endif
#if defined(BM3_PLATFORM_playstation3)
	#include "!playstation3/bm3_SeqentialVertex.h"
	#include "!playstation3/bm3_SeqentialUpVertex.h"
	#include "!playstation3/bm3_SeqentialLocalVertex.h"
#endif
#if defined(BM3_PLATFORM_xbox360)
	#include "!xbox360/bm3_SeqentialUpVertex.h"
	#include "!xbox360/bm3_SeqentialVertex.h"
	#include "!xbox360/bm3_DiscreteVertex.h"
#endif
#if defined(BM3_PLATFORM_psp)
	#include "!psp/bm3_SeqentialUpVertex.h"
#endif
#if defined(BM3_PLATFORM_gamebryodx9)
	#include "!gamebryodx9/bm3_SeqentialUpVertex.h"
#endif
#if defined(BM3_PLATFORM_opengles)
	#include "!opengles/bm3_SeqentialUpVertex.h"
#endif
#if defined(BM3_PLATFORM_nw4c)
	#include "!nw4c/bm3_SeqentialVertex.h"
#endif
#if defined(BM3_PLATFORM_psvita)
	#include "!psvita/bm3_SequentialVertex.h"
#endif

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE


/// @brief VertexManagerImplƒNƒ‰ƒX
class VertexManagerImpl{

public:
	VertexManagerImpl() :
		vmi_(NULL){
	}
	~VertexManagerImpl(){
		vmi_	= NULL;
	}

	void BeginEffect(BeginVertexInfo &info){
		ML_PTR_ASSERT(vmi_);
		vmi_->BeginEffect(info);
	}
	void EndEffect(EndVertexInfo &info){
		ML_PTR_ASSERT(vmi_);
		vmi_->EndEffect(info);
	}
	
	void*	Alloc(VertexMemoryInfo &info){
		ML_PTR_ASSERT(vmi_);
		return vmi_->Alloc(info);
	}
	bool	Alloc(int size){
		ML_PTR_ASSERT(vmi_);
		return vmi_->Alloc(size);
	}
	void	Free(void* handle, int size){
		ML_PTR_ASSERT(vmi_);
		vmi_->Free(handle, size);
	}
	void	Free(int size){
		ML_PTR_ASSERT(vmi_);
		vmi_->Free(size);
	}
	
	void BeginMakeVertex(DrawInfo &draw_info, VertexMemoryInfo &vertex_info){
		ML_PTR_ASSERT(vmi_);
		vmi_->BeginMakeVertex(draw_info, vertex_info);
	}
	void EndMakeVertex(DrawInfo &draw_info, VertexMemoryInfo &vertex_info){
		ML_PTR_ASSERT(vmi_);
		vmi_->EndMakeVertex(draw_info, vertex_info);
	}

	void PrepareDrawPrimitive(DrawInfo &draw_info, VertexContext &ctx){
		ML_PTR_ASSERT(vmi_);
		vmi_->PrepareDrawPrimitive(draw_info, ctx);
	}
	void DrawPrimitive(DrawInfo &draw_info, VertexContext &ctx){
		ML_PTR_ASSERT(vmi_);
		vmi_->DrawPrimitive(draw_info, ctx);
	}
#ifdef BM3_PLATFORM_psp
	void DrawPrimitiveN(DrawInfo &draw_info, VertexContext &ctx){
		ML_PTR_ASSERT(vmi_);
		vmi_->DrawPrimitiveN(draw_info, ctx);
	}
#endif	

	int MakeVertexIndex() const{
		ML_PTR_ASSERT(vmi_);
		return vmi_->MakeVertexIndex();
	}
	int DrawIndex() const{
		ML_PTR_ASSERT(vmi_);
		return vmi_->DrawIndex();
	}
	int VertexBufferCount() const{
		ML_PTR_ASSERT(vmi_);
		return vmi_->VertexBufferCount();
	}

#ifdef BM3_PLATFORM_playstation3
	int LocationType(){
		ML_PTR_ASSERT(vmi_);
		return vmi_->LocationType();
	}
#endif
	
	void RegisterManager(VertexManagerInterface &vmi)	{	vmi_	= &vmi;	}
	void UnRegisterManager()							{	vmi_	= NULL;	}

private:
	VertexManagerInterface	*vmi_;
	
};

typedef	ml::singleton<VertexManagerImpl>	VertexManager;

	
BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_BM3_VERTEXMANAGER_H
