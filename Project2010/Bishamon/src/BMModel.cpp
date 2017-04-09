/*
 BMModel.cpp
 BISHAMON sample model
 Copyright (C) 2011 Matchlock, Inc.
 All Rights Reserved.
 */

#include "stdafx.h"
#include "BMModel.h"

BMModel::BMModel() {
}

BMModel::~BMModel(){
	Finalize();
}

bm3::model::HandleBase *BMModel::Load(const std::wstring path, LPDIRECT3DDEVICE9 device){
	data.load_from_file(path);
	Initialize(data.memory<void *>(), device);
	return this;
}
