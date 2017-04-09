/*
 BMManager.cpp
 BISHAMON sample manager
 Copyright (C) 2011 Matchlock, Inc.
 All Rights Reserved.
 */

#include "stdafx.h"
#include "BMManager.h"
#include <locale.h>

BMManager::BMManager(LPDIRECT3DDEVICE9 device, const char *data_path, unsigned int maxVertexBufferSize) {
	this->device = device;
	WCHAR wStr[260];
	size_t wLen = 0;
	mbstowcs_s(&wLen, wStr, 260, data_path, _TRUNCATE);
	std::wstring wdata_path(wStr);
	DataPath = wdata_path;
	// 初期設定
	UsePNGTexture = false;
	m_VertexCurrentPointerFlag = 0;

	Initialize(maxVertexBufferSize);
}

BMManager::~BMManager() {
	assert(binaries.size() == 0);
	assert(textures.size() == 0);
	assert(models.size() == 0);
	if(hash) {
		delete [] hash;
		hash = NULL;
	}
	// 追加ここから
	vertexManager.Release();
	bm3::RenderStateManager::instance().Finalize();
	bm3::System::instance().Finalize();
	bm3::System::finalize();
	// 追加ここまで
}

void BMManager::SetMaxGeneratingScale(float scale) {
	bm3::System::instance().SetMaxGenerateScale(scale);
	maxGeneratingScale = scale;
}

void BMManager::Initialize(unsigned int maxVertexBufferSize) {

	// check environment
	assert(bm3::CheckEnvironment());

	// initialize system
	bm3::System::initialize();
	bm3::System::instance().Initialize();

	// register memory allocator
	bm3::MemoryManager::instance().RegisterAllocator(allocator);

	// initialize render state manager
	bm3::RenderStateInitializeInfo info(device);
	bm3::RenderStateManager::instance().Initialize(info);

	// register resource hash
	hash = new bm3::StandardHash[bm3::ResourceManager_UseHashCount];
	bm3::ResourceManager::instance().RegisterHash(hash);

	// register vertex manager & double buffering
	vertexManager.Initialize(device, maxVertexBufferSize, 2);
	bm3::VertexManager::instance().RegisterManager(vertexManager);

	// default values
	// view
	ml::vector3d eye(0, 20, -20, 0);
	ml::vector3d at(0, 0, 0, 0);
	ml::vector3d up(0, 1, 0, 0);
	view.lookat_r(eye, at, up);
	// view inverse
	viewInverse = view;
	viewInverse.inverse();
	// projection
	projection.perspectivefov_r(40, 4.f/3.f, 0.1f, 1000);

}

BMBinary *BMManager::LoadBinary(std::wstring name) {

	if(binaries[name] != NULL) {
		binaries[name]->Retain();
		return binaries[name];
	}
	
	std::wstring path = DataPath + L"bmsln/" + name + L".bmb";
	BMBinary *binary = new BMBinary;
	binary->Load(path);

	// load textures
	{
		int count = bm3::fileformat::bmb::TextureCount(binary->Bytes());
		for(int i=0; i<count; i++) {
			const wchar_t *name = bm3::fileformat::bmb::TextureNameFromIndex(binary->Bytes(), i);
			BMTexture *texture = RegisterTexture(name, bm3::TextureCategory::Effect);
			if(texture != NULL) {
				bm3::ResourceManager::instance().RegisterTexture(name, texture);
			}
		}
	}

	// load model textures
	{
		int count = bm3::fileformat::bmb::ModelTextureCount(binary->Bytes());
		for(int i=0; i<count; i++) {
			const wchar_t *name = bm3::fileformat::bmb::ModelTextureNameFromIndex(binary->Bytes(), i);
			BMTexture *texture = RegisterTexture(name, bm3::TextureCategory::Model);
			if(texture != NULL) {
				bm3::ResourceManager::instance().RegisterTexture(name, texture);
			}
		}
	}

	// load model
	{
		int count = bm3::fileformat::bmb::ModelCount(binary->Bytes());
		for(int i=0; i<count; i++) {
			const wchar_t *name = bm3::fileformat::bmb::ModelNameFromIndex(binary->Bytes(), i);
			BMModel *model = RegisterModel(name);
			if(model != NULL) {
				bm3::ResourceManager::instance().RegisterModel(name, model);
			}
		}
	}

	// load reference binary
	{
		int count = bm3::fileformat::bmb::BmslnCount(binary->Bytes());
		for(int i=0; i<count; i++) {
			const wchar_t *name = bm3::fileformat::bmb::BmslnNameFromIndex(binary->Bytes(), i);
			const BMBinary *binary = LoadBinary(name);
			if(binary != NULL) {
				bm3::ResourceManager::instance().RegisterBmsln(name, const_cast<BMBinary *>(binary));
			}
		}
	}

	binaries[name] = binary;

	return binary;

}

