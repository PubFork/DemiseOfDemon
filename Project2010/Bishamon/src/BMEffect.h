/*
 BMEffect.h
 BISHAMON sample effect
 Copyright (C) 2011 Matchlock, Inc.
 All Rights Reserved.
 */

#ifndef	__BM_EFFECT_H__
#define	__BM_EFFECT_H__

#include "BMBinary.h"

class BMEffect : public BMObject {
public:
	BMEffect();
	~BMEffect();

	bool Create(const BMBinary *binary, std::wstring name);
	void Update(float frame = 1);
	void Draw(bm3::SequentialVertex &vertexManager, bm3::DrawInfo &info);

	void Reset() { bm3::ResetInfo info; effect->Reset(info); }
	void Retire() { effect->RequestRetire(); }
	void SetMatrix(const ml::matrix44 &matrix) { effect->SetMatrix(matrix); }
	void SetColorScale(const ml::color_rgba<float> &scale) { effect->SetColorScale(scale); }
	void SetColorScale(float r, float g, float b, float a) { effect->SetColorScale(ml::color_rgba<float>(r, g, b, a)); }
	void SetOffsetPosition(const ml::vector3d &offset) { effect->SetOffsetPosition(offset); }
	void SetOffsetPosition(float x, float y, float z) { effect->SetOffsetPosition(ml::vector3d(x, y, z, 0)); }

	void SetGeneratingScale(float scale) { generatingScale = scale; }
	bool IsExpired() { return effect->IsExpired(); }
	std::wstring GetName() const { return name; }

	void ReleaseResource(){effect->DestroyResource();}
	void RestoreResource(IDirect3DDevice9 *device);

private:
	bm3::EffectHandle   effect;
	float               generatingScale;
	std::wstring		name;
};

#endif /* __BM_EFFECT_H__ */
