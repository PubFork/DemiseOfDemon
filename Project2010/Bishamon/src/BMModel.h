/*
 BMModel.h
 BISHAMON sample model
 Copyright (C) 2011 Matchlock, Inc.
 All Rights Reserved.
 */

#ifndef	__BM_MODEL_H__
#define	__BM_MODEL_H__

#include <bm3.h>
#include "BMObject.h"

class BMModel : public BMObject, public bm3::model::M3dgModelHandle {

public:
	BMModel();
	~BMModel();

	bm3::model::HandleBase *Load(const std::wstring path, LPDIRECT3DDEVICE9 device);

private:
	ml::memory_stream data;
};

#endif /* __BM_MODEL_H__ */
