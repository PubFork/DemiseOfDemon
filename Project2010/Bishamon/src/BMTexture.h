/*
 BMTexture.h
 BISHAMON sample texture
 Copyright (C) 2011 Matchlock, Inc.
 All Rights Reserved.
 */

#ifndef	__BM_TEXTURE_H__
#define	__BM_TEXTURE_H__

#include <bm3.h>
#include "BMObject.h"

class BMTexture : public BMObject, public bm3::texture::HandleBase {

public:
	BMTexture();
	~BMTexture();

	void Bind(bm3::TextureLayer::Type layer_index, bm3::DrawInfo &draw_info);

	void Load(std::wstring name, LPDIRECT3DDEVICE9 device);

private:
	LPDIRECT3DTEXTURE9	texture;

};

#endif /* __BM_TEXTURE_H__ */
