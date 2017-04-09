/*
 BMTexture.cpp
 BISHAMON sample texture
 Copyright (C) 2011 Matchlock, Inc.
 All Rights Reserved.
 */

#include "stdafx.h"
#include "BMTexture.h"

BMTexture::BMTexture() {
	texture = NULL;
}
BMTexture::~BMTexture() {
	if(texture) {
		texture->Release();
		texture = NULL;
	}
}
void BMTexture::Bind(bm3::TextureLayer::Type layer_index, bm3::DrawInfo &draw_info) {
	bm3::texture::HandleBase::Bind(layer_index, draw_info, texture);
}

void BMTexture::Load(std::wstring name, LPDIRECT3DDEVICE9 device) {
	D3DXCreateTextureFromFileW(device, name.c_str(), &texture);
}
