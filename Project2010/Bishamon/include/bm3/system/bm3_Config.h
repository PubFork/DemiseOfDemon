#ifndef BM3_SDK_INC_BM3_SYSTEM_BM3_CONFIG_H
#define BM3_SDK_INC_BM3_SYSTEM_BM3_CONFIG_H

#ifdef BM3_PLATFORM_opengl_glsl
#include <gl/glew.h>
#endif
#if defined(BM3_PLATFORM_xbox360)
#include <xtl.h>
#include <stdio.h>
#endif
#ifdef BM3_PLATFORM_psp
#include <kernel.h>
#endif
#ifdef BM3_PLATFORM_psvita
#include <gxm.h>
#endif
#ifdef BM3_PLATFORM_playstation3
#ifndef SPU
#include <ppu_intrinsics.h>
#endif
#endif

#include "bm3_PlatformNamespace.h"
namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

#if defined(_MSC_VER)
	#define	BM3_FORCEINLINE	__forceinline
#else
	#define	BM3_FORCEINLINE	inline
#endif

#define	BM3_INLINE	inline


#if defined(_MSC_VER)
	#define	BM3_ASSUME(C)	__assume(C)
#else
	#define	BM3_ASSUME(C)
#endif

#if defined(BM3_PLATFORM_xbox360)
	
namespace internal{
template<int size>
struct CacheLineCount{
	enum{
		Value	= (1 + size) / XM_CACHE_LINE_SIZE
	};
};

template<int LineCount>	struct PrefetchDataImpl{
	BM3_FORCEINLINE	void Execute(const void *ptr){
		for(int i = 0;i < (LineCount + XM_CACHE_LINE_SIZE);i += XM_CACHE_LINE_SIZE){
			__dcbt(i, ptr);
		}
	}
};

template<>	struct PrefetchDataImpl<0>{
	BM3_FORCEINLINE	void Execute(const void *ptr){
		__dcbt(XM_CACHE_LINE_SIZE*0, ptr);
	}
};

template<>	struct PrefetchDataImpl<1>{
	BM3_FORCEINLINE	void Execute(const void *ptr){
		__dcbt(XM_CACHE_LINE_SIZE*0, ptr);
		__dcbt(XM_CACHE_LINE_SIZE*1, ptr);
	}
};

template<>	struct PrefetchDataImpl<2>{
	BM3_FORCEINLINE	void Execute(const void *ptr){
		__dcbt(XM_CACHE_LINE_SIZE*0, ptr);
		__dcbt(XM_CACHE_LINE_SIZE*1, ptr);
		__dcbt(XM_CACHE_LINE_SIZE*2, ptr);
	}
};

template<>	struct PrefetchDataImpl<3>{
	BM3_FORCEINLINE	void Execute(const void *ptr){
		__dcbt(XM_CACHE_LINE_SIZE*0, ptr);
		__dcbt(XM_CACHE_LINE_SIZE*1, ptr);
		__dcbt(XM_CACHE_LINE_SIZE*2, ptr);
		__dcbt(XM_CACHE_LINE_SIZE*3, ptr);
	}
};

template<>	struct PrefetchDataImpl<4>{
	BM3_FORCEINLINE	void Execute(const void *ptr){
		__dcbt(XM_CACHE_LINE_SIZE*0, ptr);
		__dcbt(XM_CACHE_LINE_SIZE*1, ptr);
		__dcbt(XM_CACHE_LINE_SIZE*2, ptr);
		__dcbt(XM_CACHE_LINE_SIZE*3, ptr);
		__dcbt(XM_CACHE_LINE_SIZE*4, ptr);
	}
};

template<>	struct PrefetchDataImpl<5>{
	BM3_FORCEINLINE	void Execute(const void *ptr){
		__dcbt(XM_CACHE_LINE_SIZE*0, ptr);
		__dcbt(XM_CACHE_LINE_SIZE*1, ptr);
		__dcbt(XM_CACHE_LINE_SIZE*2, ptr);
		__dcbt(XM_CACHE_LINE_SIZE*3, ptr);
		__dcbt(XM_CACHE_LINE_SIZE*4, ptr);
		__dcbt(XM_CACHE_LINE_SIZE*5, ptr);
	}
};

template<>	struct PrefetchDataImpl<6>{
	BM3_FORCEINLINE	void Execute(const void *ptr){
		__dcbt(XM_CACHE_LINE_SIZE*0, ptr);
		__dcbt(XM_CACHE_LINE_SIZE*1, ptr);
		__dcbt(XM_CACHE_LINE_SIZE*2, ptr);
		__dcbt(XM_CACHE_LINE_SIZE*3, ptr);
		__dcbt(XM_CACHE_LINE_SIZE*4, ptr);
		__dcbt(XM_CACHE_LINE_SIZE*5, ptr);
		__dcbt(XM_CACHE_LINE_SIZE*6, ptr);
	}
};

template<>	struct PrefetchDataImpl<7>{
	BM3_FORCEINLINE	void Execute(const void *ptr){
		__dcbt(XM_CACHE_LINE_SIZE*0, ptr);
		__dcbt(XM_CACHE_LINE_SIZE*1, ptr);
		__dcbt(XM_CACHE_LINE_SIZE*2, ptr);
		__dcbt(XM_CACHE_LINE_SIZE*3, ptr);
		__dcbt(XM_CACHE_LINE_SIZE*4, ptr);
		__dcbt(XM_CACHE_LINE_SIZE*5, ptr);
		__dcbt(XM_CACHE_LINE_SIZE*6, ptr);
		__dcbt(XM_CACHE_LINE_SIZE*7, ptr);
	}
};

}	// internal

