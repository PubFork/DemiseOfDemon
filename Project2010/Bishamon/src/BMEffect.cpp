/*
 BMEffect.cpp
 BISHAMON sample effect
 Copyright (C) 2011 Matchlock, Inc.
 All Rights Reserved.
 */

#include "stdafx.h"
#include "BMEffect.h"

BMEffect::BMEffect() {
	effect = NULL;
	generatingScale = 1;
	name = L"";
}

BMEffect::~BMEffect() {
	if(effect) {
		bm3::System::instance().ReleaseEffect(effect);
		effect = NULL;
	}
}

bool BMEffect::Create(const BMBinary *binary, std::wstring name) {
	effect = bm3::System::instance().CreateEffect(binary->Bytes());
	this->name = name;
	return effect != NULL;
}

void BMEffect::Update(float frame) {
	bm3::UpdateInfo info;
	info.SetGenerateChildScale(generatingScale);
	info.SetUpdateFrame(frame);
	effect->Update(info);
}

void BMEffect::Draw(bm3::SequentialVertex &vertexManager, bm3::DrawInfo &info) {
	vertexManager.BeginMakeVertex();
	effect->MakeVertex(info);
	vertexManager.EndMakeVertex();
	effect->Draw(info);
}

void BMEffect::RestoreResource(IDirect3DDevice9 *device) {
	bm3::RestoreInfo	restore_info;
	{
		bm3::RendererInfo	renderer_info;
		renderer_info.d3d_device_ = device;
		restore_info.SetRenderer(renderer_info);
	}
	effect->RestoreResource(restore_info);
}