void BMManager::ReleaseBinary(std::wstring name) {

	BMBinaryMap::iterator itr = binaries.find(name);
	BMBinary *binary = (itr != binaries.end()) ? itr->second : NULL;

	if(binary == NULL) return;

	if(binary->RetainCount() > 1) {
		binary->Release();
		return;
	}

	// release textures
	{
		int count = bm3::fileformat::bmb::TextureCount(binary->Bytes());
		for(int i=0; i<count; i++) {
			const wchar_t *key = bm3::fileformat::bmb::TextureNameFromIndex(binary->Bytes(), i);
			BMTextureMap::iterator itr = textures.find(key);
			BMTexture *texture = (itr != textures.end()) ? itr->second : NULL;
			if(texture) {
				if(texture->RetainCount() == 1) {
					// unregister
					bm3::ResourceManager::instance().UnregisterTexture(key);
					textures.erase(key);
				}
				// retain -1
				texture->Release();
			}
		}
	}

	// release model textures
	{
		int count = bm3::fileformat::bmb::ModelTextureCount(binary->Bytes());
		for(int i=0; i<count; i++) {
			const wchar_t *key = bm3::fileformat::bmb::ModelTextureNameFromIndex(binary->Bytes(), i);
			BMTextureMap::iterator itr = textures.find(key);
			BMTexture *texture = (itr != textures.end())? itr->second : NULL;
			if(texture) {
				if(texture->RetainCount() == 1) {
					// unregister
					bm3::ResourceManager::instance().UnregisterTexture(key);
					textures.erase(key);
				}
				// retain -1
				texture->Release();
			}
		}
	}

	// release model
	{
		int count = bm3::fileformat::bmb::ModelCount(binary->Bytes());
		for(int i=0; i<count; i++) {
			const wchar_t *key = bm3::fileformat::bmb::ModelNameFromIndex(binary->Bytes(), i);
			BMModelMap::iterator itr = models.find(key);
			BMModel *model = (itr != models.end()) ? itr->second : NULL;
			if(model) {
				if(model->RetainCount() == 1) {
					// unregister
					bm3::ResourceManager::instance().UnregisterModel(key);
					models.erase(key);
				}
				// retain -1
				model->Release();
			}
		}
	}

	// release reference binary
	{
		int count = bm3::fileformat::bmb::BmslnCount(binary->Bytes());
		for(int i=0; i<count; i++) {
			const wchar_t *key = bm3::fileformat::bmb::BmslnNameFromIndex(binary->Bytes(), i);
			BMBinaryMap::iterator itr = binaries.find(key);
			BMBinary *binary = (itr != binaries.end()) ? itr->second : NULL;
			if(binary) {
				if(binary->RetainCount() == 1) {
					// unregister
					bm3::ResourceManager::instance().UnregisterBmsln(key);
				}
				ReleaseBinary(key);
			}
		}
	}

	// release
	binary->Release();
	binaries.erase(name);

}

