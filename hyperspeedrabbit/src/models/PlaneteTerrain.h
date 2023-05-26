#pragma once
#include "Objet3d.h"
class PlaneteTerrain : public Objet3D
{
public:
	PlaneteTerrain(const char* filename, XMFLOAT3 scale, PM3D::CDispositifD3D11* pDispositif, XMFLOAT3 _position = {}, XMFLOAT4 _rotation = {0.0f,0.f,0.f,1.f});

	// Hérité via Objet3D
	virtual void anime(float) override;
	// Hérité via Objet3D
};
