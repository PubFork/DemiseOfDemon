#ifndef BM3_SDK_INC_BM3_CORE_BM3_MEMORYALLOCATOR_H
#define BM3_SDK_INC_BM3_CORE_BM3_MEMORYALLOCATOR_H

#if defined(BM3_PLATFORM_nw4c)
#pragma diag_suppress 1300
#endif

#include "../system/bm3_Config.h"
#include "bm3_CoreType.h"

namespace bm3{
BM3_BEGIN_PLATFORM_NAMESPACE

/// @brief MemoryAllocatorインターフェース
class MemoryAllocatorInterface{
public:
	virtual ~MemoryAllocatorInterface();

	virtual void *Alloc(int byte_size, MemoryCategory::Type type) = 0;
	virtual void  Free(void *memory, MemoryCategory::Type type) = 0;

	virtual int   MaxSize(MemoryCategory::Type type) const = 0;
};

/// @brief 標準C++ライブラリでのMemoryAllocatorクラス
class StandardMemoryAllocator : public MemoryAllocatorInterface{
public:
	StandardMemoryAllocator();
	~StandardMemoryAllocator();

	void *Alloc(int byte_size, MemoryCategory::Type type);
	void  Free(void *memory, MemoryCategory::Type type);

	int   MaxSize(MemoryCategory::Type type) const;
};


BM3_END_PLATFORM_NAMESPACE
}	// namespace bm3

#endif	// #ifndef BM3_SDK_INC_BM3_CORE_BM3_MEMORYALLOCATOR_H