template<int size>
BM3_FORCEINLINE	void PrefetchData(const void *ptr){

#if 0
	// 受け取ったアドレスが XM_CACHE_LINE_SIZE にアライメントされているとは限らないので補正
	for(int i = 0;i < (size + XM_CACHE_LINE_SIZE);i += XM_CACHE_LINE_SIZE){
		__dcbt(i, ptr);
	}
#else
	internal::PrefetchDataImpl<internal::CacheLineCount<size>::Value>().Execute(ptr);
#endif
}

template<int size>
BM3_FORCEINLINE	void PrefetchCacheLine(void * /*ptr*/){
}

	
	
#endif

#if defined(BM3_PLATFORM_psp)

template<int size>
BM3_FORCEINLINE	void PrefetchData(const void * /*ptr*/){
}

// pspは少しずつキャッシュに乗せる方が効率がいいらしい
BM3_FORCEINLINE	void PrefetchCacheLine(void *ptr){
	__builtin_allegrex_cache(SCE_ALLEGREX_CACHEOP_FILL_D , ptr);
}

BM3_FORCEINLINE	void PrefetchCacheLineExclusive(void *ptr){
	__builtin_allegrex_cache(SCE_ALLEGREX_CACHEOP_CREATE_DIRTY_EXCLUSIVE_D, ptr);
}
	
	
#endif

#if defined(BM3_PLATFORM_playstation3)
#ifndef SPU


namespace internal{

#define BM3_PLAYSTATION3_CACHE_LINE_SIZE	(128)

template<int size>
struct CacheLineCount{
	enum{
		Value	= (1 + size) / BM3_PLAYSTATION3_CACHE_LINE_SIZE
	};
};


template<int LineCount>	struct PrefetchDataImpl{
	BM3_FORCEINLINE	void Execute(const unsigned char *ptr){
		for(int i = 0;i < (LineCount + BM3_PLAYSTATION3_CACHE_LINE_SIZE);i += BM3_PLAYSTATION3_CACHE_LINE_SIZE){
			__dcbt(ptr + BM3_PLAYSTATION3_CACHE_LINE_SIZE*i);
		}
	}
};

template<>	struct PrefetchDataImpl<0>{
	BM3_FORCEINLINE	void Execute(const unsigned char *ptr){
		__dcbt(ptr + BM3_PLAYSTATION3_CACHE_LINE_SIZE*0);
	}
};

template<>	struct PrefetchDataImpl<1>{
	BM3_FORCEINLINE	void Execute(const unsigned char *ptr){
		__dcbt(ptr + BM3_PLAYSTATION3_CACHE_LINE_SIZE*0);
		__dcbt(ptr + BM3_PLAYSTATION3_CACHE_LINE_SIZE*1);
	}
};

template<>	struct PrefetchDataImpl<2>{
	BM3_FORCEINLINE	void Execute(const unsigned char *ptr){
		__dcbt(ptr + BM3_PLAYSTATION3_CACHE_LINE_SIZE*0);
		__dcbt(ptr + BM3_PLAYSTATION3_CACHE_LINE_SIZE*1);
		__dcbt(ptr + BM3_PLAYSTATION3_CACHE_LINE_SIZE*2);
	}
};

template<>	struct PrefetchDataImpl<3>{
	BM3_FORCEINLINE	void Execute(const unsigned char *ptr){
		__dcbt(ptr + BM3_PLAYSTATION3_CACHE_LINE_SIZE*0);
		__dcbt(ptr + BM3_PLAYSTATION3_CACHE_LINE_SIZE*1);
		__dcbt(ptr + BM3_PLAYSTATION3_CACHE_LINE_SIZE*2);
		__dcbt(ptr + BM3_PLAYSTATION3_CACHE_LINE_SIZE*3);
	}
};

template<>	struct PrefetchDataImpl<4>{
	BM3_FORCEINLINE	void Execute(const unsigned char *ptr){
		__dcbt(ptr + BM3_PLAYSTATION3_CACHE_LINE_SIZE*0);
		__dcbt(ptr + BM3_PLAYSTATION3_CACHE_LINE_SIZE*1);
		__dcbt(ptr + BM3_PLAYSTATION3_CACHE_LINE_SIZE*2);
		__dcbt(ptr + BM3_PLAYSTATION3_CACHE_LINE_SIZE*3);
		__dcbt(ptr + BM3_PLAYSTATION3_CACHE_LINE_SIZE*4);
	}
};

template<>	struct PrefetchDataImpl<5>{
	BM3_FORCEINLINE	void Execute(const unsigned char *ptr){
		__dcbt(ptr + BM3_PLAYSTATION3_CACHE_LINE_SIZE*0);
		__dcbt(ptr + BM3_PLAYSTATION3_CACHE_LINE_SIZE*1);
		__dcbt(ptr + BM3_PLAYSTATION3_CACHE_LINE_SIZE*2);
		__dcbt(ptr + BM3_PLAYSTATION3_CACHE_LINE_SIZE*3);
		__dcbt(ptr + BM3_PLAYSTATION3_CACHE_LINE_SIZE*4);
		__dcbt(ptr + BM3_PLAYSTATION3_CACHE_LINE_SIZE*5);
	}
};

template<>	struct PrefetchDataImpl<6>{
	BM3_FORCEINLINE	void Execute(const unsigned char *ptr){
		__dcbt(ptr + BM3_PLAYSTATION3_CACHE_LINE_SIZE*0);
		__dcbt(ptr + BM3_PLAYSTATION3_CACHE_LINE_SIZE*1);
		__dcbt(ptr + BM3_PLAYSTATION3_CACHE_LINE_SIZE*2);
		__dcbt(ptr + BM3_PLAYSTATION3_CACHE_LINE_SIZE*3);
		__dcbt(ptr + BM3_PLAYSTATION3_CACHE_LINE_SIZE*4);
		__dcbt(ptr + BM3_PLAYSTATION3_CACHE_LINE_SIZE*5);
		__dcbt(ptr + BM3_PLAYSTATION3_CACHE_LINE_SIZE*6);
	}
};

template<>	struct PrefetchDataImpl<7>{
	BM3_FORCEINLINE	void Execute(const unsigned char *ptr){
		__dcbt(ptr + BM3_PLAYSTATION3_CACHE_LINE_SIZE*0);
		__dcbt(ptr + BM3_PLAYSTATION3_CACHE_LINE_SIZE*1);
		__dcbt(ptr + BM3_PLAYSTATION3_CACHE_LINE_SIZE*2);
		__dcbt(ptr + BM3_PLAYSTATION3_CACHE_LINE_SIZE*3);
		__dcbt(ptr + BM3_PLAYSTATION3_CACHE_LINE_SIZE*4);
		__dcbt(ptr + BM3_PLAYSTATION3_CACHE_LINE_SIZE*5);
		__dcbt(ptr + BM3_PLAYSTATION3_CACHE_LINE_SIZE*6);
		__dcbt(ptr + BM3_PLAYSTATION3_CACHE_LINE_SIZE*7);
	}
};

#undef BM3_PLAYSTATION3_CACHE_LINE_SIZE
}	// internal

