/*
 BMAllocator.h
 BISHAMON sample allocator
 Copyright (C) 2011 Matchlock, Inc.
 All Rights Reserved.
 */

#ifndef __BM_ALLOCATOR_H__
#define __BM_ALLOCATOR_H__

#include <bm3.h>
#include "BMObject.h"

class BMAllocator : public BMObject, public bm3::MemoryAllocatorInterface {

public:
	BMAllocator() {}
	~BMAllocator() {}

	void *Alloc(int byte_size, bm3::MemoryCategory::Type type) {
		// todo: ƒƒ‚ƒŠ‚ğŠm•Û‚µ‚Ü‚·
		return malloc(byte_size);
	}

	void Free(void *memory, bm3::MemoryCategory::Type type) {
		// todo: ƒƒ‚ƒŠ‚ğ‰ğ•ú‚µ‚Ü‚·
		free(memory);
	}

	int MaxSize(bm3::MemoryCategory::Type type) const {
		return INT_MAX;
	}

};

#endif /* __BM_ALLOCATOR_H__ */
