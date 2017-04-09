/*
 BMBinary.h
 BISHAMON sample binary
 Copyright (C) 2011 Matchlock, Inc.
 All Rights Reserved.
 */

#ifndef	__BM_BINARY_H__
#define	__BM_BINARY_H__

#include <bm3.h>
#include "BMObject.h"

class BMBinary : public BMObject, public bm3::bmsln::HandleBase {

public:
	BMBinary();
	~BMBinary();

	bm3::bmsln::HandleBase *Load(std::wstring path);

private:
	const void *Image();
public:
	const void *Bytes() const { return data.memory<const void *>(); }

private:
	ml::memory_stream data;	// エフェクトデータ
};

#endif /* __BM_BINARY_H__ */