template<int size>
BM3_FORCEINLINE	void PrefetchData(const void *ptr){

	internal::PrefetchDataImpl<internal::CacheLineCount<size>::Value>().Execute((unsigned char*)ptr);
}

#endif // ifndef SPU

#ifdef SPU
template<int size>
BM3_FORCEINLINE	void PrefetchData(const void *){
}

template<int size>
BM3_FORCEINLINE	void PrefetchCacheLine(void * /*ptr*/){
}
#else


template<int size>
BM3_FORCEINLINE	void PrefetchCacheLine(void * /*ptr*/){
}
#endif

	
#endif

#if defined(BM3_PLATFORM_win_opengl)
template<int size>
BM3_FORCEINLINE	void PrefetchData(const void *){}
template<int size>
BM3_FORCEINLINE	void PrefetchCacheLine(void * /*ptr*/){}
#endif
	
#if defined(BM3_PLATFORM_win_directx9) || defined(BM3_PLATFORM_win_directx10)
template<int size>
BM3_FORCEINLINE	void PrefetchData(const void *){}
template<int size>
BM3_FORCEINLINE	void PrefetchCacheLine(void * /*ptr*/){}
#endif
	
#if defined(BM3_PLATFORM_gamebryodx9)
template<int size>
BM3_FORCEINLINE	void PrefetchData(const void *){}
template<int size>
BM3_FORCEINLINE	void PrefetchCacheLine(void * /*ptr*/){}
#endif
	
#if defined(BM3_PLATFORM_opengles)
template<int size>
BM3_FORCEINLINE	void PrefetchData(const void *){}
template<int size>
BM3_FORCEINLINE	void PrefetchCacheLine(void * /*ptr*/){}
#endif

#if defined(BM3_PLATFORM_nw4c)
template<int size>
BM3_FORCEINLINE	void PrefetchData(const void *){}
template<int size>
BM3_FORCEINLINE	void PrefetchCacheLine(void * /*ptr*/){}
#endif

#if defined(BM3_PLATFORM_psvita)
template<int size>
BM3_FORCEINLINE	void PrefetchData(const void *){}
template<int size>
BM3_FORCEINLINE	void PrefetchCacheLine(void * /*ptr*/){}
#endif







BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_SYSTEM_BM3_CONFIG_H