BMEffect *BMManager::CreateEffect(std::wstring name) {

	BMBinary *binary = LoadBinary(name);
	if(binary == NULL) return NULL;

	BMEffect *effect = new BMEffect();
	if(!effect->Create(binary, name)) {
		effect->Release();
		return NULL;
	}

	return effect;

}

BMEffect *BMManager::CreateEffect(const wchar_t *name) {
	return CreateEffect(std::wstring(name));
}
BMEffect *BMManager::CreateEffect(const char *name) {
	wchar_t wStr[260];
	size_t wLen = 0;
	mbstowcs_s(&wLen, wStr, 260, name, _TRUNCATE);
	return CreateEffect(std::wstring(wStr));
}

void BMManager::ReleaseEffect(BMEffect *effect) {
	std::wstring name = effect->GetName();
	effect->Release();
	ReleaseBinary(name);
}

void BMManager::DrawEffect(BMEffect *effect) {
	effect->Draw(vertexManager, drawInfo);
}

void BMManager::Begin() {

	// info
	drawInfo = bm3::DrawInfo(viewInverse, view, projection);
	bm3::RendererInfo renderer;
	renderer.d3d_device_ = device;
	drawInfo.SetRenderer(renderer);

	// lighting
	bm3::RenderStateManager::instance().SetLightDirection(ml::vector3d(1, 1, 1, 1));
	bm3::RenderStateManager::instance().SetLightColor(ml::color_rgba<float>(1, 1, 1, 1));
	bm3::RenderStateManager::instance().SetAmbientLightColor(ml::color_rgba<float>(0.2f, 0.2f, 0.2f, 1));

	// reset vertex buffer
	// 何回も呼ぶとエラーになるので1フレームあたり1度だけ呼ぶ
	if (m_VertexCurrentPointerFlag == 0) {
		m_VertexCurrentPointerFlag = 1;
		vertexManager.ResetCurrentPointer();
	}

}

void BMManager::Begin2D(float width, float height) {

	// view
	ml::matrix44 v;
	v.set_unit();
	SetView(v);

	// projection
	ml::matrix44 p;
	p.orthographic(0, width, 0, height, -100, 100);
	SetProjection(p);

	// begin
	Begin();

}

void BMManager::End() {
	//device->SetTexture(0, NULL);	// DxBishamon内でセットしている
	device->SetVertexShader(NULL);
	device->SetPixelShader(NULL);
}














void BMManager::DeviceLost() {
	// 頂点バッファを解放する
	vertexManager.Release();
}
void BMManager::ResetDevice(IDirect3DDevice9 *device,unsigned int maxVertexBufferSize)
{
	// register vertex manager & double buffering
	vertexManager.Initialize(device, maxVertexBufferSize, 2);
	bm3::VertexManager::instance().RegisterManager(vertexManager);
}

BMTexture *BMManager::RegisterTexture(std::wstring name, bm3::TextureCategory::Type type) {

	if(textures[name] != NULL) {
		textures[name]->Retain();
		return NULL;
	}

	std::wstring path = DataPath + ((type == bm3::TextureCategory::Model) ? L"model/" : L"texture/") + name + (UsePNGTexture ? L".png" : L".dds");

	BMTexture *texture = new BMTexture();
	textures[name] = texture;
	texture->Load(path, device);

	return texture;

}

BMModel *BMManager::RegisterModel(std::wstring name) {

	if(models[name] != NULL) {
		models[name]->Retain();
		return NULL;
	}

	std::wstring path = DataPath + L"/model/" + name + L".m3r";

	BMModel *model = new BMModel();
	models[name] = model;
	model->Load(path, device);

	return model;

}

void BMManager::SetView(const ml::matrix44 &view) {
	this->view = view;
	viewInverse = view;
	viewInverse.inverse();
}

void BMManager::SetProjection(const ml::matrix44 &projection) {
	this->projection = projection;
}
