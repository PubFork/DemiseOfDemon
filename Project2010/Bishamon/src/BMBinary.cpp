/*
 BMBinary.cpp
 BISHAMON sample binary
 Copyright (C) 2011 Matchlock, Inc.
 All Rights Reserved.
 */

#include "stdafx.h"
#include "BMBinary.h"

BMBinary::BMBinary() {
}

BMBinary::~BMBinary(){
}

bm3::bmsln::HandleBase * BMBinary::Load(std::wstring path) {
	data.load_from_file(path);
	bm3::fileformat::bmb::Initialize(data.memory<void *>());
	return this;
}

const void *BMBinary::Image() {
	return data.memory<const void *>();
}
