/*
 DxBMManager.h
 BISHAMON sample manager
 Copyright (C) 2011 Matchlock, Inc.
 All Rights Reserved.
 */

#ifndef __BM_MANAGER_H__
#define __BM_MANAGER_H__

#include "BMBinary.h"
#include "BMTexture.h"
#include "BMModel.h"
#include "BMEffect.h"
#include "BMAllocator.h"

#define MAX_VERTEX_BUFFER_SIZE  (4*1024*1024)

typedef std::map<std::wstring, BMBinary *>	BMBinaryMap;
typedef std::map<std::wstring, BMTexture *> BMTextureMap;
typedef std::map<std::wstring, BMModel *>	BMModelMap;

class BMManager : public BMObject {

public:
	BMManager(LPDIRECT3DDEVICE9 device, const char *data_path, unsigned int maxVertexBufferSize = MAX_VERTEX_BUFFER_SIZE);
	~BMManager();

	void Initialize(unsigned int maxVertexBufferSize = MAX_VERTEX_BUFFER_SIZE);
	BMBinary *LoadBinary(std::wstring name);
	void ReleaseBinary(std::wstring name);
	BMEffect *CreateEffect(std::wstring name);
	BMEffect *CreateEffect(const wchar_t *name);
	BMEffect *CreateEffect(const char *name);
	void ReleaseEffect(BMEffect *effect);
	void DrawEffect(BMEffect *effect);
	void Begin();
	void Begin2D(float width, float height);
	void End();
	void SetView(const ml::matrix44 &view);
	void SetProjection(const ml::matrix44 &projection);

	void SetMaxGeneratingScale(float scale);
	void DeviceLost();
	void ResetDevice(IDirect3DDevice9 *device,unsigned int maxVertexBufferSize = MAX_VERTEX_BUFFER_SIZE);

	void SetUsePNGTexture(bool flag) { UsePNGTexture = flag; }
	void ResetCurrentPointerFlag() { m_VertexCurrentPointerFlag = 0; }

private:
	BMManager() {}
	BMTexture *RegisterTexture(std::wstring name, bm3::TextureCategory::Type type);
	BMModel *RegisterModel(std::wstring name);
	BMBinary *RegisterBinary(std::wstring name);

private:
	bm3::SequentialVertex vertexManager;
	bm3::StandardHash *hash;
	bm3::DrawInfo   drawInfo;

	BMAllocator allocator;

	std::map<std::wstring, BMBinary *> binaries;
	std::map<std::wstring, BMTexture *> textures;
	std::map<std::wstring, BMModel *> models;

	ml::matrix44    view;
	ml::matrix44    viewInverse;
	ml::matrix44    projection;

	std::wstring DataPath;
	bool UsePNGTexture;

	float   maxGeneratingScale;

	LPDIRECT3DDEVICE9 device;

	int m_VertexCurrentPointerFlag;
};

#endif /* __BM_MANAGER_H__ */
